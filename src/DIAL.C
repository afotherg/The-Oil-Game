/****************************************************************************/
/*        This file holds dialog drawing and interaction routines	    */
/*        Which we couldn't think where else to put			    */
/****************************************************************************/
#include	"header.h"
#include	"variab.h"

/****************************************************************************/
DRAWFUNC( mprices )
{
int	dummy;

if( vp_cont[wind->gpp] >= 0 ) {
	close_wind( wh );
	return;
	}
wind_get(wh,WF_WORKXYWH,&(prices->ob_x),&(prices->ob_y),&dummy,&dummy);
OBS(prices,STATENM) = statename[wind->gpp];

str((int)(sellmin[0][wind->gpp] * 100),TXTPNT(prices,MINP),4);
SLIDER(prices,MINPRSLD,MINPRBOX,0,100,sellmin[0][wind->gpp]);
if(mode == 2)
	objc_draw(prices,0,10,x,y,w,h);
if(mode == 3)
	{
	objc_draw(prices,MINPRBOX,10,x,y,w,h);
	objc_draw(prices,MINP,10,x,y,w,h);
	}
}
/****************************************************************************/
BUTFUNC( butinprices )
{
int		x,y,w,h,ret,obj, dummy, keybd_state;

wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
prices->ob_x = x;	prices->ob_y = y;
switch(obj = objc_find(prices,0,10,mx,my))
	{
	case	MINPRSLD:
		graf_mkstate(&dummy, &dummy, &dummy, &keybd_state);
		if(keybd_state & K_CTRL)
			ret = keybd_slidebox( prices, MINPRBOX, MINPRSLD);
		else
			ret = graf_slidebox( prices, MINPRBOX, MINPRSLD, 0);
		sellmin[0][wind->gpp] = (ret*99.0)/1000.0;
		drawprice = 3;
		break;

	case	MINPRUP:
		sellmin[0][wind->gpp] += 0.1;
		if(sellmin[0][wind->gpp] > 99.9)
			sellmin[0][wind->gpp] = 99.9;
		drawprice = 3;
		break;

	case	MINPRDN:
		sellmin[0][wind->gpp] -= 0.1;
		if(sellmin[0][wind->gpp] < 0.0)
			sellmin[0][wind->gpp] = 0.0;
		drawprice = 3;
		break;

	}
}
/****************************************************************************/
DRAWFUNC( chout )		/*does spec of well dialogue print screen*/
{
int	dummy,plotnumber,plotsel,statesel;

wind_get(wh,WF_WORKXYWH,&(chdial->ob_x),&(chdial->ob_y),&dummy,&dummy);
plotsel				= wind->gpp;
statesel			= STATE(plotsel);
plotnumber			= TOPLOT(plotsel);
if( vp_cont[statesel] >= 0 ) {
	close_wind( wh );
	return;
	}
(chdial + CHSTNM)->ob_spec	= statename[statesel];
formnum(TXTPNT(chdial,OUTACT),plot[plotsel].actual);
NUMTEXT( chdial, COSTPROD, total_cost( plot[plotsel].actual,
	plot[plotsel].type, 0));
str(plotnumber,(chdial+CHPLOTNM)->ob_spec,2);
NUMTEXT(chdial,AMTTO,plot[plotsel].output);
NUMTEXT(chdial,OUTACT,plot[plotsel].actual);
SLIDER(chdial,CHSLD,CHBOX,0,
	2*param_wells[plot[plotsel].type].capacity,plot[plotsel].output);

if(mode == 2)
	objc_draw(chdial,0,10,x,y,w,h);
else
	{
	objc_draw(chdial,OUTACT,2,x,y,w,h);
	objc_draw(chdial,COSTPROD,2,x,y,w,h);
	}
}
/***************************************************************************/
BUTFUNC( butinch )		/*button pressed in well set up*/
{
int		x,y,w,h, plotsel;

wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
chdial->ob_x = x;	chdial->ob_y = y;
plotsel  = wind->gpp;
switch(objc_find(chdial,0,10,mx,my))
	{
	case	CHSLD:
		slider(chdial,CHBOX,CHSLD,&(plot[plotsel].output),0.0,2*param_wells[plot[plotsel].type].capacity,AMTTO,0,x,y,w,h);
		break;
	}
}
/************************************************************************/
DRAWFUNC( menpros )			/*p2=image*/
{
int	dummy,pl,p ,s,imageno,i;
static	int	types[] = {PROSTP0, PROSTP1, PROSTP2, PROSTP3};

wind_get(wh,WF_WORKXYWH,&(prosdial->ob_x),&(prosdial->ob_y),&dummy,&dummy);

imageno	= wind->p2;
p    	= wind->gpp;
s	= STATE(p);
pl	= TOPLOT(p);

for(i = 0 ; i < 4 ; ++i)
	prosdial[prostypes[i]].ob_state = NORMAL;
prosdial[prostypes[imageno]].ob_state = SELECTED;
str((int)prostime[imageno],TXTPNT(prosdial,PROSTIME),2);
formnum(TXTPNT(prosdial,PROSCOST),proscost[imageno]);
str(pl, prosdial[PROSPLOT].ob_spec, 2);
prosdial[PROSST].ob_spec	= statename[s];
TXTPNT( prosdial, PROSBY)	= st[types[imageno]];
if(mode == 2)
	objc_draw(prosdial,0,10,x,y,w,h);
else
	{	
	objc_draw(prosdial,PROSTIME,1,x,y,w,h);
	objc_draw(prosdial,PROSSURR,1,x,y,w,h);
	objc_draw(prosdial,PROSCOST,1,x,y,w,h);
	objc_draw(prosdial,PROSBY,1,x,y,w,h);
	}
}
/***************************************************************************/
BUTFUNC( butinpros )		/*button pressed in well set up*/
{
int	x,y,w,h,obj,i;

wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
prosdial->ob_x  = x;	prosdial->ob_y = y;
switch((obj = objc_find(prosdial,0,10,mx,my)))
	{
	case	PROSGO:		/*submit spec*/
		(prosdial+obj)->ob_state = SELECTED;
		objc_draw(prosdial,obj,0,x,y,w,h);
		if(obj == PROSGO)
			startpros((int)wind->p2,0,wind->gpp);
		(prosdial+obj)->ob_state = NORMAL|SHADOWED;
		close_wind(wh);
		break;
	default:
		for(i = 0 ; i < 4 && prostypes[i] != obj ; ++i)
			;
		if(i == 4 || i == (int)wind->p2)
			break;
		wind->p2 = i;
	}
drawpros = 1;
}
/****************************************************************************/
startpros(prostype, c, p ) /*start prospecting-reset flags in plot*/
int	prostype, c, p;
{
JOBS	job;
extern	int	doprosjob();

if( c == 0 && cashchange( 0, RDCASH) < proscost[prostype] )
	{
	form_alert( 1, st[CANTPROS] );
	return;
	}
	
plot[p].flags[c]	&= 0xf8; 
plot[p].flags[c]	|= 2*prostype + 1; /*prospecting taking place*/
plot[p].prosres		= -1;	       /*not valid*/
job.company		= c;
job.delay		= (c == 0)?(30*prostime[prostype]):(1);
job.type		= PROSJOB;
job.p1			= p;
job.p2			= prostype;
cashchange( c, ACOST, proscost[prostype] );
addjob(&job);
}
/*********************************************************************/
av_reserves(amount)
real	*amount;
{
int	i;
real	sum=0.0;

for(i = 0 ; i < 256 ; ++i)
	sum += plot[i].reserves;
*amount = sum / 256;
}
/********************************************************************/
doprosjob(job)			/*do prospecting job*/
JOBS	*job;
{
int	c, p, type, state, plotnumber;
real	amount;
char	cost[20], string[150];

state			= STATE( p );
plotnumber		= TOPLOT( p );

c	= job->company;
p	= job->p1;
type	= job->p2;
prosjob( c, p, type, &amount );
state		= STATE( p );
plotnumber	= TOPLOT( p );
plot[p].flags[c]	&= 0xf7;		/*prospecting finished*/
plot[p].prosres		= MIN( amount/1000.0, 32000);
if( c == 0 )
	{
	formnum(cost, amount);
	sprintf(string, st[PROSFIN], statename[state],plotnumber,cost);
	addmsg( string );
	}
drawstates	= 1;
}

/*
**
**
*/
prosjob( c, p, type, amount )
int	c, p, type;
real	*amount;
{
int	randacc;
static	int	accuracy[] = {50,30,10,1};

randacc			= (rand()%accuracy[type]) * ((rand()<16384)?(-1):(1));
*amount			= plot[p].reserves*(randacc+100.0)/100.0;

if( rand()%100 < accuracy[type] )
	if( *amount > 1.0)
		*amount = 0.0;
	else
		av_reserves( amount );

}
/****************************************************************************/
DRAWFUNC( inshow )
{
int	dummy;
MSG	*msgpoint;

wind_get(wh,WF_WORKXYWH,&(intray->ob_x),&(intray->ob_y),&dummy,&dummy);
if(mode == 2)
	objc_draw(intray,0,10,x,y,w,h);
msgpoint = (firstmsg.next)?firstmsg.next:&firstmsg;
puttext(msgpoint,x,y,w,h);
}
/*****************************************************************************/
BUTFUNC( butinshow )
{
int	x,y,w,h,obj;
MSG	*lastmsg,*msg,*msgpoint;

msgpoint = (firstmsg.next)?firstmsg.next:&firstmsg;
wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
intray->ob_x  = x;	intray->ob_y = y;
switch((obj = objc_find(intray,0,10,mx,my)))
	{
	case	INBOTTOM:
		(intray+INBOTTOM)->ob_state = SELECTED|SHADOWED;
		objc_draw(intray,INBOTTOM,0,x,y,w,h);
		if(msgpoint->next)	/*put current message to end*/
			{
			firstmsg.next = msgpoint->next;
			lastmsg = msgpoint->next;
			while(lastmsg->next)
				lastmsg = lastmsg->next;
			lastmsg->next = msgpoint;
			msgpoint->next = 0L;
			msgpoint = firstmsg.next;
			puttext(msgpoint,x,y,w,h);
			}
		(intray+INBOTTOM)->ob_state = NORMAL|SHADOWED;
		objc_draw(intray,INBOTTOM,0,x,y,w,h);
		break;

	case	INEXIT:		/*submit spec*/
		(intray+obj)->ob_state = SELECTED|SHADOWED;
		objc_draw(intray,obj,0,x,y,w,h);
		(intray+obj)->ob_state = NORMAL|SHADOWED;
		close_wind(wh);
		break;

	case	INNEXT:
		(intray+INNEXT)->ob_state = SELECTED|SHADOWED;
		objc_draw(intray,INNEXT,0,x,y,w,h);
		if(firstmsg.next)/*ie there is at least 1 msg*/
			{
			firstmsg.next = msgpoint->next;
			msg = msgpoint;
			free(msgpoint);
			intraydraw = 1;	/*cond redraw*/
			}
		(intray+obj)->ob_state = NORMAL|SHADOWED;
		objc_draw(intray,INNEXT,0,x,y,w,h);
	}
}
/*****************************************************************************/
puttext(msgpoint,x,y,w,h)
int	x,y,w,h;
MSG	*msgpoint;
{
TXTPNT(intray,INDATE) = &msgpoint->date[0];
objc_draw(intray,INDATE,0,x,y,w,h);

TXTPNT(intray,INTEXT0) = msgpoint->text0;
TXTPNT(intray,INTEXT1) = msgpoint->text1;
TXTPNT(intray,INTEXT2) = msgpoint->text2;
objc_draw(intray, INTEXTBX,2,x,y,w,h);
}
/****************************************************************************/
DRAWFUNC( audraw )
{
int	p, s, dummy, i, width1, width2;
real	max;
BIDTYPE	*pt;
long	currtime, totwidth;
static	int auboxes[] = {AU0BOX,AU1BOX,AU2BOX,AU3BOX};
static	int auslds[]  = {AU0SLD,AU1SLD,AU2SLD,AU3SLD};
static	int auprcs[]  = {AU0PRC,AU1PRC,AU2PRC,AU3PRC};

wind_get(wh,WF_WORKXYWH,&(auction->ob_x),&(auction->ob_y),&dummy,&dummy);

p	= bidtype.plot;
if( p < 0 )
	{
	close_wind(wh);
	return;
	}

pt	= &bidtype;
max	= MAX(MAX(pt->offer[0],pt->offer[1]),MAX(pt->offer[2],pt->offer[3]));

if(max > 0.75 * pt->max)
	pt->max *= 2.1;
if(max < 0.25 * pt->max)
	pt->max *= 0.55;
max = pt->max;

totwidth	= auction[AUSURR].ob_width;
time(&currtime);
width1			= MAX(0,((SELLTIME - (currtime - pt->time)) * totwidth) / SELLTIME);
width2			= totwidth - width1;
auction[AUSLD].ob_width	= width1 - 1; 
auction[AUBOX].ob_width	= width2 - 1;
auction[AUSLD].ob_x	= 1;
auction[AUBOX].ob_x	= width1;

if(mode == 3)
	{
	objc_draw(auction,AUBOX,2,x,y,w,h);
	objc_draw(auction,AUSLD,2,x,y,w,h);
	}
NUMTEXT(auction, AURESPRC, pt->reserve);
for(i = 0 ; i < 4 ; ++i)
	{
	NUMTEXT(auction,auprcs[i],pt->offer[i]);	
	SLIDER(auction,auslds[i],auboxes[i],0,max,pt->offer[i]);
	if(mode != 2)
		{
		objc_draw(auction,auboxes[i],10,x,y,w,h);
		objc_draw(auction,auprcs[i],10,x,y,w,h);
		}
	}
s		= STATE(p);
str( TOPLOT( p ), TXTPNT(auction,AUPLOT), 2);
TXTPNT(auction,AUSELLER) = statename[s];
if(mode == 2)
	objc_draw(auction,0,10,x,y,w,h);
}
/******************************************************************************/
BUTFUNC( butinau )
{
int	x,y,w,h,obj;

if(bidtype.plot == -1)
	return;
wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
auction->ob_x  = x;		auction->ob_y = y;
switch((obj = objc_find(auction,0,10,mx,my)))
	{
	case	AULEFT:
		bidtype.offer[0] *= 0.96;
		break;
	case	AURIGHT:
		bidtype.offer[0] *= 1.06;
		break;
	}
}
/******************************************************************************/
startbid( p )
int	p;
{
int	i;
long	t1;

if(bidtype.plot != -1)
	return( 0 );
bidtype.plot	= p;
bidtype.dutch	= ( rand()%2)?(0):(1);
bidtype.reserve	= (tot_land/numauc) * (1 + 2*bidtype.dutch);
for(i = 0 ; i < 4 ; ++i)
	bidtype.offer[i] = 0.9001 * bidtype.reserve;
bidtype.max	= 2.0 * bidtype.reserve;
time( &t1 );
bidtype.time	= t1;

return( 1 );
}
/**************************************************************************/
bidtime()
{
int	maxcomp,c, p;
long	t1;
real	max = 0.0;
char	string[150];
char	cost[20];

p	= bidtype.plot;
if(p == -1)
	return;
al_get_bids( p );			/* get bids from AI companies for plot p */
plot[p].flags[0]	&= 0x7f;	/* turn off hammer */
if( bidtype.dutch )
	{
	if( bidtype.reserve < 0.9*tot_land/numauc )
		{
		bidtype.plot	= -1;
		return;
		}
	for( c = 0 ; c < 4 ; ++c )
		{
		if( bidtype.offer[c] < 0.0 )
			{
			max	= bidtype.reserve;
			plot[p].owner	= c;
			if( c != 0 )
				startloan( c, MAXLOANTERM, max );
			cashchange( c, LAND, max );
			plot[p].val_land	= max/10000.0;
			formnum( cost, max);
			sprintf(string,st[PLOTSOLD], TOPLOT(p),
				statename[STATE(p)],comp[c],cost);
			addmsg( string );
			tot_land	+= 2.0 * max;
			++numauc;
			bidtype.plot	= -1;		/* auction finished */
			drawstates	= 1;
			updt_sell_menu();
			break;
			}
		}
	bidtype.reserve *= 0.98;
	return;
	}

time( &t1 );
if((t1 - bidtype.time) > SELLTIME)
	{
	for(c = 0 ; c < 4 ; ++c )
		{
		if(bidtype.offer[c] > max)
			{
			max	= bidtype.offer[c];
			maxcomp	= c;
			}
		}
	if(max > bidtype.reserve)
		{
		plot[p].owner	= maxcomp;
		if( maxcomp != 0 )
			startloan( maxcomp, MAXLOANTERM, max );
		cashchange(maxcomp,LAND,max);
		plot[p].val_land	= max/10000.0;
		formnum(cost,max);
		sprintf(string,st[PLOTSOLD], TOPLOT(p),
			statename[STATE(p)],comp[maxcomp],cost);
		addmsg( string );
		tot_land	+= 2.0 * max;
		updt_sell_menu();
		}
	else
		{
		formnum(cost,bidtype.reserve);
		sprintf(string, st[NOTSOLD], TOPLOT(p),
			statename[STATE(p)],cost);
		addmsg( string );
		}
	++numauc;
	bidtype.plot			= -1;
	drawstates			= 1;
	}
}
/*************************************************************************/
DRAWFUNC( dutch_auction )		/* handles Dutch auction */
{
int	state, x1, y1, w1, h1;
char	text[100],text1[100];

if( bidtype.plot < 0 )
	{
	close_wind( wh );
	return;
	}

wind_get( wh, WF_WORKXYWH, &x1, &y1, &w1, &h1);
if( mode == 2 )
	cls( wh );
sprintf( text, st[STATEWSTATE], statename[STATE(bidtype.plot)]);
v_gtext( vdi_handle, x1, y1+gl_hchar, text );
sprintf( text, st[PLOTWNUM], TOPLOT(bidtype.plot) );
v_gtext( vdi_handle, x1, y1+2*gl_hchar, text);

vst_height( vdi_handle, 3*attrib[7], &state, &state, &state, &state );

formnum( text, bidtype.reserve);
sprintf( text1, "$%s", text);
v_gtext( vdi_handle, x1, y1+3*attrib[7]+2*gl_hchar, text1 );

v_gtext( vdi_handle, x1, y1+6*attrib[7]+2*gl_hchar, st[DUTCHPRESS] );
vst_height( vdi_handle, attrib[7], &state, &state, &state, &state );

}
/*****************************************************************************/
BUTFUNC( butindutch )
{
int	x,y,w,h, buyx, buyy;

if( bidtype.plot < 0 )
	return;				/* too late */

wind_get( wh, WF_WORKXYWH, &x, &y, &w, &h);
buyx	= x;
buyy	= y+3*attrib[7]+2*gl_hchar;
if( mx>buyx && my>buyy && mx<buyx+10*gl_wchar && my<buyy+3*gl_hchar)
	bidtype.offer[0] = -1.0;		/* signals buy */
}
/*****************************************************************************/

