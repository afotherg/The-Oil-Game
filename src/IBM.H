#include	<malloc.h>
extern	int	c_dist();
#define	DB	fprintf(stderr, "F:%s  line %d\n",__FILE__,__LINE__)
#define		GETBUFFER(xyz)	\
{\
int	o1, o2, o3, o4;\
wind_get( 0,WF_SCREEN, &o1, &o2, &o3, &o4 );\
xyz	= (((long)o2)<<16) + o1;\
}

#define	lmalloc(xyz)	(char*)halloc( xyz, 1)
#define	STTMDLY		1000

