/*
 * $Source: f:/miner/source/main/editor/rcs/objpage.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:34:43 $
 * 
 * object selection stuff.
 * 
 * $Log: objpage.c $
 * Revision 2.0  1995/02/27  11:34:43  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.37  1995/01/14  19:17:55  john
 * First version of object paging.
 * 
 * Revision 1.36  1995/01/05  16:20:13  mike
 * bah, remove the int3.
 * 
 * Revision 1.35  1995/01/05  12:47:13  mike
 * Move code about to prevent compiler warning.
 * 
 * Revision 1.34  1994/11/18  15:23:55  john
 * Made so the int3 for drawing an invalib object just returns.
 * 
 * Revision 1.33  1994/11/02  16:19:18  matt
 * Moved draw_model_picture() out of editor, and cleaned up code
 * 
 * Revision 1.32  1994/10/23  02:11:18  matt
 * Got rid of obsolete hostage_info stuff
 * 
 * Revision 1.31  1994/09/09  14:41:35  matt
 * New parms for draw_polygon_model()
 * 
 * Revision 1.30  1994/08/29  19:26:53  matt
 * Fixed botches change from yesterday
 * 
 * Revision 1.29  1994/08/28  23:40:35  matt
 * 
 * 
 * Revision 1.28  1994/08/14  23:15:32  matt
 * Added animating bitmap hostages, and cleaned up vclips a bit
 * 
 * Revision 1.27  1994/08/13  14:59:02  matt
 * Finished adding support for miscellaneous objects
 * 
 * Revision 1.26  1994/08/09  16:06:15  john
 * Added the ability to place players.  Made old
 * Player variable be ConsoleObject.
 * 
 * Revision 1.25  1994/07/28  16:59:23  mike
 * objects containing objects.
 * 
 * Revision 1.24  1994/06/08  18:17:25  john
 * Changed the way object types and id's work for hostages
 * and powerups.
 * 
 * Revision 1.23  1994/06/07  16:52:30  matt
 * Made object lighting work correctly; changed name of Ambient_light to
 * Dynamic_light; cleaned up polygobj object rendering a little.
 * 
 * Revision 1.22  1994/05/31  18:42:07  matt
 * Made robot photos unlighted
 * 
 * Revision 1.21  1994/05/26  21:09:34  matt
 * Moved robot stuff out of polygon model and into robot_info struct
 * Made new file, robot.c, to deal with robots
 * 
 * Revision 1.20  1994/05/19  12:10:11  matt
 * Use new vecmat macros and globals
 * 
 * Revision 1.19  1994/05/17  14:45:27  mike
 * Get object type and id from ObjType and ObjId.
 * 
 * Revision 1.18  1994/05/17  12:03:36  mike
 * Deal with little known fact that polygon object != robot.
 * 
 * Revision 1.17  1994/04/29  09:16:56  matt
 * Added support for multiple-piece explosions
 * 
 * Revision 1.16  1994/04/18  14:15:00  john
 * Initial version of robot dialog box.
 * 
 * Revision 1.15  1994/04/11  12:01:36  yuan
 * Fixed resetting to first object on page annoyance.
 * 
 * Revision 1.14  1994/04/01  14:35:50  yuan
 * Removed debug "id %d\n" for placing objects
 * 
 * Revision 1.13  1994/04/01  11:17:04  yuan
 * Added objects to objpage. Added buttons for easier tmap scrolling.
 * Objects are selected fully from objpage and add object menu or pad.
 * 
 * Revision 1.12  1994/03/25  18:42:26  matt
 * Adjusted constant to make robot pictures more correct size
 * 
 * Revision 1.11  1994/03/25  16:57:17  matt
 * New parm to draw_polygon_object(), and draw object "snapshots" in
 * correct size (although this probably doesn't work yet).
 * 
 * Revision 1.10  1994/03/25  14:23:01  matt
 * Disabled lighting when taking "snapshots" of robots
 * 
 * Revision 1.9  1994/03/17  10:47:24  john
 * Corrected all kinds of problems associated with N_polygon_objects.
 * 
 * Revision 1.8  1994/03/16  11:07:31  john
 * Made zoom work a bit better.
 * 
 * Revision 1.7  1994/03/16  10:50:32  john
 * fixed warning with constant length.
 * 
 * Revision 1.6  1994/03/16  10:43:18  john
 * Added controls to rotate/zoom object.
 * 
 * Revision 1.5  1994/03/15  22:23:24  matt
 * Render little bitmap pictures of robots for display on editor page
 * 
 * Revision 1.4  1994/02/01  11:27:14  john
 * Hacked in 8 object types for demo.
 * 
 * Revision 1.3  1994/01/26  16:42:19  john
 * Display numbers instead of bitmaps... made 
 * Num_robot_types or whatever be set to 4
 * in init_object_page... this is a hack!!!
 * 
 * Revision 1.2  1993/12/16  17:26:24  john
 * Moved texture and object selection to texpage and objpage
 * 
 * Revision 1.1  1993/12/16  16:12:57  john
 * Initial revision
 * 
 * 
 */



#pragma off (unreferenced)
static char rcsid[] = "$Id: objpage.c 2.0 1995/02/27 11:34:43 john Exp $";
#pragma on (unreferenced)

// Num_robot_types -->  N_polygon_models
// Cur_robot_type --> Cur_robot_type
// Texture[Cur_robot_type]->bitmap  ---> robot_bms[robot_bm_nums[ Cur_robot_type ] ] 

#include <stdlib.h>
#include <stdio.h>


#include "inferno.h"
#include "screens.h"			// For GAME_SCREEN?????
#include "editor.h"			// For TMAP_CURBOX??????
#include "gr.h"				// For canves, font stuff
#include "ui.h"				// For UI_GADGET stuff
#include "object.h"			// For robot_bms
#include "mono.h"				// For debugging
#include "error.h"

#include "objpage.h"
#include "bm.h"
#include "player.h"
#include "piggy.h"


#define OBJS_PER_PAGE 8

static UI_GADGET_USERBOX * ObjBox[OBJS_PER_PAGE];
static UI_GADGET_USERBOX * ObjCurrent;

static int ObjectPage = 0;

//static char Description[8][20] = { "Pig", "Star", "Little\nJosh", "Big\nJosh", "Flying\nPig", "Flying\nStar", 
//"Little\nFlying\nJosh", "Big\nFlying\nJosh" };

//static grs_canvas * ObjnameCanvas;
//static char object_filename[13];

//static void objpage_print_name( char name[13] ) {
//	 short w,h,aw;
//
//    gr_set_current_canvas( ObjnameCanvas );
//    gr_get_string_size( name, &w, &h, &aw );
//    gr_string( 0, 0, name );			  
//	 //gr_set_fontcolor( CBLACK, CWHITE );
//}

//static void objpage_display_name( char *format, ... ) {
//	va_list ap;
//
//	va_start(ap, format);
//   vsprintf(object_filename, format, ap);
//	va_end(ap);
//
//   objpage_print_name(object_filename);
//		
//}

#include "vecmat.h"
#include "3d.h"
#include "polyobj.h"
#include "texmap.h"

#include "hostage.h"
#include "powerup.h"

vms_angvec objpage_view_orient;
fix objpage_view_dist;

//this is bad to have the extern, but this snapshot stuff is special
extern int polyobj_lighting;


//canvas set
//	Type is optional.  If you pass -1, type is determined, else type is used, and id is not xlated through ObjId.
draw_robot_picture(int id, vms_angvec *orient_angles, int type)
{

	if (id >= Num_total_object_types)
		return;

	if ( type == -1 )	{
		type = ObjType[id];		// Find the object type, given an object id.
		id = ObjId[id];	// Translate to sub-id.
	}

	switch (type) {

		case OL_HOSTAGE:
			PIGGY_PAGE_IN(Vclip[Hostage_vclip_num[id]].frames[0]);
			gr_bitmap(0,0,&GameBitmaps[Vclip[Hostage_vclip_num[id]].frames[0].index]);
			break;

		case OL_POWERUP:
			if ( Powerup_info[id].vclip_num > -1 )	{
				PIGGY_PAGE_IN(Vclip[Powerup_info[id].vclip_num].frames[0]);
				gr_bitmap(0,0,&GameBitmaps[Vclip[Powerup_info[id].vclip_num].frames[0].index]);
			}
			break;

		case OL_PLAYER:
			draw_model_picture(Player_ship->model_num,orient_angles);		// Draw a poly model below
			break;

		case OL_ROBOT:
			draw_model_picture(Robot_info[id].model_num,orient_angles);	// Draw a poly model below
			break;

		case OL_CONTROL_CENTER:
		case OL_CLUTTER:
			draw_model_picture(id,orient_angles);
			break;
		default:
			//Int3();	// Invalid type!!!
			return;
	}

}

void redraw_current_object()
{
	grs_canvas * cc;

	cc = grd_curcanv;
	gr_set_current_canvas(ObjCurrent->canvas);
	draw_robot_picture(Cur_robot_type,&objpage_view_orient, -1);
	gr_set_current_canvas(cc);
}

void gr_label_box( int i)
{

	gr_clear_canvas(BM_XRGB(0,0,0));
	draw_robot_picture(i,&objpage_view_orient, -1);

//	char s[20];
//	sprintf( s, " %d ", i );
//	gr_clear_canvas( BM_XRGB(0,15,0) );
//	gr_set_fontcolor( CWHITE, BM_XRGB(0,15,0) );
//	ui_string_centered(  grd_curcanv->cv_bitmap.bm_w/2, grd_curcanv->cv_bitmap.bm_h/2, Description[i] );
}

int objpage_goto_first()
{
	int i;

	ObjectPage=0;
	for (i=0;  i<OBJS_PER_PAGE; i++ ) {
		gr_set_current_canvas(ObjBox[i]->canvas);
		if (i+ObjectPage*OBJS_PER_PAGE < Num_total_object_types ) {
			//gr_ubitmap(0,0, robot_bms[robot_bm_nums[ i+ObjectPage*OBJS_PER_PAGE ] ] );
			gr_label_box(i+ObjectPage*OBJS_PER_PAGE );
		} else
			gr_clear_canvas( CGREY );
	}

	return 1;
}

int objpage_goto_last()
{
	int i;

	ObjectPage=(Num_total_object_types)/OBJS_PER_PAGE;
	for (i=0;  i<OBJS_PER_PAGE; i++ )
	{
		gr_set_current_canvas(ObjBox[i]->canvas);
		if (i+ObjectPage*OBJS_PER_PAGE < Num_total_object_types )
		{
			//gr_ubitmap(0,0, robot_bms[robot_bm_nums[ i+ObjectPage*OBJS_PER_PAGE ] ] );
			gr_label_box(i+ObjectPage*OBJS_PER_PAGE );
		} else {
			gr_clear_canvas( CGREY );
		}
	}
	return 1;
}

static int objpage_goto_prev()
{
	int i;
	if (ObjectPage > 0) {
		ObjectPage--;
		for (i=0;  i<OBJS_PER_PAGE; i++ )
		{
			gr_set_current_canvas(ObjBox[i]->canvas);
			if (i+ObjectPage*OBJS_PER_PAGE < Num_total_object_types)
			{
				//gr_ubitmap(0,0, robot_bms[robot_bm_nums[ i+ObjectPage*OBJS_PER_PAGE ] ] );
				gr_label_box(i+ObjectPage*OBJS_PER_PAGE );
			} else {
				gr_clear_canvas( CGREY );
			}
		}
	}
	return 1;
}

static int objpage_goto_next()
{
	int i;
	if ((ObjectPage+1)*OBJS_PER_PAGE < Num_total_object_types) {
		ObjectPage++;
		for (i=0;  i<OBJS_PER_PAGE; i++ )
		{
			gr_set_current_canvas(ObjBox[i]->canvas);
			if (i+ObjectPage*OBJS_PER_PAGE < Num_total_object_types)
			{
				//gr_ubitmap(0,0, robot_bms[robot_bm_nums[ i+ObjectPage*OBJS_PER_PAGE ] ] );
				gr_label_box(i+ObjectPage*OBJS_PER_PAGE );
			} else {
				gr_clear_canvas( CGREY );
			}
		}
	}
	return 1;
}

int objpage_grab_current(int n)
{
	int i;

	if ( (n<0) || ( n>= Num_total_object_types) ) return 0;
	
	ObjectPage = n / OBJS_PER_PAGE;
	
	if (ObjectPage*OBJS_PER_PAGE < Num_total_object_types) {
		for (i=0;  i<OBJS_PER_PAGE; i++ )
		{
			gr_set_current_canvas(ObjBox[i]->canvas);
			if (i+ObjectPage*OBJS_PER_PAGE < Num_total_object_types)
			{
				//gr_ubitmap(0,0, robot_bms[robot_bm_nums[ i+ObjectPage*OBJS_PER_PAGE ] ] );
				gr_label_box(i+ObjectPage*OBJS_PER_PAGE );
			} else {
				gr_clear_canvas( CGREY );
			}
		}
	}

	Cur_robot_type = n;
	gr_set_current_canvas(ObjCurrent->canvas);
	//gr_ubitmap(0,0, robot_bms[robot_bm_nums[ Cur_robot_type ] ] );
	gr_label_box(Cur_robot_type);

	//objpage_display_name( Texture[Cur_robot_type]->filename );
	
	return 1;
}

int objpage_goto_next_object()
{
	int n;

	n = Cur_robot_type++;
 	if (n >= Num_total_object_types) n = 0;

	objpage_grab_current(n);

	return 1;

}

#define OBJBOX_X	(TMAPBOX_X)	//location of first one
#define OBJBOX_Y	(OBJCURBOX_Y - 24 )
#define OBJBOX_W	64
#define OBJBOX_H	64

#define DELTA_ANG 0x800

int objpage_increase_pitch()
{
	objpage_view_orient.p += DELTA_ANG;
	redraw_current_object();
	return 1;
}

int objpage_decrease_pitch()
{
	objpage_view_orient.p -= DELTA_ANG;
	redraw_current_object();
	return 1;
}

int objpage_increase_heading()
{
	objpage_view_orient.h += DELTA_ANG;
	redraw_current_object();
	return 1;
}

int objpage_decrease_heading()
{
	objpage_view_orient.h -= DELTA_ANG;
	redraw_current_object();
	return 1;
}

int objpage_increase_bank()
{
	objpage_view_orient.b += DELTA_ANG;
	redraw_current_object();
	return 1;
}

int objpage_decrease_bank()
{
	objpage_view_orient.b -= DELTA_ANG;
	redraw_current_object();
	return 1;
}

int objpage_increase_z()
{
	objpage_view_dist -= 0x8000;
	redraw_current_object();
	return 1;
}

int objpage_decrease_z()
{
	objpage_view_dist += 0x8000;
	redraw_current_object();
	return 1;
}

int objpage_reset_orient()
{
	objpage_view_orient.p = 0;
	objpage_view_orient.b = 0;
	objpage_view_orient.h = -0x8000;
	//objpage_view_dist = DEFAULT_VIEW_DIST;
	redraw_current_object();
	return 1;
}


// INIT TEXTURE STUFF

void objpage_init( UI_WINDOW *win )
{
	int i;

	//Num_total_object_types = N_polygon_models + N_hostage_types + N_powerup_types;
	//Assert (N_polygon_models < MAX_POLYGON_MODELS);
	//Assert (Num_total_object_types < MAX_OBJTYPE );
	//Assert (N_hostage_types < MAX_HOSTAGE_TYPES ); 
	//Assert (N_powerup_types < MAX_POWERUP_TYPES );
	// Assert (N_robot_types < MAX_ROBOTS);

	ui_add_gadget_button( win, OBJCURBOX_X + 00, OBJCURBOX_Y - 27, 30, 20, "<<", objpage_goto_prev );
	ui_add_gadget_button( win, OBJCURBOX_X + 32, OBJCURBOX_Y - 27, 30, 20, ">>", objpage_goto_next );

	ui_add_gadget_button( win, OBJCURBOX_X + 00, OBJCURBOX_Y - 54, 30, 20, "B", objpage_goto_first );
	ui_add_gadget_button( win, OBJCURBOX_X + 32, OBJCURBOX_Y - 54, 30, 20, "E", objpage_goto_last );

	ui_add_gadget_button( win, OBJCURBOX_X + 25, OBJCURBOX_Y + 62, 22, 13, "P-", objpage_decrease_pitch );
	ui_add_gadget_button( win, OBJCURBOX_X + 25, OBJCURBOX_Y + 90, 22, 13, "P+", objpage_increase_pitch );
	ui_add_gadget_button( win, OBJCURBOX_X + 00, OBJCURBOX_Y + 90, 22, 13, "B-", objpage_decrease_bank );
	ui_add_gadget_button( win, OBJCURBOX_X + 50, OBJCURBOX_Y + 90, 22, 13, "B+", objpage_increase_bank );
	ui_add_gadget_button( win, OBJCURBOX_X + 00, OBJCURBOX_Y + 76, 22, 13, "H-", objpage_decrease_heading );
	ui_add_gadget_button( win, OBJCURBOX_X + 50, OBJCURBOX_Y + 76, 22, 13, "H+", objpage_increase_heading );
	ui_add_gadget_button( win, OBJCURBOX_X + 00, OBJCURBOX_Y + 62, 22, 13, "Z+", objpage_increase_z );
	ui_add_gadget_button( win, OBJCURBOX_X + 50, OBJCURBOX_Y + 62, 22, 13, "Z-", objpage_decrease_z );
	ui_add_gadget_button( win, OBJCURBOX_X + 25, OBJCURBOX_Y + 76, 22, 13, "R", objpage_reset_orient );

	for (i=0;i<OBJS_PER_PAGE;i++)
		ObjBox[i] = ui_add_gadget_userbox( win, OBJBOX_X + (i/2)*(2+OBJBOX_W), OBJBOX_Y + (i%2)*(2+OBJBOX_H), OBJBOX_W, OBJBOX_H);

	ObjCurrent = ui_add_gadget_userbox( win, OBJCURBOX_X, OBJCURBOX_Y-5, 64, 64 );

	objpage_reset_orient();

	for (i=0;  i<OBJS_PER_PAGE; i++ )
	{
		gr_set_current_canvas(ObjBox[i]->canvas);
		if (i+ObjectPage*OBJS_PER_PAGE < Num_total_object_types)
		{
				//gr_ubitmap(0,0, robot_bms[robot_bm_nums[ i+ObjectPage*OBJS_PER_PAGE ] ] );
				gr_label_box(i+ObjectPage*OBJS_PER_PAGE );
		} else {
			gr_clear_canvas( CGREY );
		}
	}

// Don't reset robot_type when we return to editor.
//	Cur_robot_type = ObjectPage*OBJS_PER_PAGE;
	gr_set_current_canvas(ObjCurrent->canvas);
	//gr_ubitmap(0,0, robot_bms[robot_bm_nums[ Cur_robot_type ] ] );
	gr_label_box(Cur_robot_type);

	//ObjnameCanvas = gr_create_sub_canvas(&grd_curscreen->sc_canvas, OBJCURBOX_X , OBJCURBOX_Y + OBJBOX_H + 10, 100, 20);
	//gr_set_current_canvas( ObjnameCanvas );
	//gr_set_curfont( ui_small_font ); 
   //gr_set_fontcolor( CBLACK, CWHITE );
	//objpage_display_name( Texture[Cur_robot_type]->filename );

}

void objpage_close()
{
	//gr_free_sub_canvas(ObjnameCanvas);
}


// DO TEXTURE STUFF

void objpage_do()
{
	int i;

	for (i=0; i<OBJS_PER_PAGE; i++ )
	{
		if (ObjBox[i]->b1_clicked && (i+ObjectPage*OBJS_PER_PAGE < Num_total_object_types))
		{
			Cur_robot_type = i+ObjectPage*OBJS_PER_PAGE;
			gr_set_current_canvas(ObjCurrent->canvas);
			//gr_ubitmap(0,0, robot_bms[robot_bm_nums[ Cur_robot_type ] ] );
			gr_label_box(Cur_robot_type);
			//objpage_display_name( Texture[Cur_robot_type]->filename );
		}
	}
}
