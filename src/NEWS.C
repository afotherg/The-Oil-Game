/************************************************************************/
/*			NEWSPAPER ROUTINES				*/
/*			Started:       August  29, 1986			*/
/*			This version:  March   02, 1987			*/
/************************************************************************/
#include	"header.h"
#include	"variab.h"

#define	BOLD	0x01
#define	ITALIC	0x04
#define ULINE	0x08
#define get_width(x)	width_table[x]
int	yes;
char	*graphpoint;
int	page_effects[20], effects, width_table[128];
extern	char	news_date[];
extern	unsigned	char	*runpt;
/************************************************************************/
printnews(wh,mode,x,y,w,h,wind)
int	wh,x,y,w,h,mode;
WINDOWS	*wind;
{
int	dummy,pxy[4];

if(colour)
	{
	pxy[0] = x;
	pxy[1] = y;
	pxy[2] = x + w - 1;
	pxy[3] = y + h - 1;
	vsf_interior(vdi_handle,2);
	vsf_color(vdi_handle,RED);
	vsf_style(vdi_handle,4);
	vr_recfl(vdi_handle,pxy);
	}
else
	cls(wh);
putpage(wh, wind->gpp);
vst_height(vdi_handle,attrib[7],&dummy,&dummy,&dummy,&dummy);
}
/************************************************************************/
/*initialies *pagepnt[] to point at the beginning of successive pages	*/
/************************************************************************/
pageinit()

{
int	i = 0, dummy;

graf_mouse( BUSY_BEE );
txtpnt = newsbase;
readnode();			/*read required node*/
yes = 0;
cnt = 0;			/*graph structure allocation variable */
pagecount = 0;
pagepnt[0] = txtpnt;
pagegraph[0][0] = 0;
set_clip(0, 0, 1, 1);
while(*txtpnt != FINISH)
	{
	putpage( 0, pagecount );
	++pagecount;
	pagepnt[++i] = txtpnt;
	pagegraph[i][0] = cnt;
	page_effects[i] = effects;
	if(yes)
		{
		yes = 0;
		*graphpoint = ' ';
		}
	}
vst_height(vdi_handle,attrib[7],&dummy,&dummy,&dummy,&dummy);
graf_mouse( ARROW );
}
/************************************************************************/
/* change page								*/
/************************************************************************/
changep(wh,mode,wind)
int	wh,mode;
WINDOWS	*wind;
{
if(mode == 6 && wind->gpp > 0)
	--(wind->gpp);
if(mode == 7 && *txtpnt != FINISH)
	++(wind->gpp);
send_redraw(wh);
}
/************************************************************************/
/*		get the next line of text				*/
/************************************************************************/

txt(string,llength,options,eol)
uchar	string[];
int	*options, *eol, llength;
{
int	i, j, temp, len = 0, new;
int	setpara = 0;
static	int	para_now;

for(j = 0 ; j < 100 ; ++j)
	string[j] = 0;			/*initialise 'string'*/

do	{
	++txtpnt;
	}while( *txtpnt == ' ' || *txtpnt == PARA || *txtpnt == FLUSH);

if(yes)
	{
	*graphpoint = ' ';
	for(j = 0 ; txtpnt[j] != ' '; ++j)
		;
	txtpnt[j] = yes;
	}
for(i = 0 ; len < llength && *options == 0 ; ++i)
	{
	temp = txtpnt[i];
	switch(temp)
		{
		case ' ':		/*check whether next word will fit*/
			for(j = 1, new = 0 ; txtpnt[i + j] > ' ' && txtpnt[i + j] < 128 ; ++j)
				new += get_width(txtpnt[i + j]);
			if(get_width(' ') + len + new >= llength)
				*options = 99;
			else
				{
				string[i] = ' ';
				len += get_width(' ');
				}
			break;

		case TITLE:
		case FLUSH:
		case PARA:
			setpara = 1;
			*options = temp;
			break;
		case HDLINE: 		/* take effect at once */
		case TWOCOL:
		case NEWCOL:
		case NEWPAGE:
		case FINISH:
			*options = temp;
			break;

		case BAR:
		case PIE:
		case LINE:
		case OUT2D:
			yes = temp;
			graphpoint = txtpnt + i;
		case ONECOL:		/*take effect at end of line*/
			string[i] = ' ';
			len += get_width(' ');
			*eol = temp;
			break;

		default:
			string[i] = temp;
			if(string[i] < BBOLD)
				len += get_width(temp);
		break;
		}
	}
txtpnt += i - 1;

space_strip(string);
if(para_now)
	{
	for(i = 99 ; i > 0 ; --i)
		string[i] = string[i - 1];
	string[0] = ' ';
	para_now = 0;
	}
if(setpara)
	para_now = 1;
}
/*****************************************************************************/
/*		output the page to the window				     */
/*****************************************************************************/

putpage(wh, pagenum)
int	wh, pagenum;
{
int	dummy, wcell, hcell, twocols = 1, newpage = 0, lines, k, len;
int	options,eol,pxyarray[4];		/*various formatting options*/
int	lineno, colno, xpos, xinc, ypos, txtorg, i, linesp;
int	top, count, llength, edge, graph_cnt;			/*variables...*/
static	int	prevtitle = 0;
real	*data;
char	*title,*info;		 	/*for graph drawing*/
char	string[150];
char	empty[100][6];		/*prevents overwriting of columns*/

vst_font(vdi_handle, 2);
vst_point(vdi_handle, 14, &dummy, &dummy, &wcell, &hcell);
init_width();
vswr_mode(vdi_handle, MD_TRANS );
wind_get(wh, WF_WORKXYWH, &xwork, &ywork, &wwork, &hwork);
ywork -= 1;
wwork = wdesk - 4;
hwork = hdesk - 4 * gl_hchar;

txtpnt	= pagepnt[pagenum];
vst_effects(vdi_handle, 0);
putheader(pagenum, xwork, ywork);
effects = page_effects[pagenum];
vst_effects(vdi_handle, effects);
graph_cnt = pagegraph[pagenum][0];

linesp = (devheight <= 200) ? 1 : 2;
edge = get_width('x');
lines = (0.80 * hwork) / (hcell + linesp);
txtorg = ywork + hwork - (lines - 1) * (hcell + linesp) - linesp;

for(colno = 0 ; colno < COLUMNS ; ++colno)
	for(lineno = 0 ; lineno < lines ; ++lineno)
		empty[lineno][colno] = 0;		/*initialise empty[][]*/

for(colno = 0 ; colno < COLUMNS ; ++colno)
     {
     xpos = xwork + (wwork*colno) / COLUMNS + edge;
     for(lineno = 0 ; lineno < lines ; ++lineno)
		{
		if(newpage == NEWPAGE)  break;/*exit loop but draw col lines*/
		ypos = txtorg + lineno*(hcell + linesp);
		options = 0,	eol = 0;
		if(empty[lineno][colno]) continue;/*means that TWOCOL used this area*/
		llength = ((twocols*wwork)/COLUMNS)-(2*edge);
		txt(string, llength, &options, &eol);
		if(options != TITLE)		prevtitle = 0;
		switch(options)
			{
			case PARA:		/*new paragraph*/
	
			case FLUSH:		/*print string without justifying*/
			format_just(xpos, ypos, string, wcell, llength, 1);
			break;

			case TITLE:		/*subsidiary heading*/
			set_format(BULINE);
			if(lines-lineno < 2 && prevtitle == 0)
				{
				lineno = lines;
				txtpnt -= 2;
				while(*txtpnt < 128)/*return to beginning of title*/
					--txtpnt;
				set_format(EULINE);
				break;
				}
			capitalise(string);
			if(!prevtitle)
				line(xpos - edge, ypos - hcell + 2, twocols);
			xinc = xpos - edge + ((twocols*wwork)/COLUMNS-prop_len(string))/2;
			format_just(xinc, ypos, string, wcell, llength, 1);
			prevtitle = 1;
			set_format(EULINE);
			break;

			case HDLINE:		/*headline*/
			capitalise(string);
			vst_point(vdi_handle, 18,&dummy,&dummy,&wcell,&hcell);
			xinc = wwork;
			for(k = 0 ; string[k] ; ++k)
				{
				vqt_width(vdi_handle, string[k], &len, &dummy, &dummy);
				xinc -= len;
				}
			xinc /= 2;
			format_just(xwork + xinc, txtorg - hcell, string, wcell, llength, 1);
			vst_point(vdi_handle, 14,&dummy,&dummy,&wcell,&hcell);/* 4 debug*/
			--lineno;		/*headline does not take up a line*/
			break;

			case TWOCOL:		/*start text output over two cols*/
			if( (COLUMNS-colno) > 1 )/*if more than one col left*/
				{
				twocols = 2;
				llength *= 2;
				--lineno;
				break;
				}
			else
				--txtpnt;	/*perform newpage (below)*/

			case FINISH:
			newpage	= NEWPAGE;
			break;

			case NEWPAGE:
			if( !(colno == 0 && lineno == 0) )
				newpage = NEWPAGE;
			else
				--lineno;
			break;

			case NEWCOL:
			if(lineno != 0)
				lineno = lines;   /*force exit from lines loop*/
			else
				--lineno;
			break;

			default:
			format_just(xpos, ypos, string, wcell, llength, 0);
			}
		if( lineno >= 0 )
			{
			empty[lineno][colno] = 1;
			empty[lineno][colno+twocols-1] = 1;
			}
		if(eol == ONECOL)
			{
			llength /= 2;
			twocols = 1;
			}
		if(eol == BAR || eol == PIE || eol == LINE || eol == OUT2D)
			{
			if(lineno < (4*lines) / 10)
				{
				vst_effects(vdi_handle, 0);
				vst_font(vdi_handle, 0);
				yes = 0;
				ypos = txtorg + (lineno + 1) * (hcell + linesp);
				pxyarray[0] = xpos;
				pxyarray[1] = ypos - hcell/2;
				pxyarray[2] = xpos + llength;
				pxyarray[3] = ypos+(5*lines*(hcell+linesp))/10 - linesp;
				count = graph[graph_cnt].count;
				top   = graph[graph_cnt].top;
				data  = graph[graph_cnt].data;
				title = graph[graph_cnt].title;
				info  = graph[graph_cnt].info;
				if(eol == BAR)
					bargraph(0, pxyarray, count, 0, top, data, title, info, 2, 0, top, -1);
				if(eol == PIE)
					piegraph(0, pxyarray, count, data, title, info, 2, -1);
				if(eol == LINE || eol == OUT2D)
					linegraph(0, pxyarray, count, 0, top, data, title, info, 2, 0, top, -1, (eol == OUT2D));
				for(i = lineno ; i <= lineno + (5*lines)/10 + 1 ; ++i)
					{
					empty[i][colno] = 1;
					empty[i][colno + twocols - 1] = 1;
					}
				lineno += (5*lines) / 10 + 1;
				++graph_cnt;
				++cnt;

				vswr_mode(vdi_handle, MD_TRANS );
				vst_font(vdi_handle, 2);
				vst_point(vdi_handle, 14, &dummy, &dummy, &wcell, &hcell);
				vst_effects(vdi_handle, effects);
				}
			}
		}
		pxyarray[0] = xwork + ((colno + 1)*wwork)/COLUMNS;
		pxyarray[1] = ywork + hwork;		/*put on column lines*/
		pxyarray[2] = pxyarray[0];
		pxyarray[3] = pxyarray[1];
		for(count = 0 , i = lines - 1 ; i >= 0 ; --i)
		if(!empty[i][colno + 1])
			++count;
		pxyarray[3] -= ((hcell+linesp)*count);/*allow for double columns*/
		pxyarray[3] += 2;
		v_pline(vdi_handle,2,pxyarray);	/*add vertical column lines*/
		line(pxyarray[2], pxyarray[3], twocols);/*horiz column lines*/
	}
vswr_mode(vdi_handle, MD_REPLACE );
vst_font(vdi_handle, 1);
vst_effects(vdi_handle, 0);
}
/*****************************************************************************/
format_just(xpos, ypos, string, wcell, llength, flush)
int	xpos, ypos, wcell, llength, flush;
unsigned	char	*string;
{
int	i, start = 0, len, format, act_len, length = 0, position = 0;
unsigned	char	text[200];

for(i = 0, act_len = 0 ; string[i] ; ++i)
	if(string[i] < BBOLD)
		act_len += get_width(string[i]);
len = strlen(string);
for(i = 0 ; i < len + 1 ; ++i)
	{
	if(string[i] < BBOLD && string[i])
		text[start++] = string[i];
	else
		{
		text[start] = 0;
		if(act_len)
			length = ((long)prop_len( text ) * llength) / act_len;
		if(length)
			{
			if(flush)
				v_gtext(vdi_handle, xpos + position, ypos, text);
			else
				v_justified(vdi_handle, xpos + position, ypos, text, length, 1, 1);
			}
		position += length;
		set_format( string[i] );
		start = 0;
		}
	}
}
/*****************************************************************************/
set_format(format)
int	format;
{

switch(format)
	{
	case	BBOLD:
	effects |= BOLD;
	break;
	
	case	EBOLD:
	effects &= ~BOLD;
	break;
	
	case	BITALIC:
	effects |= ITALIC;
	break;
	
	case	EITALIC:
	effects &= ~ITALIC;
	break;
	
	case	BULINE:
	effects |= ULINE;
	break;
	
	case	EULINE:
	effects &= ~ULINE;
	break;
	}
vst_effects(vdi_handle, effects);
}
/*****************************************************************************/
line(xpos, ypos, twocols)
int	xpos, ypos, twocols;
{
int	pxyarray[4];

pxyarray[0] = xpos;
pxyarray[1] = ypos - 1;
pxyarray[2] = xpos + (twocols*wwork)/COLUMNS;
pxyarray[3] = ypos - 1;
v_pline(vdi_handle,2,pxyarray);
}
/*****************************************************************************/
putheader(pagenum, xwork, ywork)
int	pagenum, xwork, ywork;
{
char	label[50], month[20];

sprintf(label, "PAGE %d", pagenum + 1);
v_gtext(vdi_handle, xwork, ywork + gl_hchar, label);
v_gtext(vdi_handle, xwork + wwork - prop_len(news_date) - gl_wchar, ywork + gl_hchar, news_date);
}
/*****************************************************************************/
dopipe(c, parameter)		/* newspaper generated pipe building */
int	c, parameter;
{
int	from, to;
real	price;

from	= (parameter>>4)&0x0F;
to	= parameter&0x0F;
price	= pricepipe( from, to, 6 );
startloan( c, MAXLOANTERM, price );	/* fund by loan */
startpipe( c, from, to, 6);
}
/*****************************************************************************/
capitalise(string)
char	*string;
{
register	int	i = 0;

while(string[i])
	{
	if(string[i] >= 'a')
		string[i] -= 'a' - 'A';
	++i;
	}
}
/*****************************************************************************/
prop_len(string)
char	*string;	/* width, in pixels, of a proportional text face */
{
int	len = 0, i;

for(i = 0 ; string[i] ; ++i)
	len += get_width(string[i]);
return(len);
}
/*****************************************************************************/
readnode()		/*reads required node from disk.Writes to txtpnt*/
{
FILE	*nodes;
FILE	*strings, *meg;
FILE	*rti_str, *rti_meg;
int	i,j,alpha = 0,beta = 0,string,replstr,rndnds[6], g, k, plotnum;
unsigned	int	instruction, read;
int		parameter, from, to;
int		rti_num;
unsigned	char	*replc;
int	extra = 0, prev = 'A';				/* extra parameter when needed*/
int	types_used[8];
char	month_date[30];
long	seek, pick;

runpt = txtpnt;	
nodes	= fopen("story.dat","rb");		/* read binary data*/
meg	= fopen("init.meg", "r");
strings = fopen("story.str","rb");		/* read binary data*/
rti_meg = fopen("rti.meg", "r");		/* run time stories */
rti_str = fopen("rti.str", "rb");		/* run time text    */
fscanf(rti_meg, "%d", &rti_num);
types_used[0] = -1;

fseek(nodes,(long)node * 284,0);
for(i = 0 ; i < 5 ; ++i)
	{
	string = getw( nodes );
	pick = string * 12 + 12;
	fseek(meg, pick, 0);
	fscanf(meg, "%ld", &seek);
	fseek(strings,seek,0);
	while((read = getc(strings)) != ENDSTR)
		{
		if(read == ALPHA || read == BETA)
			{
			}
		else
			{
			if(read != ' ')
				*runpt++ = read;
			else
				{
				if(prev != ' ')
					*runpt++ = ' ';
				}
			}
		prev = read;
		}
	if( i == 1 && node)
		for(j = 0, g = 0 ; j < 1 && g <  5 ; j += add_run_time(rti_str, rti_meg, rti_num, types_used), ++g)
			;
	if( i == 3 && node)
		for(j = 0, g = 0 ; j < 2 && g < 10 ; j += add_run_time(rti_str, rti_meg, rti_num, types_used), ++g)
			;
	}
*runpt++ = FINISH;
for( i = 0 ; i < 64 ; ++i )
	{
	instruction = fgetc( nodes );
	parameter   = fgetc( nodes );
	switch( instruction )
		{
		case	PCDOR:
			changedem[0] = parameter;
			break;
		case	PCDMT:
			changedem[1] = parameter;
			break;
		case	PCDMN:
			changedem[2] = parameter;
			break;
		case	PCDWI:
			changedem[3] = parameter;			
			break;
		case	PCDNY:
			changedem[4] = parameter;
			break;
		case	PCDCA:
			changedem[5] = parameter;
			break;
		case	PCDNV:
			changedem[6] = parameter;
			break;
		case	PCDCO:
			changedem[7] = parameter;
			break;
		case	PCDWY:
			changedem[8] = parameter;
			break;
		case	PCDKA:
			changedem[9] = parameter;
			break;
		case	PCDIL:
			changedem[10] = parameter;
			break;
		case	PCDOH:
			changedem[11] = parameter;
			break;
		case	PCDTX:
			changedem[12] = parameter;
			break;
		case	PCDAL:
			changedem[13] = parameter;
			break;
		case	PCDNC:
			changedem[14] = parameter;
			break;
		case	PCDFL:
			changedem[15] = parameter;
			break;
		case	BCDOR:
			demand[0] += parameter*demand[0]/100.0;
			break;
		case	BCDMT:
			demand[1] += parameter*demand[1]/100.0;
			break;
		case	BCDMN:
			demand[2] += parameter*demand[2]/100.0;
			break;
		case	BCDWI:
			demand[3] += parameter*demand[3]/100.0;
			break;
		case	BCDNY:
			demand[4] += parameter*demand[4]/100.0;
			break;
		case	BCDCA:
			demand[5] += parameter*demand[5]/100.0;
			break;
		case	BCDNV:
			demand[6] += parameter*demand[6]/100.0;
			break;
		case	BCDCO:
			demand[7] += parameter*demand[7]/100.0;
			break;
		case	BCDWY:
			demand[8] += parameter*demand[8]/100.0;
			break;
		case	BCDKA:
			demand[9] += parameter*demand[9]/100.0;
			break;
		case	BCDIL:
			demand[10] += parameter*demand[10]/100.0;
			break;
		case	BCDOH:
			demand[11] += parameter*demand[11]/100.0;
			break;
		case	BCDTX:
			demand[12] += parameter*demand[12]/100.0;
			break;
		case	BCDAL:
			demand[13] += parameter*demand[13]/100.0;
			break;
		case	BCDNC:
			demand[14] += parameter*demand[14]/100.0;
			break;
		case	BCDFL:
			demand[15] += parameter*demand[15]/100.0;
			break;
		case	LABFACT:
			chlabfact = parameter;
			break;
		case	BLDFACT:
			chbldfact = parameter;
			break;
		case	TRANFACT:
			chrailfact = parameter;
			break;
		case	NEWWELL:
			whichwell[(parameter>>6)&3] = parameter&0x3F;
			break;
		case	NEWRAIL:
			for(k = 0 ; k < 16 ; ++k)
				if(railmap[0][k])
					railmap[k][parameter] = railmap[parameter][k] = 19*c_dist(parameter, k);
			if( railroad )
				redraw_desktop();	/* to show new line */
			break;
		case	NEWINT:
			interest = parameter/2.0;
			break;
		case	TAXRATE:
			taxrate = parameter;
			break;
		case	PROPLOAN:
			proploan = parameter/10.0;
			break;
		case	DEMFACT:
			demandfact = 5.0 + (((int)parameter)&0xFF) * 0.1;
			break;
		case	PRICEIMP:
			price_imp = (((int)parameter)&0xFF)*0.50;
			break;
		case	KILLWELL:
			break;
		case	PIPEGW:			/* pipe for G & W corp */
			dopipe( 1, parameter);
			break;
		case	PIPERO:
			dopipe( 2, parameter);
			break;
		case	PIPEBU:
			dopipe( 3, parameter);
			break;
		case	EXTRA:
			extra	= parameter;
			break;
		case	NEWPLOT:
			for(k = 0 ; k < 25 ; ++k)
				{
				plotnum = rand()%256;
				if(plot[plotnum].reserves < 10.0)
					plot[plotnum].reserves += 1000 * (rand()%100);
				}
			break;
		case	SPHEREGW:
			sphere[1][parameter]	= extra;
			break;
		case	SPHERERO:
			sphere[2][parameter]	= extra;
			break;
		case	SPHEREBU:
			sphere[3][parameter]	= extra;
			break;
		case	DEMALL:
			for(g = 0 ; g < 16 ; ++g)
				changedem[g] += parameter;
			break;
		default:
			change_ai( ((int)extra)&0xFF, ((int)parameter)&0xFF, instruction);
			break;
		}
	}
for(g = 0 ; g < 2 ; ++g)
	{
	graph[g].count = 10;
	graph[g].top = getw( nodes );
	for(i = 0 ; i < 10 ; ++i)
		{
		instruction = getw( nodes );
		if(instruction == 255)
			graph[g].count = i;
		graph[g].data[i] = instruction;
		}
	for(i = 0 ; i < 10 ; ++i)
		graph[g].title[i] = getc( nodes );
	graph[g].title[10] = 0;
	for(i = 0 ; i < 40 ; ++i)
		graph[g].info[i] = getc( nodes );
	graph[g].info[40] = 0;
	}
node = getw( nodes );
fclose(nodes);
fclose(strings);
fclose(meg);
fclose(rti_meg);
fclose(rti_str);
strcpy(month_date, months[mm]);
space_strip(month_date);
sprintf(news_date, "%s, %d", month_date, yy);
}
/*****************************************************************************/
init_width()
{
int	i, d;

for(i = 32 ; i < 128 ; ++i)
	vqt_width(vdi_handle, i, &width_table[i], &d, &d);
}
/*****************************************************************************/


