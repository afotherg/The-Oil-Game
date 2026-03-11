/*****************************************************************************/
/*	This file holds code to handle Vice President events		     */
/*	Started 07 September 1987					     */
/*****************************************************************************/
#include	"header.h"
#include	"variab.h"

GRECT	dumpos;				/* dummy pos window grect*/
char	vp_bankbal[50];
char	vp_banktext[]		= "Your bank balance is $%s.";

/*
**	finds object pointed to by mx, my in vpwindow
*/
vpobj( mx, my)
int	mx, my;
{
return( objc_find( vpwindow, 0, 10, mx, my) );
}

/*
**	general exit from VP window code
*/
BUTFUNC(but_exit)
{
if( vpobj(mx, my) == VPBUT3 )
	exitvp( wh );
}

/*
**	opens VP window for final exit
*/
vpexitwindow( wh, vp, text)
int	wh, vp;
char	*text;
{					/* resets vptime, vpmaxtime */
close_wind( wh );			/* there will always be one open */
dovpwindow( vp, text, 0L, 0L,0L, st[VPEXITBUT], but_exit, 0, 0.0, 0.0);
}

/*
**	when exit button pressed in VP window
*/
exitvp( wh )
int	wh;
{
isevent	= FALSE;
}

/*
**	opens daily price graph window
*/
open_dprice( s, x, y)
int	s, x, y;
{
extern	int	outpriall();

open_window( x, y-DAILYH, DAILYW, DAILYH,
NAME|CLOSER, st[WIPRICE],
2,outpriall, 0L, 0L, 0L, 0L, 0L, s, 0.0, 0.0, &dumpos, 0L);
}

/*
**	opens daily revenue graph
*/
open_drev( x, y)
int	x, y;
{
extern	int	outearningsall();

open_window( x, y - DAILYH, DAILYW, DAILYH, NAME|CLOSER, st[WIREVALL],
2,outearningsall, 0L, 0L, 0L, 0L, 0L, 0, 0.0, 0.0, &dumpos, 0L);
}

/*	opens a build storage window.
**	y is bottom of window
*/
open_buildst( s, x, y )
int	s, x, y;
{
extern	int	storemen(), butinstore();
	
open_window( x, y-tanks->ob_height, tanks->ob_width, tanks->ob_height,
NAME|CLOSER, st[WIBS],2, storemen,0L,0L,0L,butinstore,0L, s ,0.0,
4.0,&dumpos, tanks);
}

/*
**	opens an historic costs window
*/
open_costs( s, x ,y )
int	s, x, y;
{
extern	int	costhist();

open_window( x, y-HISTH, HISTW, HISTH, NAME|CLOSER, st[WIHISTCOST],
0, costhist, 0L,0L,0L,0L, &drawhist, s, 0.0, 0.0, &dumpos, 0L);
}

/*
**	open historic revenue window
**	state = s, pos = (x, y)
*/
open_rev( s, x, y )
int	s,x,y;
{
extern	int	revhist();

open_window(x, y - HISTH, HISTW, HISTH,
	NAME|CLOSER, st[WIHISTREV],0,revhist,0L,0L,0L,0L,
	&drawhist, s , 0.0, 0.0, &dumpos, 0L);
}

/*
**	open price strategy window
*/
open_prc( s, x, y )
int	s, x, y;
{
extern	int	mprices(), butinprices();

open_window(x, y - prices->ob_height, prices->ob_width,prices->ob_height,
	NAME|CLOSER, st[WICHPRC], 0, mprices, 0L, 0L, 0L,
	butinprices, &drawprice, s, 0.0, 0.0,&dumpos, prices);
}

/*
**	open an historic supply window
*/
open_sup( s, x, y )
int	s, x, y;
{
extern	int	supphist();

open_window( x, y-HISTH, HISTW, HISTH,	
	NAME|CLOSER, st[WIHISTSUP],0,supphist, 0L,0L,0L,0L,
	&drawhist, s, 0.0, 0.0, &dumpos, 0L);
}

/*
**	open transport window
*/
open_tran( from, to, x, y)
int	from, to, x, y;
{
extern	int	transrail(), butinrail();

open_window( x, y-raildial->ob_height ,raildial->ob_width,raildial->ob_height,
NAME|CLOSER, st[WITRAN],1,transrail,0L,0L,0L,butinrail,&drawrail,from,
0.0,(real)to,&dumpos, raildial);
}

/*
**	open state window
*/
open_state( s, x, y)
{
extern	int	statedraw(), selectplot();

open_window( x, y-plots->ob_height, plots->ob_width, plots->ob_height,
	NAME|CLOSER,statename[s],0,statedraw,
	0l,0l,0l,selectplot,&drawstates, s,0.0,0.0,&dumpos, plots);
}

/*
**	open a change output window.
**	Plot in p
*/
open_changeout( p, x, y)
int	p, x, y;
{
extern	int	chout(), butinch();

open_window( x, y-chdial->ob_height, chdial->ob_width,chdial->ob_height,
NAME|CLOSER, st[WICHOUT], 2, chout,0L,0L,0L,butinch, 0l, p, plot[p].output,
plot[p].output, &dumpos, chdial);
}

/*
**	opens a loan window
*/
open_loan(x, y)
int	x, y;
{
extern	int	menloan(), butinloan();

open_window(x, y-loan->ob_height, loan->ob_width, loan->ob_height,
	NAME|CLOSER, st[WIAL], 0,
	menloan, 0l, 0l, 0l, butinloan, &drawloan, 1, 0.0, maxloan(0), 
	&dumpos, loan);
}

/*
**	redraw code for the VP window
*/
vicepresdraw(wh,mode,x,y,w,h,wind)
int	wh,mode,x,y,w,h;
WINDOWS	*wind;
{
int	dummy;

wind_get(wh,WF_WORKXYWH,&(vpwindow->ob_x),&(vpwindow->ob_y),&dummy,&dummy);
if( mode == 2)
	objc_draw(vpwindow,0,10,x,y,w,h);
}

/*
**	Generalised routine to put VP window onto screen
*/
dovpwindow( vp,text,but0,but1,but2,but3,but,gpp,p1,p2 )
int	vp;
char	*text,*but0,*but1,*but2,*but3;
int	(*but)(),gpp;
real	p1,p2;
{
static	int	textobj[] = {	VPTEXT0,VPTEXT1,VPTEXT2,VPTEXT3};
static	int	vps[]	= {
				MEMPRODVP,
				MEMPLANVP,
				MEMMARKVP,
				MEMSALEVP,
				MEMSALESVP,
				MEMTRANVP,
				MEMFINVP,
				MEMDEVVP,
				MEMOPVP,
				MEMDIRTYVP
			};

int	obj = 0,i=0,j=0,c;
char	*runpt;

TXTPNT( vpwindow, VPTITLE) = st[ vps[vp]];

while( c = text[i] )
	{
	runpt = vpwindow[textobj[obj]].ob_spec;
	runpt[j] = c;
	if(j == 59 )
		{
		while( text[i] != ' ' )
			{
			runpt[j] = ' ';
			--i;	--j;
			}
		++obj;
		j = -1;
		}
	++j;
	++i;
	}
runpt[j] = '\0';
++obj;
for( ; obj < 4 ; ++obj)
	{
	runpt = vpwindow[textobj[obj]].ob_spec;
	for( i = 0 ; i < 60 ; ++i )
		runpt[i] = ' ';
	}
vpwindow[VPBUT0].ob_spec = (but0)?(but0):("");
vpwindow[VPBUT1].ob_spec = (but1)?(but1):("");
vpwindow[VPBUT2].ob_spec = (but2)?(but2):("");
vpwindow[VPBUT3].ob_spec = (but3)?(but3):("");
if( !window[4].handle )
	open_window(xdesk,ydesk,vpwindow[0].ob_width,vpwindow[0].ob_height,0,"",0,
	vicepresdraw,0L,0L,0L,but,&drawvp,gpp,p1,p2,&dumpos, vpwindow);
else
	{
	window[4].butpress	= but;
	window[4].gpp		= gpp;
	window[4].p1		= p1;
	window[4].p2		= p2;
	send_redraw( window[4].handle );
	}
wind_set( window[4].handle, WF_TOP, 0, 0, 0, 0);	/* ensure topped */
}

/*
**	export oil job has come up
*/
expoildo( job )
JOBS	*job;
{
int	state,amount,penalty;
real	price;
char	text[300];

state  = job->p1;
price  = job->p2;
amount = job->p3;
if( supply[0][state] >= amount*1000.0 )
	{
	supply[0][state] -= amount*1000.0;
	cashchange( 0, REV, amount*1000.0*price );
	sprintf( text, "You have received $%d thousand for supplying %s.",
		(int)(amount*price),statename[state]);
	addmsg( text );
	}
else
	{
	penalty = amount * price / 4;	/* penalty in 1000's */
	cashchange( 0 , ACOST, penalty*1000.0 );
	sprintf( text, "You failed to reach your obligation to supply %d \
thousand barrels of oil to %s. Penalty $%d thousand", amount,statename[state],
penalty);
	addmsg( text );
	}
}


/*
**
**	Asks if Gitty wants to export oil to a coast plot.
**
*/
BUTFUNC(butexpoil)
{
int	obj;
JOBS	job;

obj = vpobj( mx, my );
if( obj == VPBUT3 )
	exitvp( wh );

if( obj == VPBUT0 )	/* accept export oil */
	{
	job.delay    = 30;		/* 1 month */
	job.type     = EXPOILJOB;
	job.p1       = wind->gpp;	/* state */
	job.p2       = wind->p1;	/* price */
	job.p3       = wind->p2;	/* amount in thousands */
	addjob( &job );
	exitvp( wh );
	}
}

expoilevent()
{
static	int	portdes[] = {0, 4, 5, 12, 13, 14, 15};
char	text[300];
real	price;
int	state;
int	amount;		/* amount in thousands to transport */
int	penalty;	/* penalty for non-delivery in thousands */

state = portdes[rand()%7];
price = al_minprice( state ) * ( 1.0 + (rand() / 32768.0));	/* 1.0-2.0*/
amount = 1 + rand()%2;
penalty = amount * price/4;

sprintf( text, st[VPEXPOILT], amount, price, statename[state], penalty);
dovpwindow( VPMARK, text, st[VPACCBUT], 0L, 0L, st[VPEXITBUT],
	butexpoil, state, price, (real)amount );
return( TRUE );
}

/*
**
**	Asks if Gitty wants to buy a random plot
**
*/
BUTFUNC(butoffplot)
{
int	p;
real	price;

p	= wind->gpp;
price	= wind->p1;

switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		if( plot[p].owner < 0 )
			{
			if( cashchange( 0, RDCASH) < price )
				form_alert( 1, st[NOLAND] );
			else
				{
				cashchange( 0, LAND, price );
				plot[p].val_land	= price/10000.0;
				tot_land	+= 2.0*price;
				plot[p].owner	= 0;
				drawstates	= 1;
				}
			}
		exitvp( wh );
		break;		
		
	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

offplotevent()			/* plot exclusively for sale */
{
char	text[300],pricetext[20];
int	state, plotnum, p;
real	price;

price	= tot_land/numauc;
p	= rand()%256;
state	= STATE( p );
plotnum	= TOPLOT( p );
if( plot[p].owner >= 0 || (plot[p].flags[0] & 0x80) )
	return( FALSE );
formnum( pricetext, price );
sprintf( text, st[VPOFFPLOT], statename[state], comp[0], plotnum, pricetext);
dovpwindow( VPSALE, text, st[VPACCBUT], 0L, 0L, st[VPEXITBUT],
	butoffplot, p, price, 0.0 );
return( TRUE );
}

/*
**
**	Finds a state that has production or transport into it, and has
**	sold everything at the end of the day. Alerts the player to
**	this.
**	User has option of Raising Prices, or Raising Production.
**
**	Selecting "raise price" results in the next VP window saying to raise
**	price until revenue graph peaks. Needs to be altered in non-interactive
**	environment. The revenue and price change windows are opened.
**
**	Selecting "raise prod" results in the message that the cheapest well has
**	been selected, and to raise production until the difference in revenue
**	and costs is greatest. The revenue, costs, and production change windows
**	are displayed.
**
*/

BUTFUNC(but_sellall)
{
int	state, p;
char	text[300];

state = wind->gpp;
switch( vpobj( mx, my ) )
	{
	case	VPBUT0:			/* to raise price */
		open_rev( state, xdesk, ydesk+hdesk );
		open_prc( state, xdesk+HISTW,ydesk+hdesk );
		vpexitwindow( wh, VPPROD, st[VPSELLALL1M] );
		break;

	case	VPBUT1:			/* raise production */
		sprintf( text, st[VPSELLALL2M], statename[state] );
		open_rev( state, xdesk, ydesk+hdesk );
		open_costs( state, xdesk+HISTW, ydesk+hdesk );
		if ( (p = al_min_prod_cost( 0, state )) < 0 )
			break;
		open_changeout( p, xdesk+2*HISTW, ydesk+hdesk );
		vpexitwindow( wh, VPPROD, text );
		break;

	case	VPBUT3:			/* exit */
		exitvp( wh );
		break;
	}
}

sellall_event()		/* when Gitty is selling all in a state*/
{
char	text[300];
int	s,i,off;

off = rand()%16;
for( i = 0 ; i < 16 ; ++i )
	{
	s = (i+off)%16;			/* so not always same message! */
	if( produce[0][s] + al_transin(0, s) > 0.0 && endsupply[0][s] < 10.0 )
		{
		sprintf( text, st[VPSELLALLT], statename[s] );
		dovpwindow( VPPROD, text, st[VPRSPRBUT], 
			st[VPRSPDBUT], 0L,st[VPEXITBUT], but_sellall, s, 0.0, 0.0 );
		return( TRUE );
		}
	}
return( FALSE );
}

/*
**	Finds state where expenses are exceeding revenue and tells of this.
**
**	Player has option of asking for more.
**
**	If more is asked for, then revenue and price change graphs are pulled
**	up, and the user is told to raise price until revenue peaks.
**
*/
BUTFUNC(but_make_loss)
{
int	state;

state = wind->gpp;
switch (vpobj( mx, my ))
	{
	case	VPBUT0:			/* asked for more */
		open_rev( state, xdesk, ydesk+hdesk );
		open_prc( state, xdesk+HISTW, ydesk+hdesk );
		vpexitwindow( wh, VPPROD, st[VPSELLALL1M] );
		break;

	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

making_loss_event()		/* signalled when making a loss */
{
char	text[300];
int	s,i,off;

off = rand()%16;
for( i = 0 ; i < 16 ; ++i )
	{
	s = (i+off)%16;
	if( expstate[0][s] > cashstate[0][s] && al_transout(0,s) == 0.0)
		{
		sprintf( text, st[VPMKLOSST], statename[s] );
		dovpwindow( VPPROD, text, st[VPMOREBUT], 0L, 0L, 
		st[VPEXITBUT], but_make_loss, s, 0.0, 0.0);
		return( TRUE );
		}
	}
return( FALSE );
}


