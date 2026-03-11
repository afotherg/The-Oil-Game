#include	"header.h"
#include	"variab.h"
extern	int	easy;
/**********************************************************************/
transrail(wh,mode,x,y,w,h,wind)	/*does spec of well dialogue print screen*/
int	wh,mode,x,y,w,h;
WINDOWS	*wind;
{
int	stateto,statefrom,i,dummy;
int	transfer,available,left;
long	limit;

wind_get(wh,WF_WORKXYWH,&(raildial->ob_x),&(raildial->ob_y),&dummy,&dummy);

statefrom = wind->gpp;
stateto = wind->p2;
transfer = tranoil[0][statefrom][stateto];

available = MIN( endsupply[0][statefrom] - al_transout( 0,statefrom), 30000 );
if( available >= 0 )
	str( available, TXTPNT(raildial,RAILOIL),5 );
else
	strcpy( TXTPNT(raildial,RAILOIL), "-----" );
left = MIN(storage[0][stateto]-endsupply[0][stateto]-al_transin( 0, stateto),30000);
if( left >= 0 )
	str( left, TXTPNT(raildial,RAILCAP),5 );
else
	strcpy( TXTPNT(raildial,RAILCAP), "-----" );
str( transfer, TXTPNT(raildial,RAILAMT),5 );
formnum(TXTPNT(raildial,RAILCOST),
	pricerail(statefrom,stateto,(real)transfer,0)/ (real)(transfer + 0.01) );
sprintf(TXTPNT(raildial,RAILFROM),"%-11s",statename[statefrom]);
sprintf(TXTPNT(raildial,RAILTO),  "%-11s",statename[stateto]);
limit = MIN(storage[0][statefrom],storage[0][stateto]);
limit = MIN(limit, 30000.0);
limit = MAX(limit, 1.0);
SLIDER(raildial,RAILSLD,RAILBOX,0,limit,transfer);
for(i = 0 ; i < 16 ; ++i)
	{
	(raildial + statesto[i])->ob_state = DISABLED;	
	if( pipe[0][statefrom] & (1<<i) )
		(raildial + statesto[i])->ob_state = NORMAL;
	else
		if( railmap[statefrom][i] )
			(raildial + statesto[i])->ob_state = NORMAL;
	}
if(pipe[0][statefrom] & (1<<stateto) )
	TXTPNT(raildial,TRANMODE) = st[TRANPIPE];
else
	TXTPNT(raildial,TRANMODE) = st[TRANRAIL];
(raildial + statesto[stateto])->ob_state   = SELECTED;

if(mode == 2)
	objc_draw(raildial,0,10,x,y,w,h);
else
	{
	if(mode == 3)
		{
		(raildial + statesto[(int)wind->p2])->ob_state = SELECTED;
		objc_draw(raildial,statesto[(int)wind->p2],2,x,y,w,h);
		objc_draw(raildial,TRANMODE,1,x,y,w,h);
		objc_draw(raildial,RAILTO,10,x,y,w,h);
		objc_draw(raildial,RAILAMT,10,x,y,w,h);
		}
	objc_draw(raildial,RAILBOX, 1,x,y,w,h);
	objc_draw(raildial,RAILCOST,1,x,y,w,h);
	objc_draw(raildial,RAILCAP,10,x,y,w,h);
	objc_draw(raildial,RAILOIL,10,x,y,w,h);
	}
}
/***************************************************************************/
BUTFUNC( butinrail )		/*button pressed in well set up*/
{
int	x,y,w,h,obj,i;
int	statefrom,stateto;
real	transfer,limit;

statefrom = wind->gpp;		stateto = wind->p2;
transfer = tranoil[0][statefrom][stateto];
wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
raildial->ob_x = x;	raildial->ob_y = y;
limit = MIN(storage[0][stateto],storage[0][statefrom]);
limit = MIN(limit, 30000.0);
switch(obj = objc_find(raildial,0,10,mx,my))
	{
	case	RAILSLD:
		slider(raildial,RAILBOX,RAILSLD,&transfer,0.0,limit,RAILAMT,5,x,y,w,h);
		tranoil[0][statefrom][stateto] = transfer;
		break;

	default:
		for(i = 0 ; i <16 && statesto[i] != obj; ++i)
			;
		if( !railmap[statefrom][i] && !(pipe[0][statefrom] & (1<<i)) )
			break;
		if(stateto == i || i == 16)
			break;
		(raildial + statesto[(int)wind->p2])->ob_state = NORMAL;
		objc_draw(raildial,statesto[(int)wind->p2],2,x,y,w,h);
		wind->p2 = i;
		drawrail = 3;
		break;
	}
}
/**************************************************************************/
trarrow(wh,mode,wind)
int	wh,mode;
WINDOWS	*wind;
{
int	state;

state = wind->gpp;
if(mode == 6)
	state = (state + 15)%16;
if(mode == 7)
	state = (state + 1)%16;
wind->p2 = state;
wind->gpp = state;
send_redraw(wh);
}
/**************************************************************************/
do_trans()		/*outer level rail transport scenario*/
{
register	int	company,from,to;
real	amount;

for(company = 0 ; company < 4 ; ++company)
	{
	for(from = 0 ; from < 16 ; ++from)
		{
		for(to = 0 ; to < 16 ; ++to)
			{
			amount = tranoil[company][from][to];
			if( amount > 0.0 )
				startrail(company,from,to,amount);
			}
		}
	}
}
/*************************************************************************/
startrail(company,from,to,amount)
int	company,from,to;
real	amount;			/*barrels*/
{
real	cost;

amount = MIN(amount,supply[company][from]);
amount = MIN(amount,storage[company][to] - supply[company][to]);

cost = pricerail(from,to,amount,company); /* same whether all or none moved*/
cashchange(company,ACOST,cost);

supply[company][from] -= amount;

supply[company][to]   += amount;
}
/***************************************************************************/
slider(tree,box,sld,windparam,bot,top,name,len,x,y,w,h)
OBJECT	*tree;
int	box,sld,name,len,x,y,w,h;
real	*windparam,bot,top;
{
int	ret, dummy, keybd_state;
real	amount;

graf_mkstate(&dummy, &dummy, &dummy, &keybd_state);
if(keybd_state & K_CTRL)
	ret = keybd_slidebox(tree, box, sld);
else
	ret = graf_slidebox(tree,box,sld,0);
*windparam = amount = bot + (ret * (top-bot))/1e3;
if( top-bot == 0 )
	tree[sld].ob_x = 0;
else
	tree[sld].ob_x = ((long)(tree[box].ob_width-2*gl_wchar)*((amount - bot)))/((long)(top-bot));
objc_draw(tree,box,10,x,y,w,h);
if(len)
	str((int)amount,TXTPNT(tree,name),len);
else
	formnum(TXTPNT(tree,name),amount);
objc_draw(tree,name,0,x,y,w,h);
}
/****************************************************************************/
buildpipe(job)
JOBS	*job;
{
int	s, newpipe,c, from, to;
uint	pipeval;
char	string[150];

c		= job->company;
from		= job->p1;
to		= job->p2;
newpipe		= pipe[c] [to] | pipe[c][from];
pipeval		= (1<<from) | (1<<to);
for(s = 0 ; s < 16; ++s )
	{
	if(pipe[c][s]&pipeval)
		pipe[c][s] |= newpipe;
	}
sprintf(string, st[PIPEFIN], statename[from],statename[to],comp[c]);
if( c == compsel && pipemap )
	redraw_desktop();
if( !easy || c != 0 )
	addmsg( string );
}
/*************************************************************************/
startpipe(c, from, to, months)
int	c, from, to, months;
{
JOBS	job;
real	price;
extern	int	buildpipe();

price		= pricepipe( from, to, months );
if( c == 0 && price > cashchange( c, RDCASH ) )
	{
	form_alert( 1, st[CANTPIPE] );
	return;
	}
job.company	= c;
job.delay	= 30*months;
job.type	= PIPEJOB;
job.p1		= from;
job.p2		= to;
addjob(&job);
cashchange( c, CAPITAL, price );
}
/****************************************************************************/
pipemen(wh,mode,x,y,w,h,wind)
int	wh,mode,x,y,w,h;
WINDOWS	*wind;
{
int	stateto, statefrom ,dummy, i;

wind_get(wh,WF_WORKXYWH,&(pipedial->ob_x),&(pipedial->ob_y),&dummy,&dummy);

for( i = 0 ; i < 16 ; ++i )
	pipedial[pipestates[i]].ob_state = NORMAL;

stateto		= wind->p2;
statefrom	= wind->gpp;
NUMTEXT( pipedial, PIPECOST, pricepipe( statefrom, stateto, (int)wind->p1) );
str((int)wind->p1,TXTPNT(pipedial,PIPETIME),2);
(pipedial+PIPEFROM)->ob_spec = statename[statefrom];
SLIDER(pipedial,PIPESLD,PIPEBOX,6,31,wind->p1);

if(stateto >= 0 )
	{
	(pipedial + pipestates[stateto])->ob_state   = SELECTED;
	sprintf(TXTPNT(pipedial, PIPETO),"%-12s",statename[stateto]);
	}
else
	sprintf(TXTPNT(pipedial, PIPETO),"%-12s",st[UNASS] );
(pipedial+pipestates[statefrom])->ob_state = DISABLED;
if( mode == 1)
	objc_draw(pipedial,PIPECOST,0,x,y,w,h);
if(mode == 2)
	objc_draw(pipedial,0,10,x,y,w,h);
if(mode == 3)
	{
	(pipedial+pipestates[(int)wind->p2])->ob_state = SELECTED;
	objc_draw(pipedial,pipestates[(int)wind->p2],2,x,y,w,h);
	objc_draw(pipedial,PIPETO,2,x,y,w,h);
	}
}
/***************************************************************************/
BUTFUNC( butinpipe )		/*button pressed in well set up*/
{
int	x,y,w,h,obj,i;

wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
pipedial->ob_x = x;	pipedial->ob_y = y;
switch(obj = objc_find(pipedial,0,10,mx,my))
	{
	case	PIPESLD:
		slider(pipedial,PIPEBOX,PIPESLD,&(wind->p1),6.0,31.0,PIPETIME,2,x,y,w,h);
		drawpipe = 1;
		break;
	case	PIPEGO:		/*submit spec*/
		(pipedial+obj)->ob_state = SELECTED;
		objc_draw(pipedial,obj,1,x,y,w,h);
		if(obj == PIPEGO)
			startpipe(0,wind->gpp,(int)wind->p2,(int)wind->p1);
		(pipedial+obj)->ob_state = NORMAL|SHADOWED;
		close_wind(wh);
		break;
	default:
		for(i = 0 ; i <16 && pipestates[i] != obj; ++i)
			;
		if(( int)wind->p2 == i || i == 16)
			break;
		if( wind->gpp == i )
			break;
		if( wind->p2 >= 0 )
			{
			(pipedial+pipestates[(int)wind->p2])->ob_state = NORMAL;
			objc_draw(pipedial,pipestates[(int)wind->p2],2,x,y,w,h);
			}
		wind->p2 = i;
		drawpipe = 3;
		break;
	}
}
/*************************************************************************/
real	pricerail(from,to,amount,company)/*calculates the cost of rail transport for from,to*/
int	from,to,company;
real	amount;				/*in barrels*/
{
int	distance;
real	price;

if( amount == 0.0 || from == to )
	return( 0.0 );
distance = railmap[from][to];
price = (distance*railfact*amount)/10.0;
if(price == 0.0)
	price = -1.0;		/*impossible route*/
if(pipe[company][from] & (1<<to))
	return(0.0);
else
	return(price);
}
/**************************************************************************/
real	pricepipe( from, to, months )
int	from,to, months;
{
return( 5e3 * bldfact * (1+numjobs(PIPEJOB)) * (40-months) *c_dist(from, to));
}
/**************************************************************************/
random_events()			/*generates random happenings*/
{
int	type, p, time;
char	string[150];
JOBS	job;

type	= rand()%31;
switch(type)	{
	
	case	1:
	case	2:
		p	= rand()%256;
		if(plot[p].owner >= 0)
			break;
		time			= rand()%3+1;
		job.delay		= 30*time;
		job.type		= SELLJOB;
		job.p1			= p;
		addjob(&job);
		sprintf(string, st[PLOTSALE], statename[STATE(p)],
			TOPLOT(p), time);
		addmsg( string );
		plot[p].flags[0]	|= 0x80;
		drawstates		= 1;
		al_notice_auct( p );	/* give notice of auction taking place*/
		break;

	case	3:
		switch( rand()%10)
			{
#if	0
			case	1:
				random_well();	/* well blows up */
				break;
#endif
			case	2:
				random_store();	/* random destroying storage */
				break;
			case	3:
				random_money();	/* Government gives money to comp*/
				break;
			case	4:
				random_fine();	/* company fined */
				break;
			case	5:
				random_gusher();/* produce oil where none originally*/
				break;
			case	6:
				random_plot();	/* plot given randomly to company*/
				break;
#if	0
			case	7:
				random_lose_plot(); /* lose a plot */
				break;
#endif
			}
		break;
	default:
		break;
	}
}
/*****************************************************************************/
#if	0
random_lose_plot()
{
int	c, p;
char	text[300];

p	= rand()%256;
if( (c=plot[p].owner) >= 0 )
	{
	plot[p].owner	= -1;
	plot[p].type	= -1;
	sprintf( text, st[RANDOMLPLOT], TOPLOT(p),comp[c],statename[STATE(p)] );
	addmsg( text );
	drawstates	= 1;
	updt_sell_menu();
	}
}
#endif
/*****************************************************************************/
random_plot()
{
int	c, p;
char	text[300];

c	=rand()%4;
p	= rand()%256;
if( plot[p].owner < 0 )
	{
	plot[p].owner		= c;
	plot[p].val_land	= 0;
	sprintf( text, st[RANDOMPLOT], comp[c], TOPLOT(p), statename[STATE(p)]);
	addmsg( text );
	drawstates	= 1;
	updt_sell_menu();
	}
}
/*****************************************************************************/
random_gusher()
{
int	p, i, s, plotinstate;
char	text[300];
JOBS	job;

for( i = 0 ; i < 10 ; ++i)	/* max of 10 attempts to find plot with right criteria*/
	{
	p	= rand()%256;
	if( plot[p].owner < 0 && plot[p].reserves < 10000.0 )
		{
		plot[p].reserves	= 1e6;
		plotinstate		= TOPLOT(p);
		s			= STATE(p);
		sprintf( text, st[GUSHER], statename[s], plotinstate);
		addmsg( text );

		job.delay		= 60;
		job.type		= SELLJOB;
		job.p1			= p;
		addjob(&job);
		plot[p].flags[0]	|= 0x80;
		drawstates		= 1;
		al_notice_auct( p );	/* give notice of auction taking place*/
		break;
		}
	}
}
/*****************************************************************************/
biddo(job)
JOBS	*job;
{
int	p, ret;

p	= job->p1;
if( plot[p].owner >= 0 )
	return;				/* already sold somehow */
if( month_sale >= 0)
	{
	job->delay = 25;
	return;
	}
month_sale = p;
if(rand()%10 == 0)
	{
	ret	= startbid( p );
	if(!ret)		/*no room to sell*/
		{
		job->delay = 15;
			return;
		}
	domenu( BIDLAND );
	}
}
/*****************************************************************************/
random_money()				/* Government gives company some money*/
{
int	c;
char	text[300];

c	= rand()%4;
sprintf( text, st[GIVEMONEY], comp[c] );
addmsg( text );
cashchange( c, REV, 1e6);
}
/*****************************************************************************/
random_fine()
{
int	c;
char	text[300];

c	= rand()%4;
sprintf( text, st[RANDFINE], comp[c] );
addmsg( text );
cashchange( c, ACOST, 1e6);
}
/*****************************************************************************/
#if	0
random_well()				/* well blows up randomly */
{
int	p, off, i;
char	text[300];

if( numperiods < 5 )
	return;
off	= rand()%256;
for( i = 0 ; i < 256 ; ++i )
	{
	p	= (i+off)%256;
	if( plot[p].type >= 0 )
		{
		plot[p].type	= -1;
		sprintf( text, st[RANDBLOW], statename[STATE(p)], 
			comp[plot[p].owner] );
		addmsg( text );
		drawstates	= 1;
		updt_sell_menu();
		break;
		}
	}
}
#endif
/*****************************************************************************/
random_store()				/* randomly destroys storage */
{
int	s, i, off, c;
long	destroyed;
char	text[300];

c	= rand()%4;
off	= rand()%16;
for( i = 0 ; i < 16 ; ++i )
	{
	s	= (i + off)%16;
	destroyed	= storage[c][s]*( rand()/32768.0);
	if( destroyed == 0 )
		continue;
	sprintf( text, st[RANDSTORE], destroyed, comp[c], statename[s] );
	addmsg( text );
	storage[c][s]	= storage[c][s] - destroyed;
	drawstates	= 1;
	updt_sell_menu();
	break;
	}
}
/*****************************************************************************/


