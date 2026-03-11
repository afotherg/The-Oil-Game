/*********************************************************************/
/* This file holds routines to handle GEM interaction in Mega Game.  */
/*		started on	19 May      1986		     */
/*		this version	14 February 1987		     */
/*********************************************************************/
/* INCLUDE FILES						     */
#include	"header.h"
#include	"variab.h"
GRECT	desk;
/*********************************************************************/
/*  GSX UTILITY ROUTINES.					*/
/****************************************************************/
hidemouse()
{
if(! hidden)
	{
	graf_mouse(M_OFF,0x0L);
	hidden=1;
	}
}
/****************************************************************/
showmouse()
{
if(hidden)
	{
	graf_mouse(M_ON,0x0L);
	hidden=0;
	}
}
/****************************************************************/
/* set clipping rectangle					*/
/****************************************************************/
set_clip(x,y,w,h)
int x,y,w,h;
{
int clip[4];

clip[0] = x;
clip[1] = y;
clip[2] = x+w-1;
clip[3] = y+h-1;
vs_clip(vdi_handle,1,clip);
}
/****************************************************************/
/* close window							*/
/****************************************************************/
close_wind(wh)
int	wh;
{
int	index,x,y,w,h,i;
GRECT	*pos;

for(index = 0 ; window[index].handle != wh && index < 5 ; ++index)
	;
if(index == 5)
	return;
wind_get( wh,WF_WORKXYWH,&x,&y,&w,&h );
pos = window[index].pos;
pos->g_x = x;
pos->g_y = y;
pos->g_w = w;
pos->g_h = h;
if(growbox)
	graf_shrinkbox(0,0,0,0,x,y,w,h);
wind_close(wh);
wind_delete(wh);
redraw_desktop();
if( index == 4)
	window[index].handle = 0;
else
	{
	for( i = index ; i < 3 ; ++i)
		window[i] = window[i+1];
	window[3].handle = 0;
	}
}
/************************************************************************/
closeall()			/*close all windows*/
{
int	top,dummy;

do
	{
	wind_get(0,WF_TOP,&top,&dummy,&dummy,&dummy);
	if(top)
		close_wind(top);
	}
	while(top);
isevent = 0;			/* can't be an event */
}
/***************************************************************************/
ison( redraw )			/* checks to see if a certain window with  */
int	(*redraw)();		/* a certain redraw is on screen 	   */
{
int	i;

for( i = 0 ; i < 5 ; ++ i)
	{
	if( window[i].handle && window[i].redraw == redraw )
		return	( TRUE );
	}
return	( FALSE );
}
/*****************************************************************************/
int	open_window(x,y,w,h,kind,name,animate,
	redraw,arrowed,hslid,vslid,butpress,condred,gpp,p1,p2,pos, tree)
int	x,y,w,h,kind;
char	*name;
int	animate,(*redraw)(),(*arrowed)(),(*hslid)(),(*vslid)(),(*butpress)();
int	gpp,*condred;
real	p1,p2;
GRECT	*pos;
OBJECT	*tree;
{
int	wi_handle,index;

if( kind == 0 )			/* indicates VP window */
	index = 4;
else
	{
	for(index = 0 ; window[index].handle && index < 4 ; ++index )
		;
	if(index == 4)
		{			/*too many windows on screen*/
		if(!autoclose)
			return( 0 );
		close_wind(window[0].handle);
		index = 3;
		}
	}
wind_calc(0,kind,x,y,w,h,&x,&y,&w,&h);
wi_handle = wind_create(kind,x,y,w,h);
if( wi_handle < 0 )
	return 0;
wind_set(wi_handle, WF_NAME,name,0,0);
if(growbox)
	graf_growbox(0,0,0,0,x,y,w,h);
wind_open(wi_handle,x,y,w,h);
window[index].handle = wi_handle;
window[index].animate = animate;
window[index].redraw = redraw;
window[index].arrowed = arrowed;
window[index].hslid   = hslid;
window[index].vslid   = vslid;
window[index].butpress = butpress;
window[index].condred = condred;
window[index].gpp   = gpp;
window[index].p1    = p1;
window[index].p2    = p2;
window[index].pos   = pos;
window[index].kind  = kind;
window[index].tree  = tree;
return(wi_handle);
}
/*********************************************************************/
docondred()	/*do conditional redraws*/
{
int	index, wh, con;
GRECT	t;

for(index = 0 ; index < 5 ; ++index)
	{
	if(window[index].condred)
		{
		if(( con = *window[index].condred) && (wh=window[index].handle))
			{
			wind_update(1);
			hidemouse();
			scalechange = 0;
			wind_get(wh,WF_FIRSTXYWH,&t.g_x,&t.g_y,&t.g_w,&t.g_h);
			while(t.g_w && t.g_h)
				{
				if(rc_intersect(&desk,&t))
					{
					set_clip(t.g_x,t.g_y,t.g_w,t.g_h);
					(*window[index].redraw)(wh, con ,t.g_x,t.g_y,t.g_w,t.g_h,&window[index]);
					}
				wind_get(wh,WF_NEXTXYWH,&t.g_x,&t.g_y,&t.g_w,&t.g_h);
				}
			showmouse();
			wind_update(0);
			}
		}
	}
for(index = 0; index < 5 ; ++index)
	if(window[index].condred && window[index].handle)
		*(window[index].condred) = 0;
}
/***************************************************************************/
drawall()	/*redraws all window bits of windows with animate = TRUE*/
{
int	wh,index;
GRECT	t;

wind_update(1);
hidemouse();
for(index = 0 ; index < 5 ; ++index)
	{
	if((wh = window[index].handle) && window[index].animate)
		{
		wind_get(wh,WF_FIRSTXYWH,&t.g_x,&t.g_y,&t.g_w,&t.g_h);
		while(t.g_w && t.g_h)
			{
			if(rc_intersect(&desk,&t))
				{
				set_clip(t.g_x,t.g_y,t.g_w,t.g_h);
				(*window[index].redraw)(wh,1,t.g_x,t.g_y,t.g_w,t.g_h,&window[index]);
				}
			wind_get(wh,WF_NEXTXYWH,&t.g_x,&t.g_y,&t.g_w,&t.g_h);
			}
		}
	}
showmouse();
wind_update(0);
}
/****************************************************************/
/* find and redraw all clipping rectangles			*/
/****************************************************************/
doredraw(handle,x,y,w,h)
int	handle,x,y,w,h;
{
GRECT t1,t2;
int	index;

for(index = 0 ; window[index].handle != handle && index < 5; ++index)
	;
if( index == 5 )
	return;
set_clip(x,y,w,h);
wind_update(1);
hidemouse();
t2.g_x = x;
t2.g_y = y;
t2.g_w = w;
t2.g_h = h;
wind_get(handle,WF_FIRSTXYWH,&t1.g_x,&t1.g_y,&t1.g_w,&t1.g_h);
scalechange = 0;
while (t1.g_w && t1.g_h)
	{
	if (rc_intersect(&t2,&t1))
		{
		set_clip(t1.g_x,t1.g_y,t1.g_w,t1.g_h);
		(*window[index].redraw)(handle,2,t1.g_x,t1.g_y,t1.g_w,t1.g_h,&window[index]);
		}
		wind_get(handle,WF_NEXTXYWH,&t1.g_x,&t1.g_y,&t1.g_w,&t1.g_h);
	}
wind_update(0);
showmouse();
}
/****************************************************************/
/*	Initialise all of GEM-put on desktop window		*/
/****************************************************************/
initgem()
{
int	i,work_in[11],work_out[57];

vdi_handle = graf_handle(&gl_wchar,&gl_hchar,&gl_wbox,&gl_hbox);
for(i = 0 ; i < 10 ; work_in[i++] = 1);
work_in[10] = 2;
v_opnvwk(work_in,&vdi_handle,work_out);
devwidth		= work_out[0] + 1;
devheight		= work_out[1] + 1;
colour			= work_out[35];	/* colour? TRUE/FALSE */
numcols			= work_out[39];
vqt_attributes(vdi_handle,attrib);		/* is here for standard text?*/
vq_extnd( vdi_handle, 1, work_out );
nplanes			= work_out[4];

vst_load_fonts( vdi_handle, 0 );

wind_get(0,WF_WORKXYWH, &xdesk, &ydesk, &wdesk, &hdesk);	/*desktop size*/
for(i = 0 ; i < 4 ; ++i)
	window[i].handle = 0;

desk.g_x	= xdesk;
desk.g_y	= ydesk;
desk.g_w	= wdesk;
desk.g_h	= hdesk;
txtpnt		= newsbase;
graf_mouse(ARROW,0x0L);
}

	
/*
** dispatches all accessory tasks.
*/

multi()
{
int	event,msgbuff[16],mx,my,dummy,index,obj,pks,pkr;
int	finished = 0,nubuts,i;
long	t1;
int	currcomp = 0;

do
	{
	wind_update(1);
	wind_update(0);		/*So that everything stops for menus*/
	wind_get(0,WF_TOP,&topwindow,&dummy,&dummy,&dummy);
	if(topwindow)
		{
		for(index = 0 ; window[index].handle != topwindow ; ++index)
			;
		}
	event = evnt_multi(MU_MESAG | MU_BUTTON | MU_TIMER | MU_KEYBD,
			2,1,1,
			0,0,0,0,0,
			0,0,0,0,0,
			msgbuff,0,0,&mx,&my,&dummy,&pks,&pkr,&nubuts);

	if( event & MU_KEYBD )
		do_keyboard(pks, pkr, index, topwindow, &finished, &event, msgbuff);

	if (event & MU_MESAG)
	    switch (msgbuff[0])
		{
		case	MN_SELECTED:
			if(msgbuff[4] == QTMENU)
				finished = 1;
			else
				{
				domenu(msgbuff[4]);
				menu_tnormal(menu,msgbuff[3],1);
				}
			break;
	  	case WM_REDRAW:
			doredraw(msgbuff[3],msgbuff[4],msgbuff[5],msgbuff[6],msgbuff[7]);
			redraw_desktop();
			break;

	  	case WM_NEWTOP:
	  	case WM_TOPPED:
			for(i = 0 ; window[i].handle != msgbuff[3] && i < 5 ; ++i)
				;
			if( i != 5)
				{
				index = i;
				topwindow = msgbuff[3];
				wind_set( topwindow, WF_TOP, 0, 0, 0, 0);
				}
			break;

		case WM_SIZED:
		case WM_MOVED:
			if(msgbuff[6] < MIN_WIDTH) msgbuff[6] = MIN_WIDTH;
			if(msgbuff[7] < MIN_HEIGHT) msgbuff[7] = MIN_HEIGHT;
			wind_set(msgbuff[3],WF_CURRXYWH,msgbuff[4],msgbuff[5],msgbuff[6],msgbuff[7]);
			wind_get(msgbuff[3],WF_WORKXYWH,&xwork,&ywork,&wwork,&hwork);
			if(msgbuff[0] == WM_SIZED)
				send_redraw(msgbuff[3]);
			redraw_desktop();
	    		break;

		case WM_ARROWED:
			if( *window[index].arrowed )
				(*window[index].arrowed)(msgbuff[3],msgbuff[4],&window[index]);
			break;

		case	WM_CLOSED:
			close_wind(msgbuff[3]);
			wind_get(0,WF_TOP,&topwindow,&dummy,&dummy,&dummy);
			if(topwindow)
				{
				for(index = 0 ; window[index].handle != topwindow ; ++index)
					;
				}
			break;
		}

	if( event & MU_BUTTON )
		{
		if( ! wind_find(mx,my) )
			{
			obj = objc_find(desktop,0,10,mx,my);
			if( obj == 0 )
				see_if_map( mx,my );
			}
		if((wind_find(mx,my) == topwindow) && topwindow)
			if(window[index].butpress)
				(*window[index].butpress)(topwindow,mx,my,&window[index],nubuts);
		}
	econ_loop();
	drawbid = 3;
	docondred();		/*do conditional redraws*/
	}
	while(! finished );
}
/*****************************************************************************/
redraw_desktop()		/* redraws desktop rectangles*/
{
int	x,y,w,h;
PARMBLK	mapparm;

if( !deskmap )
	return;
wind_update(1);
hidemouse();
wind_get( 0 ,WF_FIRSTXYWH,&x,&y,&w,&h);
while(w && h)
	{
	mapparm.pb_x	= xdesk;
	mapparm.pb_y	= ydesk;
	mapparm.pb_w	= wdesk;
	mapparm.pb_h	= hdesk;
	mapparm.pb_xc	= x;
	mapparm.pb_yc	= y;
	mapparm.pb_wc	= w;
	mapparm.pb_hc	= h;
	mapdraw( &mapparm );
	objc_draw( desktop,0,10,x,y,w,h);
	wind_get( 0 ,WF_NEXTXYWH,&x,&y,&w,&h);
	}
showmouse();
wind_update(0);
}
/*****************************************************************************/
doclose()
{
int	i;

for(i = 0 ; i < 5 ; ++i)
	{
	if(window[i].handle)
		{
		wind_close(window[i].handle);
		wind_delete(window[i].handle);
		}
	}

vst_unload_fonts( vdi_handle, 0 );

v_clsvwk(vdi_handle);
appl_exit();
}
/************************************************************************/
/*			clear window					*/
/************************************************************************/
cls(wh)
int wh;

{
int	pxy[4];

wind_get(wh,WF_WORKXYWH,&xwork,&ywork,&wwork,&hwork);
vsf_interior(vdi_handle,2);
vsf_style(vdi_handle,8);
vsf_color(vdi_handle,WHITE);
pxy[0]=xwork;
pxy[1]=ywork;
pxy[2]=xwork+wwork-1;
pxy[3]=ywork+hwork-1;
vr_recfl(vdi_handle,pxy);
}
/*****************************************************************************/
send_redraw(wh)
int	wh;

{
int	msg[8],x,y,w,h;

wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
msg[0] = WM_REDRAW;
msg[1] = gl_apid;			/* As returned by appl_init */
msg[2] = 0;
msg[3] = wh;/* Handle of window to redraw */
msg[4] = x;
msg[5] = y;
msg[6] = w;
msg[7] = h;
appl_write(gl_apid, 16, &msg[0]);/* Use ADDR(msg) for portability */
}
/*****************************************************************************/
chdate()
{			/*Jan......................................Dec*/
static	int	save_days[]= {31,59,90,120,151,181, 212,243,273,304,334,365,999};
int	days[14];
register	int	i;
int	leap;

leap = (yy%4 == 0 && yy%100 !=0 && mm != 0);

for(i = 0 ; i < 13 ; ++i)
	{
	days[i] = save_days[i];
	if( leap  && i != 0 )
		++days[i];
	}
for(i = 0 ; day >= days[i] ; ++i)
	;
if(i == 12)
	{
	i = 0;
	day = 0;
	++yy;
	}
mm = i;
dd = 1 + day;
dd -=( (i > 0)?(days[i-1]):(0) );
sprintf(currdate,"%10s %2d, %d",months[mm],dd,yy);
TXTPNT(desktop,DDATE) = currdate;
chngobj(desktop, DDATE, NORMAL);
}
/****************************************************************************/
uniarrow(wh,mode,wind)
int	wh,mode;
WINDOWS	*wind;
{
if(mode == 6)
	wind->gpp = (wind->gpp + 15)%16;
if(mode == 7)
	wind->gpp = (wind->gpp + 1)%16;

if(mode == 3)
	wind->p2 = ((int)wind->p2 + 1)%4;
if(mode == 2)
	wind->p2 = ((int)wind->p2 + 3)%4;

send_redraw(wh);
}
/***************************************************************************/


