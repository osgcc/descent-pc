/*
 * $Source: f:/miner/source/main/editor/rcs/centers.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:35:30 $
 * 
 * Dialog box stuff for control centers, material centers, etc.
 * 
 * $Log: centers.c $
 * Revision 2.0  1995/02/27  11:35:30  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.9  1994/11/27  23:17:28  matt
 * Made changes for new mprintf calling convention
 * 
 * Revision 1.8  1994/10/05  22:13:46  mike
 * Clean up Centers dialog.
 * 
 * Revision 1.7  1994/10/03  23:39:55  mike
 * Call fuelcen_activate instead of fuelcen_create.
 * 
 * Revision 1.6  1994/08/02  12:16:35  mike
 * Change materialization center functionality.
 * 
 * Revision 1.5  1994/08/01  11:04:42  yuan
 * New materialization centers.
 * 
 * Revision 1.4  1994/07/22  17:19:10  yuan
 * Working on dialog box for refuel/repair/material/control centers.
 * 
 * Revision 1.3  1994/07/21  19:35:09  yuan
 * Fixed #include problem
 * 
 * Revision 1.2  1994/07/21  19:02:41  yuan
 * *** empty log message ***
 * 
 * Revision 1.1  1994/07/18  16:00:54  yuan
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: centers.c 2.0 1995/02/27 11:35:30 john Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "fuelcen.h"
#include "screens.h"
#include "inferno.h"
#include "segment.h"
#include "editor.h"

#include "timer.h"
#include "objpage.h"
#include "fix.h"
#include "mono.h"
#include "error.h"
#include "kdefs.h"
#include	"object.h"
#include "polyobj.h"
#include "game.h"
#include "powerup.h"
#include "ai.h"
#include "hostage.h"
#include "eobject.h"
#include "medwall.h"
#include "eswitch.h"
#include "ehostage.h"
#include "key.h"
#include "medrobot.h"
#include "bm.h"
#include "centers.h"

//-------------------------------------------------------------------------
// Variables for this module...
//-------------------------------------------------------------------------
static UI_WINDOW 				*MainWindow = NULL;
static UI_GADGET_BUTTON 	*QuitButton;
static UI_GADGET_RADIO		*CenterFlag[MAX_CENTER_TYPES];
static UI_GADGET_CHECKBOX	*RobotMatFlag[MAX_ROBOT_TYPES];

static int old_seg_num;

extern	char	center_names[MAX_CENTER_TYPES][CENTER_STRING_LENGTH] = {
	"Nothing",
	"FuelCen",
	"RepairCen",
	"ControlCen",
	"RobotMaker"
};

//-------------------------------------------------------------------------
// Called from the editor... does one instance of the centers dialog box
//-------------------------------------------------------------------------
int do_centers_dialog()
{
	int i;

	// Only open 1 instance of this window...
	if ( MainWindow != NULL ) return 0;

	// Close other windows.	
	close_trigger_window();
	hostage_close_window();
	close_wall_window();
	robot_close_window();

	// Open a window with a quit button
	MainWindow = ui_open_window( TMAPBOX_X+20, TMAPBOX_Y+20, 765-TMAPBOX_X, 545-TMAPBOX_Y, WIN_DIALOG );
	QuitButton = ui_add_gadget_button( MainWindow, 20, 252, 48, 40, "Done", NULL );

	// These are the checkboxes for each door flag.
	i = 80;
	CenterFlag[0] = ui_add_gadget_radio( MainWindow, 18, i, 16, 16, 0, "NONE" ); 			i += 24;
	CenterFlag[1] = ui_add_gadget_radio( MainWindow, 18, i, 16, 16, 0, "FuelCen" );		i += 24;
	CenterFlag[2] = ui_add_gadget_radio( MainWindow, 18, i, 16, 16, 0, "RepairCen" );	i += 24;
	CenterFlag[3] = ui_add_gadget_radio( MainWindow, 18, i, 16, 16, 0, "ControlCen" );	i += 24;
	CenterFlag[4] = ui_add_gadget_radio( MainWindow, 18, i, 16, 16, 0, "RobotCen" );		i += 24;

	// These are the checkboxes for each door flag.
	for (i=0; i<N_robot_types; i++)
		RobotMatFlag[i] = ui_add_gadget_checkbox( MainWindow, 128 + (i%2)*92, 20+(i/2)*24, 16, 16, 0, Robot_names[i]);
																									  
	old_seg_num = -2;		// Set to some dummy value so everything works ok on the first frame.

	return 1;
}

void close_centers_window()
{
	if ( MainWindow!=NULL )	{
		ui_close_window( MainWindow );
		MainWindow = NULL;
	}
}

void do_centers_window()
{
	int i;
//	int robot_flags;
	int redraw_window;

	if ( MainWindow == NULL ) return;

	//------------------------------------------------------------
	// Call the ui code..
	//------------------------------------------------------------
	ui_button_any_drawn = 0;
	ui_window_do_gadgets(MainWindow);

	//------------------------------------------------------------
	// If we change walls, we need to reset the ui code for all
	// of the checkboxes that control the wall flags.  
	//------------------------------------------------------------
	if (old_seg_num != Cursegp-Segments) {
		for (	i=0; i < MAX_CENTER_TYPES; i++ ) {
			CenterFlag[i]->flag = 0;		// Tells ui that this button isn't checked
			CenterFlag[i]->status = 1;		// Tells ui to redraw button
		}

		Assert(Cursegp->special < MAX_CENTER_TYPES);
		CenterFlag[Cursegp->special]->flag = 1;

		mprintf((0, "Cursegp->matcen_num = %i\n", Cursegp->matcen_num));

		//	Read materialization center robot bit flags
		for (	i=0; i < N_robot_types; i++ ) {
			RobotMatFlag[i]->status = 1;		// Tells ui to redraw button
			if (RobotCenters[Cursegp->matcen_num].robot_flags & (1 << i))
				RobotMatFlag[i]->flag = 1;		// Tells ui that this button is checked
			else
				RobotMatFlag[i]->flag = 0;		// Tells ui that this button is not checked
		}

	}

	//------------------------------------------------------------
	// If any of the radio buttons that control the mode are set, then
	// update the corresponding center.
	//------------------------------------------------------------

	redraw_window=0;
	for (	i=0; i < MAX_CENTER_TYPES; i++ )	{
		if ( CenterFlag[i]->flag == 1 )
			if ( i == 0)
				fuelcen_delete(Cursegp);
			else if ( Cursegp->special != i ) {
				fuelcen_delete(Cursegp);
				redraw_window = 1;
				fuelcen_activate( Cursegp, i );
			}
	}

	for (	i=0; i < N_robot_types; i++ )	{
		if ( RobotMatFlag[i]->flag == 1 ) {
			if (!(RobotCenters[Cursegp->matcen_num].robot_flags & (1<<i) )) {
				RobotCenters[Cursegp->matcen_num].robot_flags |= (1<<i);
				mprintf((0,"Segment %i, matcen = %i, Robot_flags %d\n", Cursegp-Segments, Cursegp->matcen_num, RobotCenters[Cursegp->matcen_num].robot_flags));
			} 
		} else if (RobotCenters[Cursegp->matcen_num].robot_flags & 1<<i) {
			RobotCenters[Cursegp->matcen_num].robot_flags &= ~(1<<i);
			mprintf((0,"Segment %i, matcen = %i, Robot_flags %d\n", Cursegp-Segments, Cursegp->matcen_num, RobotCenters[Cursegp->matcen_num].robot_flags));
		}
	}
	
	//------------------------------------------------------------
	// If anything changes in the ui system, redraw all the text that
	// identifies this wall.
	//------------------------------------------------------------
	if (redraw_window || (old_seg_num != Cursegp-Segments ) ) {
//		int	i;
//		char	temp_text[CENTER_STRING_LENGTH];
	
		ui_wprintf_at( MainWindow, 12, 6, "Seg: %3d", Cursegp-Segments );

//		for (i=0; i<CENTER_STRING_LENGTH; i++)
//			temp_text[i] = ' ';
//		temp_text[i] = 0;

//		Assert(Cursegp->special < MAX_CENTER_TYPES);
//		strncpy(temp_text, Center_names[Cursegp->special], strlen(Center_names[Cursegp->special]));
//		ui_wprintf_at( MainWindow, 12, 23, " Type: %s", temp_text );
		Update_flags |= UF_WORLD_CHANGED;
	}

	if ( QuitButton->pressed || (last_keypress==KEY_ESC) )	{
		close_centers_window();
		return;
	}		

	old_seg_num = Cursegp-Segments;
}



