/****************************************************************************/
/*        This file holds the keyboard interaction routines  		    */
/*        Started on January 22, 1988					    */
/****************************************************************************/
#include	"header.h"
#include	"variab.h"
#include	<ctype.h>

#define	ANY		0xffff

#define	UP_ARR		0x4800		/*  standard VDI scan codes */
#define	DN_ARR		0x5000
#define	LF_ARR		0x4b00
#define	RT_ARR		0x4d00
#define	SHIFT_UP_ARR	0x4838
#define	SHIFT_DN_ARR	0x5032
#define	SHIFT_LF_ARR	0x4b34
#define	SHIFT_RT_ARR	0x4d36
#define	CTRL_RT_ARR	0x7400
#define	CTRL_LF_ARR	0x7300
#define	CR		0x1c0d
#define	ESC		0x011b
#define	HOME		0x4700
#define	END		0x4f00
#define	SPACE		0x3920
#define	TAB		0x0f09
#define	SHIFT_TAB	0x0f00
#define	F1		0x3B00
#define	F2		0x3C00
#define	F3		0x3D00
#define	F4		0x3E00
#define	F5		0x3F00
/****************************************************************************/

do_keyboard(pks, pkr, index, topwindow, finished, event, msgbuff)
int	pks, pkr, index, topwindow, *finished, *event, *msgbuff;
{
int	i, dummy;
static	int	values[]={3,0,2,0,0,6,0,7};
int	x, y, w, h;
OBJECT	*tree;

tree	= window[index].tree;
if( topwindow )
	wind_get( topwindow, WF_WORKXYWH, &x, &y, &w, &h );
if( tree ) {
	tree->ob_x	= x;
	tree->ob_y	= y;
	}

hidemouse();
switch( pkr )
	{
	case	SPACE:
		if( ! topwindow )
			break;
		for(i = 0 ; window[i].handle != topwindow && i < 5 ; ++i)
				;
		do	{
			i = ++i % 5;
			}while( ! window[i].handle );
		msgbuff[3] = window[i].handle;
		*event |= MU_MESAG;
		msgbuff[0] = WM_TOPPED;
		break;

	case	ESC:
		if( ! topwindow || !(window[index].kind & CLOSER) )
			break;
		*event |= MU_MESAG;
		msgbuff[0] = WM_CLOSED;
		msgbuff[3] = topwindow;
		break;

	case	SHIFT_LF_ARR:
	case	SHIFT_DN_ARR:
	case	SHIFT_RT_ARR:
	case	SHIFT_UP_ARR:
		if( ! topwindow )
			break;
		
		graf_mkstate(&dummy, &dummy, &dummy, &pks);
		if( (pks & K_LSHIFT) && !(window[index].kind & MOVER) )
				break;
		if( (pks & K_RSHIFT) && !(window[index].kind & SIZER) )
				break;
		do_window_move(topwindow, event, msgbuff, pks);
		break;

	case	LF_ARR:
	case	DN_ARR:
	case	RT_ARR:
	case	UP_ARR:
		if( ! topwindow )
			break;
	
		{
		extern	int	audraw();
		
		if(window[index].redraw == audraw )
			{
			if( pkr == LF_ARR )
				{
				objc_offset(tree, AULEFT, &x, &y);
				(*window[index].butpress)( topwindow, 
				x+1, y+1, &window[index], 1 );
				}
			if( pkr == RT_ARR )
				{
				objc_offset(tree, AURIGHT, &x, &y);
				(*window[index].butpress)( topwindow,
				x+1, y+1, &window[index], 1 );
				}
			break;
			}
		if( !window[index].arrowed )
			break;
		(*window[index].arrowed)(topwindow, values[(pkr / 256)%10],&window[index]);
		break;
		}

	case	CTRL_LF_ARR:
	case	CTRL_RT_ARR:
		if(topwindow)
			activate_slider(topwindow, index, pkr, tree);
		break;

	case	CR:
		{
		extern	int	dutch_auction();

		if( topwindow ) {
			if( window[index].redraw == dutch_auction ) {
				butindutch( topwindow, x+1,
					y+3*attrib[7]+2*gl_hchar+1,
					&window[index] );
				break;
				}
			exit_window(topwindow, index);
			}
		break;
		}

	case	SHIFT_TAB:
	case	TAB:
		{
		int	dummy, pks;
		graf_mkstate(&dummy, &dummy, &dummy, &pks);
		if( pks & 3 )
			pkr	= SHIFT_TAB;		/* shift key pressed */
		r_button( topwindow, index, pkr );
		}
		break;

	case	F1:
	case	F2:
	case	F3:
	case	F4:
	case	F5:
		key_sel_icon( topwindow, index, pkr );
		break;

	default:
		key_menu(pkr, finished);
		break;
	}
showmouse();
}
/*****************************************************************************/
r_button(topwindow, index, pkr)
int	topwindow, index, pkr;
{
int	number, first, last, current, i, x, y;
OBJECT	*tree;

number = -1;
tree = window[index].tree;
if( !tree )
	return;
find_type(tree, 0, G_IBOX, ANY, DEFAULT, &number);
if(number  == -1)
	find_type(tree, 0, G_BOX, ANY, DEFAULT, &number);

if(number == -1)
	return;
first = tree[number].ob_head;
last  = tree[number].ob_tail;
current = first;
for(i = first ; i <= last ; ++i)
	if( tree[i].ob_state & SELECTED )
		current = i;

do	{
	if(pkr == TAB)
		{
		++current;
		if(current > last)
			current = first;
		}
	if(pkr == SHIFT_TAB)
		{
		--current;
		if(current < first)
			current = last;
		}
	}while(tree[current].ob_state & DISABLED);

objc_offset(tree, current, &x, &y);
if(window[index].butpress)
	(*window[index].butpress)(topwindow, x + 1, y + 1, &window[index], 1);
} 
/*****************************************************************************/
find_type(tree, num, type, state, flags, number)
OBJECT	*tree;
int	num, type, state, *number;
{
if( (((tree[num].ob_state & state) || (state == ANY)))	&& 
    (tree[num].ob_type == type)				&& 
    (((tree[num].ob_flags & flags) || (flags == ANY))) )
	{
	*number = num;
	return;
	}
if( tree[num].ob_next != -1 )
	if( tree[tree[num].ob_next].ob_tail != num )
		find_type(tree, tree[num].ob_next, type, state, flags, number);
if( tree[num].ob_head != -1)
	find_type( tree, tree[num].ob_head, type, state, flags, number );
}
/*****************************************************************************/
exit_window(topwindow, index)
int	topwindow, index;
{
int	number, x, y;

number = -1;
if( !window[index].tree )
	return;
find_type(window[index].tree, 0, G_BUTTON, ANY, DEFAULT, &number);
if( number == -1 )
	return;

objc_offset(window[index].tree, number, &x, &y);
if(window[index].butpress)
	(*window[index].butpress)(topwindow, x + 1, y + 1, &window[index], 1);
}
/*****************************************************************************/
activate_slider( topwindow, index, pkr, tree )
int topwindow, index, pkr;
OBJECT	*tree;
{
int	number, x, y;

if( !tree )
	return;
number = -1;
find_type(tree, 0, G_BOX, DISABLED, ANY, &number);
if( number == -1 )
	return;

objc_offset( tree, tree[number].ob_head, &x, &y);
if(window[index].butpress)
	(*window[index].butpress)(topwindow, x + 1, y + 1, &window[index], 1);
}
/*****************************************************************************/
keybd_slidebox(tree, box, sld)
OBJECT	*tree;
int	box, sld;
{
int	result, dummy[16], pks, pkr, pxy[10], i;
int	xp, yp, wp, hp, xc, yc, wc, hc, change;

set_clip(0, 0, devwidth, devheight);
objc_offset(tree, sld, &xc, &yc);
objc_offset(tree, box, &xp, &yp);
wc = tree[sld].ob_width;
hc = tree[sld].ob_height;
wp = tree[box].ob_width;
pxy[8] = pxy[6] = pxy[0] = xc;		pxy[9] = pxy[3] = pxy[1] = yc;
pxy[4] = pxy[2] = xc + wc;		pxy[7] = pxy[5] = yc + hc;
vswr_mode(vdi_handle, MD_XOR );
v_pline(vdi_handle, 5, pxy);

do	{
	result = evnt_multi(MU_TIMER | MU_KEYBD,
		2, 1, 1,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		dummy, 100, 0, dummy, dummy, dummy, &pks, &pkr, dummy);

	if( result & MU_KEYBD )
		{
		v_pline(vdi_handle, 5, pxy);
		switch( pkr )
			{
		case	CTRL_LF_ARR:
			change = MIN(tree[box].ob_width / 20, pxy[0] - xp);
			for(i = 0 ; i < 10 ; i += 2)
				pxy[i] -= change;
			break;

		case	CTRL_RT_ARR:
			change = MIN(tree[box].ob_width / 20, xp + wp - pxy[2]);
			for(i = 0 ; i < 10 ; i += 2)
				pxy[i] += change;
			break;
			}
		v_pline(vdi_handle, 5, pxy);
		}
	graf_mkstate(dummy, dummy, dummy, &pks);
	}while( pks & K_CTRL );
v_pline(vdi_handle, 5, pxy);
vswr_mode(vdi_handle, MD_REPLACE);
return( (int) ( ((long)(pxy[0] - xp) * 1000L) / (wp-wc)) );
}
/*****************************************************************************/

key_menu(pkr, finished)
int	pkr, *finished;
{
int	press, i, num, curr;
int	names, defns, dir, j;

menu_bar(menu, 0);	/* disable menu bar */
press = pkr & 0x00ff;	/* low byte holds ASCII code */
names = menu[menu[0].ob_head].ob_head;	/* this is the first in the list of menu TITLES - which are stored in the tree separately form the menu items */
num   = menu[names].ob_tail - menu[names].ob_head + 1;	/* number of menus */
defns = menu[menu[0].ob_tail].ob_head;	/* this is the first of the menu definition trees */
for(i = 2 ; i <= num ; ++i)
	{
	if( match(press, &menu[names + i]) )	/* check first letter */
		{
		while( 1 )
			{
			curr  = defns;	/* 'curr' is the obj. number of the menu ITEMS */
			for(j = 1 ; j < i ; ++j)
				curr = menu[curr].ob_next;
			menu[names + i].ob_state = SELECTED;
			objc_draw(menu, names + i, 10, 0, 0, devwidth, devheight);
			dir = interface(curr, finished);
			menu[names + i].ob_state = NORMAL;
			menu_bar(menu, 1);
			menu_bar(menu, 0);	/* redraw menu ! */
			switch( dir )
				{
				case	-1:
					--i;	/* 'i' is the obj. number of the menu tree NAME */
					if( i == 1 )
						i = num;
					break;

				case	0:
					goto finish;	/* ! */

				case	1:
					if(i++ == num)
						i = 2;
					break;
				}
			} 
		}
	}
finish:
	menu_bar(menu, 1);		/* re-enable menu bar */
}
/*****************************************************************************/

match(press, obj)
int	press;
OBJECT	*obj;
{
char	*string;

string = obj->ob_spec;

while(*string == ' ')
	++string;
if( tolower(press) == tolower(*string) )
	return(TRUE);
return(FALSE);
}
/*****************************************************************************/

interface(curr, finished)
int	curr, *finished;
{
int	first, last, key, new, old, oldstate, pxy[12], leave = 0;
int	off_x, off_y, x, y, w, h, dir, execute = 0;
char	*buffer;
MFDB	source, dest;

GETBUFFER( buffer )			/* machine dependent macro */

objc_offset(menu, curr, &x, &y);
w = menu[curr].ob_width;
h = menu[curr].ob_height;
x -= 1;	w += 2;	h += 1;		/* fiddle factors */

dest.fd_addr		= ( long )buffer;	/* save screen under menu */
dest.fd_w		= (w&0xFFF0)+16;
dest.fd_wdwidth		= dest.fd_w / 16;
dest.fd_stand		= 0;
dest.fd_h		= h;
dest.fd_nplanes		= nplanes;

source.fd_addr		= 0L;	/* the screen! */

pxy[0] = x;
pxy[1] = y;
pxy[2] = x + w - 1;
pxy[3] = y + h - 1;

pxy[4] = 0;
pxy[5] = 0;
pxy[6] = w - 1;
pxy[7] = h - 1;
set_clip(x, y, w, h);
vro_cpyfm( vdi_handle,S_ONLY,pxy, &source, &dest);

objc_draw(menu, curr, 10, x, y, w, h);
first = menu[curr].ob_head;	/*obj. number of first menu item*/
while(menu[first].ob_state == DISABLED)
	++first;
last  = menu[curr].ob_tail;	/* obj. number of last menu item */
while(menu[last].ob_state == DISABLED)
	--last;
old = new = first;
oldstate = menu[new].ob_state;
while( ! leave )
	{
	menu[old].ob_state  = oldstate;
	redraw_over(menu, old);

	oldstate = menu[new].ob_state;
	menu[new].ob_state |= SELECTED;
	objc_draw(menu, new, 0, 0, 0, devwidth, devheight);
	old = new;

	key = evnt_keybd();
	switch(key)
		{
		case	DN_ARR:
			do	{
				++new;
				if(new > last)
					new = first;
				}while(menu[new].ob_state == DISABLED);	
			break;

		case	UP_ARR:
			do	{
				--new;
				if(new < first)
					new = last;
				}while(menu[new].ob_state == DISABLED);
			break;

		case	LF_ARR:
			leave = 1;
			dir = -1;	/* previous menu */
			break;

		case	RT_ARR:
			leave = 1;
			dir = 1;	/* next menu */
			break;
	
		case	CR:
			execute = 1;
			leave = 1;
			dir = 0;
			break;

		case	ESC:
			leave = 1;
			dir = 0;
			break;

		case	HOME:
			new = first;
			break;

		case	END:
			new = last;
			break;
		}
	}

source = dest;		/* copy back raster area */
dest.fd_addr = 0L;	/* the screen! */

pxy[0] = 0;
pxy[1] = 0;
pxy[2] = w - 1;
pxy[3] = h - 1;

pxy[4] = x;
pxy[5] = y;
pxy[6] = x + w - 1;
pxy[7] = y + h - 1;
set_clip(x, y, w, h);
vro_cpyfm( vdi_handle,S_ONLY,pxy, &source, &dest);

if( execute )
	{
	menu[new].ob_state = oldstate;
	if(new == QTMENU)
		*finished = 1;
	else
		domenu(new);
	oldstate = menu[new].ob_state;
	}

menu[new].ob_state = oldstate;
return( dir );
}
/*****************************************************************************/
redraw_over(tree, obj)		/* redraws an object in overwrite mode */
OBJECT	*tree;
int	obj;
{
int	off_x, off_y;
int	pxy[4];

objc_offset(tree, obj, &off_x, &off_y);
pxy[0] = off_x;
pxy[1] = off_y;	
pxy[2] = menu[obj].ob_width + pxy[0] - 1;
pxy[3] = menu[obj].ob_height + pxy[1] - 1;
vsf_interior(vdi_handle,2);
vsf_style(vdi_handle,8);
vsf_color(vdi_handle,WHITE);
vs_clip(vdi_handle, 1, pxy);
vr_recfl(vdi_handle, pxy);
objc_draw(tree, obj, 0, pxy[0], pxy[1], pxy[2], pxy[3]);
}
/*****************************************************************************/

do_window_move(topwindow, event, msgbuff, key_state)
int	topwindow, *event, *msgbuff, key_state;
{
int	dummy[16], pks, pkr, result;
int	x, y, w, h, pxy[10];

set_clip(0, 0, devwidth, devheight);
wind_get(topwindow, WF_CURRXYWH, &x, &y, &w, &h);
pxy[8] = pxy[6] = pxy[0] = x;		pxy[9] = pxy[3] = pxy[1] = y;
pxy[4] = pxy[2] = x + w;		pxy[7] = pxy[5] = y + h;
vswr_mode(vdi_handle, MD_XOR );
v_pline(vdi_handle, 5, pxy);

do	{
	result = evnt_multi(MU_TIMER | MU_KEYBD,
		2, 1, 1,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		dummy, 100, 0, dummy, dummy, dummy, dummy, &pkr, dummy);
	graf_mkstate(dummy, dummy, dummy, &pks);
	if( pkr && (result & MU_KEYBD) )
		{
		v_pline(vdi_handle, 5, pxy);	/* erase previous line */
		if(pks == K_LSHIFT)
			move_wind(pkr, pxy);	/* get new co-ords */
		if(pks == K_RSHIFT)
			size_wind(pkr, pxy);
		v_pline(vdi_handle, 5, pxy);	/* draw new line */
		}
	}while( pks & (K_LSHIFT | K_RSHIFT) );	
v_pline(vdi_handle, 5, pxy);		/* erase box */
vswr_mode(vdi_handle, MD_REPLACE );

if(key_state == K_LSHIFT)	/* set up GEM parameters in message pipe*/
	msgbuff[0] = WM_MOVED;
else
	msgbuff[0] = WM_SIZED;
*event |= MU_MESAG;
msgbuff[3] = topwindow;
msgbuff[4] = pxy[0];
msgbuff[5] = pxy[1];
msgbuff[6] = pxy[2] - pxy[0];
msgbuff[7] = pxy[5] - pxy[1];
}
/*****************************************************************************/

move_wind(pkr, pxy)
int	pkr, pxy[];
{
int	change, i;

switch( pkr )
	{
	case	SHIFT_UP_ARR:
		change = MIN(devheight / 20, pxy[1] - ydesk);
		for(i = 1 ; i < 10 ; i += 2)
			pxy[i] -= change;
		break;

	case	SHIFT_LF_ARR:
		change = MIN(devwidth / 40, pxy[0] - xdesk);
		for(i = 0 ; i < 10 ; i += 2)
			pxy[i] -= change;
		break;

	case	SHIFT_RT_ARR:
		change = MIN(devwidth / 40, xdesk + wdesk - 10 - pxy[0]);
		for(i = 0 ; i < 10 ; i += 2)
			pxy[i] += change;
		break;

	case	SHIFT_DN_ARR:
		change = MIN(devheight / 20, ydesk + hdesk - 10 - pxy[1]);
		for(i = 1 ; i < 10 ; i += 2)
			pxy[i] += change;
		break;
	}
}
/*****************************************************************************/

size_wind(pkr, pxy)
int	pkr, pxy[];
{
int	change;

pxy[2] = MIN(pxy[2], devwidth - 1);
pxy[5] = MIN(pxy[5], devheight - 1 );
pxy[4] = MIN(pxy[4], devwidth - 1);
pxy[7] = MIN(pxy[7], devheight - 1);
switch( pkr )
	{
	case	SHIFT_UP_ARR:
		change = MIN(devheight / 20, pxy[5] - MIN_HEIGHT - pxy[3]);
		pxy[5] -= change;
		pxy[7] -= change;
		break;

	case	SHIFT_LF_ARR:
		change = MIN(devwidth / 40, pxy[4] - MIN_WIDTH - pxy[6]);
		pxy[2] -= change;
		pxy[4] -= change;
		break;

	case	SHIFT_RT_ARR:
		change = MIN(devwidth / 40, xdesk + wdesk - pxy[2]);
		pxy[2] += change;
		pxy[4] += change;
		break;

	case	SHIFT_DN_ARR:
		change = MIN(devheight / 20, ydesk + hdesk - pxy[5]);
		pxy[5] += change;
		pxy[7] += change;
		break;
	}
}

/*
**	This routine is to select an icon from the children of of an invisible
**	shadowed box.
*/
key_sel_icon( topwindow, index, pkr )
int	topwindow, index, pkr;
{
OBJECT	*tree;
int	selected, number, x, y, i;

tree = window[index].tree;
if( !tree )
	return;
number	= -1;
find_type(tree, 0, G_IBOX, SHADOWED, ANY, &number);
if(number  == -1)
	return;
selected = tree[number].ob_head + ((pkr-F1)>>8);
if( selected > tree[number].ob_tail )
	return;

objc_offset(tree, selected, &x, &y);
if(window[index].butpress)
	(*window[index].butpress)(topwindow, x + 1, y + 1, &window[index], 1);
}


