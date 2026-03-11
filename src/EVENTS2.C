/*****************************************************************************/
/*		More events						     */
/*		Started	09 October	1987				     */
/*****************************************************************************/
#include	<header.h>
#include	<variab.h>
/*****************************************************************************/


/*
**	This event appears if it seems likely that Gitty will be unable to
**	pay its tax bill.
**	If the player presses "loan", then the loan dialog is pulled up,
**	and the player is told of the amount estimated to be needed.
*/
BUTFUNC( tax_but )
{
char	text[300], need[40];
switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		formnum( need, wind->p1 );
		sprintf( text, st[VPTAXM], need );
		open_loan( xdesk, ydesk+hdesk);
		vpexitwindow(wh, st[MEMACC], text);
		break;
	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

tax_event()
{
char	text[300], need[30];
real	money;

money = 2*(pros_tax[0] - cashchange(0, RDCASH) );
if( mm%3 == 2 && money > 0.0 )
	{
	formnum( need, money);
	sprintf( text, st[VPTAXT], need );
	dovpwindow( st[MEMACC], text, st[VPLOANBUT], 0L, 0L,
		st[VPEXITBUT], tax_but, 0, money, 0.0 );
	return( TRUE );
	}
return( FALSE );
}


/*
**	This event appears for cases when two wells are in operation in the
**	same state, and an efficiency advantage could be gained by altering
**	their productions.
**	If the player selects "do it", then the changes are made automatically
*/
BUTFUNC( but_well_ef )
{
int	minp, maxp;

switch( vpobj( mx, my ) )
	{
	case	VPBUT0:
		minp = wind->p1;
		maxp = wind->p2;
		if( plot[minp].output < 10.0 )
			plot[minp].output = 10.0;
		if( plot[maxp].output < 10.0 )
			plot[maxp].output = 10.0;
		plot[minp].output = MIN( plot[minp].output * 1.05, 
				2.0*param_wells[plot[minp].type].capacity);
		plot[maxp].output *= 0.95;
		exitvp( wh );
		break;		/* increase alertness rating */
	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

well_ef_event()			/* checks if two wells used efficiently */
{
int	s, ref, x, y, p, num, owned[16], i, maxp, minp;
real	maxmcost, minmcost, mcost;
char	text[300];

for( s = 0 ; s < 16 ; ++s )
	{
	num 	= 0;
	ref	= REFPLOT( s );
	for( x = 0 ; x < 4; ++x )
		{
		for( y = 0 ; y < 4 ; ++y )
			{
			p = ref+x+y*16;
			if( plot[p].owner == 0 && plot[p].type >= 0 )
				owned[num++] = p;
			}
		}
	if( num > 1 )
		{
		maxmcost = -9E9;
		minmcost =  9E9;
		for( i = 0 ; i < num ; ++i )
			{
			p = owned[i];
			mcost = marg_cost( plot[p].output, plot[p].type);
			if( mcost > maxmcost )
				{
				maxp	 = p;
				maxmcost = mcost;
				}
			if( mcost < minmcost )
				{
				minp	 = p;
				minmcost = mcost;
				}
			}
		if( maxp != minp && maxmcost > 1.2 * minmcost )
			{
			sprintf( text, st[VPWELLFT], statename[s], 
				TOPLOT(minp), TOPLOT(maxp) );
			dovpwindow( st[MEMPRODVP], text,
				st[VPDOIT], 0L, 0L, st[VPEXITBUT], but_well_ef,
				s, (real)minp, (real)maxp);
			return( TRUE );
			}
		}
	}
return( FALSE );
}

/*
**	This is a job handler for when a dirty trick has been discovered.
*/
foundout( job )
JOBS	*job;
{
int	p;
char	text[300];

p	= job->p1;

sprintf( text, st[FOUNDOUTTEXT], statename[STATE(p)]);
addmsg( text );
cashchange( 0, ACOST, 1e6);
}


/*
**	This event is from dirty tricks VP, and is in response to a competitor
**	being too successful.
**	The user is given the opportunity to blow up the competitor's well
*/
BUTFUNC( but_blowup )
{
char	text[300];
int	c, s, p;
JOBS	job;

s	= wind->gpp;
c	= wind->p1;
p	= wind->p2;
switch( vpobj( mx, my) )
	{
	case	VPBUT0:
		plot[p].type	= -1;
		sprintf( text, st[VPBLOWNUP], statename[s],
			comp[c] );
		addmsg( text );
		if( rand()%2 == 0 )
			{
			job.delay	= 30;
			job.type	= FOUNDOUT;
			job.p1		= p;
			}
		exitvp( wh );
		break;
	case	VPBUT3:
		exitvp( wh );
		break;
	}
}

blow_up_event()		/* dirty tricks event to blow up competitor's well */
{
int	p, i, off, s, c;
char	text[400];

if( rand()%5 )
	return( FALSE );

off	= rand()%256;
for( i = 0 ; i < 256 ; ++i )
	{
	p	= (i+off)%256;
	if( (c=plot[p].owner)<=0)
		continue;
	s	= STATE(p);
	if( s == NY )
		continue;			/* a special case */
	if( plot[p].type >= 0 )
		{
		sprintf( text, st[VPBLOWUP], comp[c], statename[s], TOPLOT(p) );
		dovpwindow( VPDIRTY, text, st[VPGOITBUT], 0L, 0L,
			st[VPNOCHNCE], but_blowup, s, (real)c, (real)p);
		return( TRUE );
		}
	}
return( FALSE );
}

