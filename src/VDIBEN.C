/*	vdibind.c	7/18/84 - 4/19/85	jOHN gRANT		*/

#include "c:\gemapps\portab.h"
#include "dos.h"

extern	int	vdi();
extern	int	contrl[], intin[], intout[], ptsin[], ptsout[];

union	REGS	vdi_in, vdi_out;
struct	SREGS	vdi_segs;

typedef	struct{
int	*con;
int	*iin;
int	*pin;
int	*iot;
int	*pot;
	}PBLOCK;

PBLOCK	pblk;	


    WORD
v_pline( handle, count, xy )
WORD handle, count, xy[];
{
    i_ptsin( xy );

    contrl[0] = 6;
    contrl[1] = count;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
v_gtext( handle, x, y, string)
WORD handle, x, y;
BYTE *string;
{
    WORD i;

    ptsin[0] = x;
    ptsin[1] = y;
    i = 0;
    while (intin[i++] = *string++)
        ;

    contrl[0] = 8;
    contrl[1] = 1;
    contrl[3] = --i;
    contrl[6] = handle;
    vdi();
}

    WORD
v_fillarea( handle, count, xy)
WORD handle, count, xy[];
{
    i_ptsin( xy );

    contrl[0] = 9;
    contrl[1] = count;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
v_bar( handle, xy )
WORD handle, xy[];
{
    i_ptsin( xy );

    contrl[0] = 11;
    contrl[1] = 2;
    contrl[3] = 0;
    contrl[5] = 1;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
v_justified( handle, x, y, string, length, word_space, char_space)
WORD handle, x, y, length, word_space, char_space;
BYTE string[];
{
    WORD *intstr;

    ptsin[0] = x;
    ptsin[1] = y;
    ptsin[2] = length;
    ptsin[3] = 0;
    intin[0] = word_space;
    intin[1] = char_space;
    intstr = &intin[2];
    while (*intstr++ = *string++)
        ;

    contrl[0] = 11;
    contrl[1] = 2;
    contrl[3] = intstr - intin - 1;
    contrl[5] = 10;
    contrl[6] = handle;
    vdi();
}

    WORD
vst_height( handle, height, char_width, char_height, cell_width, cell_height )
WORD handle, height, *char_width, *char_height, *cell_width, *cell_height;
{
    ptsin[0] = 0;
    ptsin[1] = height;

    contrl[0] = 12;
    contrl[1] = 1;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    *char_width = ptsout[0];
    *char_height = ptsout[1];
    *cell_width = ptsout[2];
    *cell_height = ptsout[3];
}

    WORD
vst_rotation( handle, angle )
WORD handle, angle;
{
    intin[0] = angle;

    contrl[0] = 13;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vst_font( handle, font )
WORD handle, font;
{
    intin[0] = font;

    contrl[0] = 21;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    int
vst_unload_fonts( handle, select )
int handle, select;
{
    contrl[0] = 120;
    contrl[1] = contrl[3] = 0;
    contrl[6] = handle;
    vdi();
}

    WORD
vst_load_fonts( handle, select )
WORD handle, select;
{
    contrl[0] = 119;
    contrl[1] = contrl[3] = 0;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsf_interior( handle, style )
WORD handle, style;
{
    intin[0] = style;

    contrl[0] = 23;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsf_style( handle, index )
WORD handle, index;
{
    intin[0] = index;

    contrl[0] = 24;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsf_color( handle, index )
WORD handle, index;
{
    intin[0] = index;

    contrl[0] = 25;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vswr_mode( handle, mode )
WORD handle, mode;
{
    intin[0] = mode;

    contrl[0] = 32;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
v_opnvwk( work_in, handle, work_out )
WORD work_in[], *handle, work_out[];
{
int	i;
long	ad_pblk;
	
ad_pblk = (long *)&pblk;
vdi_in.x.bx = 0;		/* initialise VDI control block */
vdi_in.x.cx = 0x473;
vdi_in.x.dx = FP_OFF(ad_pblk);
vdi_segs.ds = FP_SEG(ad_pblk);



   for(i = 0 ; i < 11 ; ++i)
	intin[i] = work_in[i];

   contrl[0] = 100;
   contrl[1] = 0;
   contrl[2] = 6;
   contrl[3] = 11;
   contrl[4] = 45;
   contrl[5] = 0;
   contrl[6] = *handle;

	pblk.con = contrl;
	pblk.iin = intin;
	pblk.pin = ptsin;
	pblk.iot = intout;
	pblk.pot = ptsout;

   vdi();

   *handle = contrl[6];    
   for(i = 0 ; i < 45 ; ++i)
	   work_out[i] = intout[i];
   for(i = 45 ; i < 57 ; ++i)
	   work_out[i] = ptsout[i-45];
}

    WORD
v_clsvwk( handle )
WORD handle;
{
    contrl[0] = 101;
    contrl[1] = contrl[3] = 0;
    contrl[6] = handle;
    vdi();
}

    WORD
vr_recfl( handle, xy )
WORD handle, *xy;
{
    i_ptsin( xy );

    contrl[0] = 114;
    contrl[1] = 2;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
vs_clip( handle, clip_flag, xy )
WORD handle, clip_flag, xy[];
{
    i_ptsin( xy );
    intin[0] = clip_flag;

    contrl[0] = 129;
    contrl[1] = 2;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

/**************/
i_intin(ptr)
int	ptr[];
{
pblk.iin = ptr;
}
/**************/
i_intout(ptr)
int	ptr[];
{
pblk.iot = ptr;
}
/**************/
i_ptsout(ptr)
int	ptr[];
{
pblk.pot = ptr;
}
/**************/
i_ptsin(ptr)
int	ptr[];
{
pblk.pin = ptr;
}
/**************/
i_ptr(ptr)
WORD	*ptr;
{
contrl[7] = FP_OFF(ptr);
contrl[8] = FP_SEG(ptr);
}
/*************/
i_ptr2(ptr)
WORD	*ptr;
{
contrl[9] = FP_OFF(ptr);
contrl[10] = FP_SEG(ptr);
}
/*****************************************************************************/
    WORD
vsm_height( handle, height )
WORD handle, height;
{
    ptsin[0] = 0;
    ptsin[1] = height;

    contrl[0] = 19;
    contrl[1] = 1;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
    return( ptsout[1] );
}

    WORD
vsl_ends( handle, beg_style, end_style)
WORD handle, beg_style, end_style;
{
    intin[0] = beg_style;
    intin[1] = end_style;

    contrl[0] = 108;
    contrl[1] = 0;
    contrl[3] = 2;
    contrl[6] = handle;
    vdi();
}

    WORD
vst_point( handle, point, char_width, char_height, cell_width, cell_height )
WORD handle, point, *char_width, *char_height, *cell_width, *cell_height;
{
    intin[0] = point;

    contrl[0] = 107;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    *char_width = ptsout[0];
    *char_height = ptsout[1];
    *cell_width = ptsout[2];
    *cell_height = ptsout[3];
    return( intout[0] );
}

    WORD
vq_extnd( handle, owflag, work_out )
WORD handle, owflag, work_out[];
{
    i_intout( work_out );
    i_ptsout( work_out + 45 );
    intin[0] = owflag;

    contrl[0] = 102;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    i_intout( intout );
    i_ptsout( ptsout );
}

    WORD
v_ellpie( handle, xc, yc, xrad, yrad, sang, eang)
WORD handle, xc, yc, xrad, yrad, sang, eang;
{
    ptsin[0] = xc;
    ptsin[1] = yc;
    ptsin[2] = xrad;
    ptsin[3] = yrad;
    intin[0] = sang;
    intin[1] = eang;

    contrl[0] = 11;
    contrl[1] = 2;
    contrl[3] = 2;
    contrl[5] = 7;
    contrl[6] = handle;
    vdi();
}

    WORD
vst_effects( handle, effect )
WORD handle, effect;
{
    intin[0] = effect;

    contrl[0] = 106;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsm_type( handle, symbol )
WORD handle, symbol;
{
    intin[0] = symbol;

    contrl[0] = 18;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsf_udpat( handle, fill_pat, planes )
WORD handle, fill_pat[], planes;
{
    i_intin( fill_pat );

    contrl[0] = 112;
    contrl[1] = 0;
    contrl[3] = 16*planes;
    contrl[6] = handle;
    vdi();
    i_intin( intin );	
}

    WORD
vro_cpyfm( handle, wr_mode, xy, srcMFDB, desMFDB )
WORD handle, wr_mode, xy[], *srcMFDB, *desMFDB;
{
    intin[0] = wr_mode;
    i_ptr( srcMFDB );
    i_ptr2( desMFDB );
    i_ptsin( xy );

    contrl[0] = 109;
    contrl[1] = 4;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
vqt_width( handle, character, cell_width, left_delta, right_delta )
WORD handle, *cell_width, *left_delta, *right_delta;
BYTE character;
{
    intin[0] = character;

    contrl[0] = 117;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    *cell_width = ptsout[0];
    *left_delta = ptsout[2];
    *right_delta = ptsout[4];
    return( intout[0] );
}

    WORD
vsl_color( handle, index )
WORD handle, index;
{
    intin[0] = index;

    contrl[0] = 17;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vqt_attributes( handle, attributes )
WORD handle, attributes[];
{
    i_intout( attributes );
    i_ptsout( attributes + 6 );

    contrl[0] = 38;
    contrl[1] = contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_intout( intout );
    i_ptsout( ptsout );
}

    WORD
vsl_type( handle, style )
WORD handle, style;
{
    intin[0] = style;

    contrl[0] = 15;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsl_width( handle, width )
WORD handle, width;
{
    ptsin[0] = width;
    ptsin[1] = 0;

    contrl[0] = 16;
    contrl[1] = 1;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
    return( ptsout[0] );
}

vdi()
{
int86x(0xEF, &vdi_in, &vdi_out, &vdi_segs);
}
