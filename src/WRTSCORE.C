#include	<stdio.h>
typedef	float	real;

struct	score {
	char	name[11];
	real	reserves;
	}   scoretable[10] =	{
"Francis   ",24.0e+06,
"Ben Molony",5.773147e+06,
"Alan      ",1.211990e+06,
"Fothergill",1.100436e+06,
"Liz       ",1.000000e+06,
"Graham    ",5.0e+05,
"Simon K   ",2.0e+05,
"Dave      ",1.0e+05,
"Ian       ",5.0e+04,
"Andy      ",1.0e+04
};

main()
{
FILE	*fp;

fp	= fopen( "easy.scr", "wb" );

fwrite( &scoretable[0], sizeof( struct score ), 10, fp );
fclose( fp );

fp	= fopen( "hard.scr", "wb" );
fwrite( &scoretable[0], sizeof( struct score ), 10, fp );
}

real	price;
int	maxage, age;

m