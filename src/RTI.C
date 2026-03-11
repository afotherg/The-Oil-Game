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
#define		ALPHABETA	0
extern	long	filelength();

FILE	*from;
FILE	*strings;			/*where strings are stored*/
FILE	*init;

char	*types[256]={
			"isloss",
			"highdebt",
			"random",
			"nomoney",
			"heavypipe",
			"heavywell",
			"heavystore",
			"heavysell",
			"valuable",
			"maxdemand",
			"maxcash",
			"maxprofit",
			"wellnooil",
			"bldhigh",
			"labhigh",
			"railhigh",
			"bldlow",
			"lablow",
			"raillow",
			"stnoprod",
			"bigdebt",
			"notvaluable",
			"highdemand",
			"lowdemand",
			"highprice",
			"highimp",
			"lowimp",
			"landup"
			};
int	story_type[256];
char	instruction[50];
char	text[5000];
int	parameter;

int	nextname;			/*next position for name*/
int	nextseek;
int	nodelen;
long	currseek;
long	seekval[500];
int	seektype[500];
/*****************************************************************************/
main(argc, argv)
int	argc;
char	*argv[];
{
char	filename[50], inputs[50];
int	i;
FILE	*header;

if(argc != 2)
	{
	printf("Usage: [d:][path]convert filename\n");
	exit(1);
	}
sprintf(inputs, "%s.TXT", argv[1]);
from	= fopen(inputs, "r");
strings	= fopen("rti.str", "wb");
init    = fopen("rti.meg","w");

if(!from || !strings )
	{
	printf("Error in opening application files\n");
	exit(1);
	}
pass_1();
rewind( from );

fflush( strings );
printf("\n\nThe input file has been successfully converted.\n");
printf("Number of stories   :  %d\n", nextname);
printf("Number of characters:  %ld\n", filelength( fileno(strings)) );
output();
printf("The story composition is:\n");
for(i = 0 ; types[i] ; ++i)
	printf("Type: %-15s     Number: %d      Index: %d\n", types[i], story_type[i], i);

fclose(strings);
fclose(from);
fclose(init);
exit(0);
}
/*****************************************************************************/
pass_1()
{
char	dummy[16], name[16], type[16];

do	{
	fgets(text, 5000, from);
	if( is_defstr(text) )
		{
		sscanf(text, "%s %s", dummy, type);
		output_text ( text );
		write_header(type);
		++nextname;
		}
	}while( ! is_end(text) );

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
printf("Outputting data from: %s\n", story);
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
write_header(type)			/*write out header in desired format*/
char	*type;
{
int	i;

for(i = 0 ; i < 256 ; ++i)
	{
	if( !strcmp(types[i], type) )
		break;
	}
if(i == 256)
	{
	printf("Unknown predicate: %s\n", type);
	exit(1);
	}
seektype[nextseek - 1] = i;
story_type[i]++;
}
/*****************************************************************************/
output()
{
int	i;

fprintf(init, "%15d\n", nextseek);
for(i = 0 ; i < nextseek ; ++i)
	fprintf(init, "%6d%9ld\n", seektype[i], seekval[i]);
}
/*****************************************************************************/
