/************************************************************************/
/*		General initialisation code				*/
/*		Started	19 February 1987.				*/
/************************************************************************/
#include	"header.h"
#include	"variab.h"
/**************************************************************************/
char	*iconpos;
char	*map_image[6];

int	lines_Or[] =	{			/* 12 lines */
			111, 129,
			45, 112,
			43, 98,
			62, 68,
			65, 29,
			82, 40,
			86, 26,
			131, 38,
			124, 71,
			126, 82,
			119, 96,
			112, 129
			};

int	lines_Mt[] =	{			/* 10 lines */
			131, 38,
			242, 56,
			236, 107,
			173, 100,
			167, 139,
			111, 128,
			116, 95,
			127, 82,
			124, 67,
			131, 38
			};

int	lines_Mn[] =	{			/* 19 lines */
			241, 57,
			321, 59,
			324, 65,
			333, 63,
			349, 71,
			354, 68,
			365, 68,
			344, 88,
			339, 115,
			356, 124,
			355, 130,
			309, 131,
			304, 131,
			305, 142,
			291, 142,
			291, 142,
			285, 137,	
			234, 133,
			241, 57
			};

int	lines_Wi1[]=	{			/* 26 lines */
			345, 88,
			356, 86,
			359, 90,
			379, 77,
			377, 87,
			385, 86,
			388, 89,
			394, 89,
			402, 83,
			412, 85,
			416, 91,
			390, 99,
			386, 109,
			390, 140,
			364, 143,
			368, 148,
			357, 159,
			359, 166,
			312, 170,
			303, 142,
			304, 132,
			354, 130,
			354, 124,
			339, 113,
			339, 101,
			343, 87
			};

int	lines_Wi2[] =	{			/* 13 lines */
			409, 98,
			408, 103,
			401, 108,
			398, 124,
			403, 135,
			399, 149,
			431, 147,
			437, 126,
			431, 116,
			423, 120,
			427, 111,
			424, 102,
			410, 97
			};

int	lines_NY[] =	{			/* 36 lines */
			457, 139,
			470, 128,
			467, 120,
			477, 117,
			486, 117,
			493, 112,
			492, 101,
			487, 100,
			487, 95,
			530, 81,
			533, 77,
			537, 76,
			540, 66,
			538, 64,
			542, 45,
			555, 47,
			571, 72,
			561, 85,
			556, 83,
			556, 86,
			548, 95,
			546, 115,
			554, 119,
			547, 125,
			536, 139,
			522, 144,
			521, 165,
			513, 164,
			520, 174,
			512, 193,
			507, 183,
			499, 179,
			497, 173,
			484, 164,
			464, 168,
			457, 138
			};
int	lines_Ca[] =	{		/* 20 points */
			83, 121,
			45, 108,
			42, 124,
			37, 130,
			40, 140,
			37, 148,
			44, 166,
			48, 179,
			46, 190,
			52, 207,
			53, 214,
			73, 226,
			73, 231,
			81, 238,
			82, 251,
			110, 252,
			113, 238,
			119, 233,
			75, 159,
			84, 121
			};

int	lines_Nv[] =	{	/* 11 lines */
			166, 140,
			82, 121,
			73, 159,
			118, 233,
			112, 237,
			105, 253,
			146, 279,
			166, 281,
			184, 155,
			164, 152,
			166, 139
			};
int	lines_Co[] =	{		/* 9 lines */
			183, 155,
			165, 282,
			176, 282,
			176, 277,
			232, 280,
			239, 213,
			249, 215,
			252, 163,
			183, 156
			};


int	lines_Wy[] =	{		/* 10 lines */
			173, 100,
			164, 152,
			252, 163,
			252, 176,
			316, 177,
			307, 143,
			285, 136,
			235, 134,
			237, 107,
			174, 99
			};

int	lines_Ka[] =	{		/* 11 lines */
			250, 175,
			247, 215,
			238, 214,
			238, 222,
			270, 224,
			267, 249,
			305, 260,
			325, 259,
			324, 188,
			316, 176,
			249, 174
			};

int	lines_Il[] =	{		/* 24 lines */
			387, 141,
			360, 144,
			365, 150,
			364, 157,
			356, 159,
			355, 168,
			313, 169,
			315, 176,
			322, 181,
			320, 185,
			323, 189,
			324, 223,
			371, 222,
			371, 228,
			375, 229,
			374, 221,
			382, 214,
			392, 205,
			409, 199,
			421, 183,
			418, 148,
			401, 148,
			394, 151,
			385, 142
			};

int	lines_Oh[] =	{		/* 18 lines */
			443, 215,
			514, 200,
			513, 196,
			509, 192,
			508, 182,
			499, 180,
			498, 173,
			485, 164,
			463, 168,
			457, 139,
			442, 147,
			417, 148,
			421, 185,
			433, 188,
			442, 186,
			451, 199,
			441, 212,
			443, 216
			};

int	lines_Tx[] =	{		/* 22 lines */
			231, 278,
			194, 275,
			211, 294,
			214, 310,
			230, 318,
			237, 308,
			248, 310,
			275, 356,
			296, 361,
			296, 352,
			295, 339,
			310, 325,
			317, 326,
			333, 312,
			337, 296,
			332, 262,
			322, 258,
			305, 259,
			269, 250,
			269, 223,
			237, 220,
			231, 279
			};

int	lines_Al[] =	{		/* 20 lines */
			332, 312,
			331, 263,
			326, 260,
			324, 223,
			368, 221,
			370, 228,
			375, 229,
			369, 240,
			419, 238,
			431, 273,
			431, 290,
			403, 293,
			405, 301,
			386, 299,
			383, 305,
			387, 305,
			381, 318,
			367, 319,
			359, 312,
			333, 312
			};

int	lines_NC[] =	{		/* 20 lines */
			371, 240,
			376, 219,
			386, 213,
			392, 204,
			411, 198,
			421, 184,
			442, 186,
			451, 200,
			440, 213,
			442, 215,
			516, 200,
			521, 213,
			511, 223,
			503, 238,
			493, 243,
			489, 252,
			473, 269,
			465, 254,
			443, 235,
			371, 241
			};

int	lines_Fl[] =	{		/* 22 lines */
			419, 236,
			431, 272,
			432, 288,
			402, 292,
			406, 300,
			419, 298,
			432, 305,
			438, 305,
			445, 299,
			461, 311,
			467, 335,
			478, 350,
			487, 354,
			488, 358,
			495, 354,
			496, 335,
			470, 284,
			474, 269,
			466, 254,
			445, 239,
			443, 235,
			418, 237
			};

struct	{
		int	color;
		int	count;
		int	*array;
		}	block[] =
		{
		0, 12, lines_Or,
		1, 10, lines_Mt,
		2, 19, lines_Mn,
		1, 26, lines_Wi1,
		1, 13, lines_Wi2,
		1, 36, lines_NY,
		1, 20, lines_Ca,
		3, 11, lines_Nv,
		2,  9, lines_Co,
		0, 10, lines_Wy,
		3, 11, lines_Ka,
		0, 24, lines_Il,
		2, 18, lines_Oh,
		1, 22, lines_Tx,
		2, 20, lines_Al,
		3, 20, lines_NC,
		0, 22, lines_Fl
		};

load_game( name )		/* loads game in binary format */
char	*name;
{
JOBS	job;
extern	int	fread();
int	numjobs, i;
FILE	*fp;

fp	= fopen( name, "rb" );
if( fp == 0 )
	return;
handle_file( fp, fread );
fread( &numjobs, sizeof( int ), 1, fp );
delete_jobs();
for( i = 0 ; i < numjobs ; ++i )
	{
	fread( &job, sizeof( JOBS ), 1, fp );
	addjob( &job );
	}
fclose( fp );
}


/*
**	saves globals
*/
save_game( name )
char	*name;
{
int	numjobs;
FILE	*fp;
JOBS	*runjob;
extern	int	fwrite();

fp = fopen(name,"wb");
if( fp == 0 )
	return;
handle_file( fp, fwrite);

numjobs	= totjobs();
fwrite( &numjobs, sizeof( int ), 1, fp );
runjob	= &firstjob;
while( runjob	= runjob->next )
	fwrite( runjob, sizeof( JOBS ), 1, fp );
fclose( fp );
}

/*
**	General purpose routine to write or read all the vital data
**
*/
handle_file( fp, func )
FILE	*fp;
int	(*func)();
{
(*func)( &revfisc[0], sizeof(real), 4, fp );
(*func)( &expfisc[0], sizeof(real), 4, fp );
(*func)( &intpdfisc[0], sizeof(real), 4, fp );
(*func)( &intearnfisc[0], sizeof(real), 4, fp );
(*func)( &adminfisc[0], sizeof(real), 4, fp );
(*func)( &accumdep[0], sizeof( real), 4, fp );
(*func)( &debentures[0], sizeof( real), 4, fp );
(*func)( &profit[0], sizeof( real), 4, fp );
(*func)( &land[0], sizeof( real), 4, fp );
(*func)( &import[0], sizeof( real), 4, fp );

(*func)( &inventory[0][0], sizeof( real), 16, fp);

(*func)( &pipe[0][0], sizeof( unsigned int), 64, fp);
(*func)( &railmap[0][0], sizeof( SBYTE ), 256, fp );
(*func)( &sphere[0][0], sizeof( SBYTE ), 64, fp );
(*func)( &vp_cont[0], sizeof( SBYTE ), 16, fp );

(*func)( &capval[0], sizeof( real ), 4, fp );
(*func)( &sellmin[0][0], sizeof( real ), 64, fp );
(*func)( &storage[0][0], sizeof( real ), 64, fp );
(*func)( &store_beingbuilt[0][0], sizeof( real ), 64, fp );

(*func)( &demand[0], sizeof( real ), 16, fp );
(*func)( &cash[0], sizeof( real ), 4, fp );

(*func)( &bldfact, sizeof( real ), 1, fp );
(*func)( &railfact, sizeof( real ), 1, fp );
(*func)( &labfact, sizeof( real ), 1, fp );

(*func)( &tranoil[0][0][0], sizeof( int ) , 4*16*16, fp );

(*func)( &changedem[0], sizeof( SBYTE), 16, fp );
(*func)( &chbldfact, sizeof( SBYTE ) ,1, fp );
(*func)( &chrailfact, sizeof( SBYTE ) ,1, fp );
(*func)( &chlabfact, sizeof( SBYTE ) ,1, fp );
(*func)( &taxreg, sizeof( SBYTE ) ,1, fp );
(*func)( &pipeindex, sizeof( SBYTE ) ,1, fp );

(*func)( &interest, sizeof( real ), 1, fp );

(*func)( &whichwell[0], sizeof( int ), 4, fp );

(*func)( &taxrate, sizeof( real ), 1, fp );
(*func)( &demandfact, sizeof( real ), 1, fp );
(*func)( &price_imp, sizeof( real ), 1, fp );
(*func)( &tot_land, sizeof( real ), 1, fp );
(*func)( &numauc, sizeof( int ), 1, fp );

(*func)( &node, sizeof( int ), 1, fp );
(*func)( &day, sizeof( int ) ,1, fp );
(*func)( &yy, sizeof( int ), 1, fp );
(*func)( &mm, sizeof( int ), 1, fp );
(*func)( &dd, sizeof( int ), 1, fp );
(*func)( &numperiods, sizeof( int ), 1, fp );

(*func)( &plot[0], sizeof( PLOT ), 256, fp );

(*func)( &placcount[0][0], sizeof( PLTYPE ), 16, fp);
(*func)( &balsheet[0][0], sizeof( BALTYPE ), 16, fp);

(*func)( &proploan, sizeof( real ), 1, fp );
}
/****************************************************************************/
chngimg( obj, num )	/* change objs for different displays */
OBJECT	*obj;		/* RBUTTON means RED */
int	num;		/* TOUCHEXIT means GREEN */
{
int	type,flags;

type = obj[num].ob_type;
flags = obj[num].ob_flags;

if( colour )
	{
	if( type == G_BOX && (flags & RBUTTON))
		obj[num].ob_spec = (char*)( ((long)obj[num].ob_spec&0xffffff80L)|(0x70|RED));

	if( type == G_BOX && (flags & TOUCHEXIT))
		obj[num].ob_spec = (char*)( ((long)obj[num].ob_spec&0xffffff80L)|(0x70|GREEN));

	if( type == G_BOX && (flags & EDITABLE) )
		obj[num].ob_spec = (char*)( ((long)obj[num].ob_spec&0xffffff87L)|(4<<4));
	
	if( type == G_STRING)
		obj[num].ob_state &= (0xffff^DISABLED);

	if( type == G_TEXT && (flags & RBUTTON))
		((TEDINFO*)obj[num].ob_spec)->te_color = ( ( ((TEDINFO*)obj[num].ob_spec)->te_color) & 0xf0ff) | (RED<<8);

	if( type == G_TEXT && (flags & TOUCHEXIT))
		((TEDINFO*)obj[num].ob_spec)->te_color = ( ( ((TEDINFO*)obj[num].ob_spec)->te_color) & 0xf0ff) | (GREEN<<8);
	}
if( obj[num].ob_next != -1 )
	if( obj[obj[num].ob_next].ob_tail != num )
		chngimg( obj, obj[num].ob_next );
if( obj[num].ob_head != -1)
	chngimg( obj, obj[num].ob_head );
}
/***************************************************************************/
audio()				/* audio feedback */
{
static	char	bell[]={			/* from MWC 1.0 p. 217 */
				
			0x00,0xf6,
			0x01,0x02,
			0x02,0xde,
			0x03,0x01,
			0x04,0x3f,
			0x05,0x01,
			0x06,0x00,

			0x07,0xf8,
			0x08,0x10,
			0x09,0x10,
			0x0a,0x10,
			0x0b,0x00,
			0x0c,0x30,
			0x0d,0x09,
			0xff,0x30,

			0xff,0x00
			};

Dosound( bell );
}
/*****************************************************************************/
init_strings()		/* initialise the st[] array of pointers to strings  */
{
FILE	*strings, *point;
int	size;				/* assume that < 32K */
int	i;
char	*put_them;
long	val;

strings = fopen( "strings",	"r" );
point	= fopen( "strings.pnt",	"r" );

fseek( strings, 0L, 2);			/* position pointer at end of file */
size	= ftell( strings );
fseek( strings, 0L, 0);			/* pointer back to beginning */

put_them	= malloc( size +10 );	/* for safety */

fread( put_them, size, 1, strings );	/* put them into buffer */

for( i = 0 ; ; ++i )
	{
	if( fscanf( point, "%ld", &val) == EOF )
		break;
	st[i] = put_them + val;
	}
fclose( strings );
fclose( point   );
}
/*****************************************************************************/
init_wells()		/* loads in well descriptions from file: WELLS */
{
FILE	*fp;
int	i = 0;

if( !(fp = fopen("wells","r") ))
	{
	form_alert( 1, st[CANTOPNWELL] );
	exit( 1 );
	}

while( fscanf( fp, "%e%e%e", &param_wells[i].cap_cost,
		&param_wells[i].capacity, &param_wells[i].lab_cost) != EOF )
	{
	while( fgetc( fp ) != '{' )
		;
	getbull( fp, &param_wells[i].bull1[0] );
	getbull( fp, &param_wells[i].bull2[0] );
	getbull( fp, &param_wells[i].bull3[0] );
	++i;
	}
fclose( fp );
}
/*****************************************************************************/
getbull( fp, string)
FILE	*fp;
char	*string;
{
int	i = 0;
char	c;

while( (c = fgetc(fp)) != '\n' )
	string[i++] = c;
string[i] = '\0';
}
/*****************************************************************************/
totjobs()			/* counts number of jobs in job queue */
{
JOBS	*runjob;
int	i = 0;

runjob	= &firstjob;

while( runjob = runjob->next )
	++i;
return( i );
}


/*
**
**	This routine allocates space for the map buffer, and draws the
**	map into the buffer at the required resolution.
**
*/

readpic()
{
int	pxy[8];
MFDB	source, dest;

/* allocate space for screen size*/

map	= lmalloc( ((long)devwidth*(long)devheight*(long)nplanes/8L) );
if( !map )
	{
	desktop[0].ob_type = G_BOX;
	return;
	}
deskmap = 1;
desktop[0].ob_type = G_IBOX;
hidemouse();
set_clip( 0, 0, devwidth, devheight );
make_color( 4 );			/* sets background colour */
pxy[0]	= 0;
pxy[1]	= 0;
pxy[2]	= devwidth - 1;
pxy[3]	= devheight - 1;
vr_recfl( vdi_handle, pxy );

draw_states();

dest.fd_addr		= ( long )map;
dest.fd_w		= devwidth;
dest.fd_wdwidth		= devwidth / 16;
dest.fd_stand		= 0;
dest.fd_h		= devheight;
dest.fd_nplanes		= nplanes;

source.fd_addr		= 0L;	/* the screen! */

pxy[0]	= pxy[4]	= 0;
pxy[1]	= pxy[5]	= 0;
pxy[2]	= pxy[6]	= devwidth  -1;
pxy[3]	= pxy[7]	= devheight - 1;

vro_cpyfm( vdi_handle,S_ONLY,pxy, &source, &dest);
showmouse();
}

/*
**
**	This routine draws the states, after scaling them to fit the screen
**	used.
**
*/
draw_states()
{
int	i;

for( i = 0 ; i < 17 ; ++i )
	{
	make_color( block[i].color );
	adjust_coords( block[i].count, block[i].array );
	v_fillarea( vdi_handle, block[i].count, block[i].array );
	}
draw_snames();
}

/*
**
**	adjust the number of coords of the given array to scale them from
**	640*400 size to (devwidth, devheight size.
**
*/
adjust_coords( count, array )
int	count, *array;
{
int	i;

for( i = 0 ; i < 2*count ; i+=2 )
	{
	array[i]	= (((long)array[i])*devwidth)/640;
	array[i+1]	= (((long)array[i+1])*devheight)/400;
	}
}


/*
**
**	sets fill patterns, and/or colour according to the required colour
**	sent as a parameter
**
*/
make_color( col )
int	col;
{
static	int	cols[]	= { 2, 1, 5, 6, 4};
static	int	redandgreen[]=	{
0x5555,0xaaaa,0x5555,0xaaaa,0x5555,0xaaaa,0x5555,0xaaaa,
0x5555,0xaaaa,0x5555,0xaaaa,0x5555,0xaaaa,0x5555,0xaaaa,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff
};

static	int	redandblack[] = {
0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,
0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,
0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,
0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa
};

static	int	greenandblack[] = {
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
0x5555,0xaaaa,0x5555,0xaaaa,0x5555,0xaaaa,0x5555,0xaaaa,
0x5555,0xaaaa,0x5555,0xaaaa,0x5555,0xaaaa,0x5555,0xaaaa
};

if( !colour )
	{
	vsf_interior( vdi_handle, 2 );
	vsf_style( vdi_handle, cols[col] );
	}
else
	{
	switch( col )
		{
		case	0:
		vsf_color( vdi_handle, BLACK);
		vsf_interior( vdi_handle, 2 );
		vsf_style( vdi_handle, 4 );	/* should be grey */
		break;	
		case	1:
		my_udpat( redandgreen );
		vsf_interior( vdi_handle, 4);
		break;
		case	2:
		my_udpat( redandblack );
		vsf_interior( vdi_handle, 4);
		break;
		case	3:
		my_udpat( greenandblack );
		vsf_interior( vdi_handle, 4);
		break;
		case	4:			/* background */
		vsf_color( vdi_handle, GREEN );
		vsf_interior( vdi_handle, 1 );		/* solid */
		break;
		}
	}
}

/*
**
**	This routine handles the wrongly documented effects of vsf_udpat()
**	Expands the specified user defined pattern to fill number of planes
**	on device.
*/
my_udpat( pattern )
int	*pattern;
{
int	i;
int	my_pattern[16*10*sizeof(int)];		/* allow space for 10 planes*/

for( i = 0 ; i < 16*10*sizeof(int) ; ++i )
	my_pattern[i]	= 0;
for( i = 0 ; i < 32 ; ++i )
	my_pattern[i]	= pattern[i];
vsf_udpat( vdi_handle, my_pattern, nplanes );
}


/*
**	This routine draws the state names to the map.
**
*/
draw_snames()
{
int	i;

adjust_coords( 16, coords );		/* adjust coords to fit resolution */

for( i = 0 ; i < 16 ; ++i )
	{
	v_gtext( vdi_handle,	coords[i].c_x - gl_wchar/2,
				coords[i].c_y + gl_hchar/2, abbrev[i] );
	}
}


/*
**
**	This routine loads icon forms to the required areas
**
*/
FILE	*iconpnt;
int	icon_height_c;		/*
				** horrible global to save code size.
				** present icon height in chars
				*/

int	char_height;		/*
				** another global for char height specific
				** to icon sizes
				*/
load_icons()
{
FILE	*iconbin;
int	size, i;
long	index;

if( gl_hchar >= 16 ) {
	iconbin		= fopen( "icon16", "rb" );
	iconpnt		= fopen( "icon16.pnt", "r" );
	char_height	= 16;
	}
else if( gl_hchar >=14 ) {
	iconbin		= fopen( "icon14", "rb" );
	iconpnt		= fopen( "icon14.pnt", "rb" );
	char_height	= 14;
	}
else
	{
	iconbin		= fopen( "icon8", "rb" );
	iconpnt		= fopen("icon8.pnt", "r" );
	char_height	= 8;
	}

fseek( iconbin, 0L, 2);			/* position pointer at end of file */
size	= ftell( iconbin );
fseek( iconbin, 0L, 0);			/* pointer back to beginning */
iconpos	= malloc( size+10 );

if( !iconpos || !iconbin || !iconpnt )
	exit( 1 );
fread( iconpos, size, 1, iconbin );

if( (devwidth == 640) && (devheight == 350) && colour )
	for( i = 0 ; i < size+3 ; ++i )
		iconpos[i]	= ~iconpos[i];	/*
						** needed for silly IBM EGA
						*/
icon_height_c	=  3;
put_icon( welldial, WELL1);
put_icon( welldial, WELL2);
put_icon( welldial, WELL3);
put_icon( welldial, WELL4);

put_icon( plots, STCHP);
put_icon( plots, STTROIL);
put_icon( plots, STVPC );

put_icon( prosdial, PROS0);
put_icon( prosdial, PROS1);
put_icon( prosdial, PROS2);
put_icon( prosdial, PROS3);

put_icon( opnplot, PLOTBWL);
put_icon( opnplot, PLOTDOPR);
put_icon( opnplot, PLOTCH);

put_icon( vpchoose, VPCTP0 );
put_icon( vpchoose, VPCTP1 );
put_icon( vpchoose, VPCTP2 );
put_icon( vpchoose, VPCTP3 );
put_icon( vpchoose, VPCTP4 );
put_icon( vpchoose, VPCTPNO);

put_icon( plots, STBLDS );
put_icon( plots, STBLDP );

icon_height_c	= 2;
put_icon( icons, PLOTBLNK );
put_icon( icons, PLOTG );
put_icon( icons, PLOTW );
put_icon( icons, PLOTR );
put_icon( icons, PLOTB );
put_icon( icons, PLOTBLD  );
put_icon( icons, PLOTWELL );
put_icon( icons, PLOTPROS );
put_icon( icons, STAR     );
put_icon( icons, HAMMER );

for( i = 0 ; i < 6 ; ++i )
	{
	fscanf( iconpnt, "%ld", &index );
	map_image[i]	= iconpos + index;
	}
fclose( iconbin );
fclose( iconpnt );
}

/*
**	This routine is sent an object tree pointer, object number, and
**	icon pointer. It puts icon pointer into correct area
**	It reads the address of the icon as an index from iconpos in the
**	file pointed to by iconpnt.
**
**	makes the icon height icon_height_c * char_height
**
*/
put_icon( object, num )
OBJECT	*object;
int	num;
{
long	index;

fscanf( iconpnt, "%ld", &index);
((BITBLK*)(object[num].ob_spec))->bi_pdata	=  iconpos + index;
((BITBLK*)(object[num].ob_spec))->bi_hl		=  icon_height_c * char_height;
}


/*
**	This routine displays a file selector box, with the path *.GAM
**	to load and save positions in the game. Any path information will
**	be ignored.
*/
get_file_name( name )
char	*name;
{
char	path[50];
int	button;

strcpy( path, ".\\*.GAM");
if( !fsel_input( path, name, &button ) )
	{
	strcpy( name, "SAVED.GAM" );
	button	= 1;
	}
redraw_desktop();
return( button );
}

delete_jobs()
{
JOBS	*job, *temp;

job	= firstjob.next;

while( temp = job )			/* !!!!intentional assignment !!!*/
	{
	job	= job->next;
	free( temp );
	}
firstjob.next	= 0L;
}


