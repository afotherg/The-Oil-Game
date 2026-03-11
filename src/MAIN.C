#include	"header.h"
/************************************************************************/
/*			ECONOMIC VARIABLES				*/
/************************************************************************/
/*  QUANTISED  */
long	_stksize = 10000;	/* for safety */
int	gl_apid;
/*****************************************************************************/
real	produce[4] [16];	/*amount produced each period*/
real	amountsold [16];	/*amnt sold in preceeding period*/
real	sold [4] [16];		/*amount each sells in each state*/
real	cashstate [4] [16];	/*amount of cash generated in each state*/
real	expstate  [4] [16];	/*expenses by company/state*/
real	supply [4] [16];	/*amount each company has in each state*/
real	endsupply[4][16];	/*amount left at end of quanta*/
real	pros_tax[4];		/* prospective tax */
real	pros_profit[4];		/* prospective profit after tax */
/*************************************************************************/
/*   FISCAL PERIOD   */
real	revfisc[4];		/*revenue in fiscal period*/
real	expfisc[4];		/*expenditure in fiscal period*/
real	intpdfisc[4];		/*interest paid in fiscal period*/
real	intearnfisc[4];		/*interest earned in fiscal period*/
real	adminfisc[4];		/*administration costs per fiscal period*/
real	accumdep[4];		/*accumulated depreciation*/
real	debentures[4];		/*level of company debt*/
real	profit[4];		/*profit in last fiscal period*/
real	land[4];		/*value of land held*/
real	import[4];

real	inventory[4][4];	/*inventory at end of period*/
/***************************************************************************/
/* <HISTORIC VARIABLES>::=<PAST HISTORY OF ANY OF ABOVE>		   */
/*									   */
uint	histsold[4][16][HISTPNTQNT];	/* historic total solds		   */
real	histcash[4][HISTPNTQNT];	/* historic cash of companies 	   */
uint	histprice[4][16][HISTPNTQNT];	/* historic price of companies 	   */
uint	histproduce[4][16][HISTPNTQNT];	/* historic produce of companies   */
uint	histsupp[4][16][HISTPNTQNT];	/* historic supplies 		   */
real	histrev[4][16][HISTPNTQNT];	/* historic revenue		   */
real	histcost[4][16][HISTPNTQNT];	/* historic drilling costs	   */
/***************************************************************************/
	/*  TIME INDEPENDENT  */
unsigned int	pipe [4] [16];	/*pipe system company/state*/
SBYTE	railmap[16][16];	/*map of costs between states by rail
				  zero if no route*/
SBYTE	sphere[4][16];		/* spheres of influence of companies */
SBYTE	vp_cont[16];		/* type of control of Gitty by state */
real	capval[4];		/*total capital value*/
real	sellmin [4] [16];	/*minimum sell price company in state*/
real	storage [4] [16];	/*storage capacity in each state*/
real	store_beingbuilt[4][16];/*storage being built*/
real	demand [16];		/*demand from each state-taken from tree*/
real	cash [4];		/*bank balance of each company*/
real	bldfact=1.0;		/*measure of demand independent building cost*/
real	railfact=1.0;		/*measure of rail costs*/
real	labfact=1.0;		/*measure of real labour costs*/
int	tranoil[4][16][16];	/*what oil is transported in barrels
				 by company,from,to*/
/************************************************************************/
/* EXOGENOUS variables 	- determined by tree structure*/
SBYTE	changedem[16];		/*change in demands over fiscal period*/
SBYTE	chbldfact;		/*percentage change in bldfact*/
SBYTE	chrailfact;		/*percentage change in rail factor*/
SBYTE	chlabfact;		/*change in labour cost factor*/
SBYTE	taxreg;			/*tax regime at moment*/
SBYTE	pipeindex;		/*pipe params index*/
real	interest = 12.0;	/*prevailing interest rate*/

int	whichwell[4]={0,1,2,3};/*holds index from well pressed in dialogue
				to which param_wells is used*/
real	taxrate = 30.0;		/* rate of tax */
real	demandfact = 14.95;	/* demand factor */
real	price_imp  = 20.00;	/* price per barrel of imported oil */
real	tot_land   = 1.0e6;	/* total of all land sold */
int	numauc	   = 1;		/* number of auctions */
/*************************************************************************/
real	proscost[] = {50e3, 1e5, 15e4, 25e4 };
real	prostime[] = {1.0 , 1.0, 2.0, 3.0};
WELLTYPE param_wells[10];
/***************************************************************************/
unsigned	char	*txtpnt;
unsigned	char	*pagepnt[20];	/*pointers to the beginning of each page*/
unsigned	char	newsbase[10000];
CHARTS		graph[2];
int	node;			/*node that we are in in newspaper*/
int	cnt=0;
int	pagegraph[30][3];
int	pagecount,counter;			/*number of pages*/
int	attrib[9];
int	statesel;
int	plotsel;			/*state and plot selected*/
int	compsel;			/* company selected */
int	drawstates;			/*conditional redraw global*/
int	drawhist;			/*cond redraw historical windows*/
int	drawrail;			/*cond redraw rail dialog*/
int	drawprice;			/*cond redraw price dialog*/
int	drawpros;			/*cond redraw prospect dialog*/
int	drawvp;				/*cond redraw for vp event*/
int	drawstore;			/*cond redraw for storage */
int	drawwell;
int	drawloan;
int	drawbid;
int	drawpipe;
int	intraydraw;

int	day;
int	yy = 1921,mm,dd;
int	numperiods;			/* number of periods played */

char	currdate[] = "August     6, 1987";
char	*months[]={"January  ","February ","March    ","April    ","May      ",
       "June     ","July     ","August   ","September","October  ","November ","December "};
char	stateabbrev[]=  "Or\0Mt\0Mn\0Wi\0NY\0Ca\0Nv\0Co\0Wy\0Ka\0Il\0Oh\0Tx\0Al\0NC\0Fl";
char	*statename[] = 	{
			"Oregon",		/* 0 */
			"Montana",		/* 1 */
			"Minnesota",		/* 2 */
			"Wisconsin",		/* 3 */
			"New York",		/* 4 */
			"California",		/* 5 */
			"Nevada",		/* 6 */
			"Colorado",		/* 7 */
			"Wyoming",		/* 8 */
			"Kansas",		/* 9 */
			"Illinois",		/*10 */
			"Ohio",			/*11 */
			"Texas",		/*12 */
			"Alabama",		/*13 */
			"Carolina",		/*14 */
			"Florida"		/*15 */
			};
char	*comp[] =       {
			"Gitty",
			"G & W",
			"Roco",
			"Burns"
			};
char	companies[] =   "Gitty\0G & W\0Roco\0Burns";
char	nodata[]    =	"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
char	compwithpc[]=	"Gitty(000%)\0G & W(000%)\0Roco(000%)\0Burns(000%)";
int	defines[]={PLOT0,PLOT1,PLOT2,PLOT3,PLOT4,PLOT5,PLOT6,PLOT7,
		   PLOT8,PLOT9,PLOT10,PLOT11,PLOT12,PLOT13,PLOT14,PLOT15};
int defineslt[] = {PLOTLT0,PLOTLT1,PLOTLT2,PLOTLT3,PLOTLT4,PLOTLT5,PLOTLT6,
		   PLOTLT7,PLOTLT8,PLOTLT9,PLOTLT10,PLOTLT11,PLOTLT12,PLOTLT13,
		   PLOTLT14,PLOTLT15};
int definesl3[] = {PLOTL30,PLOTL31,PLOTL32,PLOTL33,PLOTL34,PLOTL35,PLOTL36,
		   PLOTL37,PLOTL38,PLOTL39,PLOTL310,PLOTL311,PLOTL312,PLOTL313,
		   PLOTL314,PLOTL315};

int	plotown[]= {PLOTBLNK,PLOTG,PLOTW,PLOTR,PLOTB};
int	statesto[]={STATEOR,STATEMT,STATEMN,STATEWI,STATENY,STATECA,STATENV,
		    STATECO,STATEWY,STATEKA,STATEIL,STATEOH,STATETX,STATEAL,
		    STATENC,STATEFL};
int	pipestates[]={OHSTATE,MISTATE,TXSTATE,CASTATE,UTSTATE,NYSTATE,COSTATE,
		      ALSTATE,PASTATE,NJSTATE,WASTATE,NVSTATE,IOSTATE,MOSTATE,
		      KASTATE,AZSTATE};
int	menstates[]={OHMEN,MIMEN,TXMEN,CAMEN,UTMEN,NYMEN,COMEN,
		      ALMEN,PAMEN,NJMEN,WAMEN,NVMEN,IOMEN,MOMEN,KAMEN,AZMEN};

int	compmen[] ={GIMEN,GWMEN,ROMEN,BUMEN};

COORD	coords[] = {	91 , 40,		/* Or */
			141, 53,		/* Mt */
			255, 71,		/* Mn */
			369,103,		/* Wi */
			552, 63,		/* NY */
			55 ,130,		/* Ca */
			97 ,145,		/* Nv */
			203,175,		/* Co */
			191,127,		/* Wy */
			271,193,		/* Ka */
			379,162,		/* Il */
			444,161,		/* Oh */
			278,322,		/* Tx */
			346,245,		/* Al */
			427,204,		/* NC */
			443,262			/* Fl */
			};

int	welltypes[]={WELL1,WELL2,WELL3,WELL4};
int	prostypes[]={PROS0,PROS1,PROS2,PROS3};
char	*abbrev[]={"Or","Mt","Mn","Wi","NY","Ca","Nv","Co","Wy","Ka",
		    "Il","Oh","Tx","Al","NC","Fl"};
SBYTE	*map;			/* pointer to buffer for map */

PLOT	plot[256];
OBJECT	*desktop;
OBJECT	*raildial;
OBJECT	*welldial;
OBJECT	*prices;
OBJECT	*tanks;
OBJECT	*plots;
OBJECT	*aboutdial;
OBJECT	*pipedial;
OBJECT	*chdial;
OBJECT	*infodial;
OBJECT	*prosdial;
OBJECT	*intray;
OBJECT	*loan;
OBJECT	*icons;
OBJECT	*pandl;
OBJECT	*balance;
OBJECT	*auction;
OBJECT	*vpwindow;
OBJECT	*opnplot;
OBJECT	*vpchoose;

int	gl_hchar;
int	gl_wchar;
int	gl_wbox;
int	gl_hbox;	/* system sizes */
int 	vdi_handle;	/* app's workstation handle */
int	xdesk,ydesk,hdesk,wdesk;
int	xold,yold,hold,wold;
int	xwork,ywork,hwork,wwork;	/* desktop and work areas */
int	topwindow;			/* handle of top window*/
int	hidden;		/* current state of cursor */
int	contrl[12];
int	intin[128];
int	ptsin[128];
int	intout[128];
int	ptsout[128];	/* storage wasted for idiotic bindings */
WINDOWS	window[5];	/* window 4 reserved for VP ramblings  */
OBJECT	*menu;
JOBS	firstjob;	/* first job in linked list*/
MSG	firstmsg = {"-----------         no messages        -----------",
		    "                                                  ",
		    "                                                  ",
		    "                   "};/* first message in linked list*/
PLTYPE	placcount[4][4];
BALTYPE balsheet[4][4];
BIDTYPE	bidtype = {-1};
int	month_sale = -1;
int	colour;
int	growbox;
int	autoclose	= 1;
int	autoopen	= 1;
int	railroad	= 0;		/* TRUE if railroads to be drawn */
int	mapinfo		= 0;		/* TRUE if info on map */
int	autoin		= 0;		/* TRUE if in tray auto opening */
int	isevent		= 0;		/* TRUE if VP event happening*/
int	vphelp		= 1;		/* TRUE if VP help screens */
int	pipemap		= 0;
int	deskmap		= 0;		/* TRUE if enough memory for map */
int	scalechange;			/* Used to fudge multiple rect graph redraw*/
int	devwidth;			/* sizes of screen etc */
int	devheight;
int	numcols;
int	nplanes;
char	*st[300];			/* change eventually */
/*****************************************************************************/
			/* exogenous variables to control AI long term behaviour */
real	proploan = 2.0;		/* propensity for bank to loan */

/*****************************************************************************/
main()
{
long	time_now;

appl_init();

initgem();			/* all from DISK 1			*/

form_alert(1, "[1][Insert Story Disk][ OK ]" );				/* now ask for DISK 2			*/
init_strings();			/* initialise all to do with strings	*/
rsrc_init();			/* initialise the resource		*/
load_game("game.sav");		/* load in game start position		*/
init_wells();			/* load in wells data from disk		*/
load_icons();			/* load in icon forms */
readpic();
getlevel();			/* get level of play */
menu_bar(menu,1);
if(deskmap)
	redraw_desktop();		/* draw the desktop now			*/
else
	objc_draw(desktop, 0, 10, xdesk, ydesk, wdesk, hdesk);
initfisc();

addmsg( st[WELCOME] );
time(&time_now);
srand( (int)(time_now));

domenu( OPNPAPER );

autoin = 1;	/* enable auto in tray here to stop opening immediately*/
balfisc();	/* set up initial balance sheet */
multi();
doclose();		/*close down system*/
}
/***************************************************************************/


