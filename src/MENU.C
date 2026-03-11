/*****************************************************************************/
/*		File to hold menu, resource, and icon handling routines	     */
/*		Started 07 October 1987					     */
/*****************************************************************************/
#include	"header.h"
#include	"variab.h"

extern	int	char_height;
extern	char	*map_image[];
/*****************************************************************************/
domenu(obj)
int	obj;
{
int	x,y,w,h,i,redraw;
extern	int	outearningsall(),outsupplyall(),outprodall(),outpriall();
extern	int	outcash(),amtsold(),intearn();
extern	int	outjobs(),outplots(),specwell(),butinwell();
extern	int	mprices(),butinprices(),storemen(),butinstore();
extern	int	transrail(),butinrail(),outmarket();
extern	int	butinpipe(),pipemen(),soldhist(),outexpensesall();
extern	int	prihist(),prodhist(),cashhist(),supphist(),revhist();
extern	int	costhist();
extern	int	chout(),butinch(),infopan(),menpros(),butinpros();
extern	int	statedraw(),selectplot();
extern	int	inshow(),butinshow(),menloan(),butinloan();
extern	int	pandldraw(),plarrow(),audraw(),butinau();
extern	int	uniarrow(),trarrow(),baldraw();
extern	int	printnews(),changep();
extern	int	plotdraw(), butinplot();
extern	int	dutch_auction(), butindutch();
extern	int	activity(), butinact();

redraw = railroad || pipemap || mapinfo;

switch(obj)
	{
	case	ABOUTGM:
	form_center(aboutdial,&x,&y,&w,&h);
	aboutdial->ob_x = x;	aboutdial->ob_y = y;
	form_dial(0,x,y,w,h,x,y,w,h);
	form_dial(1,0,0,0,0,x,y,w,h);
	objc_draw(aboutdial,0,10,x,y,w,h);
	form_do(aboutdial,-1);
	(aboutdial+EXITABT)->ob_state = NORMAL|SHADOWED;
	form_dial(2,0,0,0,0,x,y,w,h);
	form_dial(3,x,y,w,h,x,y,w,h);
	redraw_desktop();
	break;

	case	SELLWELL:
		if( plot[plotsel].owner != 0 )
			break;
		sell_well( plotsel );
		updt_sell_menu();
		drawstates	= 1;
		break;

	case	SELLPLOT:
		if( plot[plotsel].owner != 0 )
			break;
		sell_land( plotsel );
		updt_sell_menu();
		drawstates	= 1;
		break;

	case	SELLSTR:
		sell_store( 0, statesel );
		updt_sell_menu();
		drawstates	= 1;
		break;

	case	STOREMEN:
	{
	static	GRECT	pos = {50,50,0,0};
	open_window(pos.g_x,pos.g_y,tanks->ob_width,tanks->ob_height,
	NAME|CLOSER|MOVER|LFARROW|RTARROW, st[WIBS],2,
	storemen,uniarrow,0l,0l,butinstore,&drawstore,statesel,0.0,2.0,&pos,tanks);
	}
	break;

	case	INSHOW:
	{
	static	GRECT	pos = {50,50,0,0};
	open_window(pos.g_x,pos.g_y,intray->ob_width,intray->ob_height,
	NAME|CLOSER|MOVER, st[WIIN],0,
	inshow,0L,0L,0l,butinshow,&intraydraw,statesel,0.0,0.0,&pos,intray);
	}
	break;

	case	PIPEMEN:
	{
	static	GRECT	pos = {70,70,0,0};
	open_window(pos.g_x,pos.g_y,pipedial->ob_width,pipedial->ob_height,
	NAME|CLOSER|MOVER|LFARROW|RTARROW, st[WIBPL],2,
	pipemen,uniarrow,0l,0l,butinpipe,&drawpipe,statesel,6.0,-1.0,&pos,pipedial);
	}
	break;

	{
	static	char	name[20] = "SAVED.GAM";

	case	SAVEGAME:
		graf_mouse( BUSY_BEE, 0L);
		if( get_file_name( name ) )
			save_game( name );
		graf_mouse( ARROW, 0L);
		break;

	case	LOADGAME:
		graf_mouse( BUSY_BEE, 0L);
		if( get_file_name( name ) )
			load_game( name );
		graf_mouse( ARROW, 0L);
		break;
		}

	case	MENLOAN:
		{
		static	GRECT	pos = {100,70,0,0};

		open_window(pos.g_x,pos.g_y,loan->ob_width,loan->ob_height,
		NAME|CLOSER|MOVER, st[WIAL],0,
		menloan,0l,0l,0l,butinloan,&drawloan,2,0.0,maxloan(0),&pos,loan);
		}
		break;

	case	RAILMEN:
		{
		static	GRECT	pos = {40,40,0,0};
		open_window(pos.g_x,pos.g_y,raildial->ob_width,raildial->ob_height,
		NAME|CLOSER|MOVER|LFARROW|RTARROW, st[WITRAN],1,
		transrail,trarrow,0l,0l,butinrail,&drawrail,statesel,0.0,(real)statesel,&pos,raildial);
		}
		break;

	case	PRODALL:
		{
		static	GRECT	pos = {96,54,230,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,NAME|MOVER|CLOSER|SIZER,
		st[WIPRODALL],2,outprodall,0l,0l,0l,0l,0l,0,0.0,0.0,&pos,0L);
		}
		break;

	case	CASHALL:
		{
		static	GRECT	pos = {32,40,230,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,NAME|MOVER|CLOSER|SIZER,
		st[WIREVALL],
		2,outearningsall,0l,0l,0l,0l,0l,0,0.0,0.0,&pos,0L);
		}
		break;

	case	EXPALL:
		{
		static	GRECT	pos = {270,70,230,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,NAME|MOVER|CLOSER|SIZER,
		st[WIEXPALL],
		2,outexpensesall,0l,0l,0l,0l,0l,0,0.0,0.0,&pos,0L);
		}
		break;

	case	MKTSHARE:
		{
		static	GRECT	pos = {100,60,280,100};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|MOVER|CLOSER|SIZER|LFARROW|RTARROW, st[WIMKTSH],
		2,outmarket,uniarrow,0l,0l,0l,0l,statesel,0.0,0.0,&pos,0L);
		}
		break;

	case	PRODHIST:
		{
		static	GRECT	pos = {300,40,200,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|MOVER|CLOSER|SIZER|ARROWS, st[WIHISTPROD],
		0,prodhist,uniarrow,0l,0l,0l,&drawhist,statesel,0.0,(real)compsel,&pos,0L);
		}
		break;	

	case	COSTHIST:
		{
		static	GRECT	pos = {300,40,200,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|MOVER|CLOSER|SIZER|ARROWS, st[WIHISTCOST],
		0,costhist,uniarrow,0l,0l,0l,&drawhist,statesel,0.0,(real)compsel,&pos,0L);
		}
		break;	

	case	REVHIST:
		{
		static	GRECT	pos = {120,120,200,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|MOVER|CLOSER|SIZER|ARROWS, st[WIHISTREV],
		0,revhist,uniarrow,0l,0l,0l,&drawhist,statesel,0.0,(real)compsel,&pos,0L);
		}
		break;	

	case	PANDLMEN:
		{
		static	GRECT	pos = {2,-1,0,0};
		if(pos.g_y == -1)
			pos.g_y = ydesk + gl_hchar + 4;
		open_window(pos.g_x,pos.g_y,pandl->ob_width,pandl->ob_height,
		NAME|MOVER|CLOSER|LFARROW|RTARROW|UPARROW|DNARROW,
		st[WIINC],0,pandldraw,plarrow,
		0l,0l,0l,0l,compsel,0.0,0.0,&pos,0L);
		}
		break;

	case	BIDLAND:
		{
		static	GRECT	pos = {120,70,0,0};
		if(bidtype.plot != -1)
			if( bidtype.dutch )
				open_window(pos.g_x,pos.g_y, 30*gl_wchar, 8*gl_hchar,
				NAME|MOVER|CLOSER, st[DUTCHBID], 1, dutch_auction,0l,
				0l,0l,butindutch, &drawbid, 0, 0.0, 0.0, &pos,0L);
			else
				open_window(pos.g_x,pos.g_y,auction->ob_width,auction->ob_height,
				NAME|MOVER|CLOSER, st[WIBID], 1, audraw, 0l,
				0l,0l,butinau,&drawbid, 0, 0.0, 0.0, &pos, auction);
		}
		break;

	case	BALMEN:
		{
		static	GRECT	pos = {320,-1,0,0};
		if(pos.g_y == -1)
			pos.g_y = ydesk + gl_hchar + 4;
		open_window(pos.g_x,pos.g_y,balance->ob_width,balance->ob_height,
		NAME|MOVER|CLOSER|ARROWS, st[WIBAL],
		0,baldraw,plarrow,0l,0l,0l,0l,compsel,0.0,0.0,&pos,balance);
		}
		break;
		
	case	SUPPHIST:
		{
		static	GRECT	pos = {340,60,200,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|MOVER|CLOSER|SIZER|ARROWS, st[WIHISTSUP],
		0,supphist,uniarrow,0l,0l,0l,&drawhist,statesel,0.0,(real)compsel,&pos,0L);
		}
		break;

	case	CASHHIST:
		{
		static	GRECT	pos = {100,75,200,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|MOVER|CLOSER|SIZER|UPARROW|DNARROW, st[WIHISTCASH],
		0,cashhist,uniarrow,0l,0l,0l,&drawhist,0,0.0,(real)compsel,&pos,0L);
		}
		break;

	case	SOLDHIST:
		{
		static	GRECT	pos = {370,100,200,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|MOVER|CLOSER|SIZER|ARROWS, st[WIHISTSOLD],
		0,soldhist,uniarrow,0l,0l,0l,&drawhist,statesel,0.0,(real)compsel,&pos,0L);
		}
		break;

	case	PRIHIST:
		{
		static	GRECT	pos = {300,40,200,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|MOVER|CLOSER|SIZER|ARROWS, st[WIHISTPRC],
		0,prihist,uniarrow,0l,0l,0l,&drawhist,statesel,0.0,(real)compsel,&pos,0L);
		}
		break;

	case	SUPPALL:
		{
		static	GRECT	pos = {64,64,230,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,NAME|MOVER|CLOSER|SIZER,
		st[WISUPPALL],2,outsupplyall,0l,0l,0l,0l,0l,0,0.0,0.0,&pos,0L);
		}
		break;

	case	TCASHALL:
		{
		static	GRECT	pos = {32,40,200,120};	
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|MOVER|CLOSER|SIZER, st[WICASH],
		2,outcash,0l,0l,0l,0l,0l,0,0.0,0.0,&pos,0L);
		}
		break;

	case	PRIALL:
		{
		static	GRECT	pos = {232,40,200,120};	
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|MOVER|CLOSER|SIZER|LFARROW|RTARROW, st[WIPRICE],
		2,outpriall,uniarrow,0l,0l,0l,0l,statesel,0.0,0.0,&pos,0L);
		}
		break;

	case	INTEARN:
		{
		static	GRECT	pos = {32,40,200,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|CLOSER|MOVER|SIZER, st[WIINTEARN],
		2,intearn,0l,0l,0l,0l,0l,0,0.0,0.0,&pos,0L);
		}
		break;
	case	ACTIVITY:
		{
		static	GRECT	pos = {300,100, 192, -1};
		if(pos.g_h == -1)
			pos.g_h = 12 * gl_hchar;
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,
		NAME|CLOSER|MOVER|UPARROW|DNARROW, comp[compsel],
		0,activity,uniarrow,0l,0l,butinact,&drawstates,0,0.0,(real)compsel,&pos,0L);
		}
		break;

	case	AMTSOLD:
		{
		static	GRECT	pos = {332,100,230,120};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,NAME|CLOSER|MOVER|SIZER,
		st[WIAMTSOLD], 2,amtsold,0l,0l,0l,0l,0l,0,0.0,0.0,&pos,0L);
		}
		break;

	case	OUTJOBS:
		{
		static	GRECT	pos = {64,64,480,112};
		open_window(pos.g_x,pos.g_y,pos.g_w,pos.g_h,NAME|CLOSER|MOVER|SIZER,
		st[WIJOBS],2,outjobs,0l,0l,0l,0l,0l,0,0.0,0.0,&pos,0L);
		}
		break;

	case	INFOPAN:
		{
		static	GRECT	pos = {32,40,0,0};
		open_window(pos.g_x,pos.g_y,infodial->ob_width,infodial->ob_height,
		NAME|CLOSER|MOVER, st[WIINFO],2,
		infopan,0l,0l,0l,0l,0l,0,0.0,0.0,&pos,0L);
		}
		break;

	case	SPECWELL:
		if(plot[plotsel].owner != 0)
			form_alert(1, st[PLOTNOBEL] );
		else
			{
			static	GRECT	pos	= {58,65,0,0};
			open_window(pos.g_x,pos.g_y,welldial->ob_width,welldial->ob_height,
			NAME|CLOSER|MOVER, st[WIBLDWELL],2,
			specwell,0l,0l,0l,butinwell,&drawwell,plotsel,6.0,0.0,&pos,welldial);
			}
		break;
	
	case	MENPROS:
		{
		static	GRECT	pos = {80,50,00,00};
		open_window(pos.g_x,pos.g_y,prosdial->ob_width,prosdial->ob_height,
		NAME|CLOSER|MOVER, st[WIPROS],0,
		menpros,0l,0l,0l,butinpros,&drawpros,plotsel,1.0,0.0,&pos,prosdial);
		}
		break;

	case	CHOUT:
		{
		static	GRECT	pos = {32,40,00,00};
		if( plot[plotsel].owner != 0 || plot[plotsel].type < 0)
			form_alert(1, st[NOCHOUT] );
		else
			open_window(pos.g_x,pos.g_y,chdial->ob_width,chdial->ob_height,
			NAME|CLOSER|MOVER, st[WICHOUT],2,
			chout,0L,0L,0L,butinch,0l,plotsel,plot[plotsel].output,plot[plotsel].output,&pos,chdial);
		}
		break;

	case	MPRICES:
		{
		static	GRECT	pos = {100,100,00,00};
		open_window(pos.g_x,pos.g_y,prices->ob_width,prices->ob_height,
		NAME|CLOSER|MOVER|LFARROW|RTARROW, st[WICHPRC] ,0,
		mprices,uniarrow,0l,0l,butinprices,&drawprice,statesel,0.0,0.0,&pos,prices);
		}
		break;

	case	OPNPAPER:
		{
		static	GRECT	pos = {2,-1,0,0};
		if(pos.g_y == -1)
			pos.g_y = ydesk+gl_hchar + 4;
		open_window(pos.g_x,pos.g_y,wdesk - 4,hdesk - 4*gl_hchar,
		WINDOW, st[WINEWS],0,printnews,changep,0L,0L,0l,0l,0,0.0,0.0,
		&pos,0L);
		}
		break;

	case	OPNPLOT:
		{
		static	GRECT	pos = {20,50,0,0};
		open_window( pos.g_x, pos.g_y, opnplot->ob_width, opnplot->ob_height,
		NAME|CLOSER|MOVER, st[PLOTMES], 0, plotdraw, 0L, 0L, 0L,
		butinplot, 0L, plotsel, 0.0, 0.0, &pos, opnplot );
		}
		break;
	case	GROWMEN:
		growbox ^= 1;
		menu_icheck(menu, GROWMEN, growbox);
		break;
	case	OPENMEN:
		autoopen ^= 1;
		menu_icheck(menu, OPENMEN, autoopen);
		break;
	case	CLOSEMEN:
		autoclose ^= 1;
		menu_icheck(menu, CLOSEMEN, autoclose);
		break;
	case	RAILROAD:
		railroad ^= 1;
		menu_icheck(menu, RAILROAD, railroad);
		redraw_desktop();
		break;
	case	MAPINFO:
		mapinfo ^= 1;
		menu_icheck( menu, MAPINFO, mapinfo );
		redraw_desktop();
		break;
	case	AUTOIN:
		autoin ^= 1;
		menu_icheck( menu, AUTOIN, autoin );
		break;
	case	PIPEMAP:
		pipemap	^= 1;
		menu_icheck( menu, PIPEMAP, pipemap );
		redraw_desktop();
		break;
	case	VPHELP:
		vphelp ^= 1;
		menu_icheck( menu, VPHELP, vphelp );
		break;
	case	GIMEN:
		menu_icheck(menu, compmen[compsel], 0);
		compsel = 0;
		menu_icheck(menu, GIMEN, 1);
		if( redraw )
			redraw_desktop();
		break;
	case	GWMEN:
		menu_icheck(menu, compmen[compsel], 0);
		compsel = 1;
		menu_icheck(menu, GWMEN, 1);
		if( redraw )
			redraw_desktop();
		break;
	case	ROMEN:
		menu_icheck(menu, compmen[compsel], 0);
		compsel = 2;
		menu_icheck(menu, ROMEN, 1);
		if( redraw )
			redraw_desktop();
		break;
	case	BUMEN:
		menu_icheck(menu, compmen[compsel], 0);
		compsel = 3;
		menu_icheck(menu, BUMEN, 1);
		if( redraw )
			redraw_desktop();
		break;

	case	OPNSTATE:
		{
		static	GRECT	pos = {16,40,0,0};
		open_window(pos.g_x,pos.g_y,plots->ob_width,plots->ob_height,
		NAME|CLOSER|MOVER,statename[statesel],0,statedraw,
		0l,0l,0l,selectplot,&drawstates,statesel,0.0,0.0,&pos,plots);
		}
		break;

	default:		/*state selected from menu*/
		for(i = 0 ; obj != menstates[i] ; ++i)
			;
		if(statesel >= 0)
			menu_icheck(menu,menstates[statesel],0);/*uncheck*/
		menu_icheck(menu,obj,1);	/*check current state*/
		statesel = i;
		if( redraw )
			redraw_desktop();	/* new map emphasis*/
		updt_sell_menu();
		break;
	}
}
/**************************************************************************/
rsrc_init()
{
static	int cols[] = { 0x665, 0x700,0x070,0x000};

if( !rsrc_load("mega.rsc") )
	{
	form_alert(1, st[NORSRC] );
	exit( 1 );
	}
rsrc_gaddr(0,MENU,&menu);
rsrc_gaddr(0,TRANSRL,&raildial);
rsrc_gaddr(0,STORAGE,&tanks);
rsrc_gaddr(0,BLDWELL,&welldial);
rsrc_gaddr(0,PRICES,&prices);
rsrc_gaddr(0,PLOTS,&plots);
rsrc_gaddr(0,PLOTICNS,&icons);
rsrc_gaddr(0,ABOUT,&aboutdial);
rsrc_gaddr(0,BLDPIPE,&pipedial);
rsrc_gaddr(0,CHOUTPUT,&chdial);
rsrc_gaddr(0,INFODL,&infodial);
rsrc_gaddr(0,PROSPECT,&prosdial);
rsrc_gaddr(0,INTRAY,&intray);
rsrc_gaddr(0,DESKTOP,&desktop);
rsrc_gaddr(0,LOAN,&loan);
rsrc_gaddr(0,PANDL,&pandl);
rsrc_gaddr(0,BALANCE,&balance);
rsrc_gaddr(0,AUCTION,&auction);
rsrc_gaddr(0,VPWINDOW,&vpwindow);
rsrc_gaddr(0,PLOTWND,  &opnplot );
rsrc_gaddr(0,VPCHOOSE, &vpchoose);

chngimg( raildial ,0 );
chngimg( tanks ,0 );
chngimg( welldial ,0 );
chngimg( prices ,0 );
chngimg( aboutdial ,0 );
chngimg( pipedial ,0 );
chngimg( chdial ,0 );
chngimg( infodial ,0 );
chngimg( prosdial ,0 );
chngimg( intray ,0 );
chngimg( desktop ,0 );
chngimg( loan ,0 );
chngimg( pandl ,0 );
chngimg( balance ,0 );
chngimg( auction ,0 );
chngimg( vpwindow ,0);
chngimg( opnplot, 0);
chngimg( vpchoose, 0);
if( colour )
	Setpallete( cols );

desktop[0].ob_x		= xdesk;
desktop[0].ob_y		= ydesk;
desktop[0].ob_width	= wdesk;
desktop[0].ob_height	= hdesk;
wind_set(0,WF_NEWDESK,desktop,0);
}
/***************************************************************************/
chngobj(tree,index,to)		/*changes object state in tree to 'to'*/
int	index,to;
OBJECT	*tree;
{
int	x,y,w,h;
GRECT	p1,p2;

wind_update( 1 );
tree[index].ob_state = to;		/*change object to*/
objc_offset(tree,index,&(p1.g_x),&(p1.g_y));	/*RECT for object*/
p1.g_w = tree[index].ob_width;	p1.g_h = tree[index].ob_height;
wind_get(0,WF_FIRSTXYWH,&x,&y,&w,&h);
while(w && h)
	{
	p2.g_x = x;	p2.g_y = y;	/*tree window*/
	p2.g_w = w;	p2.g_h = h;
	if(rc_intersect(&p1,&p2))
		objc_draw(tree, index ,10, p2.g_x, p2.g_y, p2.g_w, p2.g_h);
	wind_get(0,WF_NEXTXYWH,&x,&y,&w,&h);
	}
wind_update( 0 );
}
/******************************************************************************/
mapdraw(mapparm)		/* called from object tree to draw map */
PARMBLK	*mapparm;
{
MFDB	source,dest;
int	pxy[8];
GRECT	p1,p2;

source.fd_addr		= map;
source.fd_w		= devwidth;
source.fd_wdwidth	= devwidth / 16;
source.fd_stand		= 0;
source.fd_h		= devheight;
source.fd_nplanes	= nplanes;

dest.fd_addr		= 0L;

p1.g_x = mapparm->pb_x;
p1.g_y = mapparm->pb_y;
p1.g_w = mapparm->pb_w;
p1.g_h = mapparm->pb_h;

p2.g_x = mapparm->pb_xc;
p2.g_y = mapparm->pb_yc;
p2.g_w = mapparm->pb_wc;
p2.g_h = mapparm->pb_hc;

if( rc_intersect(&p1, &p2) )
	{
	set_clip( p2.g_x, p2.g_y, p2.g_w, p2.g_h );

	pxy[0] =	pxy[4] =	p2.g_x;
	pxy[1] =	pxy[5] =	p2.g_y;
	pxy[2] =	pxy[6] =	p2.g_x + p2.g_w - 1;
	pxy[3] =	pxy[7] =	p2.g_y + p2.g_h - 1;
	vro_cpyfm( vdi_handle,S_ONLY,pxy, &source, &dest);
	if( railroad )
		draw_lines(0);		/* draw rail lines */
	if( mapinfo )
		draw_lines(1);		/* draw transport grid */
	if( pipemap )
		draw_lines(2);		/* draw pipes */
	}
}
/**************************************************************************/
see_if_map(mx,my)
int	mx,my;
{
int	i;

for( i = 0 ; i < 16 ; ++i)
	{
	if( ABS( coords[i].c_x - mx) < 15 && ABS( coords[i].c_y - my) < 15 )
		{
		domenu( menstates[i] );
		domenu(OPNSTATE);
		break;
		}
	}
}
/*****************************************************************************/
draw_lines(type)			/* draw rail lines */
int	type;	/* 0==rail-lines; 1==transport; 2==pipes */
{
int	from, to, pxy[6];

vsl_width( vdi_handle, 3);
if( type == 1)
	vsl_ends(vdi_handle, 0, 1);
from = statesel;
for( to = 0 ; to < 16 ; ++to)
	{
	if( from == to )
		continue;
	if(	   ( (type == 0) &&  railmap[from][to] )
		|| ( (type == 1) && (tranoil[compsel][from][to]
				 ||  tranoil[compsel][to][from]) )
		|| ( (type == 2) && (pipe[compsel][from] & (1<<to)) )	   )
		{
		pxy[0] = coords[from].c_x;
		pxy[1] = coords[from].c_y;
		pxy[2] = coords[to].c_x;
		pxy[3] = coords[to].c_y;
		if( pxy[0] < pxy[2]-10 )
			{ pxy[0] += 5;	pxy[2] -= 5;}
		if( pxy[2] < pxy[0]-10 )
			{ pxy[2] += 5;	pxy[0] -= 5;}
		if( pxy[1] < pxy[3]-10 )
			{ pxy[1] += 5;	pxy[3] -= 5;}
		if( pxy[3] < pxy[1]-10 )
			{ pxy[3] += 5;	pxy[1] -= 5;}
		if(  (type == 1) && (tranoil[compsel][to][from])  )
			{
			pxy[4] = pxy[0];	pxy[5] = pxy[1];
			v_pline( vdi_handle, 2, pxy + 2);
			}
		else
			v_pline( vdi_handle, 2, pxy);
		}
	}
vsl_ends(vdi_handle, 0, 0);
vsl_width( vdi_handle, 1);
}
/*****************************************************************************/

OBJECT actwind[] = {
-1, 1, 16, G_BOX, LASTOB, NORMAL, 0x1100L, 0,0, 24,12,
2, -1, -1, G_IMAGE, LASTOB, NORMAL, 0x0L, 0,0, 6,3,
3, -1, -1, G_IMAGE, LASTOB, NORMAL, 0x1L, 6,0, 6,3,
4, -1, -1, G_IMAGE, LASTOB, NORMAL, 0x2L, 12,0, 6,3,
5, -1, -1, G_IMAGE, LASTOB, NORMAL, 0x3L, 18,0, 6,3,
6, -1, -1, G_IMAGE, LASTOB, NORMAL, 0x4L, 0,3, 6,3,
7, -1, -1, G_IMAGE, LASTOB, NORMAL, 0x5L, 6,3, 6,3,
8, -1, -1, G_IMAGE, LASTOB, NORMAL, 0x6L, 12,3, 6,3,
9, -1, -1, G_IMAGE, LASTOB, NORMAL, 0x7L, 18,3, 6,3,
10, -1, -1, G_IMAGE, LASTOB, NORMAL, 0x8L, 0,6, 6,3,
11, -1, -1, G_IMAGE, LASTOB, NORMAL, 0x9L, 6,6, 6,3,
12, -1, -1, G_IMAGE, LASTOB, NORMAL, 0xAL, 12,6, 6,3,
13, -1, -1, G_IMAGE, LASTOB, NORMAL, 0xBL, 18,6, 6,3,
14, -1, -1, G_IMAGE, LASTOB, NORMAL, 0xCL, 0,9, 6,3,
15, -1, -1, G_IMAGE, LASTOB, NORMAL, 0xDL, 6,9, 6,3,
16, -1, -1, G_IMAGE, LASTOB, NORMAL, 0xEL, 12,9, 6,3,
0, -1, -1, G_IMAGE, LASTOB, NORMAL, 0xFL, 18,9, 6,3};

DRAWFUNC( activity )
{
int	i, j, c;
int	x1, y1, w1, h1;
BITBLK	blk, blank;
static	int	changed;

c = wind->p2;
wind_set(wind->handle, WF_NAME, comp[c], 0, 0);
blk.bi_wb = 6;
blk.bi_hl = 3 * char_height;
blk.bi_x = 0;
blk.bi_y = 0;
blk.bi_color = 1;

actwind[0].ob_type = G_BOX;
wind_get(wh,WF_WORKXYWH,&x1,&y1,&w1,&h1);

for(i = 0 ; i < 17 && !changed ; ++i)
	{
	actwind[i].ob_x *= gl_wchar;
	actwind[i].ob_y *= char_height;
	actwind[i].ob_width *= gl_wchar;
	actwind[i].ob_height *= char_height;
	}
changed = 1;
blk.bi_pdata = map_image[0];
blank = blk;
for(i = 1 ; i < 17 ; ++i)
	(actwind[i].ob_spec) = (char *)&blank;
actwind[0].ob_x = x1;
actwind[0].ob_y = y1;
objc_draw(actwind, 0, 10, x, y, w, h);
actwind[0].ob_type = G_IBOX;

/*
** storage icon
*/
blk.bi_pdata = map_image[3];
for( i = 0 ; i < 16 ; ++i)
	{
	if( storage[c][i] > 0.0)
		actwind[i + 1].ob_spec = (char *)&blk;
	}
objc_draw(actwind, 0, 10, x, y, w, h);
/*
** vp icon
*/
for(i = 1 ; i < 17 ; ++i)
	actwind[i].ob_spec = (char *)&blank;
blk.bi_pdata = map_image[5];
for( i = 0 ; i < 16 ; ++i)
	{
	if( !c && vp_cont[i] >= 0)
		actwind[i + 1].ob_spec = (char *)&blk;
	}
objc_draw(actwind, 0, 10, x, y, w, h);
/*
** dollar icon
*/
for(i = 1 ; i < 17 ; ++i)
	actwind[i].ob_spec = (char *)&blank;
blk.bi_pdata = map_image[2];
for( i = 0 ; i < 16 ; ++i)
	{
	if(cashstate[c][i] > 0.0)
		actwind[i + 1].ob_spec = (char *)&blk;
	}
objc_draw(actwind, 0, 10, x, y, w, h);
/*
** land icon
*/
for(i = 1 ; i < 17 ; ++i)
	actwind[i].ob_spec = (char *)&blank;
blk.bi_pdata = map_image[4];
for( i = 0 ; i < 256 ; ++i)
	{
	if(c == plot[i].owner)
		actwind[STATE(i) + 1].ob_spec = (char *)&blk;
	}
objc_draw(actwind, 0, 10, x, y, w, h);
/*
** well icon
*/
for(i = 1 ; i < 17 ; ++i)
	actwind[i].ob_spec = (char *)&blank;
blk.bi_pdata = map_image[1];
for( i = 0 ; i < 256 ; ++i)
	{
	if( (c == plot[i].owner) && (plot[i].type >= 0 ) )
		actwind[STATE(i) + 1].ob_spec = (char *)&blk;
	}
objc_draw(actwind, 0, 10, x, y, w, h);
for(i = 0 ; i < 16 ; ++i)
	v_gtext(vdi_handle, x1 + 6 * gl_wchar * (i%4),
			    y1 + char_height * ( 3 * (i/4) + 1), abbrev[i]);
}
/*****************************************************************************/
BUTFUNC( butinact )
{
int	x1, y1, w1, h1, state;

wind_get(wh,WF_WORKXYWH,&x1,&y1,&w1,&h1);
state  = (mx - x1) / (6 * gl_wchar);
state += 4 * ((my - y1) / (3 * gl_hchar));
domenu( menstates[state] );
domenu( OPNSTATE );
}

