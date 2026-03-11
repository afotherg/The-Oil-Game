/*************************************************************************/
/*        This file converts text file into .dat file for mega game      */
/*                Started   6 December 1986				 */
/*           This version  18 February 1987				 */
/*************************************************************************/
/* The format of a node is : node-size is 112 bytes			 */
/*	alpha index (1 byte),beta index (1 byte),string index (2 bytes)	 */
/*	alpha index (1 byte),beta index (1 byte),string index (2 bytes)	 */
/*	alpha index (1 byte),beta index (1 byte),string index (2 bytes)	 */
/*	alpha index (1 byte),beta index (1 byte),string index (2 bytes)	 */
/*	alpha index (1 byte),beta index (1 byte),string index (2 bytes)	 */
/*	16 byte sized commands followed by 16 byte paramaters		 */
/*	Pointer to next node (2 bytes)					 */
/*	SEE FILE NEWS.C TO SEE HOW ALL COMMANDS WORK			 */
/*************************************************************************/
#include	<stdio.h>
#include	<string.h>

FILE	*from;
FILE	*to;
FILE	*strings;			/*where strings are stored*/

char	*commands[256]={
			"PCDOR",	/* 0 */
			"PCDMT",	/* 1 */
			"PCDMN",	/* 2 */
			"PCDWI",	/* 3 */
			"PCDNY",	/* 4 */
			"PCDCA",	/* 5 */
			"PCDNV",	/* 6 */
			"PCDCO",	/* 7 */
			"PCDWY",	/* 8 */
			"PCDKA",	/* 9 */
			"PCDIL",	/*10 */
			"PCDOH",	/*11 */
			"PCDTX",	/*12 */
			"PCDAL",	/*13 */
			"PCDNC",	/*14 */
			"PCDFL",	/*15 */
			"BCDOR",	/*16 */
			"BCDMT",	/*17 */
			"BCDMN",	/*18 */
			"BCDWI",	/*19 */
			"BCDNY",	/*20 */
			"BCDCA",	/*21 */
			"BCDNV",	/*22 */
			"BCDCO",	/*23 */
			"BCDWY",	/*24 */
			"BCDKA",	/*25 */
			"BCDIL",	/*26 */
			"BCDOH",	/*27 */
			"BCDTX",	/*28 */
			"BCDAL",	/*29 */
			"BCDNC",	/*30 */
			"BCDFL",	/*31 */
			"LABFACT",	/*32 */
			"BLDFACT",	/*33 */
			"TRANFACT",	/*34 */
			"NEWWELL",	/*35 */
			"NEWRAIL",	/*36 */
			"NEWINT",	/*37 new interest rate*/
			"TAXRATE",	/*38*/
			"PROPLOAN",	/*39 propensity for bank to loan*/
			"DEMFACT",	/*40 demand factor, used in econ */
			"PRICEIMP",	/*41 price of imported oil=PRICEIMP*$.5*/
			"KILLWELL",	/*42 Government(etc) inspired kill off of certain well type */
			"PIPEGW",	/*43 build pipe for G & W  Oil Corp */
			"PIPERO",	/*44 build pipe for Roco Oil Corp */
			"PIPEBU",	/*45 build pipe for Burns Oil Corp */
			"EXTRA",	/*46 extra parameter for other things*/
			"AIPROFPB",	/*47*/
			"AIGROWTH",	/*48*/
			"AIPROPBUILDS",	/*49*/
			"AIPROFIT",	/*50*/
			"AIDEMMARG",	/*51*/
			"AIPROPBORROW",	/*52*/
			"AIMCSTPC",	/*53*/
			"AIPIPEPROD",	/*54*/
			"AIRAILPROD",	/*55*/
			"AIPROPPROS",	/*56*/
			"AISTORERATIO",	/*57*/
			"NEWPLOT",	/*58*/
			"SPHEREGW",	/*59*/
			"SPHERERO",	/*60*/
			"SPHEREBU",	/*61*/
			"DEMALL",	/*62*/
			};

char	nmnode[500][16];		/*names of nodes*/
char	nmstring[500][16];		/*names of strings*/
int	used_string[500];
char	instruction[50];
char	text[5000];
int	parameter;

int	nextname;			/*next position for name*/
int	nextnode;
int	nextseek;
int	nodelen;
long	currseek;
long	seekval[500];
/*****************************************************************************/
main(argc, argv)
int	argc;
char	*argv[];
{
char	filename[50], inputs[50];
int	i;
FILE	*header;

if(argc != 3)
	{
	printf("Usage: [d:][path]convert filename startdate\n");
	exit(1);
	}
sprintf(inputs, "%s.TXT", argv[1]);
from	= fopen(inputs, "r");
to	= fopen("story.dat", "wb");
strings	= fopen("story.str", "wb");
header  = fopen("nodelen.h", "w");

if(!from || !to || !strings )
	{
	printf("Error in opening application files\n");
	exit(1);
	}
pass_1(argv);
rewind( from );
pass_2(argv);
write_header();

fflush( strings );
printf("\n\nThe input file has been successfully converted.\n");
printf("Number of nodes     :  %d\n", nextnode);
printf("Number of stories   :  %d\n", nextname);
for(i = 0 ; i < nextname ; ++i)
	if(!used_string[i])
		printf("Story defined but not used:  %s\n",nmstring[i]);
fprintf(header, "#define NODELEN %d\n", nodelen);
fclose(strings);
fclose(header);
fclose(from);
fclose(to);
exit(0);
}
/*****************************************************************************/
pass_1(argv)
char	*argv[];
{
char	dummy[16], name[16];

do	{
	fgets(text, 5000, from);
	if( is_defnode(text) )
		{
		sscanf(text, "%s %s", dummy, name);
		if(strncmp(name, argv[2], 6) >= 0)
			{
			add_node(name);
			add_replacement();
			}
		}
	if( is_defstr(text) )
		{
		sscanf(text, "%s %s", dummy, name);
		add_string(name);
		output_text ( text );
		}
	}while( ! is_end(text) );

}
/*****************************************************************************/
pass_2(argv)
char	*argv[];
{
char	dummy[16], name[16];

do	{
	fgets(text, 5000, from);
	if( is_defnode(text) )
		{
		sscanf(text, "%s %s", dummy, name);
		if(strncmp(name, argv[2], 6) >= 0)
			output_node( text );
		}
	}while( ! is_end(text) );

}
/*****************************************************************************/
is_defnode(string)
char	*string;
{
return( ! strncmp("DEFNODE", string, 7) );
}
/*****************************************************************************/
is_defstr(string)
char	*string;
{
return( ! strncmp("DEFSTR", string, 6) );
}
/*****************************************************************************/
is_end(string)
char	*string;
{
return( ! strncmp("END", string, 3) );
}
/*****************************************************************************/
add_string(name)
char	*name;
{
int	i;

for(i = 0 ; i < nextname ; ++i)
	{
	if (!strcmp(name, &nmstring[i][0]))
		{
		printf("Duplicate story name: %s\n", name);
		exit(1);
		}
	}
strcpy(&nmstring[nextname][0], name);
++nextname;
if(nextname == 500)
	{
	printf("Out of storage space for strings!\n");
	exit(1);
	}
}
/*****************************************************************************/
add_node(name)
char	*name;
{
int	i;

for(i = 0 ; i < nextnode ; ++i)
	{
	if(!strcmp(&nmnode[i][0], name))
		{
		printf("Duplicate node name: %s\n", name);
		exit(1);
		}
	}	
strcpy(&nmnode[nextnode][0], name);
++nextnode;
}
/*****************************************************************************/
add_replacement()
{
char	dummy[50], alpha[50], beta[50];
int	i;


for(i = 0 ; i < 5 ; ++i)
	{
	fgets(text, 500, from);
	sscanf(text, "%s %s %s", alpha, beta, dummy);
	}
}
/*****************************************************************************/
output_text( text )
char	*text;
{
char	dummy[50], name[50], story[50];
int	i, j, k, next, read;
static	char	*commands[] = {
	"para","title","hdline","bar","pie","line","flush","twocol",
	"onecol","newcol","newpage","finish","alpha","beta","endstr",
	"out2d", "bbold", "ebold", "bitalic", "eitalic", "buline", "euline"};


seekval[nextseek] = currseek;
++nextseek;
sscanf(text, "%s %s", dummy, story);
for(i = 0 ; text[i] != '{' ; ++i)
	;

while ( (next = text[++i]) != '}' )
	{
	switch(next)
		{
		case	'\n':
			break;

		case '[':			/*control string*/
		j = 0;
		while( (read = text[++i]) != ']' && (j < 20) )
			name[j++] = read;
		if(j == 20)
			{
			printf("No closing brace ']' in story: %s\n", story);
			exit(1);
			}
		name[j] = 0;
		for(k = 0 ; k < 22 && strcmp(commands[k],name); ++k)
			;
		if(k == 22)
			{
			printf("Unknown control string: %s\n",name);
			exit(1);
			}
		read = k + 128;		/*correct command code*/
		fputc(read, strings);
		currseek += 1;
		break;

		case	'{':
		case	']':
			printf("No matching brace in story: %s\n", story);
			exit( 1 );

		default:
		fputc(next, strings);
		++currseek;
		break;
		}
	}
}
/*****************************************************************************/
write_header()			/*write out header in desired format*/
{
FILE	*init;
int	i;

init = fopen("init.meg","w");

fprintf(init,"%10d\n",nextseek);		/*number of seek values*/
for(i = 0 ; i < nextseek ; ++i)
	fprintf(init,"%10ld\n",seekval[i]);
fclose(init);
}
/*****************************************************************************/
output_node( text )			/*def node required*/
char	*text;
{
char	name[50], alpha[50], beta[50], story[50];
int	i, j, val, g;

for(i = 0 ; i < 5 ; ++i)
	{
	fscanf(from, "%s %s %s", alpha, beta, story);
	for(j = 0; j < nextname ; ++j)
		if(!strcmp(&nmstring[j][0], story))
			break;			/*OK*/
	used_string[j] = -1;
	if(j == nextname )
		{
		printf("Cannot find story reference: %s\n", story);
		exit( 1 );
		}
	else
		{
		putw( j, to );
		nodelen += 2;
		}
	}

for( i = 0 ; i < 64 ; ++i)
	{
	fscanf( from, "%s %d", instruction, &parameter);
	for( j = 0 ; j < 256 ; ++j)
		{
		if( !strcmp( commands[j], instruction) )
			break;
		}
	if( j == 256 )
		{
		printf("Unknown instruction on node: %s\n", instruction);
		exit( 1 );
		}
	fputc( j, to );			/* write out command */
	fputc( parameter, to);		/* write out parameter */
	nodelen += 2;
	}

for(g = 0 ; g < 2 ; ++g)
	{
	fscanf(from, "%d", &parameter);	/* graph data */
	putw(parameter, to);
	nodelen += 2;
	for(i = 0 ; i < 10 ; ++i)
		{
		fscanf(from, "%d", &parameter);	/* graph data */
		putw( parameter, to );
		nodelen += 2;
		}

	fscanf(from, "%s", name);
	for(i = 0 ; i < 10 ; ++i)
		if(name[i] == '*')
			fputc( 0, to);
		else
			if(name[i] == '_')
				fputc( ' ', to);
			else
				fputc(name[i], to);
	nodelen += 10;
	fscanf(from, "%s", name);
	for(i = 0 ; i < 40 ; ++i)
		if(name[i] == '*')
			fputc( 0, to);
		else
			if(name[i] == '_')
				fputc( ' ', to);
			else
				fputc(name[i], to);
	nodelen += 40;
	}

fscanf(from,"%s",name);		/*to nodes*/
for(j = 0 ; j < nextnode ; ++j)
	if( !(strcmp(nmnode[j], name)) )
		break;
if(j == nextnode)
	{
	printf("Unknown story pointer: %s\n", name);
	exit( 1 );
	}
putw( j, to );
nodelen += 2;
}
/*****************************************************************************/
