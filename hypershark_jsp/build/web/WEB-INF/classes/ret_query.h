#include<linux/pf_ring.h>
#include "pf_kernel.h"
#include "bitmap.h"
#include "filelist.h"

typedef struct display_rule
{
	short *sip_oct;//change from int *
	short *dip_oct;//change from int *
	loff_t src_port;
	loff_t dst_port;
	int protocol;
}display_rule;

typedef struct flow_rec_nos
{
	int no;
	flow_record fr; 
	hs_pkt_hdr *pkt_list;
	struct flow_rec_nos *next;
}flow_rec_nos;

typedef struct comp_flow_list
{
	flow_rec_nos *frec;
	struct comp_flow_list *down;		
}comp_flow_list;


typedef struct mapstr
{
	int type;
	char typestr[90];
}mapstr;

flow_rec_nos* decode_bitmap(unsigned char *map,int max_bits);
