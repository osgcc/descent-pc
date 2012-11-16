/*
 * $Source: f:/miner/source/main/editor/rcs/ksegsel.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:35:33 $
 *
 * Functions for selecting segments
 *
 * $Log: ksegsel.c $
 * Revision 2.0  1995/02/27  11:35:33  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.12  1994/08/25  21:57:02  mike
 * IS_CHILD stuff.
 * 
 * Revision 1.11  1994/05/23  14:48:35  mike
 * make current segment be add segment.
 * 
 * Revision 1.10  1993/12/06  19:33:43  yuan
 * Fixed autosave stuff so that undo restores Cursegp and
 * Markedsegp
 * 
 * Revision 1.9  1993/12/02  12:39:37  matt
 * Removed extra includes
 * 
 * Revision 1.8  1993/11/12  13:08:17  yuan
 * Fixed warning for concave segment so it appears after any
 * "less important" diagnostic messages.
 * 
 * Revision 1.7  1993/11/05  17:32:49  john
 * added funcs
 * .,
 * 
 * Revision 1.6  1993/11/01  09:53:18  mike
 * Write functions get_next_segment and get_previous_segment.
 * 
 * Revision 1.5  1993/10/31  18:06:56  mike
 * Only set_view_target_from_segment if in that mode.
 * 
 * Revision 1.4  1993/10/28  15:01:09  matt
 * Mucked with update flags
 * 
 * Revision 1.3  1993/10/14  18:07:47  mike
 * Change use of CONNECTIVITY to MAX_SIDES_PER_SEGMENT
 * 
 * Revision 1.2  1993/10/14  11:47:34  john
 * *** empty log message ***
 * 
 * Revision 1.1  1993/10/13  18:53:39  john
 * Initial revision
 * 
 *
 */

#pragma off (unreferenced)
static char rcsid[] = "$Id: ksegsel.c 2.0 1995/02/27 11:35:33 john Exp $";
#pragma on (unreferenced)

#include <string.h>

#include "inferno.h"
#include "editor.h"


// ---------------------------------------------------------------------------------------
// Select previous segment.
//	If there is a connection on the side opposite to the current side, then choose that segment.
// If there is no connecting segment on the opposite face, try any segment.
void get_previous_segment(int curseg_num, int curside,int *newseg_num, int *newside)
{
	int     s;

	*newseg_num = curseg_num;

	if (IS_CHILD(Segments[curseg_num].children[Side_opposite[curside]]))
		*newseg_num = Segments[curseg_num].children[Side_opposite[curside]];
	else        // no segment on opposite face, connect to anything
		for (s=0; s<MAX_SIDES_PER_SEGMENT; s++)
			if ((s != curside) && IS_CHILD(Segments[curseg_num].children[s]))
				*newseg_num = Segments[curseg_num].children[s];

	// Now make Curside point at the segment we just left (unless we couldn't leave it).
	if (*newseg_num != curseg_num)
		*newside = find_connect_side(&Segments[curseg_num],&Segments[*newseg_num]);
	else
		*newside = curside;
}


// --------------------------------------------------------------------------------------
// Select next segment.
//	If there is a connection on the current side, then choose that segment.
// If there is no connecting segment on the current side, try any segment.
void get_next_segment(int curseg_num, int curside, int *newseg_num, int *newside)
{
	int	s;

	if (IS_CHILD(Segments[curseg_num].children[curside])) {

		*newseg_num = Segments[curseg_num].children[Curside];

		// Find out what side we came in through and favor side opposite that
		*newside = Side_opposite[find_connect_side(&Segments[curseg_num],&Segments[*newseg_num])];

		// If there is nothing attached on the side opposite to what we came in (*newside), pick any other side
		if (!IS_CHILD(Segments[*newseg_num].children[*newside]))
			for (s=0; s<MAX_SIDES_PER_SEGMENT; s++)
				if ((Segments[*newseg_num].children[s] != curseg_num) && IS_CHILD(Segments[*newseg_num].children[s]))
					*newside = s;
	} else {
		*newseg_num = curseg_num;
		*newside = curside;
	}

}

// ---------- select current segment ----------
int SelectCurrentSegForward()
{
	int	newseg_num,newside;

	get_next_segment(Cursegp-Segments,Curside,&newseg_num,&newside);

	if (newseg_num != Cursegp-Segments) {
		Cursegp = &Segments[newseg_num];
		Curside = newside;
		Update_flags |= UF_ED_STATE_CHANGED;
		if (Lock_view_to_cursegp)
			set_view_target_from_segment(Cursegp);

		med_create_new_segment_from_cursegp();
		mine_changed = 1;
	}

	return 1;
}

// -------------------------------------------------------------------------------------
int SelectCurrentSegBackward()
{
	int	newseg_num,newside;

	get_previous_segment(Cursegp-Segments,Curside,&newseg_num,&newside);

	Cursegp = &Segments[newseg_num];
	Curside = newside;

	if (Lock_view_to_cursegp)
		set_view_target_from_segment(Cursegp);
	Update_flags |= UF_ED_STATE_CHANGED;
	mine_changed = 1;
	med_create_new_segment_from_cursegp();

	return 1;
}


// ---------- select next/previous side on current segment ----------
int SelectNextSide()
{
	if (++Curside >= MAX_SIDES_PER_SEGMENT)
		Curside = 0;
	Update_flags |= UF_ED_STATE_CHANGED;
	mine_changed = 1;
	return 1;
}

int SelectPrevSide()
{
	if (--Curside < 0)
		Curside = MAX_SIDES_PER_SEGMENT-1;
	Update_flags |= UF_ED_STATE_CHANGED;
	mine_changed = 1;
	return 1;
}

//  ---------- Copy current segment and side to marked segment and side ----------

int CopySegToMarked()
{
   autosave_mine(mine_filename);
   strcpy(undo_status[Autosave_count], "Mark Segment UNDONE.");
	Markedsegp = Cursegp;
	Markedside = Curside;
	Update_flags |= UF_ED_STATE_CHANGED;
	mine_changed = 1;
	return 1;
}

// ---------- select absolute face on segment ----------

int SelectBottom()
{
	Curside = WBOTTOM;
	Update_flags |= UF_ED_STATE_CHANGED;
	mine_changed = 1;
	return 1;
}

int SelectFront()
{
	Curside = WFRONT;
	Update_flags |= UF_ED_STATE_CHANGED;
	mine_changed = 1;
	return 1;
}

int SelectTop()
{
	Curside = WTOP;
	Update_flags |= UF_ED_STATE_CHANGED;
	mine_changed = 1;
	return 1;
}

int SelectBack()
{
	Curside = WBACK;
	Update_flags |= UF_ED_STATE_CHANGED;
	mine_changed = 1;
	return 1;
}

int SelectLeft()
{
	Curside = WLEFT;
	Update_flags |= UF_ED_STATE_CHANGED;
	mine_changed = 1;
	return 1;
}

int SelectRight()
{
	Curside = WRIGHT;
	Update_flags |= UF_ED_STATE_CHANGED;
	mine_changed = 1;
	return 1;
}

