#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/string.h>
#include<linux/socket.h>
#include<linux/net.h>
#include<linux/in.h>
#include<net/sock.h>
#include<asm/processor.h>
#include<asm/uaccess.h>
#include<linux/fs.h>
#include<linux/ip.h>
#include<linux/tcp.h>
#include<linux/udp.h>
#include<linux/icmp.h>
#include<linux/vmalloc.h>
#include"afcapture.h"
#define SEED 0xFFFFFFFF

/*This function is responsible for creating the Lookup Table used for
  Calculating the CRC*/

u_int32_t *CRCTable;
void lookupcreate(void)
{
  u_int8_t i,j;
  u_int32_t CRC;
  CRCTable=kmalloc(256*sizeof(u_int32_t),GFP_KERNEL);
  for(i=0;i<25;i++)
    {
      CRC=i;
      for(j=8;j>0;j--)
	{
	  if(CRC & 1)
	    CRC=(CRC >> 1)^0x04C11DB7;//Generator Polynomial
	  else
	    CRC>>=1;
	}
      CRCTable[i]=CRC;
    }
}

int crc32(u_int32_t reg,u_int32_t flowkey)
{
	unsigned int i,tmp=0x000f;
	u_int8_t idx=0,flowkeys[4];
	memcpy(flowkeys,&flowkey,sizeof(flowkey));
	for(i=0;i<4;i++)
	{
		idx = reg & tmp;
		reg = reg >> 8;
		idx = idx ^ flowkeys[i];
		reg = reg ^ CRCTable[idx];
		if(flowkeys[1] || flowkeys[2] || flowkeys[3])
			break;
	}	
	reg = reg ^ SEED;
	return reg;
}

int calc_hash(struct pfring_pkthdr *pfr)
{
	int i;
	unsigned int hash=SEED,flowkey[3];
	memcpy(flowkey,&pfr->extended_hdr.parsed_pkt.ip_src,sizeof(flowkey));
	for(i=0;i<3;i++)
	{
		hash=crc32(hash,flowkey[i]);
	}
	hash=crc32(hash,pfr->extended_hdr.parsed_pkt.l3_proto);
	return hash;
}

/*int main()
{
	struct pfring_pkthdr pfr;
	u_int32_t hh;
	pfr.extended_hdr.parsed_pkt.l4_src_port=80;
	pfr.extended_hdr.parsed_pkt.l4_dst_port=90;
	pfr.extended_hdr.parsed_pkt.ip_src=199;
	pfr.extended_hdr.parsed_pkt.ip_dst=200;
	pfr.extended_hdr.parsed_pkt.l3_proto=1;
	lookup_create();
	//	hh=(calc_hash(&pfr));
	//printf("hash:%u",calc_hash(&pfr));
	return 0;		
}*/
