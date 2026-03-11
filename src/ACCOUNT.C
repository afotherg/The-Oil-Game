/*****************************************************************************/
/*       This file holds accounting routines for Mega Game                   */
/*                 Started       17 May      1986.                           */
/*                 This Version  08 March    1987.			     */
/*****************************************************************************/
#include	"header.h"
#include	"variab.h"
/*****************************************************************************/
accquant()		/*Accounting for every quanta (day!)*/
{
accint();		/*account for bank interest*/
accoil();		/*account for oil revenue*/
}
/*****************************************************************************/
accint()
{
int	c;
real	money;

for( c = 0 ; c < 4 ; ++c )
	{
	cashchange( c, ICOST, ((real)interest*debentures[c]/(3.65E4/3)));
	if( cashchange(c,RDCASH) > 0.0)
		{
		money = (cashchange(c,RDCASH) * interest/2)/(3.65E4/3);
		cashchange(c,IEARN,money);
		}
	else
		{
		money = (cashchange(c,RDCASH) * (interest)) / (3.65E4/3);
		cashchange(c,ICOST,-1 * money);
		}
	}
}
/****************************************************************************/
accoil()
{
int	c,s;

for( c = 0 ; c < 4 ; ++c)
	{
	for(s = 0 ; s < 16; ++s)
		{
		cashchange(c,REV,cashstate[c][s]);
		cashchange(c,SCOST,expstate[c][s]);
		}
	}
}
/****************************************************************************/
endfisc()		/* end a fiscal period */
{
int	saveautoin;

saveautoin = autoin;	autoin = 0;	/* will stop any auto opening of tray*/
dep_wells();
pandlfisc();
balfisc();
if( mm == 0 )
	{
	/*
	**	do only once per year
	*/
	tot_land	= tot_land / numauc;
	numauc		= 1;
	}
autoin = saveautoin;
}
/*****************************************************************************/
initfisc()		/*zeroes all fiscal variables*/
{
int	c;

for(c = 0 ; c < 4 ; ++c)
	{
	revfisc[c]	= 0.0;
	expfisc[c]	= 0.0;
	intpdfisc[c]	= 0.0;
	intearnfisc[c]	= 0.0;
	adminfisc[c]	= 0.0;
	accumdep[c]       = 0.0;
	}
pageinit();
}
/****************************************************************************/
initquant()		/*zeroes all quanta variables*/
{
int	c,s;

for(c = 0 ; c < 4 ; ++c)
	{
	for(s = 0 ; s < 16 ; ++s)
		{
		produce[c] [s] = 0.0;
		amountsold [s] = 0.0;
		sold [c] [s] = 0.0;
		cashstate [c] [s] = 0.0;
		expstate  [c] [s] = 0.0;
		}
	}
}
/**************************************************************************/
endquant()		/*end a quanta*/
{
int	c, s;

for(c = 0 ; c < 4 ; ++c )
	{
	for(s = 0 ; s < 16 ; ++s )
		endsupply[c][s] = supply[c][s]; /*store supply left at end of quant*/
	}
}
/***************************************************************************/	
pandldraw(wh,mode,x,y,w,h,wind)
int	wh,mode,x,y,w,h;
WINDOWS	*wind;
{
int	dummy,com,per,type;
PLTYPE	*pt;
real	scaler = 1000.0,costs,gprof,preprof;
real	max1,max2,max;

com = wind->gpp;
per = wind->p1;			/*period 0 = most recent*/
wind_get(wh,WF_WORKXYWH,&pandl->ob_x,&pandl->ob_y,&dummy,&dummy);
pt = &placcount[com][per];

TXTPNT(pandl,PLSCALE) = st[THOUS];
costs	= pt->scost + pt->acost;
gprof	= pt->rev - costs;
preprof = gprof + pt->iearn + pt->inv - pt->dep;
max1	= MAX(pt->rev,fabs(pt->inv));
max2	= MAX(costs,fabs(pt->iearn));
max	= MAX(max1,max2);
max	= MAX(max,pt->tax);
max	= MAX(max,pt->dep);
if(max >= 1e7)
	{
	scaler = 1e6;
	TXTPNT(pandl,PLSCALE) = st[MILLS];
	}
TXTPNT(pandl,PLDATE) = &placcount[com][per].date[0];
TXTPNT(pandl,PLCOMP) = comp[com];
type = gettype(max/scaler);

accnum(type,pt->rev,scaler,pandl,PLREV);
accnum(type,pt->rev,scaler,pandl,PLREV2);
accnum(type,pt->scost,scaler,pandl,PLSCOST);
accnum(type,pt->acost,scaler,pandl,PLACOST);
accnum(type,pt->inv,scaler,pandl,PLINVCH);
accnum(type,pt->dep * -1.0,scaler,pandl,PLDEP);
accnum(type,costs,scaler,pandl,PLCOSTS);
accnum(type,gprof,scaler,pandl,PLGRPROF);
accnum(type,pt->iearn,scaler,pandl,PLIEARN);
accnum(type,pt->tax,scaler,pandl,PLTAX);
accnum(type,preprof,scaler,pandl,PLPRETAX);
accnum(type,preprof - pt->tax,scaler,pandl,PLNET);
objc_draw(pandl,0,10,x,y,w,h);
}
/************************************************************************/
plarrow(wh,press,wind)
int	wh,press;
WINDOWS	*wind;
{
if(press == 6)
	--(wind->p1);
if(wind->p1 < 0.0)	 wind->p1 = 3.0;
if(press == 7)
	++(wind->p1);
if(wind->p1 > 3.0)	wind->p1 = 0.0;
if(press == 2)
	wind->gpp = (wind->gpp + 3)%4;
if(press == 3)
	wind->gpp = (wind->gpp + 1)%4;
send_redraw(wh);
}
/******************************************************************************/
real	cashchange(c,type,amount)
int	c,type;
real	amount;
{
switch(type)
	{
	case	RDCASH:
		return ( cash[c] );
	case	REV:
		cash[c] += amount;
		revfisc[c] += amount;
		break;
	case	ICOST:
		cash[c] -= amount;
		intpdfisc[c] += amount;
		break;
	case	IEARN:
		cash[c] += amount;
		intearnfisc[c] += amount;
		break;
	case	SCOST:
		cash[c] -= amount;
		expfisc[c] += amount;
		break;
	case	ACOST:
		cash[c] -= amount;
		adminfisc[c] += amount;
		break;
	case	TAX:
		cash[c] -= amount;
		break;
	case	CAPITAL:
		capval[c] +=  amount;
		cash[c] -= amount;
		break;
	case	LOANS:
		cash[c] += amount;
		debentures[c] += amount;
		break;
	case	LAND:
		cash[c] -= amount;
		land[c] += amount;
		break;
	case	WRITEOFF:
		capval[c] -= amount;
		accumdep[c] += amount;
		break;
	}
pros_tax[c]	= (revfisc[c]-expfisc[c]-adminfisc[c])*taxrate/100.0;
pros_tax[c]	= (pros_tax[c] > 0.0)?(pros_tax[c]):(0.0);
pros_profit[c]	= revfisc[c]-expfisc[c]-adminfisc[c]-pros_tax[c];
}
/*************************************************************************/
pandlfisc()
{
int	c,s;
PLTYPE	*pt;
real	prof;

for(c = 0 ; c < 4 ; ++c)
	{
	pt = &placcount[c][0];
	placcount[c][3] = placcount[c][2];
	placcount[c][2] = placcount[c][1];
	placcount[c][1] = placcount[c][0];
	inventory[c][3] = inventory[c][2];
	inventory[c][2] = inventory[c][1];
	inventory[c][1] = inventory[c][0];
	inventory[c][0] = 0.0;
	STATEFOR(s)
		inventory[c][0] += endsupply[c][s];
	strcpy(&pt->date[0],currdate);
	pt->rev   = revfisc[c];
	pt->inv   = 10.0 * (inventory[c][0] - inventory[c][1]);
	pt->scost = expfisc[c];
	pt->acost = adminfisc[c];

	pt->iearn = intearnfisc[c] - intpdfisc[c];
	pt->dep   = accumdep[c];
	prof = pt->rev - pt->scost-pt->acost+pt->inv+pt->iearn - pt->dep;
	if(prof > 0.0)
		pt->tax = taxrate * prof / 100.0;
	else
		pt->tax = 0.0;
	cashchange( c, TAX, pt->tax );
	profit[c] = prof - pt->tax;
	}
}
/*****************************************************************************/
baldraw(wh,mode,x,y,w,h,wind)
int	wh,mode,x,y,w,h;
WINDOWS	*wind;
{
int	dummy,com,per,type;
BALTYPE	*pt;
real	scaler = 1000.0,assets,liabs,max1,max2,max;

com = wind->gpp;
per = wind->p1;			/*period 0 = most recent*/
wind_get(wh,WF_WORKXYWH,&balance->ob_x,&balance->ob_y,&dummy,&dummy);
pt = &balsheet[com][per];

TXTPNT(balance,BALSCALE) = st[THOUS];
assets	= pt->cash + pt->inv + pt->land + pt->equip;
liabs	= pt->loans + pt->reserves;
max1	= MAX(MAX(fabs(pt->cash),pt->inv),MAX(pt->land,pt->equip));
max2	= MAX( MAX(fabs(pt->reserves),liabs),pt->loans);
max	= MAX(max1,max2);
if(max >= 1e7)
	{
	scaler = 1e6;
	TXTPNT(balance,BALSCALE) = st[MILLS];
	}
TXTPNT(balance,BALDATE) = &balsheet[com][per].date[0];
TXTPNT(balance,BALCOMP) = comp[com];
type = gettype(max/scaler);

accnum(type,pt->cash,     scaler,balance,BALCASH);
accnum(type,pt->inv,      scaler,balance,BALINV);
accnum(type,pt->land,     scaler,balance,BALLAND);
accnum(type,pt->equip,    scaler,balance,BALEQUIP);
accnum(type,assets,       scaler,balance,BALASSET);

accnum(type,pt->loans,    scaler,balance,BALLOANS);
accnum(type,pt->reserves, scaler,balance,BALRES);
accnum(type,liabs,scaler, balance,BALLIAB);

objc_draw(balance,0,10,x,y,w,h);
}
/*****************************************************************************/
balfisc()
{
int	c;
BALTYPE	*pt;
real	value = 0.0;

for(c = 0 ; c < 4 ; ++c)
	{
	pt = &balsheet[c][0];
	balsheet[c][3] = balsheet[c][2];
	balsheet[c][2] = balsheet[c][1];
	balsheet[c][1] = balsheet[c][0];
	strcpy(&pt->date[0],currdate);
	pt->cash  = cashchange(c,RDCASH);
	pt->inv   = inventory[c][0] * 10.0;	/*notional $10/barrel*/
	pt->land  = land[c];			/*implement later*/
	pt->equip = capval[c];
	pt->loans = debentures[c];
	pt->reserves = pt->cash + pt->inv + pt->land + pt->equip - pt->loans;
	}
}
/******************************************************************************/
accnum(type,n,scaler,tree,obj)		/*formats a number for the accounts*/
real	n,scaler;
OBJECT	*tree;
int	obj,type;
{
real	num;

num = n/scaler;
switch(type)	{

	case	0:
	sprintf(TXTPNT(tree,obj),"%5ld",0L);
	break;

	case	1:
	sprintf(TXTPNT(tree,obj),"%5.2f",num);
	break;

	case	2:
	sprintf(TXTPNT(tree,obj),"%5.1f",(num));
	break;

	default:
	sprintf(TXTPNT(tree,obj),"%5ld",(long)(num+0.5));
	break;
	
/*	case	3:
	sprintf(TXTPNT(tree,obj),"%5ld",(long)(num+0.5));
	break;

	case	4:
	sprintf(TXTPNT(tree,obj),"%5ld",10*(long)(num/10.0+0.5));
	break;

	case	5:
	sprintf(TXTPNT(tree,obj),"%5ld",100*(long)(num/1e2+0.5));
	break;

	case	6:
	sprintf(TXTPNT(tree,obj),"%5ld",1000*(long)(num/1e3+0.5));
	break;

	case	7:
	sprintf(TXTPNT(tree,obj),"%5ld",(long)(num+0.5)); */
	}
}
/**************************************************************************/
gettype(max)
real	max;
{
if(max < 0.01)	return(0);
if(max < 10.0)	return(1);
if(max < 1e2)	return(2);
if(max < 1e3)	return(3);
if(max < 1e4)	return(4);
if(max < 1e5)	return(5);
if(max < 1e6)	return(6);
return(7);
}
/*****************************************************************************/
dep_wells()			/* depreciates wells at end of period */
{
int	i, c, type;
char	string[300];

for( i = 0 ; i < 256; ++i)
	{
	c = plot[i].owner;
	type = plot[i].type;
	if( c >=0 && type >= 0 )
		{
		cashchange( c, WRITEOFF, (plot[i].cost * 10000.0 / plot[i].maxage));
		if( (++plot[i].age) == plot[i].maxage )
			{
			sprintf(string, st[ENDWELL], statename[STATE(i)],
				TOPLOT(i),comp[c]);
			addmsg( string );
			plot[i].type = -1;	/* decommission */
			drawstates = 1;		/* so state window drawn */
			}
		if( plot[i].age == plot[i].maxage-1 && c == 0)
			{
			sprintf(string, st[NRENDWELL], statename[STATE(i)], TOPLOT(i), comp[0]);
			addmsg( string );
			}
		}
	}
}
/*****************************************************************************/
real	maxloan( c )			/* max loan available for c */
{
real	loan;

if(( loan = cashchange(c,RDCASH) + capval[c] + land[c] - proploan * debentures[c]) < 10000.0)
	loan = 1.0;
return( loan );
}
/*****************************************************************************/


