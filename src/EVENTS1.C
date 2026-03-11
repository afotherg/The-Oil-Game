/*****************************************************************************/
/*	This file holds code to handle Vice President events		     */
/*	Started 07 September 1987					     */
/*****************************************************************************/
#include	"header.h"
#include	"variab.h"
/*****************************************************************************/
extern	GRECT	dumpos;				/* dummy pos window grect*/
extern	char	vp_bankbal[50];
extern	char	vp_banktext[];

/*
**	Finds a plot that has un used supply, and suggest that it is transported
**	out to a named plot that has unused storage.
**
**	Player has option for "more"
**
**	If "more" is pressed, then the supply graph for the destination is
**	placed on the screen, and the properly set up transport oil window is
**	pulled up.
**	The player is advised to increase transport out until the supply graph
**	of the destination state shows a value.
*/
BUTFUNC(but_trans_oil)
{
int	from, to;
char	text[300];

switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		from = wind->gpp;	to = wind->p1;
		sellmin[0][to] = 50.0;
		sprintf( text, st[VPTROILM], statename[to], sellmin[0][to]);
		open_sup( to, xdesk + raildial->ob_width, ydesk+hdesk );
		open_tran( from, to, xdesk,ydesk+hdesk );
		vpexitwindow( wh, VPTRAN, text );
		break;

	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

trans_oil_event()		/* looks for transport possibilities */
{
char	text[300];
int	from,to,i,j,off1,off2;

off1 = rand()%16;	off2 = rand()%16;
for( i = 0 ; i < 16 ; ++i )
	{
	to = (i+off1)%16;
	if( storage[0][to] > 0.0 && amountsold[to] < 10.0 && 
		endsupply[0][to] < 10.0 )
		{
		for( j = 0 ; j < 16 ; ++j )
			{
			from = (j+off2)%16;
			if( from == to )
				continue;
			if( (endsupply[0][from] - al_transout(0,from) ) > 0.0 
				&& railmap[from][to] > 0)
				{
				sprintf(text, st[VPTROILT], statename[from],
					statename[to]);
				dovpwindow( VPTRAN, text, st[VPMOREBUT], 
					0L, 0L, st[VPEXITBUT], but_trans_oil, 
					from, (real)to, 0.0);
				return( TRUE );
				}
			}
		}
	}
return( FALSE );
}

/*
**	Finds a state where Gitty has no storage, and says it would be a
**	good strategy to get in their from the start.
**	If "more" is asked for, then the build storage window is pulled up,
**	with a recommended ampunt to build.
*/
BUTFUNC(but_build_st)
{
int	s;
char	text[300];

switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		s = wind->gpp;
		sprintf( text, st[VPBUILDSTM], (int)MIN( demandset( 30.0, s),
			 1000), vp_bankbal );
		open_buildst( s, xdesk, ydesk+hdesk );
		vpexitwindow( wh, VPPLAN, text );
		break;

	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

build_st_event()
{
int	i, s, off;
char	text[300];

off = rand()%16;

for( i = 0 ; i < 16 ; ++i)
	{
	s = (i+off)%16;
	if( amountsold[s] == 0.0 && storage[0][s] == 0.0 && 
			store_beingbuilt[0][s] == 0.0)
		{
		sprintf(text, st[VPBLDSTT], statename[s]);
		dovpwindow( VPPLAN, text, st[VPMOREBUT], 0L, 0L, 
			st[VPEXITBUT], but_build_st, s, 0.0, 0.0);
		return	( TRUE );
		}
	}
return	( FALSE );
}

/*
**	Recommends that Gitty tries prospecting in a plot.
**	If "more" is asked for, then the prospecting window for that plot is
**	opened.
*/
BUTFUNC( but_prospect )
{
int	p;
extern	int	menpros(), butinpros();

switch( vpobj( mx, my ))
	{
	case	VPBUT0:
		p = wind->gpp;
		plotsel = p;
		drawstates = 1;		/* smart hey? Think of everything */
		open_state( STATE(p), xdesk, ydesk+hdesk );
		open_window(xdesk+plots->ob_width,ydesk+hdesk-prosdial->ob_height,prosdial->ob_width,prosdial->ob_height,
		NAME|CLOSER,"prospect",0,
		menpros,0l,0l,0l,butinpros,&drawpros,p ,1.0,0.0,&dumpos,plots);
		vpexitwindow( wh, VPOP, st[VPPROSM] );
		break;

	case	VPBUT3:
		exitvp( wh );
	}
}

prospect_event()		/* recommend prospecting in a plot */
{
int	i, off, p;
char	text[300];

off = rand()%256;

for( i = 0 ; i < 256 ; ++i )
	{
	p = (i+off)%256;
	if( plot[p].owner == 0 && plot[p].type < 0 && !(plot[p].flags[0]&1) )
		{
		sprintf( text, st[VPPROST], TOPLOT(p),statename[STATE(p)]);
		dovpwindow( VPDEV, text, st[VPMOREBUT], 0L, 0L, 
			st[VPEXITBUT], but_prospect, p, 0.0, 0.0 );
		return( TRUE );
		}
	}
return( FALSE );
}

/*
**	Recommends that a well is built in one of the unused plots.
**	If "more" is asked for, then player given estimate for demand at a
**	certain price.
*/
BUTFUNC( but_well )
{
int	p,s;
char	text[300],number[50];
real	price;
extern	int	specwell(), butinwell();

switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		p = wind->gpp;
		s = STATE(p);
		price = 30.0;	/* some sort of function or exogenous later */
		plotsel = p;
		formnum( number, demandset( price, s)*1.3);	/* 1.2 is VP's error */
		sprintf(text, st[VPWELLM], statename[s], number, price);
		open_state( s, xdesk, ydesk+hdesk );
		open_window( xdesk + plots->ob_width, ydesk+hdesk-welldial->ob_height,welldial->ob_width,welldial->ob_height,
		NAME|CLOSER,"build well",2,
		specwell,0l,0l,0l,butinwell,&drawwell,p, 6.0, 0.0,&dumpos,welldial);
		drawstates = 1;				/* just in case */
		vpexitwindow( wh, VPPLAN, text );
		break;

	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

well_event()
{
int	i,p,off;
real	estres;
char	text[300],number[50];

off = rand()%256;

for( i = 0 ; i < 256 ; ++i)
	{
	p = (i+off)%256;
	if( plot[p].owner == 0 && plot[p].type < 0 && plot[p].flags[0]&1 )
		{
		if((estres = plot[p].prosres)>100 && !(plot[p].flags[0]&8))
			{
			formnum( number, estres );
			sprintf(text, st[VPWELLT], TOPLOT(p), 
				statename[STATE(p)],number);
			dovpwindow( VPPLAN, text, st[VPMOREBUT],0L,0L,
				st[VPEXITBUT], but_well, p, 0.0, 0.0);
			return( TRUE );
			}
		}
	}
return( FALSE );
}

/*
**	Finds a state where there is too much supply.
**
**	If player is transporting into that state, then he is alerted to this.
**	If "more" is pressed, then the state that the transport is taking place
**	from is found, and the relevant transport dialog, and supply graph are
**	displayed.
**
**	If transport is not taking place into the state, then the player is
**	given the option of "lowering production", "lowering price", or
**	"transporting out".
**
**	if one is selected, then the relevant graphs, etc are displayed.
*/
BUTFUNC(but_supply1)		/* when too much supply, & transporting in */
{
int	s, i;

s = wind->gpp;		/* transporting into this state */
switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		for( i = 0 ; i < 16 ; ++i )
			{
			if( tranoil[0][i][s] == 0 || i == s)
				continue;
			open_sup( s, xdesk + raildial->ob_width, ydesk+hdesk );
			open_tran(i, s, xdesk, ydesk+hdesk );
			vpexitwindow( wh, VPPROD, st[VPSUP1M] );
			}
		break;		


	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

BUTFUNC(but_supply2)
{
int	s, p;
char	text[300];

s	= wind->gpp;
switch( vpobj( mx, my ) )
	{
	case	VPBUT0:			/* lower production */
		p = al_max_prod_cost(0, s);
		if( p < 0 )
			break;
		sprintf( text, st[VPSUP20M], statename[s]);
		open_sup( s, xdesk, ydesk+hdesk );
		open_changeout( p, xdesk+HISTW, ydesk+hdesk );
		vpexitwindow( wh, VPPLAN, text );
		break;
		
	case	VPBUT1:
		open_sup( s, xdesk, ydesk+hdesk );
		open_prc( s, xdesk+HISTW, ydesk+hdesk );
		vpexitwindow( wh, VPPLAN, st[VPSUP21M] );
		break;

	case	VPBUT2:		/*search for somewhere to transport to*/
		open_sup( s, xdesk, ydesk+hdesk );
		open_tran( s, s, xdesk+HISTW, ydesk+hdesk );
		vpexitwindow( wh, VPPROD, st[VPSUP22M] );
		break;

	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

supply_event()
{
int	i, s, off;
char	text[300];

off	= rand()%16;

for( i = 0 ; i < 16 ; ++i )
	{
	s = (i+off)%16;
	if( endsupply[0][s] < 10.0 )
		continue;			/* no problem */
	if( histsupp[0][s][HISTPNTQNT-1] < 0.95 * histsupp[0][s][HISTPNTQNT-2])
		continue;
	if( al_transin( 0, s) > 0.0 )
		{
		sprintf( text, st[VPSUP1T], statename[s] );
		dovpwindow( VPPROD, text, st[VPMOREBUT], 0L, 0L, st[VPEXITBUT],
			but_supply1, s, 0.0, 0.0 );
		return( TRUE );
		}
	else
		{
		sprintf( text, st[VPSUP2T], statename[s]);
		dovpwindow( VPPROD, text, st[VPLPDBUT], 
			st[VPLPRBUT],st[VPTROUTBUT], st[VPEXITBUT],
			but_supply2, s, 0.0, 0.0 );
		return( TRUE );
		}
	}
return( FALSE );
}

/*
**	To give a more global view of the game, several competitive type
**	scenarios are included.
**	This event finds a state where another company has a monopoly, and
**	Gitty has no storage. Shows the revenue graphs for the offending
**	company, and recommends that storage is built.
**
**	If "build storage" is selected, then an aggressive message to do more
**	building is shown.
**
*/
BUTFUNC( but_compete1 )
{
int	s, c;
char	text[300];

s = wind->gpp;
c = wind->p1;

switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		sprintf( text, st[VPCOMP1M], comp[c], vp_bankbal );
		open_buildst( s, xdesk, ydesk+hdesk );
		vpexitwindow( wh, VPDIRTY, text );
		break;

	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

compete1_event()
{
int	i, off1, s, c;
char	text[300];

off1 = rand()%16;

for( i = 0 ; i < 16 ; ++i)
	{
	s = (i+off1)%16;
	c = monopoly( s );
	if( c <= 0 )
		continue;	/* if Gitty or no monopoly, then forget */
	if( storage[0][s] + store_beingbuilt[0][s] == 0.0 )
		{
		sprintf( text, st[VPCOMP1T], comp[c], statename[s] );
		open_drev( xdesk, ydesk + hdesk );
		dovpwindow( VPDIRTY, text, st[VPBUILDST], 0L, 0L, 
			st[VPEXITBUT], but_compete1, s, (real)c, 0.0 );
		return( TRUE );
		}
	}
return	FALSE;
}

/*
**	This dirty tricks message finds a state where Gitty has got storage
**	and capacity, but is not selling anything.
**
**	If the user goes for "war", then the price change window is
**	pulled up.
*/
BUTFUNC( but_compete2 )
{
int	s, c;
char	text[300];

s = wind->gpp;
c = wind->p1;

switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		sprintf( text, st[VPCOMP2M], comp[c], statename[s] );
		open_dprice( s, xdesk, ydesk+hdesk );
		open_prc( s, xdesk + DAILYW, ydesk+hdesk);
		vpexitwindow( wh, VPDIRTY, text );
		break;

	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

compete2_event()
{
int	i, off1, s, c;
char	text[300];

off1 = rand()%16;

for( i = 0 ; i < 16 ; ++i)
	{
	s = (i+off1)%16;
	c = monopoly( s );
	if( c <= 0 )
		continue;	/* if Gitty or no monopoly, then forget */

	if( storage[0][s] > 0.0 && al_capacity( 0, s ) > 0.0 )
		{
		sprintf( text, st[VPCOMP2T], comp[c], statename[s] );
		open_drev( xdesk, ydesk + hdesk );
		dovpwindow( VPDIRTY, text, st[VPWAR], 0L, 0L,
			st[VPEXITBUT], but_compete2, s, (real)c, 0.0);
		return( TRUE );
		}
	}
return( FALSE );
}


/*
**	This event proposes shipping in oil from one state to another to
**	kill an opposing company, and hurting one's own profits
**
**	If war is selected, then the transport dialog is pulled up
*/
BUTFUNC( but_compete4 )
{
int	from, to, c, p;
extern	int	but_compete3();

to	= wind->gpp;
c	= wind->p1;
from	= wind->p2;

switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		close_wind( wh );
		open_sup( from, xdesk, ydesk+hdesk );
		open_prc( from, xdesk+HISTW, ydesk+hdesk);
		dovpwindow( VPDIRTY, st[VPCOMP41M], st[VPCONTBUT],
			 0L, 0L, st[VPEXITBUT], but_compete3, to, (real)c,
				(real)from );
		break;

	case	VPBUT1:
		p = al_min_prod_cost(0, from );
		if( p < 0 )
			break;
		close_wind( wh );
		open_sup( from, xdesk, ydesk+hdesk );
		open_changeout( p, xdesk+HISTW, ydesk+hdesk);
		dovpwindow( VPDIRTY, st[VPCOMP42M], st[VPCONTBUT],
			0L, 0L, st[VPCONTBUT],but_compete3, to, (real)c,
				(real)from );
		break;

	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

BUTFUNC( but_compete3 )
{
int	from, to, c;
char	text[400];

to	= wind->gpp;
c	= wind->p1;
from	= wind->p2;

switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		if( endsupply[0][from] < 10.0 )
			{
			sprintf( text, st[VPCOMP4M], statename[from] );
			close_wind( wh );
			dovpwindow( VPDIRTY, text, st[VPRSPRBUT],
				st[VPRSPDBUT], 0L, st[VPEXITBUT], but_compete4,
				to, (real)c, (real)from );
			break;
			}
			
		sellmin[0][to] = MAX( sellmin[c][to] - 0.10, 0.10);
		sprintf( text, st[VPCOMP3M], statename[to], sellmin[0][to],
			comp[c] );
		open_drev( xdesk, ydesk+hdesk);
		open_tran( from, to, xdesk+HISTW, ydesk+hdesk );
		vpexitwindow( wh, VPDIRTY, text ); 
		break;

	case	VPBUT3:
		exitvp( wh );
	}
}

compete3_event()
{
int	i, j, off1, off2, s, c, from;
char	text[300];

off1 = rand()%16;	off2 = rand()%16;

for( i = 0 ; i < 16 ; ++i)
	{
	s = (i+off1)%16;
	c = monopoly( s );
	if( c <= 0 )
		continue;	/* if Gitty or no monopoly, then forget */

	for( j = 0 ; j < 16 ; ++j )
		{
		from = (j+off2)%16;
		if( from == s )
			continue;
		if(    	   railmap[from][s] > 0
			&& storage[0][s] > 0.0
			&& al_capacity( 0, from) > 0.0
			&& al_transin(0, s) == 0.0)
			{
			sprintf( text, st[VPCOMP3T], comp[c], statename[s],
				statename[from]);
			open_drev( xdesk, ydesk + hdesk );
			dovpwindow( VPDIRTY, text, st[VPWAR], 0L, 0L,
			 st[VPEXITBUT], but_compete3, s, (real)c, (real)from);
			return( TRUE );
			}
		}
	}
return( FALSE );
}


/*
**	this event checks if storage is needed. ie it checks if ability to
**	produce exceeds storage capacity. If it does, then this dialog is
**	brought up.
**	If the user selects "more", then he is told the amount that it would
**	be a good idea to build, and is given the build well dialog.
*/
BUTFUNC( but_need_store )
{
int	s, extra;
char	text[300];

s	= wind->gpp;
extra	= wind->p1;
switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		sprintf( text, st[VPNEEDSTM], extra, vp_bankbal);
		open_buildst( s, xdesk, ydesk+hdesk );
		vpexitwindow( wh, VPPLAN, text );
		break;
	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

need_store_event()			/* need storage, as too much prod */
{
real	needed, got;
int	i, off, s, extra;
char	text[300];

off = rand()%16;
for( i = 0 ; i < 16 ; ++i )
	{
	s = (i+off)%16;
	got	= storage[0][s] + store_beingbuilt[0][s];
	needed	= al_store_need( 0, s);
	extra	= MIN( 1.5*needed - got, 1000 );
	if( extra > 0 )
		{
		sprintf( text, st[VPNEEDSTT], statename[s], extra );
		dovpwindow( VPPLAN, text, st[VPMOREBUT], 0L, 0L,
			st[VPEXITBUT], but_need_store, s, (real)extra, 0.0 );
		return( TRUE );
		}
	}
return( FALSE );
}

/*
**	This event looks for excessive transportation out.
**	If this is found, then the user is given the option of "raising
**	production", "raising prices" or "lowering transportation"
**
*/
BUTFUNC( but1_ex_trans )
{
int	s, p;
char	text[300];

s = wind->gpp;
switch( vpobj( mx, my ) )
	{
	case	VPBUT0:			/* raise prod */
		p = al_min_prod_cost( 0, s );
		if( p < 0 )
			{
			sprintf( text, st[VPEXTR11M], statename[s] );
			open_sup( s, xdesk, ydesk+hdesk );
			open_changeout(p, xdesk+HISTW, ydesk+hdesk );
			vpexitwindow( wh, VPTRAN, text );
			}
		break;

	case	VPBUT1:
		sprintf( text, st[VPEXTR12M], statename[s] );
		open_sup( s, xdesk, ydesk+hdesk );
		open_prc( s, xdesk+HISTW, ydesk+hdesk );
		vpexitwindow( wh, VPTRAN, text );
		break;
		
	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

ex_trans1_event()
{
int	i, s, off;
real	t;
char	text[300];

off	= rand()%16;

for( i = 0 ; i < 16 ; ++i )
	{
	s = (i+off)%16;
	t = al_transout( 0, s );
	if( t > 0.0 )
		{
		if( endsupply[0][s]/t < 1.0 )
			{
			sprintf( text, st[VPEXTR1T], statename[s] );
			dovpwindow( VPTRAN, text, st[VPRSPDBUT],
				st[VPRSPRBUT], st[VPLTRBUT],st[VPEXITBUT],
				but1_ex_trans, s, 0.0, 0.0 );
			return( TRUE );
			}
		}
	}
return( FALSE );
}


/*
**	This events is when too much oil is being transported into a state.
**	The player has the option of "lowering prices", or "lowering 
**	transportation".
**	Both options are carried out automatically. ie the appropriate
**	variables are changed.
*/
BUTFUNC( but2_ex_trans )
{
int	from, to;

to	= wind->gpp;

switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		sellmin[0][to]	*= 0.95;		/* auto */
		exitvp( wh );
		break;

	case	VPBUT1:
		for( from = 0 ; from < 16 ; ++from )
			{
			if( from == to )
				continue;
			tranoil[0][from][to] = tranoil[0][from][to]*0.95;
			}
		break;

	case	VPBUT3:
		exitvp( wh );
		break;
	}	
}

ex_trans2_event()
{
int	i, s, off;
real	t;
char	text[300];

off	= rand()%16;

for( i = 0 ; i < 16 ; ++i )
	{
	s = (i+off)%16;
	t = al_transin( 0, s );
	if( t > 0.0 )
		{
		if( (storage[0][s] - endsupply[0][s])/t < 1.2 )
			{
			sprintf( text, st[VPEXTR2T], statename[s] );
			dovpwindow( VPTRAN, text, st[VPLPRBUT],
				st[VPLTRBUT], 0L, st[VPEXITBUT],
				but2_ex_trans, s, 0.0, 0.0 );
			return( TRUE );
			}
		}
	}
return( FALSE );
}


