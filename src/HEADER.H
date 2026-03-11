typedef	float	real;
typedef char	SBYTE;
#include	"ibm.h"
#include	"mega.h"
#include	<obdefs.h>
#include	<gemdefs.h>
#include	<osbind.h>
#include	<stdio.h>
#include	<math.h>
#include	"strings.h"
#define	uint	unsigned int
#define	uchar	unsigned char
#define	MIN(x,y)	(((x) < (y))?(x):(y))
#define	MAX(x,y)	(((x) > (y))?(x):(y))
#define	ABS(x)		(((x) < 0)?(-1 * (x)):(x))
#define STATEFOR(x)	for(x = 0 ; x < 16 ; ++x)
#define	TXTPNT(a,b)	((TEDINFO*)(a[b].ob_spec))->te_ptext
#define	NUMTEXT(a,b,c)	formnum(TXTPNT((a),(b)),(c))
#define TOPLOT(x)	( (((x)%64)/16)*4 + ((x)%4) )
#define	STATE(x)	( ((x)/64)*4 + ((x)%16)/4 )
#define	SLIDER(tree,sld,box,l,h,val)	tree[sld].ob_x = (real)(\
tree[box].ob_width - 2*gl_wchar) * (real)((val) - (l))/((h)-(l))
#define	OBS(tree,obj)	tree[obj].ob_spec
#define	REFPLOT(x)	(((x)%4)*4 + ((x)/4)*64)
#define		BUTFUNC(x)	x(wh,mx,my,wind)\
int	wh,mx,my;\
WINDOWS	*wind;
#define		DRAWFUNC(f)	f(wh,mode,x,y,w,h,wind)\
int	wh,mode;\
int	x,y,w,h;\
WINDOWS	*wind;

/****************************************************************************/
/*		Selective compilation flags*/
#define	TRFLAG		01	/*pipe,rail transport*/
#define	PIPEFLAG	01	/*building pipelines*/
#define	GRILLFLAG	0	/*grilling*/
#define	CONCOFLAG	0
#define	PRFLAG		01	/*price menu*/
#define	CHFLAG		01	/*change output*/
#define	PROSFLAG	01	/*prospect*/
#define	INFLAG		01	/*in-tray*/
#define	AUFLAG		01	/*bidding for land*/
#define	NEWSFLAG	01	/*newspaper*/
#define	LOANFLAG	01
#define	WELLFLAG	01
#define	STOREFLAG	01
#define	HISTFLAG	01
#define	AIFLAG		01
/************************************************************************/
typedef struct {
	int count;
	int top;
	real data[10];
	char title[12];
	char info[60];
} CHARTS;
/****************************************************************************/
typedef	struct {
	int	c_x;
	int	c_y;
} COORD;
/****************************************************************************/
typedef struct	{
	SBYTE	owner;		/*-1~not owned. Company number*/
	SBYTE	type;		/* pointer to well type*/ 
	real	output;		/* user's desired output/quanta */
	uint	cost;		/* cost of well * $10000*/
	uint	val_land;	/* cost of land * $10000*/
	SBYTE	age;		/* age of well in PERIODS*/
	SBYTE	maxage;		/* max of above */
	real	actual;		/* actual output in preceeding quanta*/
	real	reserves;	/* total reserves in  field */
	SBYTE	flags[4];	/*various flags*/
				/*bit 0   - set means pros taking place*/
				/*bit 1,2 - type of prospecting taking place*/
				/*bit 3   - well being built*/
				/*bit 7   - land being sold*/
	
	int	prosres;	/*resves that prosping found -ve if not valid*/
	}	PLOT;
/*****************************************************************************/
typedef	struct
	{
	int	plot;
	int	dutch;		/* =1 if Dutch auction */
	real	reserve;
	real	offer[4];
	real	max;		/*limit of slider*/
	long	time;
	}	BIDTYPE;
/*****************************************************************************/
typedef struct
	{
	int	handle;
	int	animate;		/*TRUE if window to be animated*/
	int	(*redraw)();		/*index of routine which owns window*/
	int	(*arrowed)();
	int	(*hslid)();
	int	(*vslid)();
	int	(*butpress)();	/*button pressed in window*/
	int	*condred;	/*window redrawn if contents true*/
	int	gpp;		/*general purpose parameter (usually state)*/
	real	p1;		/*General purpose parameters*/
	real	p2;
	GRECT	*pos;		/*holds current width,height etc*/
	OBJECT	*tree;
	int	kind;
	} WINDOWS;
/****************************************************************************/
typedef	struct
	{
	real	cap_cost;	/*capital cost of well*/
	real	capacity;	/*max efficiency capacity*/
	real	lab_cost;	/*wage cost per barrel*/
	int	eta;		/*the reliability*/
	char	bull1[45];
	char	bull2[45];
	char	bull3[45];		/*pointers to text strings (max 44 chars)*/
	} WELLTYPE;
/*****************************************************************************/
typedef struct
	{
	char	date[20];
	real	rev;
	real	inv;
	real	acost;
	real	scost;
	real	dep;
	real	iearn;
	real	tax;
	}PLTYPE;
/**************************************************************************/
typedef	struct
	{
	char	date[20];
	real	cash;
	real	inv;
	real	land;
	real	equip;
	real	loans;
	real	reserves;
	}BALTYPE;
/*************************************************************************/
#define JOBS struct jobs
JOBS
	{
	int	company;	/*company responsible for job*/
	int	delay;		/*days to completion. Do job when null*/ 
	int	type;		/*type of job*/
	real	p1;		/*list of parameters (depends on type)*/
	real	p2;
	real	p3;
	real	p4;
	real	p5;
	JOBS	*next;		/*pointer to next in linked list*/
	};
/*****************************************************************************/
#define	MSG	struct msg
MSG
	{
	char	text0[51];	/*pointer to text string (50 chars plus null)*/
	char	text1[51];	/*pointer to text string*/
	char	text2[51];	/*pointer to text string*/
	char	date[20];
	MSG	*next;
	};
/*****************************************************************************/
#define WINDOW (NAME|MOVER|CLOSER|LFARROW|RTARROW)
#define BLOCK 1
#define	BOLD	0x01
#define	ITALIC	0x04
#define ULINE	0x08
#define PARA    128		/*formatting options for newspaper layout*/
#define TITLE   129
#define HDLINE  130
#define BAR     131
#define PIE     132
#define LINE    133
#define FLUSH   134
#define TWOCOL  135
#define ONECOL  136
#define NEWCOL  137
#define NEWPAGE 138
#define FINISH  139
#define	ALPHA   140
#define	BETA	141
#define	ENDSTR	142
#define	OUT2D	143
#define LINES 12		/*lines & columns on each paper page*/
#define COLUMNS 3
#define	BBOLD	144
#define	EBOLD	145
#define	BITALIC	146
#define	EITALIC	147
#define	BULINE	148
#define	EULINE	149

/****************************************/
#define	PIPEJOB		0
#define	WELLJOB		1
#define	STOREJOB	2
#define	PROSJOB		3
#define LOANJOB		4
#define	SELLJOB		5
#define	EXPOILJOB	6
#define	FOUNDOUT	7
/*********************************************/
#define	HISTPNTQNT	15	/*number of historic data points for quantised variables*/
#define	BARTOTON	(1.0/1000.0)
#define	TONTOBAR	1000.0
#define	SELLTIME	15L	/*#of frames that bidding is allowed for*/
#define	GRAPHINT	2	/*rescale factor for graphs*/
#define	EVT		200	/* time for events */
#define	MAXLOANTERM	20	/* max period for a loan */
#define	MINTMWELL	1	/* minimum time to build well */
#define	MAXTMWELL	10	/* maximum time to build well */
/*******************************************/
#define WI_KIND		(SIZER|MOVER|FULLER|CLOSER|NAME)
#define	MIN_WIDTH (15 * gl_wchar)
#define	MIN_HEIGHT (8 * gl_hchar)
#define	ARROWS	UPARROW|DNARROW|RTARROW|LFARROW
#define	TRUE	1
#define	FALSE	0
#define	HISTW	200
#define	HISTH	120
#define	DAILYW	230
#define	DAILYH	120
/******************************************/
extern	real	cashchange();
extern	double	exponent();
extern	real	demandset();
extern	real	pricewell();
extern	real	pricerail();
extern	real	findmin();
extern	real	maxallowed(),total_cost();
extern	real	marg_cost();
extern	real	demandgrad();
extern	real	pricepipe();
extern	real	pricestore();
extern	real	maxloan();
extern	real	maxprod();
extern	char	*strcat();

extern	int	gettime();
extern	int	mapdraw();

extern	real	ben_storeup();
extern	real	ben_margcost();
extern	real	ben_priceelas();

extern	real	al_minprice();
extern	real	al_mcost();
extern	real	al_costprod();
extern	real	al_avepri();
extern	real	al_transin();
extern	real	al_transout();
extern	real	al_estdem();
extern	real	al_trmcost();
extern	real	al_capacity();
extern	real	al_store_need();
extern	real	al_output();
extern	real	costvp();
/***********************************************/
#define	RDCASH		0		/* read cash in company*/
#define	REV		1
#define	ICOST		2
#define	SCOST		3
#define	ACOST		4
#define	IEARN		5
#define	TAX		6
#define	CAPITAL		7
#define	LOANS		8
#define	WRITEOFF	9
#define	ACPAY		10
#define	LAND		11
/***************************************************************************/
#define	WHITE	0
#define	BLACK	1
#define	RED	2
#define	GREEN	3
/***************************************************************************/
#define	OR	0
#define	MT	1
#define	MN	2
#define	WI	3
#define	NY	4
#define	CA	5
#define	NV	6
#define	CO	7
#define	WY	8
#define	KA	9
#define	IL	10
#define	OH	11
#define	TX	12
#define	AL	13
#define	NC	14
#define	FL	15
/*****************************************************************************/
#define	PCDOR		 0
#define	PCDMT		 1
#define	PCDMN		 2
#define	PCDWI		 3
#define	PCDNY		 4
#define	PCDCA		 5
#define	PCDNV		 6
#define	PCDCO		 7
#define	PCDWY		 8
#define	PCDKA		 9
#define	PCDIL 		10
#define	PCDOH 		11
#define	PCDTX 		12
#define	PCDAL 		13
#define	PCDNC 		14
#define	PCDFL 		15
#define	BCDOR 		16
#define	BCDMT 		17
#define	BCDMN 		18
#define	BCDWI 		19
#define	BCDNY 		20
#define	BCDCA 		21
#define	BCDNV 		22
#define	BCDCO 		23
#define	BCDWY 		24
#define	BCDKA 		25
#define	BCDIL 		26
#define	BCDOH 		27
#define	BCDTX 		28
#define	BCDAL 		29
#define	BCDNC 		30
#define	BCDFL		31
#define	LABFACT		32
#define	BLDFACT 	33
#define	TRANFACT 	34
#define	NEWWELL		35
#define	NEWRAIL		36
#define	NEWINT 		37
#define TAXRATE 	38
#define	PROPLOAN 	39
#define	DEMFACT 	40
#define	PRICEIMP	41
#define	KILLWELL	42
#define	PIPEGW		43
#define	PIPERO		44
#define	PIPEBU		45
#define	EXTRA		46
#define	AIPROFPB	47
#define	AIGROWTH	48
#define	AIPROPBUILDS	49
#define	AIPROFIT	50
#define	AIDEMMARG	51
#define	AIPROPBORROW	52
#define	AIMCSTPC	53
#define	AIPIPEPROD	54
#define	AIRAILPROD	55
#define	AIPROPPROS	56
#define	AISTORERATIO	57
#define	NEWPLOT		58
#define	SPHEREGW	59
#define	SPHERERO	60
#define	SPHEREBU	61
#define	DEMALL		62
/*****************************************************************************/
#define	VPPROD		0
#define	VPPLAN		1
#define	VPMARK		2
#define	VPSALE		3
#define	VPSALES		4
#define	VPTRAN		5
#define	VPFIN		6
#define	VPDEV		7
#define	VPOP		8
#define	VPDIRTY		9

