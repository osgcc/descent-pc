/*
THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.  
COPYRIGHT 1993-1998 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/
/*
 * $Source: f:/miner/source/main/editor/rcs/medsel.h $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:34:28 $
 * 
 * rountines stipped from med.c for segment selection
 * 
 * $Log: medsel.h $
 * Revision 2.0  1995/02/27  11:34:28  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.2  1993/12/17  12:18:35  john
 * Moved selection stuff out of med.c
 * 
 * Revision 1.1  1993/12/17  09:29:51  john
 * Initial revision
 * 
 * 
 */



#ifndef _MEDSEL_H
#define _MEDSEL_H

extern void sort_seg_list(int n_segs,short *segnumlist,vms_vector *pos);

#endif

