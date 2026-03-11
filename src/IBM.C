#include	"header.h"
#include	<dos.h>

Setpallete()		/*sic*/
{
}
/****************************************************************************/
rc_intersect(p1, p2)		/* compute inter of two rectangles	*/
GRECT		*p1, *p2;
{
int		tx, ty, tw, th;
tw = MIN(p2->g_x + p2->g_w, p1->g_x + p1->g_w);
th = MIN(p2->g_y + p2->g_h, p1->g_y + p1->g_h);
tx = MAX(p2->g_x, p1->g_x);
ty = MAX(p2->g_y, p1->g_y);
p2->g_x = tx;
p2->g_y = ty;
p2->g_w = tw - tx;
p2->g_h = th - ty;
return( (tw > tx) && (th > ty) );
}
/****************************************************************************/
Dosound()
{
}

read_key()
{
union	REGS	inregs, outregs;

outregs.h.al = 0;
inregs.h.ah = 11;
intdos(&inregs, &outregs);

if(outregs.h.al == 0)
	return(0);

inregs.h.ah = 7;
intdos(&inregs, &outregs);

if(outregs.h.al == 0)
	{
	inregs.h.ah = 7;
	intdos(&inregs,&outregs);
	return(0);
	}
return(outregs.h.al);
}
