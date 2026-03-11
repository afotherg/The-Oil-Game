/****************************************************************************/
/*        This file holds routines to do with outputing graphical data      */
/*		Started  	02 November 1986.			    */
/*		This version	15 March    1987.			    */
/****************************************************************************/
#include	"header.h"
#include	"variab.h"
/****************************************************************************/
DRAWFUNC(outcash)			/*outputs bank balance*/
{
real	data[4];
int	company;

for(company = 0; company < 4 ; ++company)
	data[company] = MAX(0.0,cashchange(company,RDCASH));
graphic(wind,wh,mode,BAR,data,4," ",companies,"$",1);
}
/***********************************************************************/
DRAWFUNC(outearningsall)
{
graphic(wind,wh,mode,OUT2D,&cashstate[0][0],16,comp[0],0l,"$",4);
}
/***********************************************************************/
DRAWFUNC(outpriall)
{
real	data[4];
int	i;
static	char	title[50];

sprintf(title, "%s - %s", st[WIPRICE], statename[wind->gpp]);
wind_set(wind->handle, WF_NAME, title, 0, 0);

for(i = 0 ; i < 4 ; ++i)
	data[i] = sellmin[i][wind->gpp];
graphic(wind,wh,mode,BAR,data,4,statename[wind->gpp],companies,"$",1);
}
/*************************************************************************/
DRAWFUNC( outprodall )
{
graphic(wind,wh,mode,OUT2D,&produce[0][0],16,comp[0],0l,"b",4);
}
/***************************************************************************/
DRAWFUNC( outexpensesall )
{
graphic(wind,wh,mode,OUT2D,&expstate[0][0],16,comp[0],0l,"$",4);
}
/***************************************************************************/
DRAWFUNC( outmarket )
{
int	i;
real	data[4];

for(i = 0 ; i < 4 ; ++i)
	data[i] = sold[i][wind->gpp];

graphic(wind,wh,mode,PIE,data,4,statename[wind->gpp],compwithpc,"\0",1);
}
/***************************************************************************/
DRAWFUNC( outsupplyall )
{
graphic(wind,wh,mode,OUT2D,&endsupply[0][0],16,comp[0],0l,"b",4);
}
/***************************************************************************/
DRAWFUNC( intearn )
{
int	company;
real	data[4];

for( company = 0 ; company < 4 ; ++company)
	data[company] = MAX(0.0,intearnfisc[company] - intpdfisc[company]);
graphic(wind,wh,mode,BAR,data,4," ",companies,"$",1);
}
/****************************************************************************/
DRAWFUNC( amtsold )
{
graphic(wind,wh,mode,OUT2D,&amountsold[0],16,comp[0],0l,"b",1);
}
/****************************************************************************/
out2D(wh,data,nocomp,mode,top,ytitle)
int	wh,top;
real	data[4][16];
int	mode,nocomp;
char	*ytitle;
{
int	x,y,w,h,barheight,barwidth,company,state,pxy[4],height[16],i;
int	xorg,yorg,wbar,hbar,pxyarray[4],xpos,ypos;
char	point[6];

wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
pxyarray[0] = x;
pxyarray[1] = y;
pxyarray[2] = x + w - 1;
pxyarray[3] = y + h - 1;

axes(wh,pxyarray,w,h,&xorg,&yorg,&wbar,&hbar,mode, 0);
if(mode == 2)
	{
	putlegend(pxyarray,xorg,yorg,16*(wbar/16),w,h,16,stateabbrev,0);
	puttitles(pxyarray,ytitle,0);
	}
barheight = hbar;
barwidth  = wbar/16;
if( (barheight < 0) || (barwidth < 0) )
	return;
y = yorg;
x = xorg;
for(i = 0 ; i < 16 ; height[i++] = y)
	;
for(company = 0 ; company < nocomp ; ++company)
	{
	getcolour(company);
	for(state = 0; state < 16 ; ++state)
		{
		pxy[0] = x+(barwidth)*state;
		pxy[2] = pxy[0] + barwidth - 3;
		pxy[1] = height[state]-data[company][state]*barheight/1000;
		pxy[3] = height[state];
		if(pxy[1] != pxy[3])
			{
			height[state] = pxy[1] - 1;
			v_bar(vdi_handle,pxy);
			}
		}
	}
vsf_color(vdi_handle, WHITE);
vsf_interior(vdi_handle,2);
vsf_style(vdi_handle,8);
for(state = 0 ; state < 16 ; ++state)
	{
	pxy[0] = x + barwidth*state;
	pxy[2] = pxy[0] + barwidth - 3;
	pxy[1] = y - barheight;
	pxy[3] = height[state];
	if(pxy[1] < pxy[3])
		v_bar(vdi_handle,pxy);
	}
if(mode == 2)
	{
	number_axis(pxyarray, top, 0, xorg, yorg, 0, hbar);
	pxyarray[2] = pxyarray[0] + 4 * (xorg - pxyarray[0]) / 5;
	putkey(wh, pxyarray, 4, "Gi\0GW\0Ro\0Bu", 2, -1L, &data[0][0], yorg);
	}
}	
/***************************************************************************/
DRAWFUNC( statedraw )
{
int	dummy,state,plotnumber,i,numplot,x1,y1,owner,p, s;
static	int	avail[] = {AMTSTORE, PSTW, PSTR, PSTB };

for(i = 0; i < 16 ; ++i)
	(plots + definesl3[i])->ob_state = NORMAL;

wind_get(wh,WF_WORKXYWH,&(plots->ob_x),&(plots->ob_y),&dummy,&dummy);
s	= wind->gpp;
for(y1 = 0 ; y1 < 4 ; ++y1)
	{
	for(x1 = 0 ; x1 < 4 ; ++x1)
		{
		p = 4*y1 + x1;
		numplot  = (4*(s%4) + 64*(s/4)) + x1 + 16 * y1;
		owner = plot[numplot].owner;
		OBS(plots,defines[p]) = OBS(icons,plotown[owner + 1]);
		if(plot[numplot].type != -1)
			OBS(plots,defineslt[p]) = OBS(icons,PLOTWELL);
		else
			{
			if((plot[numplot].flags[owner] & 0x08) && (owner!=-1)) 
				OBS(plots,defineslt[p]) = OBS(icons,PLOTBLD);
			else
				OBS(plots,defineslt[p]) = OBS(icons,PLOTBLNK);
			}
		if(plot[numplot].flags[0] & 0x80)
			OBS(plots,defineslt[p]) = OBS(icons,HAMMER);
		if(plot[numplot].flags[0] & 0x01)
			if(plot[numplot].prosres >= 0 )
				OBS(plots,definesl3[p]) = OBS(icons,STAR);
			else	
				OBS(plots,definesl3[p]) =
					OBS(icons,PLOTPROS);
		if(!(plot[numplot].flags[0] & 0x01))
			OBS(plots,definesl3[p]) = OBS(icons,PLOTBLNK);
		}
	}
state = (plotsel/64)*4+(plotsel%16)/4; 
if(state == s )
	{
	plotnumber  = plotsel - (4*(state%4) + 64*(state/4));
	plotnumber -= 12*(plotnumber/16);
	(plots+definesl3[plotnumber])->ob_state = SELECTED;
	}
formnum( TXTPNT(plots, PESTDEM), al_estdem( s, 5.0 ) );
for( i = 0 ; i < 4 ;++i )
	sprintf(TXTPNT(plots,avail[i]),"%ld",(long)MIN( storage[i][s], 99999L));
sprintf(TXTPNT(plots,BLTSTORE),"%ld",(long)MIN(store_beingbuilt[0][s],99999L));
objc_draw(plots,0,10,x,y,w,h);
}
/****************************************************************************/
BUTFUNC(selectplot)
{
int	dummy, p, obj, number, i, s, savestate;

wind_get(wh,WF_WORKXYWH,&(plots->ob_x),&(plots->ob_y),&dummy,&dummy);
s		= wind->gpp;
savestate	= statesel;
obj = objc_find(plots,0,10,mx,my);
switch( obj )				/* first try icons at bottom */
	{
	case	STCHP:
		statesel = s;
		domenu( REVHIST );
		domenu( MPRICES );
		break;
	case	STBLDS:
		statesel = s;
		domenu( STOREMEN );
		break;
	case	STTROIL:
		statesel = s;
		domenu( RAILMEN );
		break;
	case	STBLDP:
		statesel = s;
		domenu( PIPEMEN );
		break;
	case	STVPC:
		statesel = s;
		opn_vpchoose();
		break;
	}
statesel = savestate;

for(number = 0 ; obj != definesl3[number] && number < 16; ++number)
	;
if(number != 16)
	{
	p = (4*(s%4) + 64*(s/4)) + 16*(number/4) + number%4;
	if( plotsel == p )		/* signal to open plot window */
		domenu( OPNPLOT );
	else
		{
		plotsel = p;
		updt_sell_menu();
		}
	drawstates = 1;		/*ensures that all state windows are redrawn*/
	}
}
/****************************************************************************/
DRAWFUNC(plotdraw)
{
int	dummy, p, c, prostype, type, days;
static	prtp[] = {PROSTP0, PROSTP1, PROSTP2, PROSTP3};
char	amount[50];

wind_get(wh, WF_WORKXYWH, &(opnplot->ob_x),&(opnplot->ob_y),&dummy,&dummy);
p	= wind->gpp;

c = plot[p].owner;
TXTPNT( opnplot, PLOTSTAT ) = statename[STATE(p)];
str( TOPLOT(p), TXTPNT( opnplot, PLOTPLOT), 2);
TXTPNT( opnplot, PLOTOWN) = ((c<0)?(st[NOTOWN]):(comp[c]));
if( plot[p].flags[0] & 1 )
	{
	if(  plot[p].prosres >= 0 )
		{
		prostype = (plot[p].flags[0]&6)>>1;
		TXTPNT( opnplot, PLOTPRBY) = st[prtp[prostype]];
		formnum( amount, plot[p].prosres * 1000.0 );
		sprintf( TXTPNT(opnplot, PLOTFND), st[PROSFOUND], amount );
		}
	else
		{
		TXTPNT( opnplot, PLOTPRBY) = st[PROSHAP];
		sprintf( TXTPNT( opnplot, PLOTFND), st[NULLSTRING] );
		}
	}
else
	{
	TXTPNT( opnplot, PLOTPRBY)	= st[NOPROS];
	sprintf( TXTPNT( opnplot, PLOTFND), st[NULLSTRING] );
	}

type	= plot[p].type;
if( type < 0 )
	{
	if( (type = well_bng_blt( 0, p, &days )) >=0 )
		{
		sprintf( TXTPNT( opnplot, PLOTWLTP), st[WELLNEAR],days);
		TXTPNT( opnplot, PLOTBL1)  = param_wells[type].bull1;
		TXTPNT( opnplot, PLOTBL2)  = param_wells[type].bull2;
		TXTPNT( opnplot, PLOTBL3)  = param_wells[type].bull3;
		}
	else
		{
		sprintf( TXTPNT( opnplot, PLOTWLTP), st[NOWELL] );
		TXTPNT( opnplot, PLOTBL1)  = st[NULLSTRING];
		TXTPNT( opnplot, PLOTBL2)  = st[NULLSTRING];
		TXTPNT( opnplot, PLOTBL3)  = st[NULLSTRING];
		}
	}
else
	{
	sprintf( TXTPNT( opnplot, PLOTWLTP), st[WELLIN],
		(plot[p].maxage - plot[p].age));
	TXTPNT( opnplot, PLOTBL1)  = param_wells[type].bull1;
	TXTPNT( opnplot, PLOTBL2)  = param_wells[type].bull2;
	TXTPNT( opnplot, PLOTBL3)  = param_wells[type].bull3;
	}
opnplot[PLOTBWL].ob_state	= (plot[p].owner == 0)?(NORMAL):(DISABLED);
opnplot[PLOTDOPR].ob_state	= (plot[p].owner <= 0 )?(NORMAL):(DISABLED);
opnplot[PLOTCH].ob_state	= ( plot[p].type >= 0 && plot[p].owner == 0 )?
				  (NORMAL):(DISABLED);

objc_draw( opnplot, 0, 10, x, y, w, h);
}
/*****************************************************************************/
BUTFUNC( butinplot )
{
int	p;

p	= wind->gpp;
switch( objc_find( opnplot, 0, 10, mx, my) )
	{
	case	PLOTBWL:
		if( plot[p].owner == 0 )
			{
			plotsel	= p;	/* just in case changed */
			domenu( SPECWELL );
			}
		break;

	case	PLOTDOPR:
		if( plot[p].owner <= 0 )
			{
			plotsel	= p;
			domenu( MENPROS );
			}
		break;

	case	PLOTCH:
		if( plot[p].owner == 0 && plot[p].type >= 0 )
			{
			plotsel = p;
			domenu( CHOUT );
			}
		break;
	}
}
/*****************************************************************************/
outjobs(wh, mode)			/*outputs jobs in progress*/
int	wh, mode;
{
int	x,y,w,h,i=2,index;
JOBS	*job;
char	number[6], text[200], cost[50], text1[100];

if(mode == 2)
	cls(wh);
wind_get(wh, WF_WORKXYWH, &x, &y, &w, &h);

job = firstjob.next;
sprintf( text, st[JOBTITLE], comp[compsel]);
sprintf( text1, "%-80s", text );
v_gtext(vdi_handle, x, y + gl_hchar, text1);
while( job && i < 15  )
	{
	text[0] = 0;
		switch(job->type)
		{
		case	PIPEJOB:
		if(job->company == compsel)
			sprintf(text, st[PIPEJOBT],
			statename[(int)job->p1], statename[(int)job->p2], job->delay);
		break;

		case	WELLJOB:
		if(job->company == compsel)
			sprintf(text, st[WELLJOBT], 
			statename[STATE((int)job->p1)], TOPLOT((int)job->p1), job->delay);
		break;

		case	STOREJOB:
		if(job->company == compsel)
			sprintf(text, st[STOREJOBT], 
			statename[(int)job->p1], (int)job->p2, job->delay);
		break;

		case	LOANJOB:
		if(job->company == compsel)
			{
			formnum(cost, job->p2);
			space_strip( cost );
			sprintf(text, st[LOANJOBT], cost, (int)(job->delay / 30) );
			}
		break;

		case	SELLJOB:
			sprintf(text, st[SELLJOBT],
			TOPLOT((int)job->p1), statename[STATE((int)job->p1)],job->delay/30 + 1 );
		break;

		case	EXPOILJOB:
		if(compsel == 0)
			sprintf(text, st[EXPOILJOBT],
			(int)job->p3, statename[(int)job->p1], job->delay);
		break;
		}
	if(text[0])
		{
		sprintf(text1, "%-80s", text);
		v_gtext(vdi_handle, x, y + i*gl_hchar, text1);
		++i;
		}
	job = job->next;
	}
sprintf(text1, "%-80s", " ");
for(; i < 15 ; ++i)
	v_gtext(vdi_handle,x,y+i*gl_hchar, text1);
}
/************************************************************************/
/*			returns the largest number sent			*/
/************************************************************************/
scale(numbers,num,scaler,max)	/*numbers is a 1D array (16 elements)*/
real	*numbers,*scaler,*max;
int	num;
{
real	temp;
int	divs = 0,i;

*max = -1E20;
*scaler = 1.0;
for(i = 0 ; i < num ; ++i)
	if(numbers[i] > *max)
		*max = numbers[i];

temp = *max;
if(*max > 1000.0)
	{
	while(temp > 1000.0)
		{
		temp /= GRAPHINT;
		*scaler /= GRAPHINT;
		++divs;
		}
	}
else
	{
	while(temp < (1000.0 / GRAPHINT)  && temp > 0.0)
		{
		temp *= GRAPHINT;
		*scaler *= GRAPHINT;
		--divs;
		}
	}
for(i = 0 ; i < num ; ++i)
	numbers[i] *= *scaler;
return(divs);
}
/***************************************************************************/
DRAWFUNC( soldhist )
{
int	i,c;
real	data[HISTPNTQNT];
char	name[50];

c = wind->p2;
strcpy(name,statename[wind->gpp]);
strcat(name,", ");
strcat(name,comp[c]);
for(i = 0 ; i < HISTPNTQNT ; ++i)
	data[i] = histsold[c][wind->gpp][i];
graphic(wind,wh,mode,LINE,data,HISTPNTQNT,name,nodata,"b",1);
}
/***************************************************************************/
DRAWFUNC( costhist )
{
int	i,c;
real	data[HISTPNTQNT];
char	name[50];

c = wind->p2;
strcpy(name,statename[wind->gpp]);
strcat(name,", ");
strcat(name,comp[c]);
for(i = 0 ; i < HISTPNTQNT ; ++i)
	data[i] = histcost[c][wind->gpp][i];
graphic(wind,wh,mode,LINE,data,HISTPNTQNT,name,nodata,"$",1);
}
/***************************************************************************/
DRAWFUNC( prodhist )
{
int	i,c;
real	data[HISTPNTQNT];
char	name[50];

c = wind->p2;
strcpy(name,statename[wind->gpp]);
strcat(name,", ");
strcat(name,comp[c]);
for(i = 0 ; i < HISTPNTQNT ; ++i)
	data[i] = histproduce[c][wind->gpp][i];
graphic(wind,wh,mode,LINE,data,HISTPNTQNT,name,nodata,"b",1);
}
/***************************************************************************/
DRAWFUNC( revhist )
{
int	i,c;
real	data[HISTPNTQNT];
char	name[50];

c = wind->p2;
strcpy(name,statename[wind->gpp]);
strcat(name,", ");
strcat(name,comp[c]);
for(i = 0 ; i < HISTPNTQNT ; ++i)
	data[i] = histrev[c][wind->gpp][i];
graphic(wind,wh,mode,LINE,data,HISTPNTQNT,name,nodata,"$",1);
}
/***************************************************************************/
DRAWFUNC( prihist )
{
char	dummy;
int	i,c;
real	data[HISTPNTQNT];
char	name[50];

c = wind->p2;
strcpy(name,statename[wind->gpp]);
strcat(name,", ");
strcat(name,comp[c]);
for(i = 0 ; i < HISTPNTQNT ; ++i)
	data[i] = histprice[c][wind->gpp][i]/100.0;
graphic(wind,wh,mode,LINE,data,HISTPNTQNT,name,nodata,"$",1);
}
/***************************************************************************/
DRAWFUNC( cashhist )
{
int	i,c;
real	data[HISTPNTQNT];

c = wind->p2;
for(i = 0 ; i < HISTPNTQNT ; ++i)
	data[i] = MAX(0,histcash[c][i]);
graphic(wind,wh,mode,LINE,data,HISTPNTQNT,comp[c],nodata,"$", 1);
}
/***************************************************************************/
DRAWFUNC( supphist )
{
int	i,c;
real	data[HISTPNTQNT];
char	name[50];

c = wind->p2;
strcpy(name,statename[wind->gpp]);
strcat(name,", ");
strcat(name,comp[c]);
for(i = 0 ; i < HISTPNTQNT ; ++i)
	data[i] = histsupp[c][wind->gpp][i];
graphic(wind,wh,mode,LINE,data,HISTPNTQNT,name,nodata,"brls",1);
}
/**********************************************************************/
DRAWFUNC( infopan )
{
int	dummy;

wind_get(wh,WF_WORKXYWH,&(infodial->ob_x),&(infodial->ob_y),&dummy,&dummy);
TXTPNT(infodial,INFODATE) = currdate;
TXTPNT(infodial,MINUS)= (cashchange(0,RDCASH) < 0.0)?"-$":"+$";
formnum(TXTPNT(infodial,INFOCASH),ABS(cashchange(0,RDCASH)));
str((int)interest,TXTPNT(infodial,INFORATE),2);
sprintf(TXTPNT(infodial,LABIND),"%.1f",labfact*100.0);
sprintf(TXTPNT(infodial,CONIND),"%.1f",bldfact*100.0);
sprintf(TXTPNT(infodial,RAILIND),"%.1f",railfact*100.0);
sprintf(TXTPNT(infodial,INFOTAX),"%d",(int)taxrate);
sprintf(TXTPNT(infodial,IMPPRICE),"%2.2f", price_imp);
if(mode == 2)
	objc_draw(infodial,0,10,x,y,w,h);
else
	{
	objc_draw(infodial,INFODATE,1,x,y,w,h);
	objc_draw(infodial,MINUS,1,x,y,w,h);
	objc_draw(infodial,INFOCASH,1,x,y,w,h);
	objc_draw(infodial,INFORATE,1,x,y,w,h);
	objc_draw(infodial,CONIND,1,x,y,w,h);
	objc_draw(infodial,RAILIND,1,x,y,w,h);
	objc_draw(infodial,LABIND,1,x,y,w,h);
	}

}
/*********************************************************************/
graphic(wind,wh,mode,type,data,numpoints,title,info,units,nocomp)
int	wh,mode,type,numpoints,nocomp;
real	*data;
char	*title,*info,*units;
WINDOWS	*wind;
{
int	x,y,w,h,pxyarray[4],i,j,range;
real	top, oldtop, max, divisor;
int	divs, ldivs, factor;
real	partsum[16],scaler,graphdata[4][16];
char	cents[2];
static	char	*types[]={"Mil","100 Thou","10 Thou",
			  "Thou","Hund","Ten"," "," "," "};
char	ytitle[70];

cents[0] = 155;				/*›*/
cents[1] = '\0';
if(type != OUT2D)
	{
	for(j = 0 ; j < numpoints ; ++j)
		graphdata[0][j] = data[j];
	divs = scale(&graphdata[0][0],numpoints,&scaler,&max);
	}
else
	{
	for(j = 0 ; j < 16 ; ++j)
		partsum[j] = 0;
	for(i = 0 ; i < nocomp ; ++i)
		for(j = 0 ; j < 16 ; ++j)
			partsum[j] += *(data + 16*i + j);
	divs = scale(partsum,numpoints,&scaler,&max);
	for(i = 0 ; i < nocomp ; ++i)
		for(j = 0 ; j < 16 ; ++j)
			graphdata[i][j] = *(data + 16 * i + j)* scaler;

	}

ldivs =  -1.0 *  log10( scaler ) + 0.4;
if(ldivs >= 4)
	range = 0;
if(ldivs == 3 || ldivs == 2 || ldivs == 1 )
	range = 3;
if(ldivs <= 0)
	range = 6;
if(ldivs == -3 && *units == '$')
	{
	range = 8;
	units = cents;
	}

oldtop = pow(10.0, (real)(range - 3) ) / scaler;
top = oldtop;

divs = log10( top );
for( i = 0 , factor = 2 ; i < divs ; ++i)
	factor *= 10.0;
top = (( (int) (top + factor) ) / factor ) * factor;
divisor = oldtop / top;

for(i = 0 ; i < nocomp ; ++i)
	for(j = 0 ; j < numpoints ; ++j)
		graphdata[i][j] *= divisor;

if( wind->p1 != top)
	mode = 2;		/*scale has changed*/
if(mode == 2)
	cls( wh );
wind->p1 = top;
sprintf(ytitle,"%s %s",types[range],units);

wind_get(wh,WF_WORKXYWH,&x,&y,&w,&h);
pxyarray[0] = x;
pxyarray[1] = y;
pxyarray[2] = x+w;
pxyarray[3] = y+h;
switch(type)
	{
	case	LINE:
		linegraph(wh,pxyarray,numpoints,0,1000,&graphdata[0][0],ytitle,title,mode,0,(int)top,0,1);
		break;

	case	BAR:
		bargraph(wh,pxyarray,numpoints,0,1000,&graphdata[0][0],ytitle,info,mode,0,(int)top,0);
		break;

	case	PIE:
		piegraph(wh,pxyarray,numpoints,&graphdata[0][0],title,info,mode,0);
		break;

	case	OUT2D:
		out2D(wh,&graphdata[0][0],nocomp,mode,(int)top,ytitle);
		break;
	}
}
/*****************************************************************************/
int	vpctps[] = {VPCTP0, VPCTP1, VPCTP2,VPCTP3, VPCTP4, VPCTPNO};
int	drawvpchoose;
/*****************************************************************************/
BUTFUNC( butvpc )
{
int	dummy, s;

s	=	wind->gpp;
wind_get(wh, WF_WORKXYWH, &(vpchoose->ob_x),&(vpchoose->ob_y),&dummy,&dummy);
switch( objc_find(vpchoose, 0, 10, mx, my))
	{
	case	VPCTP0:
		vp_cont[s]	= 0;
		drawvpchoose	= 1;
		break;
	case	VPCTP1:
		vp_cont[s]	= 1;
		drawvpchoose	= 1;
		break;
	case	VPCTP2:
		vp_cont[s]	= 2;
		drawvpchoose	= 1;
		break;
	case	VPCTP3:
		vp_cont[s]	= 3;
		drawvpchoose	= 1;
		break;
	case	VPCTP4:
		vp_cont[s]	= 4;
		drawvpchoose	= 1;
		break;
	case	VPCTPNO:
		vp_cont[s]	= -1;
		drawvpchoose	= 1;
		break;
	}
}
/*****************************************************************************/
DRAWFUNC( drawvpc )
{
int	dummy, i, type;
long	cost;

wind_get(wh, WF_WORKXYWH, &(vpchoose->ob_x),&(vpchoose->ob_y),&dummy,&dummy);
for( i = 0 ; i < 6 ; ++i )
	vpchoose[vpctps[i]].ob_state	= NORMAL;
type	= vp_cont[ wind->gpp ];
TXTPNT( vpchoose, VPCST) = statename[ wind->gpp ];

if( type < 0 )
	{
	cost = 0L;
	TXTPNT(vpchoose, VPCBL1 ) = "";
	TXTPNT(vpchoose, VPCBL2 ) = "";
	TXTPNT(vpchoose, VPCBL3 ) = "";
	vpchoose[VPCTPNO].ob_state = SELECTED;
	}
else
	{
	vpchoose[vpctps[type]].ob_state = SELECTED;
	cost = costvp( type );
	TXTPNT( vpchoose, VPCBL1) = st[VPCT01+type*2];
	TXTPNT( vpchoose, VPCBL2) = st[VPCT02+type*2];
	TXTPNT( vpchoose, VPCBL3) = "";
	}

sprintf( TXTPNT( vpchoose, VPCOST), "%ld", cost );
objc_draw( vpchoose, 0, 10, x, y, w, h);
}
/*****************************************************************************/
opn_vpchoose()			/* open vp choose window */
{
static	GRECT pos = {40,50,0,0};

open_window( pos.g_x, pos.g_y, vpchoose->ob_width, vpchoose->ob_height,
	NAME|CLOSER|MOVER, st[WIVPCHOOSE], 0, drawvpc, 0L, 0L, 0L, butvpc,
	&drawvpchoose, statesel, (real)vp_cont[statesel], 0.0,&pos,vpchoose);
}
/*****************************************************************************/

