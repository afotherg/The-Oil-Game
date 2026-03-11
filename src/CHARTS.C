#include	"header.h"

extern	int	colour;
extern	int	vdi_handle;
extern	int	attrib[];
/*****************************************************************************/
/*	       	utility to draw line graph				     */
/*****************************************************************************/

linegraph(wh,pxyarray,count,bot,top,data,title,info,mode,botnum,topnum,paper,line)
int	wh,count,pxyarray[],botnum,topnum,mode,paper;
int	bot,top,line;
char	*title,*info;
real	data[];
/*if mode==2 a full redraw is requested*/
{
int	len,breadth,wcell,i,xpos,ypos,x,y,dummy;
int	scale,wbar,hbar,pxy[66],inc,quadrant[4],txtht;
char	point[6];
real	num;

len = pxyarray[2] - pxyarray[0];
breadth = pxyarray[3] - pxyarray[1];
if(mode == 2)
	{
	axes(wh,pxyarray,len,breadth,&x,&y,&wbar,&hbar,mode,line);
	puttitles(pxyarray,title,paper);
	wcell = putlegend(pxyarray,x,y,wbar,len,breadth,count,info,line);
	}
else
	axes(wh,pxyarray,len,breadth,&x,&y,&wbar,&hbar,mode,line);

vsf_color(vdi_handle,0);
vsf_style(vdi_handle,8);
quadrant[0] = x; 
quadrant[1] = y - hbar;
quadrant[2] = x + wbar - 1;
quadrant[3] = y;
vr_recfl(vdi_handle,quadrant);
if(paper != -1)
	{
	vsl_type(vdi_handle,2);
	quadrant[0] = x;
	quadrant[2] = x + wbar;
	vsl_color(vdi_handle,GREEN);
	for(i = 0 ; i < 4 ; ++i)
		{
		quadrant[1] = y - (i + 1) * hbar / 4; 
		quadrant[3] = quadrant[1];
		v_pline(vdi_handle,2,quadrant);
		}
	vsl_type(vdi_handle,1);
	}

scale = top-bot;
vsm_height(vdi_handle,breadth/30);		/*set polymarker height*/
vsm_type(vdi_handle,6);				/*polymarker type, diamond*/
vsl_width(vdi_handle,3);			/*line width= 3 pixels*/
wbar -= 3;
hbar -= 3;
for(i = 0 ; i < count - 1 ; ++i)
	{
	inc = i * 4;
	pxy[0 + inc] = x + (i*wbar) / (count - 1);
	pxy[1 + inc] = y - ((data[i] - bot)*hbar) / scale;
	pxy[2 + inc] = x+( (i + 1) *wbar) / (count - 1);
	pxy[3 + inc] = y-( (data[i + 1] - bot)*hbar) / scale;
	}
wbar += 1;
hbar += 3;
/*v_pmarker(vdi_handle,(count - 1) * 2,pxy);*/
vsl_color(vdi_handle,RED);
v_pline(vdi_handle,(count - 1) * 2,pxy);
vsl_width(vdi_handle,1);
if(paper == -1)
	{
	vsf_interior(vdi_handle, 3);
	vsf_style(vdi_handle, 12);
	vsf_color(vdi_handle, GREEN);
	pxy[4 * count - 4] = pxy[4 * count - 6];
	pxy[4 * count - 3] = y;
	pxy[4 * count - 2] = x;
	pxy[4 * count - 1] = y;
	v_fillarea(vdi_handle, count * 2, pxy);
	}
if(mode == 2)
	number_axis(pxyarray, topnum, botnum, x, y, bot, hbar);
vsl_color(vdi_handle, BLACK);
vst_height( vdi_handle, attrib[7], &dummy, &dummy, &dummy, &dummy );
}
/************************************************************************/
/*		puts text on for each point on eg linegraph		*/
/************************************************************************/
putlegend(pxyarray,x,y,wbar,len,breadth,count,info,line)
char	*info;
int	pxyarray[],line;
{
int	i=0,xpos,ypos,num,txtht,dummy,wcell;
if(line != 1)
	{
	txtht = get_height(max_len(info,count),pxyarray[3] - y,(pxyarray[2] - x)/count);
	vst_height(vdi_handle,txtht,&dummy,&dummy,&wcell,&dummy);
	vst_rotation(vdi_handle, 900);
	vswr_mode(vdi_handle, MD_TRANS);
	for(i = 0 ; i < count ; ++i)
		{
		num  = strlen(info);
		xpos = x + ((i + 1)*wbar) / count - 3;
		ypos = y + 2 + num * wcell;
		v_gtext(vdi_handle, xpos, ypos, info);
		info += num + 1;
		}
	vst_rotation(vdi_handle, 0);
	vswr_mode(vdi_handle, MD_REPLACE );
	}
if(line)
	{
	int	hcell;
	txtht = get_height(strlen(info), pxyarray[2] - x, pxyarray[3] - y - 2);
	vst_height(vdi_handle, txtht, &dummy, &dummy, &wcell, &hcell);
	v_gtext(vdi_handle, x, pxyarray[3] - 2,info);
	}
vst_height( vdi_handle, attrib[7], &dummy, &dummy, &dummy, &dummy );
return(wcell);
}	
/*****************************************************************************/
/*              utility to draw bar graph 				     */
/*****************************************************************************/

bargraph(wh,pxyarray,count,bot,top,data,title,info,mode,botnum,topnum,paper)
int	wh;			/*  window handle */
int	pxyarray[];		/*  co-ords of top lft and bot rt corners*/
int	count;			/*  no. of bars to be drawn */
int	bot;			/*  minimum y-scale number */
int	top;			/*  maximum y-scale number */
real	data[];			/*  'count' numbers for bar heights*/
char	*title,*info;		/*  pointers to headings etc*/
int	mode;			/*  if mode == 2 a full redraw*/
int	botnum,topnum;
{
int x,y,i,xpos,ypos;                    /* origin of actual graph*/
int wbar,hbar,pxy[4];
int len,breadth,scale,tall,wcell, txtht, dummy;
char point[6];

if(mode == 2)
	puttitles(pxyarray,title,paper);	/* displays titles and key  */
len=pxyarray[2]-pxyarray[0];
breadth=pxyarray[3]-pxyarray[1];

axes(wh,pxyarray,len,breadth,&x,&y,&wbar,&hbar,mode,0);
wcell=putkey(wh,pxyarray,count,info,mode,-1L + paper,data,y);
scale=top-bot;
wbar=(20*wbar)/21;				/*make bars slightly less wide*/
for(i = 0 ; i < count ; ++i)
        {
	getcolour(i);
        tall=((data[i]-bot)*hbar)/scale;
        pxy[0]=x+i*wbar/count;
        pxy[1]=y-tall;
        pxy[2]=x+(i+1)*wbar/count - 2;
        pxy[3]=y;
        v_bar(vdi_handle,pxy);
        vsf_color(vdi_handle,0);
	pxy[3] = pxy[1] - 1;
	pxy[1] = y - hbar;
	v_bar(vdi_handle,pxy);
	}

if(mode == 2)
	number_axis(pxyarray, topnum, botnum, x, y, bot, hbar);
vst_height( vdi_handle, attrib[7], &dummy, &dummy, &dummy, &dummy );
}
/************************************************************************/
/*                         draws axes,returns origin co-ords            */
/************************************************************************/
axes(wh,pxyarray,len,breadth,x,y,wbar,hbar,mode,line)
int *x,*y,*wbar,*hbar,pxyarray[],line;

{
int pxy[4];

*x=pxyarray[0]+len/4;
*y=pxyarray[1]+(((line==1)?8.5:7)*breadth)/10;           /*co-ords of origin*/
*wbar=pxyarray[2]-*x;
*hbar=(*y)-(pxyarray[1]+breadth/8);
if(mode != 2)
	return;
vsl_ends(vdi_handle,0,1);                       /*arrowed polyline*/
pxy[0]=(*x-1);
pxy[1]=(*y+1);
pxy[2]=(*x-1);
pxy[3]=pxyarray[1] + 3;
v_pline(vdi_handle,2,pxy);
pxy[2]=(*x-1)+*wbar;
pxy[3]=(*y+1);
v_pline(vdi_handle,2,pxy);
vsl_ends(vdi_handle,0,0);                       /*draw axes*/
}
/*****************************************************************************/
/*              utility for drawing piecharts				     */
/*****************************************************************************/

piegraph(wh,pxyarray,count,data,title,info,mode,paper)
int	wh,pxyarray[],count;
char	*title,*info;
real	data[];
{
int	len,breadth,x,y,i;
int	xcent,ycent,xradius,yradius;
int	begang=0,endang=0;
long	tempang=0;
long	sum = 0;

if(mode == 2)
	puttitles(pxyarray,title,paper);
len=pxyarray[2]-pxyarray[0];
breadth=pxyarray[3]-pxyarray[1];
x=pxyarray[0];
y=pxyarray[1]+(7*breadth)/10;           /*co-ords of origin*/
xcent=x+len/2;
ycent=y-(29*breadth)/100;               /*co-ords of circle centre*/
xradius=((xcent-x)*3)/4;
yradius=y-ycent;

for(i = 0 ; i < count ; ++i)
        sum += data[i];                   /*sum the data items*/
putkey(wh,pxyarray,count,info,mode,sum,data,y);

vsf_color(vdi_handle,1);
for(i = 0 ; i < count ; ++i)
        {
	getcolour(i);
	if( sum == 0 )
		endang = 0;
	else
	        endang +=(3600*data[i])/sum;
        v_ellpie(vdi_handle,xcent,ycent,xradius,yradius,begang,endang);
        begang=endang;
        }
vst_height(vdi_handle,attrib[7],&i,&i,&i,&i);
}
/************************************************************************/
/*              draw box, display titles	                        */
/************************************************************************/

puttitles(pxyarray,title,paper)
int	pxyarray[],paper;
char	*title;
{
int pxy[10];
int dummy,wcell,hcell,xpos,ypos,txtht;
int i=0,len,breadth,wbox;

pxy[0]=pxyarray[0];
pxy[1]=pxyarray[1];
pxy[2]=pxyarray[2];
pxy[3]=pxyarray[1];
pxy[4]=pxyarray[2];
pxy[5]=pxyarray[3];
pxy[6]=pxyarray[0];
pxy[7]=pxyarray[3];
pxy[8]=pxyarray[0];
pxy[9]=pxyarray[1];
v_pline(vdi_handle,5,pxy);


len=pxy[2]-pxy[0];
breadth=pxy[5]-pxy[1];

ypos=pxy[1]+(65*breadth)/100;
vst_rotation(vdi_handle,900);                   /*set vertical writing*/
txtht = get_height(strlen(title),ypos - pxy[1],100);
vst_height(vdi_handle,txtht,&dummy,&dummy,&wcell,&hcell);
xpos=pxy[0] + hcell;
vswr_mode(vdi_handle, MD_TRANS );
v_gtext(vdi_handle,xpos,ypos,title);
vswr_mode(vdi_handle, MD_REPLACE );
vst_rotation(vdi_handle, 0);                     /*add y-axis title*/
vst_height(vdi_handle, attrib[7], &dummy, &dummy, &dummy, &dummy);
}
/************************************************************************/
/*                   adds the key to bar/pie graphs			*/
/************************************************************************/
putkey(wh,pxyarray,count,info,mode,sum,data,y)
int	pxyarray[],y;
char	*info;
real	data[];
long	sum;
{
int	dummy,wcell,hcell,len,breadth,i,xpos,ypos,pxy[4],percent,length,txtht;
int	x0, y0;

len = pxyarray[2]-pxyarray[0];
breadth = (3 * (pxyarray[3]-pxyarray[1]) ) / 10;
x0 = pxyarray[0];
y0 = y + (5 * breadth) / 100;
breadth = (breadth / 2) * 2;		/* make even */
txtht = get_height(max_len(info,count), (7 * len) / 20, breadth / 2);
vst_height(vdi_handle,txtht,&dummy,&dummy,&wcell,&hcell);
if(mode != 2)
	return(wcell);
vswr_mode(vdi_handle, MD_TRANS );
for(i = 0 ; i < count ; ++i)
        {
	getcolour(i);
        ypos = y0 + (3 * breadth) / 40;
        ypos += (i / 2) * (breadth / 2);
        xpos = x0 + (9 * len) / 400 + (i%2) * (len/2);
        pxy[0] = xpos;
        pxy[1] = ypos;
        pxy[2] = xpos + (21 * len) / 200;
        pxy[3] = ypos + (7 * breadth) / 20;
        v_bar(vdi_handle,pxy);
	length  =  strlen(info);
	if(sum > 0)
		{
		percent = ((data[i]*100) / sum) + 0.5;
			str(percent,info + length - 5,3);
		info[length - 2] =  '%';
		}
        v_gtext(vdi_handle, xpos + (51 * len) / 400, pxy[3], info);
        info += length + 1;
        }
vst_height(vdi_handle, attrib[7], &dummy, &dummy, &wcell, &hcell);
vswr_mode(vdi_handle, MD_REPLACE );
return(wcell);
}
/*****************************************************************************/
/*              turns a number into a string (max 5 digits)             */
/************************************************************************/

str(n,point,num)
int n,num;
char *point;
{
int i;

for(i = num-1 ; i >= 0 ; --i)
        {
        *(point+i)='0'+n%10;
        n/=10;
        }
i=0;
while(*(point+i) == '0' && i != num-1)
        *(point + i++) = ' ';		/*remove leading zeroes, bar last one*/
*(point+num)='\0';			/*add null terminator*/
}
/************************************************************************/
get_height(len,axialroom,tranroom)
int	len;			/*length of text string*/
int	axialroom,tranroom;/*length (in pixels) available along\across text*/
{
int	txtht = 10,wcell,dummy,hcell;

while( txtht > 2 )
	{ 
	vst_height(vdi_handle,txtht,&dummy,&dummy,&wcell,&hcell);
	if((axialroom > len * wcell)  && (tranroom > hcell))
		break;
	--txtht;
	}
return(txtht);
}
/*****************************************************************/
max_len(string,n)	/*returns the length of the longest string in string*/
char	*string;
int	n;		/* # of strings embedded in string*/
{
int	maxlen = 0;

while(n--)
	{
	maxlen = MAX(maxlen,strlen(string));
	string += strlen(string) + 1;
	}
return(maxlen);
}
/***************************************************************/
getcolour(i)
int	i;
{
vsf_interior(vdi_handle,2);
if(colour)
	{
	switch(i)
		{
		case	0:	
		vsf_color(vdi_handle,RED);
	        vsf_style(vdi_handle,4);
		break;
		case	1:	
		vsf_color(vdi_handle,GREEN);
	        vsf_style(vdi_handle,8);
		break;
		case	2:	
		vsf_color(vdi_handle,BLACK);
	        vsf_style(vdi_handle,4);
		break;
		case	3:	
		vsf_color(vdi_handle,RED);
	        vsf_style(vdi_handle,8);
		break;
		}
	}
else
      	{
	vsf_style(vdi_handle,6+(i%2)*3+i/2);
	vsf_color(vdi_handle,BLACK);
	}
}
/*****************************************************************************/

number_axis(pxyarray, topnum, botnum, x, y, bot, hbar)
int	pxyarray[], topnum, botnum, x, y, bot, hbar;
{
int	txtht, dummy, wcell, i, xpos, ypos, scale;
char	point[10];
real	num;

scale = topnum - botnum;
vsf_color(vdi_handle, BLACK);
txtht = get_height(5, x - pxyarray[0] - 2, (y - pxyarray[1])/5);
vst_height(vdi_handle, txtht, &dummy, &dummy, &wcell, &dummy);
vswr_mode(vdi_handle, MD_TRANS );
for(i = 0 ; i < 5 ; ++i)			/*number the y-axis*/
	{
	num = (bot + (((real)i*scale))/4);
	if(num > 9.9999)
	        sprintf(point, "%4d", (int)num);
	else
		sprintf(point, "%4.1f", num);
	xpos = x - 4*wcell - 2;
	ypos = y - (i*hbar)/4;
	v_gtext(vdi_handle, xpos, ypos, point);
	}
vswr_mode(vdi_handle, MD_REPLACE );
vst_height(vdi_handle, attrib[7], &i, &i, &i, &i);
}
/*****************************************************************************/

