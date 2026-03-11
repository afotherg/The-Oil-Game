/***************************************************************************/
/*              This file holds routines concerned with jobs	 	   */
/*		Started   	10 November 1986.			   */
/***************************************************************************/
#include	"header.h"
#include	"variab.h"
/***************************************************************************/
int	easy	= 1;			/* easy mode selected 		   */
/******************************************************************************
**	This routine is responsible for prompting the user on the required
**	level, and setting the variable easy accordingly
******************************************************************************/
getlevel()
{
int	boxx, boxy, boxw, boxh, key, pxy[4];

hidemouse();
boxw	= 15 * gl_wchar;
boxh	=  6 * gl_hchar;
pxy[0]	= boxx = xdesk+(wdesk-boxw)/2;
pxy[1]	= boxy = ydesk+(hdesk-boxh)/2;
pxy[2]	= pxy[0] + boxw-1;
pxy[3]	= pxy[1] + boxh-1;
set_clip( xdesk, ydesk, wdesk, hdesk );
vsf_interior( vdi_handle, 0 ); 
v_bar( vdi_handle, pxy );

v_gtext( vdi_handle, boxx+2*gl_wchar, boxy+2*gl_hchar, st[ENTERLEVEL] );
v_gtext( vdi_handle, boxx+2*gl_wchar, boxy+4*gl_hchar, st[EASYMODE] );
v_gtext( vdi_handle, boxx+2*gl_wchar, boxy+5*gl_hchar, st[HARDMODE] );
while( 1 )
	{
	key	= read_key();

	if( key == '1' )
		{
		easy	= 1;
		break;
		}
	if( key == '2' )
		{
		easy	= 0;
		break;
		}
	}
showmouse();
}
/*****************************************************************************/
addmsg( string)
char	*string;
{
MSG	*oldmsg,*newmsg;
int	i,j,sp;
char	*txtstart,temp,newtext[51],space;
extern	int	inshow();

audio();				/* give audio feedback */
oldmsg = firstmsg.next;
firstmsg.next	= newmsg = (MSG*)malloc((int)sizeof(MSG));
if(newmsg == 0L)
	{
	form_alert(1, st[NOMEM] );
	return;
	}
newmsg->next = oldmsg;

txtstart = string;
for(i = 0 ; i < 3 ; ++i)
	{
	for(j = 0 ; j < 51 ; ++j)
		newtext[j] = 0;
	for(j = 0 ; j < 50 ; ++j)
		{
		temp = txtstart[j];
		if(temp == '\0')
			break;
		if(temp != ' ')
			newtext[j] = temp;
		if(temp == ' ')
			{
			for(sp = 1 ; space = txtstart[j + sp] ; ++sp)
				if( space == ' ' || sp > 30)
					break;
			if(j + sp > 50)
				break;
			else
				newtext[j] = ' ';
			}
		}
	txtstart += j;
	space_strip( newtext );
	switch(i){
		case	0:
		strcpy( &newmsg->text0[0], newtext);
		break;
		case	1:
		strcpy( &newmsg->text1[0], newtext);
		break;
		case	2:
		strcpy( &newmsg->text2[0], newtext);
		break;
		}
	}
strcpy(&newmsg->date[0],currdate);
if( autoin && !ison( inshow) )
	domenu( INSHOW );
intraydraw = 1;		/* conditional redraw for in tray*/
}
/***************************************************************************/
addjob(job)
JOBS	*job;
{
JOBS	*lastjob,*newjob,*prevjob;

if( easy && job->company == 0  && (	job->type == PIPEJOB  ||
					job->type == WELLJOB  || 
					job->type == STOREJOB ||
					job->type == PROSJOB  ))
	job->delay = 1;			/* easy has no delays */

prevjob = lastjob = &firstjob;

while( (job->delay > lastjob->delay) && lastjob->next)
	{
	prevjob = lastjob;
	lastjob = lastjob->next;
	}
newjob = (JOBS*)malloc((int)sizeof(JOBS));
if(newjob == 0L)
	{
	form_alert(1, st[NOMEM]);
	return;
	}
newjob->next	= prevjob->next;
prevjob->next	= newjob;
newjob->company	= job->company;
newjob->delay	= job->delay;
newjob->type	= job->type;
newjob->p1	= job->p1;
newjob->p2	= job->p2;
newjob->p3	= job->p3;
newjob->p4	= job->p4;
newjob->p5	= job->p5;
}
/***************************************************************************/
handlejobs()
{
JOBS	*job,*prevjob;
int	company;

prevjob = &firstjob;
job = firstjob.next;
while(job)
	{
	job->delay -= 3;
	company = job->company;
	if(job->delay <= 0)
		{
		switch( job->type )
			{
			case	PIPEJOB:
				buildpipe( job );
				break;
			case	WELLJOB:
				buildwell( job );
				break;
			case	STOREJOB:
				buildstore( job );
				break;
			case	PROSJOB:
				doprosjob( job );
				break;
			case	LOANJOB:
				dolnjob( job );
				break;
			case	SELLJOB:
				biddo( job );
				break;
			case	EXPOILJOB:
				expoildo( job );
				break;
			case	FOUNDOUT:
				foundout( job );
			}
		if(job->delay <= 0)	/*not a job to continue*/
			{
			prevjob->next = job->next;
			free(job);
			job = prevjob;
			}
		}
	prevjob = job;
	job = job->next;
	}
}
/*****************************************************************************/
DRAWFUNC( specwell )		/*does spec of well dialogue print screen*/
{
real	price,cap;
int	dummy,plotnumber,plotsel,statesel,type,i,mwc;

mwc = wind->p2;
wind_get(wh,WF_WORKXYWH,&(welldial->ob_x),&(welldial->ob_y),&dummy,&dummy);

type		= whichwell[ mwc ];
price		= pricewell(type,(int)wind->p1);
plotsel		= wind->gpp;
statesel	= STATE(plotsel);
plotnumber	= TOPLOT(plotsel);
cap		= param_wells[type].capacity;

NUMTEXT(welldial,WELLCOST,price);
str(plotnumber,OBS(welldial,NUMPLOT),2);
str((int)wind->p1,TXTPNT(welldial,TIMENUM),2);
for(i = 0 ; i < 4 ; ++i)
	(welldial + welltypes[i])->ob_state = NORMAL;
(welldial + welltypes[(int)wind->p2])->ob_state = SELECTED;
OBS(welldial,NMSTATE) = statename[statesel];
SLIDER(welldial,TIMESLD,BOXTIME, MINTMWELL ,MAXTMWELL, wind->p1);
OBS(welldial,BULL1) = param_wells[type].bull1;
OBS(welldial,BULL2) = param_wells[type].bull2;
OBS(welldial,BULL3) = param_wells[type].bull3;
NUMTEXT(welldial,LABCOST,param_wells[type].lab_cost*labfact);
NUMTEXT(welldial,WELLCAP,param_wells[type].capacity);
if(mode == 2)
	objc_draw(welldial,0,10,x,y,w,h);
else
	{
	if(mode == 3)
		{
		objc_draw(welldial,TIMENUM,1,x,y,w,h);
		objc_draw(welldial,LABCOST,1,x,y,w,h);
		objc_draw(welldial,WELLCAP,1,x,y,w,h);
		objc_draw(welldial,SURRBULL,1,x,y,w,h);
		objc_draw(welldial,SURRIMS,1,x,y,w,h);
		objc_draw(welldial,BOXTIME,1,x,y,w,h);
		}
	objc_draw(welldial,WELLCOST,1,x,y,w,h);
	}
}
/***************************************************************************/
BUTFUNC( butinwell )		/*button pressed in well set up*/
{
int	x,y,w,h,obj,i,type,mwc;

mwc = wind->p2;
wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
type = whichwell[ mwc ];		/* cast to int*/
welldial->ob_x = x;	welldial->ob_y = y;
switch((obj = objc_find(welldial,0,10,mx,my)))
	{
	case	TIMESLD:
		slider(welldial,BOXTIME,TIMESLD,&(wind->p1),(real)MINTMWELL,
			(real)MAXTMWELL, TIMENUM, 2, x, y, w, h);
		break;
	case	SUBSPEC:		/*submit spec*/
		startwell((int)wind->p1,type,wind->gpp,0);
		close_wind(wh);
		break;
	default:
		for(i = 0 ; i < 4 && welltypes[i] != obj ; ++i)
			;
		if(i == 4 || i == (int)wind->p2)
			break;
		wind->p2 = i;
		drawwell = 3;
	}
}
/****************************************************************************/
well_bng_blt( c, p, days)	/* returns type if well being built, else -1*/
int	c, p;
int	*days;
{
JOBS	*runjob;

runjob = &firstjob;
while(runjob = runjob->next)
	{
	if( runjob->type == WELLJOB )
		{
		if( runjob->company == c && (int)runjob->p1 == p )
			{
			*days = runjob->delay;
			return( (int)runjob->p2 );
			}
		}
	}
return( -1 );
}
/***************************************************************************/
startwell(months, type, p, c )
int	months, type, p, c;
{
JOBS	job;
real	price;
extern	int	buildwell();

if( plot[p].owner != c )
	return;
price		= pricewell( type, months );
if( c == 0 && price > cashchange( c, RDCASH) )
	{
	form_alert( 1, st[CANTWELL] );
	return;
	}
plot[p].flags[c]|= 8;	/* well being built */
job.company	= c;
job.delay	= (c == 0)?(30*months):(1);
job.type	= WELLJOB;
job.p1		= p;
job.p2		= type;
job.p3		= price;
addjob(&job);
drawstates	= 1;
cashchange( c, CAPITAL, price );
}
/**************************************************************************/


buildwell(job)	/*builds well, obviously*/
JOBS	*job;
{
int	p, type, c;
char	string[150];
char	cost[50];

p		= job->p1;
type		= job->p2;
c		= job->company;
if( plot[p].owner != c )
	return;
plot[p].type	= type;
plot[p].output	= param_wells[type].capacity;
plot[p].cost	= job->p3 / 10000.0;
plot[p].age	= 0;
plot[p].maxage	= 8;		/* have as variable later */
plot[p].flags[c] &= 0xF7;	/* clear well being built */

formnum(cost, job->p3 );
sprintf(string, st[WELLFIN], statename[STATE(p)], TOPLOT(p), comp[c], cost);
if( !easy || c != 0)
	addmsg( string );
drawstates = 1;
if( mapinfo )
	redraw_desktop();
}
/**************************************************************************/
DRAWFUNC( storemen )
{
int	dummy;

wind_get(wh,WF_WORKXYWH,&(tanks->ob_x),&(tanks->ob_y),&dummy,&dummy);

NUMTEXT(tanks, STOREAMT,wind->p1);
NUMTEXT(tanks, STCOST, pricestore( (int)wind->p2, wind->p1) );
OBS(tanks,STORENM) = statename[wind->gpp];
SLIDER(tanks,STAMTSLD,STAMTBOX,0,1e3,wind->p1);
tanks[STRETR2].ob_state = NORMAL;
tanks[STRETR5].ob_state = NORMAL;
tanks[STRETR10].ob_state = NORMAL;
tanks[STRETR20].ob_state = NORMAL;
switch( (int)wind->p2) {
	case	2:
		tanks[STRETR2].ob_state = SELECTED;
		break;
	case	5:
		tanks[STRETR5].ob_state = SELECTED;
		break;
	case	10:
		tanks[STRETR10].ob_state = SELECTED;
		break;
	default:
		tanks[STRETR20].ob_state = SELECTED;
		wind->p2	= 20;
		break;
	}
if( mode == 1 )
	{
	objc_draw(tanks,STCOST,0,x,y,w,h);
	objc_draw(tanks,SURRTMST,2,x,y,w,h);
	}
else
	objc_draw(tanks,0,10,x,y,w,h);
}
/*************************************************************************/
numjobs(type)	/*returns number of wells and pipes being built*/
int	type;
{
JOBS	*runjob;
int	number = 0;

runjob = &firstjob;
while(runjob = runjob->next)
	if(runjob->type == type)
		++number;
return(number);
}
/***************************************************************************/
real	pricewell(type,time)	/*calculates price of well at present time*/
int	type,time;
{
real	price,x;

x = numjobs(PIPEJOB)/2 + numjobs(WELLJOB);
price = (param_wells[type].cap_cost*bldfact*(x*x*x/27+1.0)*
(2*MAXTMWELL - time))/MAXTMWELL;
return(price);
}
/***************************************************************************/
DRAWFUNC( menloan )		/*does get loan dialogue*/
{
int	dummy;

wind_get(wh,WF_WORKXYWH,&(loan->ob_x),&(loan->ob_y),&dummy,&dummy);

NUMTEXT(loan,AMTVAL,wind->p1);
SLIDER(loan,BOXAMT,SURRAMT,0,wind->p2,wind->p1);

wind->gpp	= sel_loan_time( wind->gpp );

if(mode == 2)
	objc_draw(loan,0,10,x,y,w,h);
else
	{
	objc_draw(loan,SURRAMT,2,x,y,w,h);
	objc_draw(loan,AMTVAL,2,x,y,w,h);
	objc_draw(loan,SURRTMLN, 2, x, y, w, h);
	}
}
/*****************************************************************************/
BUTFUNC( butinloan )		/*button pressed in loan arrange*/
{
int	x,y,w,h,obj;

wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
loan->ob_x = x;	loan->ob_y = y;
switch((obj = objc_find(loan,0,10,mx,my)))
	{
	case	BOXAMT:
		slider(loan,SURRAMT,BOXAMT,&(wind->p1),0.0,wind->p2,AMTVAL,0,x,y,w,h);
		break;

	case	TAKELOAN:
		startloan(0,wind->gpp,wind->p1);
		close_wind(wh);
		break;

	case	LOANTR2:
		wind->gpp	= 2;
		break;
	case	LOANTR5:
		wind->gpp	= 5;
		break;
	case	LOANTR10:
		wind->gpp	= 10;
		break;
	case	LOANTR20:
		wind->gpp	= 20;
		break;
	}
drawloan = 1;
}

/*
**	this routine turns off all time boxes in loan window
**	and selects box of required time.
**	Returns time. If an unknown time was sent, then sets time to 20.
*/
sel_loan_time( time )
int	time;
{
loan[LOANTR2].ob_state	= NORMAL;
loan[LOANTR5].ob_state	= NORMAL;
loan[LOANTR10].ob_state	= NORMAL;
loan[LOANTR20].ob_state	= NORMAL;
switch( time ) {
	case	2:
		loan[LOANTR2].ob_state = SELECTED;
		break;
	case	5:
		loan[LOANTR5].ob_state = SELECTED;
		break;
	case	10:
		loan[LOANTR10].ob_state = SELECTED;
		break;
	default:
		loan[LOANTR20].ob_state = SELECTED;
		return 20;
		break;
	}
return( time );
}


startloan(company,term,amount)
int	company,term;
real	amount;
{
JOBS	job;
extern	int	dolnjob();

job.company   = company;
job.delay     = term*30;
job.type      = LOANJOB;
job.p2        = amount;			/*the original total loan*/
addjob(&job);

cashchange(company,LOANS,amount);
}
/*****************************************************************************/ 
dolnjob(job)
JOBS	*job;
{
char	string[150], money[50];
real	repayment;			/*calculate repayment*/

if( job->company == 0 )
	{
	formnum( money, job->p2 );
	sprintf(string, st[LOANPAID], money);
	addmsg( string );
	}
cashchange(job->company,LOANS,-1.0 * job->p2);
}
/***************************************************************************/ 
formnum(text,price)	/*prints price to text in most efficient way*/
char	*text;
real	price;
{

if(price < 1e1) {
	sprintf(text,"%-16.2f",price);
	return;
	}
if(price < 1e2) {
	sprintf(text,"%-16.1f",price);
	return;
	}
if(price < 1e3) {
	sprintf(text,"%-16ld",(long)price);
	return;
	}
if(price < 1e4) {
	sprintf(text,"%-5.2f thousand ",price/1e3);
	return;
	}
if(price < 1e5) {
	sprintf(text,"%-5.1f thousand ",price/1e3);
	return;
	}
if(price < 0.9999e6) {
	sprintf(text,"%-5ld thousand ",(long)(price/1e3));
	return;
	}
if(price < 1e7) {
	sprintf(text,"%-5.2f million ",price/1e6);
	return;
	}
if(price < 1e8) {
	sprintf(text,"%-5.1f million ",price/1e6);
	return;
	}
if(price < 1e9) {
	sprintf(text,"%-5ld million  ",(long)(price/1e6));
	return;
	}
sprintf(text,"%-5.2f billion   ",price/1e9);
}
/***************************************************************************/
BUTFUNC( butinstore )
{
int	x,y,w,h,obj;

wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
tanks->ob_x = x;	tanks->ob_y = y;
switch(obj = objc_find(tanks,0,10,mx,my) )
	{
	case	STOREDO:
	startstore(0,(int)(wind->p2),wind->gpp,wind->p1);
	close_wind(wh);
	break;

	case	STAMTSLD:
	slider(tanks,STAMTBOX,STAMTSLD,&(wind->p1),0.0,1e3,STOREAMT,0,x,y,w,h);
	NUMTEXT(tanks, STCOST, pricestore( (int)wind->p2, wind->p1) );
	break;

	case	STRETR2:
		wind->p2	= 2;
		break;
	case	STRETR5:
		wind->p2	= 5;
		break;
	case	STRETR10:
		wind->p2	= 10;
		break;
	case	STRETR20:
		wind->p2	= 20;
		break;
	}
drawstore	= 1;
}
/***************************************************************************/
startstore( c, time, s, amount)
int	c, time, s;
real	amount;
{
JOBS	job;
real	price;
extern	int	buildstore();

price		= pricestore( time, amount);
if( c == 0 && price > cashchange( 0, RDCASH) )
	{
	form_alert(1, st[CANTSTORE] );
	return;
	}
job.company 	= c;
job.delay	= (c == 0)?(time * 30):(1);
job.type	= STOREJOB;			/*the add storage type*/
job.p1		= s;
job.p2		= amount;			/*amount of storage to build*/
job.p3		= price;
addjob(&job);
cashchange( c, CAPITAL, price );
store_beingbuilt[c][s] += amount;
drawstates = 1;
}
/******************************************************************************/
buildstore(job)
JOBS	*job;
{
char	string[150];
char	cost[20];
int	c, s;

c		= job->company;
s		= job->p1;

storage[c][s]		+= job->p2;
store_beingbuilt[c][s]	-= job->p2;
formnum(cost, job->p3 );
sprintf(string,st[STOREFIN], statename[s], comp[c], cost);
if( !easy || c != 0)
	addmsg( string );
drawstates = 1;
updt_sell_menu();
if( mapinfo )
	redraw_desktop();
}
/*****************************************************************************/
real	pricestore(time,amount)		/*returns price of storage*/
int	time;	/*time in months*/
real	amount;	/*amount of storage in barrels*/
{
real	price;
real	x;

x = 2;
price = ((amount*bldfact*(x*x*x/27+1.0)*(50.0 - time))/44.0)*150.0;
return(price);
}
/**************************************************************************/


