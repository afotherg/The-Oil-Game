#include	<stdio.h>

FILE	*in, *header, *point, *strings;

main()
{
int	i = 0, c;
char	name[20];

in	= fopen( "strings.txt",	"r" );
header	= fopen( "strings.h",	"w" );
point	= fopen( "strings.pnt",	"w" );
strings	= fopen( "strings",	"w" );

if( !( in && header && point && strings) )
	{
	printf( " Error opening files...\n" );
	exit( 1 );
	}

for( i = 0 ; ; ++i )
	{
	if( fscanf( in, "%s", name ) == EOF )
		{
		printf( "EOF encountered before END..\n");
		exit( 1 );
		}
	if( !strcmp(name, "END" ))
		break;
	fprintf( header, "#define\t%s %d\n", name, i );
	fprintf( point,  "%ld\n", ftell( strings ) );

	do	{
		c = fgetc( in );
		if( c == EOF )
			{
			printf("EOF encountered...\n" );
			exit( 1 );
			}
		}
		while( c != '{' );

	while( (c=fgetc( in )) != '}' )
		{
		if( c == EOF )
			{
			printf("EOF encountered\n");
			exit( 1 );
			}
		if( c == '\n' )
			continue;
		fputc( c, strings );
		}
	fputc('\0', strings );
	}
fclose( in );
fclose( header );
fclose( strings );
fclose( point );
}
/*****************************************************************************/


