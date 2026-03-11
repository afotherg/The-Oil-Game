/*****************************************************************************/
/*		Al's AI routines					     */
/*		Started	10 July 1987.					     */
/*****************************************************************************/
#include	"header.h"
#include	"variab.h"
#include	<assert.h>
real	capacity[4][16];
real	ai_prof_pb[4]		= {5.0, 5.0, 5.0, 5.0};	/* profit in $ */
real	ai_growth[4]		= {1.0,1.0,1.0,1.0};
real	ai_prop_buildst[4]	= {2.0,2.0,2.0,2.0};		/* propensity to build storage-
								minimum rev to costs ratio */
real	ai_interest[4]	  = {20.0,20.0,20.0,20.0};		/* interest rate assumed for investment*/
real	ai_profit[4]	  = {30.0,30.0,30.0,30.0};		/* required minimum mark up */
real	ai_dem_marg[4]	  = {1.5,1.5,1.5,1.5};		/* margin by which demand exceeds prop cap*/
real	ai_prop_borrow[4] = {1.0, 1.0, 1.0, 1.0};		/* limit of profit/interst paid */
real	ai_mcstpc[4]	  = {10.0,10.0,10.0,10.0};		/* percentage added to mcost*/
real	ai_pipe_prod[4]	  = {0.5,0.5,0.5,0.5};		/* factor prod * by for pipe trans */
real	ai_rail_prod[4]	  = {0.1,0.1,0.1,0.1};		/* factor prod * by for rail trans */
real	ai_prop_pros[4]	  = {3.0,3.0,3.0,3.0};		/* factor that profit > pros cost */
real	ai_store_ratio[4]  = {1.5,1.5,1.5,1.5};		/* ratio of storage got : storage needed*/
/*****************************************************************************/
al_short_tr(c, s)			/*Al's rules for transport */
int	c,s;				/*for company c		   */
{					/* transport rules first (to states*/
al_tr20(c, s);		/* change transport */
al_tr50(c, s);		/* begin transport rule */
al_tr60(c, s);		/* begin transport to state */
sellmin[c][s] = MIN( sellmin[c][s], 99.90 );
}
/*****************************************************************************/
al_short_pr(c, s)			/* rules for pricing/ production */
int	c, s;
{				/* production and pricing */
al_rule00(c, s);
al_rule20(c, s);
al_rule25(c, s);
sellmin[c][s] = MIN( sellmin[c][s], 99.90 );
}
/*****************************************************************************/
al_short1_pr( c, s)			/* AI rules for prod with rev max*/
int	c, s;
{					/* production and pricing */
if( c == 0 )
	al_rulerevmax(c, s);
else
	al_rule00( c, s );		/* never worked properly */
al_rule20(c, s);
al_rule25(c, s);
sellmin[c][s] = MIN( sellmin[c][s], 99.90 );
}
/*****************************************************************************/
al_rule00(c, s)				/* change price rule 	*/
int	c,s;
{
if( produce[c][s] == 0.0 && al_transin(c, s) == 0.0 && endsupply[c][s] < 10.0 )
	return;

if( endsupply[c][s] < 1.4*al_transout(c, s) + 10.0)
	sellmin[c][s] *= 1.05;
else
	sellmin[c][s] *= 0.97;
}
/*****************************************************************************/
al_rule20(c, s)				/* rule to raise production */
int	c, s;
{
real	mc, to;

if( capacity[c][s] == 0.0 && s != NY )
	return;			/* this rule exclusively for producers*/
mc	= al_mcost(c, s) * (100.0+ai_mcstpc[c])/100.0;/* slightly modified marg cost */
if( mc < sellmin[c][s] )
	{
	al_raiseprod( c, s);
	}

if( mc > sellmin[c][s] )
	{
	if( (endsupply[c][s]) >= 1.2*al_transout(c, s)) /* ie no transport out*/
		al_lowprod(c, s);
	else
		sellmin[c][s] *= 1.05;
	}
}
/*****************************************************************************/
al_rule25(c, s)				/* raise production for transport    */
int	c, s;
{
if(al_transout(c, s) > endsupply[c][s])	/*if more trans out than supply */
	if( !al_raiseprod(c,s) )
		sellmin[c][s] *= 1.05;
}
/*****************************************************************************/
al_tr20(c, s)		/* change amount to transport in rule */
{
int	from;

if( al_transin(c, s) == 0.0 )
	return;
for( from = 0 ; from < 16 ; ++from)
	{
	if( tranoil[c][from][s] == 0)
		continue;
	if( al_trmcost(c, from, s,(real)tranoil[c][from][s]) > sellmin[c][s] )
		tranoil[c][from][s] = MAX(0, tranoil[c][from][s] * 0.95 - 1);
	else
		if(al_transin(c, s)<0.9*storage[c][s] && endsupply[c][s]<10.0)
			tranoil[c][from][s] = MIN(1 + tranoil[c][from][s] * 1.05, 30000);
	}
}
/*****************************************************************************/
al_tr50(c, s)		/* begin transporting rule */
int	c, s;
{
int	from, highst = -1;
real	profit = -9.0e9, amt, cost,railcost;

if( al_transin(c, s) > 0.0)
	return;				/* doesn't concern this rule */
if( supply[c][s] > 1.0 )
	return;				/* don't bother anymore here */
if( produce[c][s] > 0.0 )
	return;				/* don't transport to where prod is */
if( storage[c][s] == 0.0)
	return;				/* no storage - no transport */

for( from = 0 ; from < 16 ; ++from)
	{
	if( from == s )
		continue;
	if( ( amt =(supply[c][from]- al_transout(c, from)) ) < 10.0)
		continue;		/* if no oil to transport */
	if( (railcost =  pricerail	(from, s, amt, c)) < 0.0 )
		continue;
	amt = MIN(amt/1.1, storage[c][s]);
	amt = MIN(amt, 30000.0);	/* max allowed is 30000 barrels*/
	cost = al_costprod(c, from) + railcost/amt;
	if( amountsold[s] == 0.0)	/* if no competition*/
		{
		if( al_estdem( s, cost * 1.4) > amt / 2.0)
			{
			sellmin[c][s] = cost * 1.4;	/*cost + 40% */
			tranoil[c][from][s] = amt;
			break;
			}
		}
	if( amountsold[s] > 0.1)
		if( al_minprice(s) > cost * 1.1 )
			{
			sellmin[c][s] = cost * 1.1; /*aggressively under*/
			tranoil[c][from][s] = amt;
			break;
			}
	}
}
/*****************************************************************************/
al_tr60( c, s)			/* does transporting when no extra supply */
int	c, s;
{
int	from;
real	estdem;

if( amountsold[s] > 0.0 )
	return;
if( storage[c][s] == 0.0 )
	return;
if( al_transin( c, s ) > 0.0 )
	return;

for( from = 0 ; from < 16 ; ++from )
	{
	if( from == s )
		continue;
	if( railmap[from][s] == 0 || (pipe[c][from]&(1<<s)))
		continue;
	if( produce[c][from] < 10.0 )
		continue;
	estdem = al_estdem( s, 99.90 );
	tranoil[c][from][s] = MIN( estdem/2.0, 30000 );
	sellmin[c][s] = 99.90;
	break;
	}
}
/*****************************************************************************/
real	al_minprice(s)
int	s;
{
real	min = 100.0;
int	c;

for(c = 0 ; c < 4; ++c)
	if(produce[c][s] > 0.0 && sellmin[c][s] < min)
		min = sellmin[c][s];
return	(min);
}

/*
**	finds cheapest marginal cost from a well that can have its production
**	increased
*/

real	al_mcost(c,s)
int	c,s;
{
int	x, y, ref, p, type;
real	mcost = 9.0e9,output,temp;

ref = REFPLOT(s);
for(y = 0 ; y < 4 ; ++y)
	{
	for(x = 0 ; x < 4 ; ++x)
		{
		p = ref + y*16 + x;
		if(plot[p].owner == c && (type = plot[p].type) >= 0)
			{
			if( plot[p].output > 1.8*param_wells[type].capacity )
				continue;
			output	= plot[p].actual;
			if( (temp = marg_cost(output, type)) < mcost)
				mcost = temp;
			}
		}
	}
if( s == NY && price_imp < mcost )
	return( price_imp );
return( mcost );
}
/*****************************************************************************/
al_raiseprod(c,s)			/*raise production in cheapest well*/
int	c,s;				/* TRUE if can do it */
{
int	minp;
real	output;

output	= al_output( c, s) + ((s == NY)?(import[c]):(0.0));
if( output > storage[c][s] )
	return( FALSE );		/* cant't raise output */
minp	= al_min_prod_cost( c, s);
if( minp == -2 )			/*
					** means that minimum marg cost
					** is in imported oil
					*/
	{
	if( import[c] < 100.0 )
		import[c] = 100.0;
	import[c] *= 1.05;
	return( TRUE );
	}

if( minp != -1 )
	{
	if( plot[minp].output < 100.0 )
		plot[minp].output = 100.0;
	plot[minp].output = MIN(2.0*param_wells[plot[minp].type].capacity,
				plot[minp].output * 1.05);
	return( TRUE );
	}
return( FALSE );
}
/*****************************************************************************/
al_lowprod(c,s)			/*lower production in most expensive well*/
int	c,s;
{
int	maxp;

maxp = al_max_prod_cost( c, s);
if( maxp == -2 )
	{
	import[c] *= 0.95;
	return;
	}
if(maxp != -1)
	plot[maxp].output = MAX(10.0,plot[maxp].output * 0.95);
}
/*****************************************************************************/
real	al_costprod(c, s)	/*average cost of production per barrel*/
int	c,s;
{
real	price, amt;
int	to;

if( produce[c][s] == 0.0 )
	{
	for( to = 0 ; to < 16 ; ++to )
		{
		if( (amt = tranoil[c][s][to])  > 0.0 )
			{
			price = (produce[c][to] == 0.0)?(90.0):
				(expstate[c][to]/produce[c][to] +
				pricerail(s, to, amt, c)/amt );
			return( price );
			}
		}
	return( 90.0 );
	}
			
price = expstate[c][s] / produce[c][s];
return (price);
}
/*****************************************************************************/
real	al_avepri(s)		/*trade weighted price in state*/
int	s;
{
real	totrev = 0.0, ave;
int	c;

for(c = 0 ; c < 4 ; ++c)
	totrev += cashstate[c][s];
if(amountsold[s] < 10.0)
	return (0.0);
ave = totrev / amountsold[s];

return( ave );
}
/****************************************************************************/
real	al_transin(c,s)		/*oil transported in (positive)*/
int	c,s;
{
int	i;
real	amount = 0.0;

for(i = 0 ; i < 16 ; ++i)
	amount += tranoil[c][i][s];
return( amount );
}
/****************************************************************************/
real	al_transout(c, s)		/*oil transported out*/
int	c,s;
{
int	i;
real	amount = 0.0;

for(i = 0 ; i < 16 ; ++i)
	amount += tranoil[c][s][i];
return( amount );
}
/****************************************************************************/
real	al_estdem(s, price)		/*gives estimated demand in state*/
int	s;
real	price;
{
real	est_dem;

est_dem = demandset(price, s) * 0.89;
return( est_dem );
}
/****************************************************************************/
real	al_trmcost(c, from, to, amt)	/*transport marginal cost for from,to*/
int	c, from, to;
real	amt;
{
real	cost;

cost = ( pricerail(from, to, amt+100.0, c) - pricerail(from, to, amt, c))/100.0;
cost += al_mcost(c, from) * (100.0+ai_mcstpc[c])/100.0;
return( cost );
}
/*****************************************************************************/
al_rulerevmax(c,s)			/* revenue maximisation rule */
int	c,s;
{
real	oldrev,newrev,oldprice,newprice;

if( produce[c][s] == 0.0 && al_transin(c, s) == 0.0 && endsupply[c][s] < 10.0 )
	return;

oldrev	 = histrev[c][s][HISTPNTQNT-2];
newrev   = histrev[c][s][HISTPNTQNT-1];
oldprice = histprice[c][s][HISTPNTQNT-2];
newprice = histprice[c][s][HISTPNTQNT-1];

if( newrev > oldrev )
	{
	if( newprice <= oldprice )
		sellmin[c][s] *= 0.95;
	else
		sellmin[c][s] *= 1.05;
	}

if( newrev < oldrev )
	{
	if( newprice < oldprice )
		sellmin[c][s] *= 1.05;
	else
		sellmin[c][s] *= 0.95;
	}

if( newrev < 100.0 && sellmin[c][s] > 20.0)
	sellmin[c][s] *= 0.6;
if( rand()%50 == 1 )
	sellmin[c][s] *= 0.75;		/* to liven things up a bit */
if( sellmin[c][s] < 0.05)
	sellmin[c][s] = 0.05;
if( s == NY && import[c] > 0.0 )
	sellmin[c][NY] = MAX( price_imp*1.05, sellmin[c][NY] );
}

/*
**	returns plot number with maximum marg cost if prod is zero, then
**	ignores.
**	returns -1 if no plot found
**	returns -2 if maximum cost is in imported oil from New York
*/
al_max_prod_cost( c, s)
int	c,s;
{
int	x, y, ref, p, maxp = -1;
real	mcost = -9.0e9, output, temp;

ref = REFPLOT(s);

for(y = 0 ; y < 4 ; ++y)
	{
	for(x = 0 ; x < 4 ; ++x)
		{
		p = ref + y*16 + x;
		if(plot[p].owner != c || plot[p].type < 0)
			continue;
		output	= plot[p].output;
		if((temp = marg_cost(output,plot[p].type)) > mcost)
			{
			maxp  = p;
			mcost = temp;
			}
		}
	}
if( s == NY && price_imp > mcost && import[c] > 10.0 )
	return( -2 );			/*
					** indicates that max marg cost is from
					** imported oil
					*/
return( maxp );
}

/*
**	Finds cheapest plot to raise production in.
**	If no plot found, then  -1 is returned.
**				-2 is returned if cheapest is imported from NY
*/

al_min_prod_cost( c, s)
int	c,s;
{
int	x, y, ref, p, minp = -1, type;
real	mcost = 9.0e9,output,temp;

ref = REFPLOT(s);
for(y = 0 ; y < 4 ; ++y)
	{
	for(x = 0 ; x < 4 ; ++x)
		{
		p 	= ref + y*16 + x;
		type	= plot[p].type;
		if( plot[p].owner != c || type < 0 )
			continue;
		output	= plot[p].output;
		if( output < 0.95 * 2.0 * param_wells[type].capacity )
			{
			if((temp = marg_cost(output,plot[p].type)) < mcost)
				{
				minp  = p;
				mcost = temp;
				}
			}
		}
	}
if( (s == NY) && price_imp < mcost )
	return( -2 );
return( minp );
}
/*****************************************************************************/
real	al_tot_dem(c, s, price)	/* gives a value for total demand, including */
int	c,s;			/* states transportable to		     */
real	price;
{
int	i;
real	dem;

dem	= al_estdem( s, price );
for( i = 0 ; i < 16 ; ++i )
	{
	if( i == s )
		continue;
	if( pipe[c][i] && 1<<s )
		{
		dem	+= ai_pipe_prod[c] * al_estdem( s, price);
		continue;
		}
	if( railmap[i][s] )
		dem	+= ai_rail_prod[c] * al_estdem( s, price);
	}
return( dem );	
}
/*****************************************************************************/
real	al_capacity( c, s )	/* gives oil production capacity in a state */
int	c, s;			/* including that being built */
{
int	x, y, ref, type, p, days;
real	capacity = 0.0;

ref = REFPLOT(s);

for( x = 0 ; x < 4; ++x ) {
	for( y = 0 ; y < 4 ; ++y ) {
		p = ref + x + y*16;
		if(plot[p].owner == c && (type = plot[p].type) >= 0 )
			capacity += MAX( plot[p].output, param_wells[type].capacity );
		if( (type = well_bng_blt( c, p, &days)) >= 0 )
			capacity += param_wells[type].capacity;
		}
	}
return ( capacity );
}
/*****************************************************************************/
real	al_prod_tr( c, s)	/* oil capacity around a state 		     */
int	c, s;			/* ie. that which can be transported in      */
{
int	i;
real	amount	= 0.0;

for( i = 0 ; i < 16 ; ++i )
	{
	if( i == s )
		continue;
	if( pipe[c][i] & (1<<s) )
		{
		amount	+= ai_pipe_prod[c] * capacity[c][i];
		continue;
		}
	if( railmap[i][s] )
		amount	+= ai_rail_prod[c] * capacity[c][i];
	}
return( amount );
}
/*****************************************************************************/
real	al_store_need( c, s )	/* returns total storage needed at moment */
int	c, s;
{
real	needed;

needed = capacity[c][s] + al_transin( c, s ) + al_prod_tr( c, s);
if( s == NY )
	needed += import[c];
return( needed );
}
/*****************************************************************************/
al_build_store( c, s)	/* builds storage in s if storage < 1.5*needed */
int	c, s;		/* primarily financed out of retained profits  */
{
real	needed, cost, yearrev, yearcost, intpq;

if( sphere[c][s] <= 0 )
	return;
if( (needed = al_store_need( c, s)) == 0.0 )
	return;
if( (storage[c][s] + store_beingbuilt[c][s])/needed < ai_store_ratio[c] )
	{
	cost		= pricestore( 4, 1000.0 );
	yearrev		= 365.0 * 1000.0 * (sellmin[c][s] - al_mcost( c, s));
	yearcost	= (cost * ai_interest[c])/100.0;
	intpq		= (debentures[c]+cost)*ai_interest[c]/(100.0*4.0);
	if( storage[c][s] != 0.0 && profit[c]/intpq < ai_prop_borrow[c] )
		return;
	if( (storage[c][s] != 0.0) && (yearrev/yearcost < ai_prop_buildst[c]) )
		return;			/* don't bother building */
#if	0
	if ( cashchange( c, RDCASH)-pros_tax[c] < cost)
		{
		loan = 2*(cost + pros_tax[c] - cashchange(c, RDCASH) );
		if( maxloan(c) > loan )
			startloan( c, MAXLOANTERM, loan );
		else
			return;
		}
#endif
	startloan( c, MAXLOANTERM, cost );
	startstore( c, 4, s, 1000.0 );
	}

}
/*****************************************************************************/
al_buildwell( c, s )	/* build a well in state */
int	c, s;
{
int	ref, x, y, p, i, type, bstwell = -1;
real	capblt, cost, cap, dcost, dprof = -2.0e10, dem;

capblt	= capacity[c][s];
ref	= REFPLOT( s );
for( x = 0 ; x < 4 ; ++x )
	{
	for( y = 0 ; y < 4 ; ++y )
		{
		p = ref + x + y*16;
		if( plot[p].owner != c || (plot[p].reserves <= 1000.0 ) )
			continue;
		if( plot[p].flags[c] & 8 )
			continue;		/* well being built   */
		if( plot[p].type  != -1 )
			continue;		/* already well there */
		if( rand()%10 )
			continue;		/* randomness/intelligence */
		for( i = 0 ; i < 4 ; ++i )
			{
			type	=  whichwell[i];
			cap	=  2.0*param_wells[type].capacity;
			cost	=  pricewell( type, 6 );
			do
				{
				cap	/= 2.0;
				dcost	=  total_cost( cap, type, c);
				dcost	+= (pricewell(type, MAXTMWELL)*ai_interest[c])/(365.0*100.0);
				dem	= al_tot_dem( c, s, dcost*((100.0+ai_profit[c])/100.0)/cap);
				}
				while( dem < ai_dem_marg[c]*(cap + capblt) && cap > 10.0);
			if( /* (cap*8.0*30.0*6.0/3.0) > plot[p].reserves ||*/ cap <= 10.0 )
				continue;		/* insufficient oil */

			if( dcost > dprof )
				{
				dprof	= dcost;
				bstwell	= type;
				}
			}
		if( bstwell != -1 )
			{
			cost	= pricewell( bstwell, MAXTMWELL );
#if	0
			if( 2.0*maxloan( c ) > cost )
#endif
				{
				startloan( c, MAXLOANTERM, cost );
				startwell( MAXTMWELL, bstwell, p, c );
				}
			return;
			}
		}
	}
}
/*****************************************************************************/
monopoly( s )			/* returns company with monopoly in s */
{				/* -ve if no monopoly */
int	i, c = -1;

for( i = 0 ; i < 4 ; ++i)	{
	if( sold[i][s] > 10.0 )
		if( c == -1 )
			c = i;
		else
			return( -1 );
	}
return( c );			/* will return -1 if none sold also */
}
/*****************************************************************************/
real	al_output( c, s )	/* gives what production set to in state */
int	c, s;
{
int	x, y, ref, type, p;
real	output = 0.0;

ref = REFPLOT(s);

for( x = 0 ; x < 4; ++x ) {
	for( y = 0 ; y < 4 ; ++y ) {
		p = ref + x + y*16;
		if(plot[p].owner != c || (type = plot[p].type)<0 )
			continue;
		output	+= plot[p].output;
		}
	}
return ( output );
}
/*****************************************************************************/
real	al_maxbid(c, p )		/* max bid offered by company */
int	c, p;
{
int	s;
real	maxbid, res, totcap, sph, term, ufd, dcost;

s	= STATE( p );
totcap	= capacity[0][s] + capacity[1][s] + capacity[2][s] + capacity[3][s];
if ( (sph = sphere[c][s]) <= 0.0 )
	return( 0.0 );

res	= plot[p].reserves;
prosjob( c, p, 2, &res );
ufd	= ai_growth[c] * al_tot_dem( c, s, 0.0) - totcap;/* un furnished demand */
ufd	= MIN( ufd, 3000.0);
term	= MIN( res/ufd, (5.0*360.0/3.0));	/* assume over max of 5 years */
maxbid	= ufd * ai_prof_pb[c] * log10( sph+1.0 ) * term;

dcost	= maxbid * ai_interest[c]/(365.0*100.0/3.0);
if( dcost > 0.75*ufd*ai_prof_pb[c] )
	maxbid	= ufd*ai_prof_pb[c]*0.75*(365.0*100.0/3.0)/ai_interest[c];
#if	0
maxbid	= MIN( maxbid, 2.0 * maxloan( c ) );
#endif
return( maxbid );
}
/*****************************************************************************/
al_get_bids( p )		/* get AI bids for plot p */
int	p;
{
int	c, i, o, off;
real	maxoff, maxbid;
static	int	plotknown = -1;
static	real	known_bids[4];

if( p != plotknown )
	{
	for( c = 1 ; c < 4 ; ++c )
		known_bids[c]	= al_maxbid(c, p);
	plotknown	= p;
	}		/* neat huh? To speed up auction processing */

off	= rand()%4;
for ( i = 0 ; i < 3 ; ++i )
	{
	c	= 1 + (i+off)%3;
	maxoff	= 0.0;
	for( o = 0 ; o < 4 ; ++o)
		{
		if( c == o )
			continue;
		if( bidtype.offer[o] > maxoff )
			maxoff	= bidtype.offer[o];
		}
	maxbid	= known_bids[c];
	if( 	   bidtype.dutch
		&& maxbid > bidtype.reserve
		&& rand()%40 == 0 ) /*randomness==intelligence*/
			{
			bidtype.offer[c] = -1.0;	/* signals buying */
			break;
			}
	if( rand()%200 == 0)
		bidtype.offer[c] /=  2;
	else
		{ 
		if(      0.95 * bidtype.offer[c] > maxoff )
			bidtype.offer[c] *= 0.95;
		else
			{
				if( (bidtype.offer[c] < maxbid)
				&&  (bidtype.offer[c] <= maxoff 
				|| bidtype.offer[c] <= bidtype.reserve) )
					bidtype.offer[c] *= ((100.0 + rand()%10)/100.0);
			}
		}
	if(maxbid > bidtype.reserve)
		bidtype.offer[c] = MAX(bidtype.offer[c], 1.05 * bidtype.reserve);
	}
}
/*****************************************************************************/
al_notice_auct( p )			/* auction about to take place in p */
int	p;
{
int	c, s, prostype;

prostype	= rand()%4;
s	= STATE( p );
for( c = 1 ; c < 4 ; ++c )
	{
	if( plot[p].flags[c] & 1 )
		continue;
	if( rand()%50 > sphere[c][s] )	/* ie sphere of 100 makes v. likely */
		continue;		/* no great interest in it */
	if( (pros_profit[c]+pros_tax[c])/proscost[0] < ai_prop_pros[c] || cashchange( c, RDCASH ) < 2*proscost[0] )
		continue;
	startpros( prostype, c, p);
	}
}
/*****************************************************************************/
real	costvp( type )
int	type;
{
static	real	base_cost[] = {90000.0, 50000.0, 20000.0, 10000.0, 30000.0};

return( MIN( base_cost[type] * labfact, 999999.0) );
}

/*
**	This is an ai pricing routine to undercut when competition
**
*/
al_undercut( c, s)
int	c, s;
{
int	i;
real	min = 100.0;

al_rule20( c, s);
al_rule25( c, s);
for( i = 1 ; i < 4 ; ++i )
	{
	if( (sold[i][s] > 0.0 || endsupply[i][s] > 0.0) && sellmin[i][s] < min )
		min = sellmin[i][s];
	}
sellmin[0][s]	= min - 0.05;		/* undercut by $0.05 */

}

/*
**	searches vp_cont array,and does AI for Gitty
*/
al_gitty()
{
int	s,type;
static	int	(*func[])() = {al_short_pr, al_short1_pr, al_undercut, 
			al_short1_pr};

for( s = 0 ; s < 16 ; ++s)
	{
	if( (type = vp_cont[s]) >= 0)
		{
		if( type < 4 )
			{
			ai_mcstpc[0]	= 5 + type*5;
			(*func[type])(0, s);
			cashchange( 0, ACOST, costvp(type) / 365.0);
			}
		}
	}
}
/*****************************************************************************/
al_init_cap( c )			/* initialises the capacity array    */
int	c;
{
register	int	s;

for( s = 0 ; s < 16 ; ++s )
	capacity[c][s] = al_capacity(c, s);
}
/*****************************************************************************/
change_ai(c, param, ins)			/* change AI parameters from story */
int	c, param, ins;
{
switch( ins )
	{
	case	AIPROFPB:
		ai_prof_pb[c]		= param/10.0;
		break;
	case	AIGROWTH:
		ai_growth[c]		= param/10.0;
		break;
	case	AIPROPBUILDS:
		ai_prop_buildst[c]	= param/20.0;
		break;
	case	AIPROFIT:
		ai_profit[c]		= param;
		break;
	case	AIDEMMARG:
		ai_dem_marg[c]		= param/20.0;
		break;
	case	AIPROPBORROW:
		ai_prop_borrow[c]	= param/20.0;
		break;
	case	AIMCSTPC:
		ai_mcstpc[c]		= param;
		break;
	case	AIPIPEPROD:
		ai_pipe_prod[c]		= param/100.0;
		break;
	case	AIRAILPROD:
		ai_rail_prod[c]		= param/100.0;
		break;
	case	AIPROPPROS:
		ai_prop_pros[c]		= param/20.0;
		break;
	case	AISTORERATIO:
		ai_store_ratio[c]	= param/20.0;
		break;
	}
}
/*****************************************************************************/

/*
**	This routine checks if the value of land is more than book value
**	If it is, then land is sold
*/

al_sell_land( c, s)
int	c, s;
{
int	x, y, rp, p;
real	value;

rp	= REFPLOT( s );

for( x = 0 ; x < 4 ; ++x )
	{
	for( y = 0 ; y < 4 ; ++y )
		{
		p	= rp + x + y*16;
		if( rand()%10)
			continue;
		if( plot[p].owner != c )
			continue;
		if( plot[p].type >= 0 )
			continue;
		if( plot[p].flags[c]&8)
			continue;
		val_land( p, &value );
 		if( value > 1.5*plot[p].val_land*10000.0 )
			sell_land( p );
		}
	}
}


