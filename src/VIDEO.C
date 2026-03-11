#include	"graph.h"
#include	"ctype.h"
#include	"stdio.h"


char	*driver_used[]	= {"IBMCHMP6", "IBMEHMP6", "IBMEH8P6", "HERMONP6", "AMSTRAD"};
char	*font_used[]		= {"L", "H"};

main(argc, argv)
int	argc;
char	*argv[];
{
int	i, mem;
FILE	*out;
struct	videoconfig ben;
char	in[200];
char	outdrive = -1, font;

out = fopen("assign.sys", "w");
_getvideoconfig( (struct videoconfig far *)&ben);
mem = _bios_memsize();

if(mem < 512)
	{
	printf("This program requires a minimum of 512k of usable memory\n");
	printf("In order to run. Sorry.\n");
	exit(1);
	}
if(argc == 1)
	{
	if(ben.monitor & _MDPA)
		printf("Unfortunately you must have a graphics monitor to run this program.");

	if(ben.monitor & _CGA)
		outdrive = 0, font = 0;

	if( (ben.monitor & _EGA) || (ben.monitor & _VGA) )
		{
		if(mem < 640)
			outdrive = 2, font = 1;
		else
			outdrive = 1, font = 1;
		}
	}

if(argc == 2)
	{
	for( i = 0 ; (argv[1])[i] ; ++i)
		in[i] = tolower( (argv[1])[i] );
	in[i] = 0;
	if( !strcmp(in, "ega") )
		outdrive = 2, font = 1;

	if( !strcmp(in, "egamono") )
		outdrive = 1, font = 1;

	if( !strcmp(in, "cga") )
		outdrive = 0, font = 0;

	if( !strcmp(in, "mono") )
		outdrive = 3, font = 1;

	if( !strcmp(in, "amstrad") )
		outdrive = 4, font = 0;
	}
if( outdrive != -1)
	{
	fprintf(out, "01  %s\n", driver_used[outdrive]);
	fprintf(out, "  IBM%sSS14.FNT\n", font_used[font]);
	fprintf(out, "  IBM%sSS18.FNT\n", font_used[font]);
	}
if(outdrive == -1)
	{
	printf("ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป\n");
	printf("บ  Your display screen has not been recognized.              บ\n");
	printf("บ  If you think you have an IBM-compatible card, you can try บ\n");
	printf("บ  one of the following:                                     บ\n");
	printf("บ                                                            บ\n");
	printf("บ  mega ega               (EGA compatible)                   บ\n");
	printf("บ  mega egamono           (EGA compatible, short of memory)  บ\n");
	printf("บ  mega cga               (CGA compatible)                   บ\n");
	printf("บ  mega mono              (Hercules compatible)              บ\n");
	printf("บ  mega amstrad           (Amstrad 1512)                     บ\n");
	printf("ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ\n");
	exit(1);
	}
return(0);
}
