/*
** This file holds routines for the end of month interaction with the VPs
** written by Alan Fothergill
** started 17-Jan-1988
*/

#include	"header.h"	
#include	"variab.h"
extern	char	vp_bankbal[], vp_banktext[];
extern	int	isevent;

/*
**	This routine does VP interaction at the end of a month
**	It does the non-interactive events, ie the ones that do not pull up
**	animating graphs.
*/

dovpmonth()
{
extern	int	expoilevent(), offplotevent(), sellall_event();
extern	int	making_loss_event(), trans_oil_event(), build_st_event();
extern	int	prospect_event(), well_event(), supply_event();
extern	int	compete1_event(), compete2_event(), compete3_event();
extern	int	need_store_event(), ex_trans1_event(), ex_trans2_event();
extern	int	well_ef_event(), blow_up_event(), tax_event();
char	text[30];
static	int	i = 0;
int	off, j;
static	int	finishvp = 0;
static	int	(*non_inter[])()=
				{
				expoilevent,
				offplotevent,
				build_st_event,
				prospect_event,
				well_event,
				compete1_event,
				need_store_event,
				ex_trans2_event,
				tax_event,
				well_ef_event,
				blow_up_event
				};

static	int	(*inter[])()	=
				{
				sellall_event,
				making_loss_event,
				supply_event,
				trans_oil_event,
				compete2_event,
				compete3_event,
				ex_trans1_event
				};

if( isevent )
	return TRUE;
if( finishvp || !vphelp )
	{
	i		= 0;
	finishvp	= 0;
	if( window[4].handle )
		close_wind( window[4].handle );
	return	FALSE;
	}

formnum( text, cashchange( 0, RDCASH ) );
sprintf( vp_bankbal, vp_banktext, text );

do
	{
	if( i == 11 )
		{
		off		= rand()%7;
		finishvp	= 1;
		for( j = 0 ; j < 7 ; ++j )
			{
			i	= (j+off)%7;
			if( (*inter[i])() )
				{
				wind_set( window[4].handle, WF_TOP, 0, 0, 0, 0);
				isevent		= 1;
				return	TRUE;
				}
			}
		return	TRUE;
		}
	}
	while( !(*non_inter[i++])() );
isevent	= TRUE;
return	TRUE;
}

/*
**	This routine clears intray when newspaper closed
*/
clear_intray()
{
MSG	*message, *temp;

message	= firstmsg.next;

while( temp = message )			/* !!!!intentional assignment !!!*/
	{
	message	= message->next;
	free( temp );
	}
intraydraw = 1;				/*cond redraw*/
firstmsg.next	= 0L;
}



