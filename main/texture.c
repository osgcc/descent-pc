/*
 * $Source: f:/miner/source/main/editor/rcs/texture.c $
 * $Revision: 2.0 $
 * $Author: john $
 * $Date: 1995/02/27 11:34:50 $
 * 
 * Texture map assignment.
 * 
 * $Log: texture.c $
 * Revision 2.0  1995/02/27  11:34:50  john
 * Version 2.0! No anonymous unions, Watcom 10.0, with no need
 * for bitmaps.tbl.
 * 
 * Revision 1.13  1994/08/04  19:13:15  matt
 * Changed a bunch of vecmat calls to use multiple-function routines, and to
 * allow the use of C macros for some functions
 * 
 * Revision 1.12  1994/08/03  10:31:56  mike
 * Texture map propagation without uv assignment.
 * 
 * Revision 1.11  1994/07/14  19:36:34  yuan
 * Tuning texture slides.
 * 
 * Revision 1.10  1994/07/14  19:29:08  yuan
 * Fixed sliding.
 * 
 * Revision 1.9  1994/07/14  14:43:06  yuan
 * Added 3x rotation.
 * 
 * Revision 1.8  1994/07/14  11:12:42  yuan
 * Made sliding 3x more sensitive
 * 
 * Revision 1.7  1994/07/14  10:49:56  yuan
 * Made texture rotation 3x finer
 * 
 * Revision 1.6  1994/02/14  12:06:00  mike
 * change segment data structure.
 * 
 * Revision 1.5  1993/12/06  13:26:52  mike
 * Make rotation and sliding work for triangulated sides.
 * 
 * Revision 1.4  1993/12/04  17:18:46  mike
 * Add tiling functions, set_default.
 * 
 * Revision 1.3  1993/12/03  18:39:12  unknown
 * Add texture map sliding, allow to work on triangulated sides.
 * 
 * Revision 1.2  1993/11/30  17:06:09  mike
 * Texture map functions.
 * 
 * Revision 1.1  1993/11/29  16:00:57  mike
 * Initial revision
 * 
 * 
 */


#pragma off (unreferenced)
static char rcsid[] = "$Id: texture.c 2.0 1995/02/27 11:34:50 john Exp $";
#pragma on (unreferenced)

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#include "inferno.h"
#include "segment.h"
#include "seguvs.h"
#include "editor.h"

#include "fix.h"
#include "mono.h"
#include "error.h"
#include "kdefs.h"

//	-----------------------------------------------------------
int	TexFlipX()
{
	uvl	uvcenter;
	fix	rotmat[4];

	compute_uv_side_center(&uvcenter, Cursegp, Curside);

	//	Create a rotation matrix
	rotmat[0] = -0xffff;
	rotmat[1] = 0;
	rotmat[2] = 0;
	rotmat[3] = 0xffff;

	rotate_uv_points_on_side(Cursegp, Curside, rotmat, &uvcenter);

 	Update_flags |= UF_WORLD_CHANGED;

	return	1;
}

//	-----------------------------------------------------------
int	TexFlipY()
{
	uvl	uvcenter;
	fix	rotmat[4];

	compute_uv_side_center(&uvcenter, Cursegp, Curside);

	//	Create a rotation matrix
	rotmat[0] = 0xffff;
	rotmat[1] = 0;
	rotmat[2] = 0;
	rotmat[3] = -0xffff;

	rotate_uv_points_on_side(Cursegp, Curside, rotmat, &uvcenter);

 	Update_flags |= UF_WORLD_CHANGED;

	return	1;
}

//	-----------------------------------------------------------
int DoTexSlideLeft(int value)
{
	side	*sidep;
	uvl	duvl03;
	fix	dist;
	byte	*vp;
	int	v;

	vp = Side_to_verts[Curside];
	sidep = &Cursegp->sides[Curside];

	dist = vm_vec_dist(&Vertices[Cursegp->verts[vp[3]]], &Vertices[Cursegp->verts[vp[0]]]);
	dist *= value;
	if (dist < F1_0/(64*value))
		dist = F1_0/(64*value);

	duvl03.u = fixdiv(sidep->uvls[3].u - sidep->uvls[0].u,dist);
	duvl03.v = fixdiv(sidep->uvls[3].v - sidep->uvls[0].v,dist);

	for (v=0; v<4; v++) {
		sidep->uvls[v].u -= duvl03.u;
		sidep->uvls[v].v -= duvl03.v;
	}

	Update_flags |= UF_WORLD_CHANGED;

	return	1;
}

int TexSlideLeft()
{
	return DoTexSlideLeft(3);
}

int TexSlideLeftBig()
{
	return DoTexSlideLeft(1);
}

//	-----------------------------------------------------------
int DoTexSlideUp(int value)
{
	side	*sidep;
	uvl	duvl03;
	fix	dist;
	byte	*vp;
	int	v;

	vp = Side_to_verts[Curside];
	sidep = &Cursegp->sides[Curside];

	dist = vm_vec_dist(&Vertices[Cursegp->verts[vp[1]]], &Vertices[Cursegp->verts[vp[0]]]);
	dist *= value;

	if (dist < F1_0/(64*value))
		dist = F1_0/(64*value);

	duvl03.u = fixdiv(sidep->uvls[1].u - sidep->uvls[0].u,dist);
	duvl03.v = fixdiv(sidep->uvls[1].v - sidep->uvls[0].v,dist);

	for (v=0; v<4; v++) {
		sidep->uvls[v].u -= duvl03.u;
		sidep->uvls[v].v -= duvl03.v;
	}

	Update_flags |= UF_WORLD_CHANGED;

	return	1;
}

int TexSlideUp()
{
	return DoTexSlideUp(3);
}

int TexSlideUpBig()
{
	return DoTexSlideUp(1);
}


//	-----------------------------------------------------------
int DoTexSlideDown(int value)
{
	side	*sidep;
	uvl	duvl03;
	fix	dist;
	byte	*vp;
	int	v;

	vp = Side_to_verts[Curside];
	sidep = &Cursegp->sides[Curside];

	dist = vm_vec_dist(&Vertices[Cursegp->verts[vp[1]]], &Vertices[Cursegp->verts[vp[0]]]);
	dist *= value;
	if (dist < F1_0/(64*value))
		dist = F1_0/(64*value);

	duvl03.u = fixdiv(sidep->uvls[1].u - sidep->uvls[0].u,dist);
	duvl03.v = fixdiv(sidep->uvls[1].v - sidep->uvls[0].v,dist);

	for (v=0; v<4; v++) {
		sidep->uvls[v].u += duvl03.u;
		sidep->uvls[v].v += duvl03.v;
	}

	Update_flags |= UF_WORLD_CHANGED;

	return	1;
}

int TexSlideDown()
{
	return DoTexSlideDown(3);
}

int TexSlideDownBig()
{
	return DoTexSlideDown(1);
}

//	-----------------------------------------------------------
//	Compute the center of the side in u,v coordinates.
void compute_uv_side_center(uvl *uvcenter, segment *segp, int sidenum)
{
	int	v;
	side	*sidep = &segp->sides[sidenum];

	uvcenter->u = 0;
	uvcenter->v = 0;

	for (v=0; v<4; v++) {
		uvcenter->u += sidep->uvls[v].u;
		uvcenter->v += sidep->uvls[v].v;
	}

	uvcenter->u /= 4;
	uvcenter->v /= 4;
}


//	-----------------------------------------------------------
//	rotate point *uv by matrix rotmat, return *uvrot
void rotate_uv_point(uvl *uvrot, fix *rotmat, uvl *uv, uvl *uvcenter)
{
	uvrot->u = fixmul(uv->u - uvcenter->u,rotmat[0]) + fixmul(uv->v - uvcenter->v,rotmat[1]) + uvcenter->u;
	uvrot->v = fixmul(uv->u - uvcenter->u,rotmat[2]) + fixmul(uv->v - uvcenter->v,rotmat[3]) + uvcenter->v;
}

//	-----------------------------------------------------------
//	Compute the center of the side in u,v coordinates.
void rotate_uv_points_on_side(segment *segp, int sidenum, fix *rotmat, uvl *uvcenter)
{
	int	v;
	side	*sidep = &segp->sides[sidenum];
	uvl	tuv;

	for (v=0; v<4; v++) {
		rotate_uv_point(&tuv, rotmat, &sidep->uvls[v], uvcenter);
		sidep->uvls[v] = tuv;
	}
}

//	-----------------------------------------------------------
//	ang is in 0..ffff = 0..359.999 degrees
//	rotmat is filled in with 4 fixes
void create_2d_rotation_matrix(fix *rotmat, fix ang)
{
	fix	sinang, cosang;

	fix_sincos(ang, &sinang, &cosang);

	rotmat[0] = cosang;
	rotmat[1] = sinang;
	rotmat[2] = -sinang;
	rotmat[3] = cosang;
	
}


//	-----------------------------------------------------------
int DoTexRotateLeft(int value)
{
	uvl	uvcenter;
	fix	rotmat[4];

	compute_uv_side_center(&uvcenter, Cursegp, Curside);

	//	Create a rotation matrix
	create_2d_rotation_matrix(rotmat, -F1_0/value);

	rotate_uv_points_on_side(Cursegp, Curside, rotmat, &uvcenter);

 	Update_flags |= UF_WORLD_CHANGED;

	return	1;
}

int TexRotateLeft()
{
	return DoTexRotateLeft(192);
}

int TexRotateLeftBig()
{
	return DoTexRotateLeft(64);
}


//	-----------------------------------------------------------
int DoTexSlideRight(int value)
{
	side	*sidep;
	uvl	duvl03;
	fix	dist;
	byte	*vp;
	int	v;

	vp = Side_to_verts[Curside];
	sidep = &Cursegp->sides[Curside];

	dist = vm_vec_dist(&Vertices[Cursegp->verts[vp[3]]], &Vertices[Cursegp->verts[vp[0]]]);
	dist *= value;
	if (dist < F1_0/(64*value))
		dist = F1_0/(64*value);

	duvl03.u = fixdiv(sidep->uvls[3].u - sidep->uvls[0].u,dist);
	duvl03.v = fixdiv(sidep->uvls[3].v - sidep->uvls[0].v,dist);

	for (v=0; v<4; v++) {
		sidep->uvls[v].u += duvl03.u;
		sidep->uvls[v].v += duvl03.v;
	}

	Update_flags |= UF_WORLD_CHANGED;

	return	1;
}

int TexSlideRight()
{
	return DoTexSlideRight(3);
}

int TexSlideRightBig()
{
	return DoTexSlideRight(1);
}

//	-----------------------------------------------------------
int DoTexRotateRight(int value)
{
	uvl	uvcenter;
	fix	rotmat[4];

	compute_uv_side_center(&uvcenter, Cursegp, Curside);

	//	Create a rotation matrix
	create_2d_rotation_matrix(rotmat, F1_0/value);

	rotate_uv_points_on_side(Cursegp, Curside, rotmat, &uvcenter);

 	Update_flags |= UF_WORLD_CHANGED;

	return	1;
}

int TexRotateRight()
{
	return DoTexRotateRight(192);
}

int TexRotateRightBig()
{
	return DoTexRotateRight(64);
}

//	-----------------------------------------------------------
int	TexSelectActiveEdge()
{
	return	1;
}

//	-----------------------------------------------------------
int	TexRotate90Degrees()
{
	uvl	uvcenter;
	fix	rotmat[4];

	compute_uv_side_center(&uvcenter, Cursegp, Curside);

	//	Create a rotation matrix
	create_2d_rotation_matrix(rotmat, F1_0/4);

	rotate_uv_points_on_side(Cursegp, Curside, rotmat, &uvcenter);

 	Update_flags |= UF_WORLD_CHANGED;

	return	1;
}

//	-----------------------------------------------------------
int	TexSetDefault()
{
	Num_tilings = 1;

	Stretch_scale_x = F1_0;
	Stretch_scale_y = F1_0;

	assign_default_uvs_to_side(Cursegp,Curside);

	Update_flags |= UF_GAME_VIEW_CHANGED;
	return	1;
}

//	-----------------------------------------------------------
int	TexIncreaseTiling()
{

	Num_tilings++;
	assign_default_uvs_to_side(Cursegp, Curside);
	Update_flags |= UF_GAME_VIEW_CHANGED;

	return	1;
}

//	-----------------------------------------------------------
int	TexDecreaseTiling()
{

	if (--Num_tilings < 1)
		Num_tilings = 1;

	assign_default_uvs_to_side(Cursegp, Curside);
	Update_flags |= UF_GAME_VIEW_CHANGED;

	return	1;
}


//	direction = -1 or 1 depending on direction
int	TexStretchCommon(int direction)
{
	fix	*sptr;

	if ((Curedge == 0) || (Curedge == 2))
		sptr = &Stretch_scale_x;
	else
		sptr = &Stretch_scale_y;

	*sptr += direction*F1_0/64;

	if (*sptr < F1_0/16)
		*sptr = F1_0/16;

	if (*sptr > 2*F1_0)
		*sptr = 2*F1_0;

	stretch_uvs_from_curedge(Cursegp, Curside);

	editor_status("Stretch scale = %7.4f, use Set Default to return to 1.0", f2fl(*sptr));

	Update_flags |= UF_GAME_VIEW_CHANGED;
	return	1;

}

int	TexStretchDown(void)
{
	return TexStretchCommon(-1);

}

int	TexStretchUp(void)
{
	return TexStretchCommon(1);

}

