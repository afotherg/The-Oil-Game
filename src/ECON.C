/*****************************************************************************/
/*    This file holds routines concerned with economics of MEGA GAME         */
/*               started        14 May      1986                             */
/*               this version   08 March    1987                             */
/*****************************************************************************/
#include	"header.h"
#include	"variab.h"
extern	GRECT	dumpos;
extern	int	easy;



#define	ENDQUANT	65		/* quanta finished */

struct	score {
	char	name[11];
	real	reserves;
	}   scoretable[10];

/*
**
**	This routine displays the high score table
**
*/
DRAWFUNC( hsdisp )
{
int	i;
char	money[30];
char	text[50];
int	x1, y1, w1, h1;

cls( wh );
wind_get( wh, WF_WORKXYWH, &x1, &y1, &w1, &h1);
for( i = 0 ; i < 10 ; ++i ) {
	formnum( money, scoretable[i].reserves );
	sprintf( text, "%-10s  $%-15s", &scoretable[i].name[0], money );
	v_gtext( vdi_handle, x1, y1+(i+1)*gl_hchar, text );
	}
}



/*
**	Max allowed to be drilled
*/
real	maxallowed(point)
PLOT *point;
{
if( point->reserves > 1000.0 )
	return( point->reserves );
return( 0.0 );
}



production()
{
int	plotnum, c, s;
real	possprod, toimp;
PLOT	*point;
real		cost;

point = (PLOT*)&(plot[0]);
for(plotnum = 0 ; plotnum < 256; ++plotnum,++point)
	{
	point->actual = 0;/*nb in case point->output is zeroed elsewhere*/
	c	= point->owner;
	if( c >= 0 && point->type >= 0)
		{
		possprod	= maxallowed( point );
		possprod	= MIN(point->output,possprod);
		s		= (plotnum/64)*4+(plotnum%16)/4;
		possprod	= MIN(possprod,storage[c][s]-supply[c][s]-produce[c][s]);
		possprod	*= (1.0 - 0.1*(rand()/32768.0));
		produce[c][s] 	+= possprod;
		cost		= total_cost(possprod, point->type, c);
		expstate[c][s]	+= cost;
		point->actual	= possprod;
		alterreserves(plotnum,possprod);
		}
	}
for(c = 0 ; c < 4 ; ++c)
	{
	toimp = MIN( import[c], storage[c][NY]-supply[c][NY]-produce[c][NY]);
	produce[c][NY]		+= toimp;
	expstate[c][NY]		+= toimp*price_imp;
	for( s = 0 ; s < 16 ; ++s )
		supply[c][s] += produce[c][s];
	}
}
/****************************************************************************/
real	demandset(price,state)
real	price;
int	state;
{
real	dem;

dem = demand[state] * (      1 + cos( (day%365) * 1.72E-2)/5     );
return(dem * exponent(-(price/demandfact )));
}
/***************************************************************************/

/*
**	new sell routines
*/

sell( s )			/*sell in state*/
int	s;
{
register	int	i, c;
int	flag;
real	partsold, rundemand, oldprice[16], min, sup;
extern	real	findmin(),demandset();

amountsold[s]	= 0.0;

do
	{
	flag = 0;
	for(i = 0 ; i < 4 ; ++i)
		oldprice[i] = sellmin[i][s];
	min = findmin(-1, oldprice, s);
	rundemand = demandset(min, s );
	if(rundemand > amountsold[s] + 10.0)
		{
		for(c = 0 ; c < 4; ++c )
			{
			sup	= supply[c][s] - al_transout(c, s);
			if( sup > 1.0  && sellmin[c][s] == min)
				{
				partsold	= MIN(rundemand, sup );
				amountsold[s]	+= partsold;
				supply[c][s]	-= partsold; 
				sold [c][s]	+= partsold;
				cashstate[c][s]	+= min*partsold;
				flag = 1;
				}
			}
		}
	}
	while(flag);
}

real	findmin(company,oldprice,state)
int	company;
real	oldprice[4];
int	state;
{
int	i;
real	min = 10000.0;

for(i = 0 ; i < 4; ++i)
	if((company!=i)&&(supply[i][state]>0.0)&&(oldprice[i]<min))
		min = oldprice[i];
return(min);
}

/*
*/
alterreserves(plotnum, possprod)
int	plotnum;
real	possprod;
{
plot[plotnum].reserves -= (( 1.0 + rand()/32000.0)*possprod);
}

/*
*/
double exponent(num)
real num;
{

if(num > 0.0)
	return( exp(num) );

return( 1.0/exp(-num) );
}

/*
*/
historicqnt( c )	/*alter all historic variables storing quantised data*/
int	c;
{
register	int	i, s;

for(s = 0 ; s < 16 ; ++s)
	{
	for(i = 1 ; i < HISTPNTQNT ; ++i)
		{
		histsold[c][s][i-1]	= histsold[c][s][i];
		histproduce[c][s][i-1]	= histproduce[c][s][i];
		histprice[c][s][i-1]	= histprice[c][s][i];
		histsupp[c][s][i-1]	= histsupp[c][s][i];
		histrev[c][s][i-1]	= histrev[c][s][i];
		histcost[c][s][i-1]	= histcost[c][s][i];
		}
	histsold[c][s][HISTPNTQNT-1]	= sold[c][s];
	histproduce[c][s][HISTPNTQNT-1]	= produce[c][s];
	histprice[c][s][HISTPNTQNT-1]	= sellmin[c][s]*100.0;
	histsupp[c][s][HISTPNTQNT-1]	= endsupply[c][s];
	histrev[c][s][HISTPNTQNT-1]	= cashstate[c][s];
	histcost[c][s][HISTPNTQNT-1]	= expstate[c][s];
	}
for(i = 1 ; i < HISTPNTQNT ; ++i)
	histcash[c][i-1] = histcash[c][i];
histcash[c][HISTPNTQNT-1] = cashchange(c,RDCASH);
}
/****************************************************************************/
real	marg_cost(output,type)
real	output;
int	type;
{
real	m1,m2;

m1 = total_cost(output+100.0,type, 0 );
m2 = total_cost(output, type, 0);
return( (m1 - m2) / 100.0);
}
/****************************************************************************/
real	total_cost(output,type ,c)	/*cost of production with type for output*/
real	output;
int	type, c;
{
real	cost,prod;

prod = param_wells[type].capacity;
cost = 0.5 * output * ( 1 + (output/prod) * (output/prod)) * 
param_wells[type].lab_cost*labfact;
if( c == 0 )
	cost += param_wells[type].capacity;		/* 1$/barrel overhead */
return(cost);
}
/****************************************************************************/ 
dodemch()	/*change demands according to percentages in changedem*/
{
int	state;

STATEFOR(state)
	demand[state] += (demand[state] * changedem[state])/(1000.0*30.0);	
bldfact += (bldfact * chbldfact)/(1000.0*30.0); 
railfact += (railfact*chrailfact)/(1000.0*30.0);
labfact  += (labfact*chlabfact)/(1000.0*30.0);
}
/*****************************************************************************/
real	maxprod( c, s)	/* works out max production by company in state */
int	c,s;
{
int	i,plotnum,type;
real	output = 0.0;

for( i = 0 ; i < 16 ; ++i )
	{
	plotnum = (s/4)*64 + (s%4)*4 ;
	plotnum += (i/4)*16 + (i%4);
	if( (plot[plotnum].owner != c) || (type=plot[plotnum].type) < 0 )
		continue;
	output += param_wells[type].capacity;
	}
return( output );
}
/*****************************************************************************/
econ_loop()
{
static	int	s, part = 100, type = 0, c;
static	unsigned	long	timethen;
static	int	daysgone;
unsigned	long	timenow;

switch( part )
	{
	case	0:
		time( &timethen);
		initquant();
		updt_sell_menu();
		part = 3;
		break;
	case	3:
		handlejobs();
		part = 8;
		break;
	case	8:
		production();
		s	= 0;
		part	= 11;
		break;
	case	11:
		sell(s++);
		if( s == 16 )
			part = 27;
		break;
	case	27:
		endquant();
		c	= 0;
		part = 29;
		break;
	case	29:
		al_init_cap( c );	/* initialise capacity array for AI*/
		if( ++c == 4 )
			{
			s	= 0;
			part	= 31;
			}
		break;
	case	31:
		al_short_tr(1, s);
		al_short_tr(2, s);
		al_short_tr(3, s);
		if( ++s == 16)
			{
			s 	= 0;
			part	= 32;
			}
		break;
	case	32:
		al_short1_pr( 1, s);
		al_short_pr( 2, s);
		al_short_pr( 3, s);
		if( ++s == 16 )
			{
			s	= 0;
			part	= 34;
		}
		break;
	case	34:
		switch( type )
			{

			case	0:
				al_sell_land( 1, s);
				al_sell_land( 2, s);
				al_sell_land( 3, s);
				break;
			case	1:
				al_buildwell( 1, s);
				al_buildwell( 2, s);
				al_buildwell( 3, s);
				break;
			case	2:
				al_build_store( 1, s);
				al_build_store( 2, s);
				al_build_store( 3, s);
				break;
			}
		if( ++s == 16 )
			{
			part = 36;
			if( ++type == 3)
				type = 0;
			}
		break;
	case	36:
		al_gitty();	/* do VP work for Gitty */
		part	= 38;
		break;
	case	38:
		do_trans();	/*outer level transport routine*/
		part = 40;
		break;
	case	40:
		accquant();
		part = 50;
		break;
	case	50:
		random_events();
		part	= 55;
		break;
	case	55:
		{
		static	int	daysbank[4];	/* days that bankrupt*/
		check_bank( daysbank );		/* check if a company is bankrupt */
		dodemch();		/*change demands*/
		drawall();
		c	= 0;
		part	= 60;
		if( daysbank[0] == 4 )
			{
			part	= 120;
			form_alert( 1, st[GAMEOVER] );
			}
		}
		break;
	case	60:
		if( daysgone == 3 )
			{
			historicqnt( c );
			if( ++c == 4 )
				{
				drawhist = 1;
				part	 = ENDQUANT;
				}
			}
		break;

	case	ENDQUANT:
		daysgone	= 0;
		part = 0;
		if( dd < 4 )
			part	= 130;		/* end of month meeting */
		break;





	case	100:		/* wait for newspaper to close */
		{
		extern	int	printnews();
			if( !ison( printnews ) )
			{
			part = 0;
			clear_intray();
			}
		break;
		}

	case	120:
		endfisc();		/* close accounts*/
		hiscore();
		open_window( 20, 40, 30*gl_wchar, 12*gl_hchar,
		NAME|MOVER, (easy)?(st[EASYTL]):(st[HARDTL]),2,
		hsdisp, 0L, 0L, 0L, 0L, 0L, 0, 0.0, 0.0,&dumpos, 0L);
		part	= 150;		/* just wait to finish */
		break;

	case	130:
		part	= 135;
		if( month_sale >= 0 )
			{
			if( plot[month_sale].owner >= 0 )
				{
				month_sale	= -1;
				break;
				}
			if( !startbid( month_sale ) )
				break;
			month_sale	= -1;
			domenu( BIDLAND );
			part	= 131;
			break;
			}
		break;

	case	131:
		if( bidtype.plot < 0 )
			part	= 135;
		break;
			
	case	135:
		if( mm%2 == 0 )
			if( dovpmonth() )
				break;
		part	= 140;
		break;
	case	140:
		if( mm%6 == 0 )
			{
			++numperiods;
			if( easy && numperiods >= 6 )
				{
				form_alert( 1, st[FINEASY]);
				part	= 120;
				break;
				}
			if( !easy && numperiods >= 39 )
				{
				form_alert( 1, st[FINHARD] );
				part	= 120;
				break;
				}

			endfisc();	/*end fiscal period*/
			initfisc();
			if( autoopen )
				{
				closeall();
				domenu(BALMEN);
				domenu(PANDLMEN);
				domenu(OPNPAPER);
				part = 100; /* will wait for news to
						close */
				break;
				}
			else
				clear_intray();
			}
		part	= 0;
		break;

	case	150:			/* wait for user to exit */
		break;
	}
bidtime();

time( &timenow);

if( daysgone != 3 && part <= ENDQUANT )
	{
	if( (timenow - timethen) >= 2 )
		{
		++day;
		chdate();
		++daysgone;
		timethen	= timenow;
		}
	}
}
/*****************************************************************************/
check_bank( daysbank )
int	daysbank[];
{
int	c;

for( c = 0 ; c < 4 ; ++c )
	{
	if( cashchange( c, RDCASH ) >= 0.0 )
		daysbank[c]	= 0;
	else
		++daysbank[c];
	}
if( daysbank[0] == 1 )			/* check Gitty's position */
	addmsg( st[BANKRUPT1] );
if( daysbank[0] == 2 )
	addmsg( st[BANKRUPT2] );
if( daysbank[0] == 4 )
	addmsg( st[BANKRUPT3] );
}



/*
**	gives distance between 2 states
*/
c_dist( from, to )
int	from, to;
{

static	char	dist[16][16] = {
{0,1,2,3,5,1,1,2,2,3,3,4,3,4,4,5},
{1,0,1,2,4,2,1,2,1,2,2,3,3,3,3,4},
{2,1,0,1,3,3,2,2,1,2,2,2,3,3,3,4},
{3,2,1,0,2,3,2,2,1,2,1,1,3,2,2,3},
{5,4,3,2,0,5,4,4,3,3,2,1,4,3,2,3},
{1,2,3,3,5,0,1,2,2,3,3,4,3,4,4,5},
{1,1,2,2,4,1,0,1,1,2,2,3,2,3,3,4},
{2,2,2,2,4,2,1,0,1,1,2,3,1,2,3,3},
{2,1,1,1,3,2,1,1,0,1,1,2,2,2,2,3},
{3,2,2,2,3,3,2,1,1,0,1,2,1,1,2,2},
{3,2,2,1,2,3,2,2,1,1,0,1,2,1,1,2},
{4,3,2,1,1,4,3,3,2,2,1,0,3,2,1,2},
{3,3,3,3,4,3,2,1,2,1,2,3,0,1,2,2},
{4,3,3,2,3,4,3,2,2,1,1,2,1,0,1,1},
{4,3,3,2,2,4,3,3,2,2,1,1,2,1,0,1},
{5,4,4,3,3,5,4,3,3,2,2,2,2,1,1,0}
};
return(dist[from][to]);
}

/*
**	This routine handles the high score table.
**	High scores are sorted by balance sheet reserves.
**
*/
hiscore()
{
FILE	*fp;
char	*fname;
int	i, j, x, y, w, h;
OBJECT	*hs;

if( easy )
	fname	= "easy.scr";
else
	fname	= "hard.scr";

if( !( fp	= fopen( fname, "rb") ))
	exit( 1 );

fread( &scoretable[0], sizeof( struct score ), 10, fp );
fclose( fp );

for( i = 0 ; i < 10 ; ++i ) {
	if( balsheet[0][0].reserves > scoretable[i].reserves )
		break;
	}
if( i != 10 ) {
	for( j = 9 ; j > i ; --j ) {
		scoretable[j] = scoretable[j-1];
		}
	scoretable[i].reserves = balsheet[0][0].reserves;
	rsrc_gaddr( 0, HISCORE, &hs );
	form_center( hs, &x, &y, &w, &h);
	form_dial( 0, x, y, w, h, x, y, w, h);
	objc_draw( hs, 0, 10, x, y, w, h );
	form_do( hs, HINAME );
	form_dial( 3, x, y, w, h, x, y, w, h);
	strncpy( &scoretable[i].name[0], TXTPNT( hs, HINAME), 10 );
	if( !( fp	= fopen( fname, "wb" ) ))
		exit( 1 );
	fwrite( &scoretable[0], sizeof( struct score ), 10, fp );
	fclose( fp );
	}
}

/*
**
**	This routine calculates the value of the well in a plot for selling
**	reasons.
**	It is assumed that the well type is valid. 
**	ie plot[p].type must be checked.
**
*/
val_well( p, value )
int	p;
real	*value;
{
real	price;
int	maxage, age;

maxage	= plot[p].maxage;
age	= plot[p].age;
price	= pricewell( plot[p].type, MAXTMWELL );
price	= price*(real)(maxage - age )/(real)maxage;	/* unexpired life */
price	/= 2.0;
*value	= price;
}


/*
**
**	This routine calculates the value of a piece of land.
**
*/
val_land( p, value )
int	p;
real	*value;
{
real	reserves;

av_reserves( &reserves );
*value	= ((tot_land/numauc)/4.0)*( plot[p].reserves + reserves) /reserves;
}

/*
**
**	This routine calculates the value of storage.
**	Owned by company c in state s.
**
*/
val_store( c, s, value )
int	c, s;
real	*value;
{
real	price;

price	= pricestore( 20, storage[c][s] );
*value	= price/2.0;
}

/*
**
**	This routine sells the plot in p
**
*/
sell_well( p )
int	p;
{
int	c, type;
real	value, age, maxage, originalcost;
char	text[200], price[50];

c	= plot[p].owner;
type	= plot[p].type;

if( type < 0 )
	return;
val_well( p, &value );
plot[p].type	= -1;			/* disable it */
maxage		= plot[p].maxage;
age		= plot[p].age;
originalcost	= plot[p].cost*10000.0*(maxage-age)/maxage;
cashchange( c, CAPITAL, -1.0*originalcost );
cashchange( c, REV, value - originalcost);
formnum( price, value );
sprintf( text, st[SELLWELLMSG], comp[c], TOPLOT(p), statename[STATE(p)], price );
addmsg( text );
}



/*
**
**	Sell land
**
*/
sell_land( p )
int	p;
{
int	c;
real	value;
char	text[200], price[50];

c	= plot[p].owner;
if( c < 0 )
	return	1;
val_land( p, &value );
cashchange( c, LAND, plot[p].val_land* (-10000.0) );
cashchange( c, REV, value - plot[p].val_land * 10000.0 );
plot[p].type	= -1;	/* just in case */
plot[p].owner	=  -1;
tot_land	+= value;
++numauc;

formnum( price, value);
sprintf( text, st[SELLANDMSG], comp[c], TOPLOT(p), statename[STATE(p)], price);
addmsg( text );

return	0;
}


/*
**
**	Sell storage
**
*/
sell_store( c, s)
int	c,s;
{
real	value;
char	text[200], price[50];

val_store( c, s, &value );
cashchange( c, CAPITAL, -1.0*value );

formnum( price, value );
sprintf( text, st[STRSELLMSG], comp[c], (long)storage[c][s], statename[s], price);
addmsg( text );


storage[c][s]	= 0.0;
}


/*
**
**	This routine updates the sell menus.
**	Should be called in response to a plot or state change, storage or
**	well build, storage or well destroy.
**
*/
updt_sell_menu()
{
real	value;
int	c, type;
char	price[50];

c	= plot[plotsel].owner;
type	= plot[plotsel].type;

if( c == 0 && type >= 0 )
	{
	val_well( plotsel, &value );
	formnum( price, value );
	sprintf( menu[SELLWELL].ob_spec, st[TSELLWELL], price );
	menu_ienable( menu, SELLWELL, 1 );
	}
else
	{
	sprintf( menu[SELLWELL].ob_spec, st[TSELLWELL], st[TNOSELL] );
	menu_ienable( menu, SELLWELL, 0 );
	}

if( c == 0 )
	{
	val_land( plotsel, &value );
	formnum( price, value );
	sprintf( menu[SELLPLOT].ob_spec, st[TSELLAND], price );
	menu_ienable( menu, SELLPLOT, (type<0)?(1):(0) );
	}
else
	{
	sprintf( menu[SELLPLOT].ob_spec, st[TSELLAND], st[TNOSELL] );
	menu_ienable( menu, SELLPLOT, 0 );
	}

val_store( 0, statesel, &value );
formnum( price, value );
sprintf( menu[SELLSTR].ob_spec, st[TSELLSTR], price);
}


