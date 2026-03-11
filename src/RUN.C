#include	"header.h"
#include	"variab.h"

extern	real	average_demand();
extern	int	isloss(), highimp(), lowimp(), highdebt();
extern	int	random_event(), stnotused(), nomoney(), wellnooil();
extern	int	heavypipe(), heavywell(), heavystore(), heavysell();
extern	int	valuable(), maxcash(), maxprofit(), maxdemand();
extern	int	bldhigh(), bldlow(), railhigh(), raillow(), lablow(), labhigh();
extern	int	stnoprod(), bigdebt(), notvaluable(), highdemand(), lowdemand();
extern	int	highprice(), landup();

int	(*predicates[])() = {

	isloss,		/*  0  */
	highdebt,	/*  1  */
	random_event,	/*  2  */
	nomoney,	/*  3  */
	heavypipe,	/*  4  */
	heavywell,	/*  5  */
	heavystore,	/*  6  */
	heavysell,	/*  7  */
	valuable,	/*  8  */
	maxdemand,	/*  9  */
	maxcash,	/*  10 */
	maxprofit,	/*  11 */
	wellnooil,	/*  12 */
	bldhigh,	/*  13 */
	labhigh,	/*  14 */
	railhigh,	/*  15 */
	bldlow,		/*  16 */
	lablow,		/*  17 */
	raillow,	/*  18 */
	stnoprod,	/*  19 */
	bigdebt,	/*  20 */
	notvaluable,	/*  21 */
	highdemand,	/*  22 */
	lowdemand,	/*  23 */
	highprice,	/*  24 */
	highimp,	/*  25 */
	lowimp,		/*  26 */
	landup		/*  27 */
	};

char	*first[] = {
	"Wolfgang",
	"Alan",
	"Ben",
	"Graham",
	"Dave",
	"Lucien",
	"Alfonso",
	"Felix",
	"Jane",
	"Anna",
	"Katrina",
	"Liz",
	};

char	*surname[] = {
	"Kolnberger",
	"Wedge",
	"Buckel",
	"Fothergill",
	"Molony",
	"Nayar",
	"Wong",
	"Kogbara",
	"Teissier",
	"Demont",
	"Bilboul",
	"Bennett"};
			
char	already_used[200];
char	news_date[50];
unsigned	char	*runpt;
/*****************************************************************************/
add_run_time(rti_str, rti_meg, rti_num, types_used)
FILE	*rti_str, *rti_meg;
int	rti_num, types_used[];
{
int	i, j, type, pick;
long	offset;
char	a[40], b[40], c[40], d[40], name[40];
unsigned	int	read, prev = 'A';

pick = rand() % rti_num + 1;	/* + 1 because rti_num stored on top */
fseek(rti_meg, pick * 17L, 0);
fscanf(rti_meg, "%d %ld", &type, &offset);
for(j = 0 ; types_used[j] != -1 ; ++j)
	if(type == types_used[j])	/* story of this type already used */
		return( 0 );
types_used[j] = type;
types_used[j + 1] = -1;
if( (yy % 100) - already_used[pick] < -16)
	return ( 0 );	/* story used within last 4 years */

/* OK, story not used - but is it relevant?? */
a[0] = 0;
b[0] = 0;
c[0] = 0;
d[0] = 0;
if( (*predicates[type])(a, b, c, d) )
	{
	fseek(rti_str, offset, 0);
	while((read = getc(rti_str)) != ENDSTR)
		{
		switch(read)
			{
			case	ALPHA:
			for(j = 0 ; a[j] ; ++j)
				*runpt++ = a[j];
			break;

			case	BETA:
			for(j = 0 ; b[j] ; ++j)
				*runpt++ = b[j];
			break;

			case	PIE:
			for(j = 0 ; c[j] ; ++j)
				*runpt++ = c[j];
			break;

			case	LINE:
			for(j = 0 ; d[j] ; ++j)
				*runpt++ = d[j];
			break;

			case	BAR:
			for(j = 0 ; (months[mm])[j] ; ++j)
				*runpt++ = (months[mm])[j];
			break;

			case	OUT2D:
			sprintf(runpt, "%d", yy);
			runpt += 4;
			break;

			case	HDLINE:
			sprintf(name, "%s %s", first[rand()%8], surname[rand()%12]);
			for(j = 0 ; name[j] ; ++j)
				*runpt++ = name[j];
			break;

			case	' ':
			if(prev == ' ')
				break;

			default:
			*runpt++ = read;
			break;
			}
		prev = read;
		}
	already_used[pick] = yy % 100 + 20; 
	return( 1 );
	}
return( 0 );
}
/*****************************************************************************/
space_strip(str)
char	*str;
{
int	i;

while(str[0] == ' ')
	for(i = 0 ; str[i] ; ++i)
		str[i] = str[i + 1];	/* remove leading spaces */

for(i = strlen(str) - 1 ; str[i] == ' ' ; --i)
	str[i] = 0;				/* remove trailing spaces */
}
/*****************************************************************************
*
*	The following is a series of predicates that either return TRUE or
*	FALSE. The character strings a,b,c,d correspond to the symbols [alpha],
*	[beta],[pie] and [line] in the 'rti.txt' file.	
*
******************************************************************************/
isloss(a, b, c, d)		/* make a loss */
char	*a, *b, *c, *d;
{
int	i, company;

company = rand()%4;
for(i = company ; i < 4 + company ; ++i)
	{
	if(profit[i%4] < -10000.0)
		{
		strcpy(a, comp[i%4]);
		formnum(b, profit[i%4] * -1.0);
		space_strip(b);
		return( TRUE );
		}
	}
return( FALSE );
}
/*****************************************************************************/
highdebt(a, b, c, d)		/* excessive borrowing */
char	*a, *b, *c, *d;
{
int	i, company;

company = rand()%4;
for(i = company ; i < 4 + company ; ++i)
	{
	if(debentures[i%4] > 1.5 * land[i%4] && debentures[i%4] > 5e6)
		{
		strcpy(a, comp[i%4]);
		formnum(b, debentures[i%4]);
		space_strip(b);
		return( TRUE );
		}
	}
return( FALSE );
}
/*****************************************************************************/
random_event(a, b, c, d)	/* completely random event */
char	*a, *b, *c, *d;
{
sprintf(a, "%s %s", first[rand()%12], surname[rand()%12]);
strcpy(b, statename[rand()%16]);
sprintf(c, "%d", rand()%5 + 2);
strcpy(d, months[rand()%12]);
return( TRUE );
}
/*****************************************************************************/
nomoney(a, b, c, d)		/* bank won't lend money */
char	*a, *b, *c, *d;
{
int	i, company;

company = rand()%4;
for(i = company ; i < 4 + company ; ++i)
	{
	if(maxloan(i%4) <= 1e3 )
		{
		strcpy(a, comp[i%4]);
		return( TRUE );
		}
	}
return( FALSE );
}
/*****************************************************************************/
heavypipe(a, b, c, d)		/* lots of pipes */
char	*a, *b, *c, *d;
{
int	i;

if( (i = numjobs( PIPEJOB )) > 1)
	{
	sprintf(a, "%d", i);
	sprintf(b, "%.1f", bldfact * 100.0);
	return( TRUE );
	}
return( FALSE );
}
/*****************************************************************************/
heavywell(a, b, c, d)		/* lots of wells */
char	*a, *b, *c, *d;
{
int	i;

if( (i = numjobs( WELLJOB )) > 1)
	{
	sprintf(a, "%d", i);
	sprintf(b, "%.1f", bldfact * 100.0);
	return( TRUE );
	}
return( FALSE );
}
/*****************************************************************************/
heavystore(a, b, c, d)		/* lots of storage */
char	*a, *b, *c, *d;
{
int	i;

if( (i = numjobs( STOREJOB )) > 1)
	{
	sprintf(a, "%d", i);
	sprintf(b, "%.1f", bldfact * 100.0);
	return( TRUE );
	}
return( FALSE );
}
/*****************************************************************************/
heavysell(a, b, c, d)		/* lots of selling */
char	*a, *b, *c, *d;
{
int	i;

if( (i = numjobs( SELLJOB )) > 3)
	{
	sprintf(a, "%d", i);
	sprintf(b, "%.1f", bldfact * 100.0);
	return( TRUE );
	}
return( FALSE );
}
/*****************************************************************************/
valuable(a, b, c, d)		/* land under auction that has oil */
char	*a, *b, *c, *d;
{
JOBS	*job;

job = &firstjob;
while(job = job->next)
	{
	if(job->type == SELLJOB && plot[(int)job->p1].reserves > 10.0)
		{
		sprintf(a, "%s", statename[STATE((int)job->p1)]);
		sprintf(b, "%d", TOPLOT((int)job->p1));
		return( TRUE );
		}
	}
return( FALSE );
}
/*****************************************************************************/
maxcash(a, b, c, d)		/* company with most cash */
char	*a, *b, *c, *d;
{
real	max = -1e20;
int	i;

for(i = 0 ; i < 4 ; ++i)
	{
	if(cash[i] > max)
		{
		max = cash[i];
		strcpy(a, comp[i]);
		formnum(b, cash[i]);
		space_strip(b);
		}
	}
if(max < 1e5 )
	return( FALSE );
return( TRUE );
}
/*****************************************************************************/
maxprofit(a, b, c, d)		/* company with most profit */
char	*a, *b, *c, *d;
{
real	max = -1e20;
int	i;

for(i = 0 ; i < 4 ; ++i)
	{
	if(profit[i] > max)
		{
		max = profit[i];
		strcpy(a, comp[i]);
		formnum(b, profit[i]);
		space_strip(b);
		}
	}
if(max < 1e5 )
	return( FALSE );
return( TRUE );
}
/*****************************************************************************/
maxdemand(a, b, c, d)		/* state with most demand */
char	*a, *b, *c, *d;
{
real	max = -1e20;
int	i, state;

for(i = 0 ; i < 16 ; ++i)
	{
	if(demand[i] > max)
		{
		max = demand[i];
		state = i;
		}
	}
strcpy(a, statename[state]);
formnum(b, demandset(5.0, state));
space_strip(b);
max = -1e20;
for(i = 0 ; i < 4 ; ++i)
	{
	if(cashstate[i][state] > max)
		{
		max = cashstate[i][state];
		strcpy(c, comp[i]);
		}
	}
if(max < 10.0)
	strcpy(c, "Morgan");
return( TRUE );
}
/*****************************************************************************/
wellnooil(a, b, c, d)		/* well being built where no oil */
char	*a, *b, *c, *d;
{
JOBS	*job;

job = &firstjob;
while(job = job->next)
	{
	if(job->type == WELLJOB && plot[(int)job->p1].reserves < 10.0)
		{
		strcpy(a, statename[STATE((int)job->p1)]);
		sprintf(b, "%d", TOPLOT((int)job->p1));
		strcpy(c, comp[job->company]);
		return( TRUE );
		}
	}
return( FALSE );
}
/*****************************************************************************/
bldhigh(a)
char	*a;
{
sprintf(a, "%.1f", bldfact * 100.0);
if(bldfact > 1.5)
	return( TRUE );
return( FALSE );
}
/*****************************************************************************/
labhigh(a)
char	*a;
{
sprintf(a, "%.1f", labfact * 100.0);
if(labfact > 1.5)
	return( TRUE );
return( FALSE );
}
/*****************************************************************************/
railhigh(a)
char	*a;
{
sprintf(a, "%.1f", railfact * 100.0);
if(railfact > 1.5)
	return( TRUE );
return( FALSE );
}
/*****************************************************************************/
bldlow(a)
char	*a;
{
sprintf(a, "%.1f", bldfact * 100.0);
if(bldfact < 0.95)
	return( TRUE );
return( FALSE );
}
/*****************************************************************************/
lablow(a)
char	*a;
{
sprintf(a, "%.1f", labfact * 100.0);
if(labfact < 0.95)
	return( TRUE );
return( FALSE );
}
/*****************************************************************************/
raillow(a)
char	*a;
{
sprintf(a, "%.1f", railfact * 100.0);
if(railfact < 0.95)
	return( TRUE );
return( FALSE );
}
/*****************************************************************************/
stnoprod(a, b, c, d)
char	*a, *b, *c, *d;
{
JOBS	*job;
int	i;
real	sum;

job = &firstjob;
while(job = job->next)
	{
	if(job->type == STOREJOB)
		{
		for(sum = 0.0, i = 0 ; i < HISTPNTQNT ; ++i)
			sum += histproduce[job->company][(int)job->p1][i];
		if(sum < 1.0)
			{
			strcpy(a, statename[STATE((int)job->p1)]);
			strcpy(b, comp[job->company]);
			sprintf(c, "%d", (int)job->delay);
			sprintf(d, "%ld", (long)job->p2);
			return( TRUE );
			}
		}
	}
return( FALSE );
}
/*****************************************************************************/
bigdebt(a, b, c, d)
char	*a, *b, *c, *d;
{
JOBS	*job;

job = &firstjob;
while(job = job->next)
	{
	if( (job->type == LOANJOB) && (job->p2 > cash[job->company]) && (job->delay < 60) )
		{
		strcpy(a, comp[job->company]);
		formnum(b, job->p2);
		space_strip(b);
		sprintf(c, "%d", job->delay);
		return( TRUE );
		}
	}
return( FALSE );
}
/*****************************************************************************/
notvaluable(a, b, c, d)		/* land under auction that has no oil */
char	*a, *b, *c, *d;
{
JOBS	*job;

job = &firstjob;
while(job = job->next)
	{
	if(job->type == SELLJOB && plot[(int)job->p1].reserves < 10.0)
		{
		sprintf(a, "%s", statename[STATE((int)job->p1)]);
		sprintf(b, "%d", TOPLOT((int)job->p1));
		return( TRUE );
		}
	}
return( FALSE );
}
/*****************************************************************************/
highdemand(a, b, c, d)
char	*a, *b, *c, *d;
{
int	i, state;

state = rand() % 16;
for(i = state ; i < 16 + state ; ++i)
	{
	if(demand[i%16] > 2.0 * average_demand())
		{
		strcpy(a, statename[i%16]);
		formnum(b, demandset(5.0, i%16));
		space_strip(b);
		return( TRUE );
		}
	}
return( FALSE );
}
/*****************************************************************************/
lowdemand(a, b, c, d)
char	*a, *b, *c, *d;
{
int	i, state;

state = rand() % 16;
for(i = state ; i < 16 + state ; ++i)
	{
	if(demand[i%16] < 0.5 * average_demand())
		{
		strcpy(a, statename[i%16]);
		formnum(b, demandset(5.0, i%16));
		space_strip(b);
		return( TRUE );
		}
	}
return( FALSE );
}
/*****************************************************************************/
real	average_demand()
{
int	i;
real	sum = 0;

for(i = 0 ; i < 16 ; ++i)
	sum += demand[i];
return(sum / 16);
}
/*****************************************************************************/
highprice(a, b, c, d)
char	*a, *b, *c, *d;
{
int	i, state;

state = rand() % 16;
for(i = state ; i < 16 + state ; ++i)
	{
	if(al_avepri(i%16) > 20.0)
		{
		strcpy(a, statename[i%16]);
		formnum(b, al_avepri(i%16));
		space_strip(b);
		return( TRUE );
		}
	}
return( FALSE );
}
/*****************************************************************************/
highimp(a)
char	*a;
{
sprintf(a, "%.2f", price_imp);
if(price_imp > 15.0)
	return( TRUE );
return( FALSE );
}
/*****************************************************************************/
lowimp(a)
char	*a;
{
sprintf(a, "%.2f", price_imp);
if(price_imp < 10.0)
	return( TRUE );
return( FALSE );
}
/*****************************************************************************/
landup(a)
char	*a;
{
formnum(a, tot_land / numauc);
if( (tot_land / numauc) > 3e6)
	return( TRUE );
return( FALSE );
}


