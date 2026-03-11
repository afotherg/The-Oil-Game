/*
**		RSCREATE   Definitive version by Ben Molony.
*/
#define	WORD	unsigned int
#define	UWORD	unsigned int
#define	LONG	long
#define	BYTE	char

#include "obdefs.h"
#include "rsrclib.h"
#include <stdio.h>

/*
** This structure is the header record that is written out at the top of
** each resource file.  The first variables (eg rsh_bitblk) give the address
** relative to the start of the file of where the various OBJECT types are
** stored.  The subsequent variables, (eg rsh_nobs) are the numbers of the
** various component parts of the resource.  rsh_rssize is the total size of
** the whole resorce file.
*/

RSHDR hdr = 
{
	0, 		/* rsh_vrsn	*/
	0, 		/* rsh_object	*/
	0,  		/* rsh_tedinfo	*/
	0, 		/* rsh_iconblk	*/
	0, 		/* rsh_bitblk	*/
	0, 		/* rsh_frstr	*/
	0, 		/* rsh_string	string data		*/
	0, 		/* rsh_imdata	image data		*/
	0, 		/* rsh_frimg	*/
	0, 		/* rsh_trindex	*/
	0, 		/* rsh_nobs	*/
	0, 		/* rsh_ntree	*/
	0, 		/* rsh_nted	*/
	0, 		/* rsh_nib	*/
	0, 		/* rsh_nbb	*/
	0, 		/* rsh_nstring	*/
	0, 		/* rsh_nimages	*/
	0		/* rsh_rssize	*/
};

#include "mega.rsh"
FILE		*name;
WORD		cnt;

main()   
{
WORD		i, len;
char		ch;

ch = 0x55;
name   = fopen(&pname[0],  "wb"); 

hdr.rsh_vrsn     = 0;
cnt += sizeof(RSHDR) * fwrite(&hdr, sizeof(RSHDR), 1, name);
hdr.rsh_string   = even(cnt);

for(i = 0 ; i < NUM_STRINGS ; ++i)
	{
	int	j;

	j = strlen(rs_strings[i]);
	cnt += j + 1;
	fwrite(rs_strings[i], j + 1, 1, name);
	printf("%s %d\n", rs_strings[i], cnt);
	}


hdr.rsh_imdata  = even(cnt);
fix_iconblk();
fix_bitblk();

hdr.rsh_frstr    = even(cnt);
hdr.rsh_bitblk   = even(hdr.rsh_frstr   +  sizeof(LONG)     *   NUM_FRSTR);
hdr.rsh_iconblk  = even(hdr.rsh_bitblk  +  sizeof(BITBLK)   *   NUM_BB);
hdr.rsh_tedinfo  = even(hdr.rsh_iconblk +  sizeof(ICONBLK)  *   NUM_IB);
hdr.rsh_object   = even(hdr.rsh_tedinfo +  sizeof(TEDINFO)  *   NUM_TI);
hdr.rsh_trindex  = even(hdr.rsh_object  +  sizeof(OBJECT)   *   NUM_OBS);

fix_trindex();
fix_objects();
fix_tedinfo();
fix_frstr();
fix_frimg();

cnt += sizeof(LONG)    * fwrite(&rs_frstr[0], sizeof(LONG), NUM_FRSTR, name);

cnt += sizeof(BITBLK)  * fwrite(&rs_bitblk[0], sizeof(BITBLK), NUM_BB, name);

cnt += sizeof(ICONBLK) * fwrite(&rs_iconblk[0], sizeof(ICONBLK), NUM_IB, name);

cnt += sizeof(TEDINFO) * fwrite(&rs_tedinfo[0], sizeof(TEDINFO), NUM_TI, name);

cnt += sizeof(OBJECT)  * fwrite(&rs_object[0], sizeof(OBJECT), NUM_OBS, name);

cnt += sizeof(LONG)    *  fwrite(rs_trindex, sizeof(LONG), NUM_TREE, name);

hdr.rsh_nobs    = NUM_OBS;
hdr.rsh_ntree   = NUM_TREE;
hdr.rsh_nted    = NUM_TI;
hdr.rsh_nib     = NUM_IB;
hdr.rsh_nbb     = NUM_BB;
hdr.rsh_nimages = NUM_FRIMG;
hdr.rsh_nstring = NUM_FRSTR;
hdr.rsh_rssize  = cnt;
rewind(name);
fwrite(&hdr, sizeof(RSHDR), 1, name);
printf("obj %d ted %d string %d imdata %d trindex %d size %d\n", 
hdr.rsh_object, hdr.rsh_tedinfo, hdr.rsh_string, hdr.rsh_imdata, hdr.rsh_trindex, hdr.rsh_rssize);
fclose(name);
}

/*
** Sets up the array rs_trindex which holds pointers to the starting address of
** the tree in the resource.
*/

fix_trindex()
	{
	WORD	test,  ii;

	for (ii = 0; ii < NUM_TREE; ii++)
		{
		test = (WORD) rs_trindex[ii];
		rs_trindex[ii] =(LONG)(sizeof(OBJECT)*test+ hdr.rsh_object);
		}
	}

/*
** Sets up the ob_spec pointer type for all objects, depending on what they are
*/

fix_objects()
	{
	WORD	test,  ii,  data;

	for (ii = 0; ii < NUM_OBS; ii++)
		{
		test = (WORD) rs_object[ii].ob_spec;
		switch (rs_object[ii].ob_type) {
			case G_TITLE:
			case G_STRING:
			case G_BUTTON:
				fix_str(&rs_object[ii].ob_spec);
				break;
			case G_TEXT:
			case G_BOXTEXT:
			case G_FTEXT:
			case G_FBOXTEXT:
				if (test != NIL)
				   rs_object[ii].ob_spec = 
			test * sizeof(TEDINFO) + hdr.rsh_tedinfo;
				break;
			case G_ICON:
				if (test != NIL)
				   rs_object[ii].ob_spec =
			test * sizeof(ICONBLK) + hdr.rsh_iconblk;

				
				break;
			case G_IMAGE:
				if (test != NIL)
				   rs_object[ii].ob_spec =
			test * sizeof(BITBLK) + hdr.rsh_bitblk;

				break;
			default:
				break;
			}
		}
	}

/*
** For the TEDINFO structure, assigns the correct string pointers
*/

fix_tedinfo()
	{
	WORD	ii;

	for (ii = 0; ii < NUM_TI; ii++)
		{
		fix_str(&rs_tedinfo[ii].te_ptext);
		fix_str(&rs_tedinfo[ii].te_ptmplt);
		fix_str(&rs_tedinfo[ii].te_pvalid);
		}
	}

/*
** Free strings?? Never used in practice.
*/

fix_frstr()
	{
	WORD	ii;
		for (ii = 0; ii < NUM_FRSTR; ii++)
		fix_str(&rs_frstr[ii]);
	}

/*
** Finds the address of the 'where-th' string in the resource.
*/

fix_str(where)
	LONG	*where;
	{
	if (*where != NIL)
	*where = (LONG)((rs_strings[(WORD) *where] - rs_strings[0]) + hdr.rsh_string);
	}
/*
** ICONBLK, sets up the two images and associated text.
*/

fix_iconblk()
	{
	WORD	ii;

	for (ii = 0; ii < NUM_IB; ii++)
		{
		fix_img(&rs_iconblk[ii].ib_pmask, rs_iconblk[ii].ib_wicon/8, rs_iconblk[ii].ib_hicon);
		fix_img(&rs_iconblk[ii].ib_pdata, rs_iconblk[ii].ib_wicon/8, rs_iconblk[ii].ib_hicon);
		fix_str(&rs_iconblk[ii].ib_ptext);
		}
	}

/*
** BITBLK, as for ICONBLK.
*/

fix_bitblk()
	{
	WORD	ii;

	for (ii = 0; ii < NUM_BB; ii++)
		fix_img(&rs_bitblk[ii].bi_pdata, rs_bitblk[ii].bi_wb, rs_bitblk[ii].bi_hl);
	}

/*
**  ???? Never used.
*/

fix_frimg()
	{
	WORD	ii;

	for (ii = 0; ii < NUM_FRIMG; ii++)
		fix_bb(&rs_frimg[ii]);
	}

fix_bb(where)
	LONG	*where;
	{
	if (*where != NIL)
	 *where = (LONG)((&rs_bitblk[(WORD) *where] - &rs_bitblk[0]) + hdr.rsh_bitblk);
	}

/*
** Writes out the image data for icons and bitblks.
*/

fix_img(where, wb, hp)
	LONG	*where;
	WORD	wb, hp;
{
WORD	data;
int	i, out, out2;

#if	0
data = wb * hp;
for(i = 0 ; i < data/2 ; ++i)
	{
	out = ~(*((int *)rs_imdope[(int)*where].image + i));
	swab((char *)&out, (char *)&out2, 2);	/*I have no idea why this needs to be done */
	putw(out2, name);
	}
if (*where != NIL)
	*where = (LONG)hdr.rsh_imdata;
cnt += data;
#endif

*where = hdr.rsh_imdata;/* 
			** REMOVE THIS LINE if above code reactivated.  The
			** variable 'where' is the address of the start of the
			** bit map.  Setting it to zero will obviously produce
			** a garbled image.
			*/
}

/*
** Each of the starting positions must be at an even address.
*/

even(addr)
WORD	addr;
{

if(addr%2 != 0)
	{
	fwrite("", 1, 1, name);
	++cnt;
	return(addr + 1);
	}
return(addr);
}

???????????????????????????????????????????????????????????????????????