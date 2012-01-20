#include<sys/socket.h>
#include<linux/pf_ring.h>
#include<netinet/if_ether.h>
#include<string.h>
#include<stdio.h>
#include <sys/mman.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <net/ethernet.h>     /* the L2 protocols */
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include"pfring_utils.h"



#define PAGE_SIZE               4096


static char hex[] = "0123456789ABCDEF";
FlowSlotInfo *slots_info;
char *slots;
u_int16_t slot_header_len;


int32_t gmt2local(time_t t) {
  int dt, dir;
  struct tm *gmt, *loc;
  struct tm sgmt;

  if (t == 0)
    t = time(NULL);
  gmt = &sgmt;
  *gmt = *gmtime(&t);
  loc = localtime(&t);
  dt = (loc->tm_hour - gmt->tm_hour) * 60 * 60 +
    (loc->tm_min - gmt->tm_min) * 60;

  /*
   * If the year or julian day is different, we span 00:00 GMT
   * and must add or subtract a day. Check the year first to
   * avoid problems when the julian day wraps.
   */
  dir = loc->tm_year - gmt->tm_year;
  if (dir == 0)
    dir = loc->tm_yday - gmt->tm_yday;
  dt += dir * 24 * 60 * 60;

  return (dt);
}


char* _intoa(unsigned int addr, char* buf, u_short bufLen) {
  char *cp, *retStr;
  u_int byte;
  int n;

  cp = &buf[bufLen];
  *--cp = '\0';

  n = 4;
  do {
    byte = addr & 0xff;
    *--cp = byte % 10 + '0';
    byte /= 10;
    if (byte > 0) {
      *--cp = byte % 10 + '0';
      byte /= 10;
      if (byte > 0)
	*--cp = byte + '0';
    }
    *--cp = '.';
    addr >>= 8;
  } while (--n > 0);

  /* Convert the string to lowercase */
  retStr = (char*)(cp+1);

  return(retStr);
}



char* proto2str(u_short proto) {
  static char protoName[8];

  switch(proto) {
  case IPPROTO_TCP:  return("TCP");
  case IPPROTO_UDP:  return("UDP");
  case IPPROTO_ICMP: return("ICMP");
  default:
    snprintf(protoName, sizeof(protoName), "%d", proto);
    return(protoName);
  }
}


/* ************************************ */

char* intoa(unsigned int addr) {
  static char buf[sizeof "ff:ff:ff:ff:ff:ff:255.255.255.255"];

  return(_intoa(addr, buf, sizeof(buf)));
}

/* ************************************ */


inline char* in6toa(struct in6_addr addr6) {
  static char buf[sizeof "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"];

  snprintf(buf, sizeof(buf),
	   "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
	   addr6.s6_addr[0], addr6.s6_addr[1], addr6.s6_addr[2],
	   addr6.s6_addr[3], addr6.s6_addr[4], addr6.s6_addr[5], addr6.s6_addr[6],
	   addr6.s6_addr[7], addr6.s6_addr[8], addr6.s6_addr[9], addr6.s6_addr[10],
	   addr6.s6_addr[11], addr6.s6_addr[12], addr6.s6_addr[13], addr6.s6_addr[14],
	   addr6.s6_addr[15]);

  return(buf);
}

char* etheraddr_string(const u_char *ep, char *buf) {
  u_int i, j;
  char *cp;

  cp = buf;
  if ((j = *ep >> 4) != 0)
    *cp++ = hex[j];
  else
    *cp++ = '0';

  *cp++ = hex[*ep++ & 0xf];

  for(i = 5; (int)--i >= 0;) {
    *cp++ = ':';
    if ((j = *ep >> 4) != 0)
      *cp++ = hex[j];
    else
      *cp++ = '0';

    *cp++ = hex[*ep++ & 0xf];
  }

  *cp = '\0';
  return (buf);
}
int recv_pack(int sock_raw, u_char** buffer, u_int buffer_len,
		    struct pfring_pkthdr *hdr,
		    u_int8_t wait_for_incoming_packet) {
  int rc = 0;
  

  if(buffer == NULL)
    return(-1);

  do_pfring_recv:
    //rmb();

    if(slots_info->tot_insert != slots_info->tot_read) {
      char *bucket = &slots[slots_info->remove_off];
      u_int32_t next_off, real_slot_len, insert_off, bktLen;

      memcpy(hdr, bucket, slot_header_len);

      if(slot_header_len != sizeof(struct pfring_pkthdr))
	bktLen = hdr->caplen;
      else
	bktLen = hdr->caplen+hdr->extended_hdr.parsed_header_len;

      real_slot_len = slot_header_len + bktLen;
      insert_off = slots_info->insert_off;
      if(bktLen > buffer_len) bktLen = buffer_len;

      if(buffer_len == 0)
	*buffer = (u_char*)&bucket[slot_header_len];
      else
	memcpy(*buffer, &bucket[slot_header_len], bktLen);

      next_off = slots_info->remove_off + real_slot_len;
      if((next_off + slots_info->slot_len) > (slots_info->tot_mem - sizeof(FlowSlotInfo))) {
        next_off = 0;
      }

#ifdef USE_MB
      /* This prevents the compiler from reordering instructions.
       * http://en.wikipedia.org/wiki/Memory_ordering#Compiler_memory_barrier */
      gcc_mb();
#endif

      slots_info->tot_read++;
      slots_info->remove_off = next_off;

      /* Ugly safety check */
      if((slots_info->tot_insert == slots_info->tot_read)
	 && (slots_info->remove_off > slots_info->insert_off)) {
	slots_info->remove_off = slots_info->insert_off;
      }
/*
      if(ring->reentrant) pthread_spin_unlock(&ring->spinlock);
      return(1);*/
	return 1;
    }

    /* Nothing to do: we need to wait */
    //if(ring->reentrant) pthread_spin_unlock(&ring->spinlock);

      return(0); /* non-blocking, no packet */
}


int pfring_recv(int sock_raw, u_char** buffer, u_int buffer_len,
		struct pfring_pkthdr *hdr,
		u_int8_t wait_for_incoming_packet) {
  
    return recv_pack(sock_raw, buffer, buffer_len, hdr, wait_for_incoming_packet);

  return -1;
}

int parse_pkt(u_char *pkt, struct pfring_pkthdr *hdr)
{
  struct iphdr *ip;
  struct eth_hdr *eh = (struct eth_hdr*)pkt;
  u_int16_t displ;

  memset(&hdr->extended_hdr.parsed_pkt, 0, sizeof(struct pkt_parsing_info));

  hdr->extended_hdr.parsed_header_len = 0;
  hdr->extended_hdr.parsed_pkt.eth_type = ntohs(eh->h_proto);
  hdr->extended_hdr.parsed_pkt.offset.eth_offset = 0;

  if(hdr->extended_hdr.parsed_pkt.eth_type == 0x8100 /* 802.1q (VLAN) */)
    {
      hdr->extended_hdr.parsed_pkt.offset.vlan_offset = hdr->extended_hdr.parsed_pkt.offset.eth_offset + sizeof(struct eth_hdr);
      hdr->extended_hdr.parsed_pkt.vlan_id = (pkt[hdr->extended_hdr.parsed_pkt.offset.eth_offset + 14] & 15) * 256
	+ pkt[hdr->extended_hdr.parsed_pkt.offset.eth_offset + 15];
      hdr->extended_hdr.parsed_pkt.eth_type = (pkt[hdr->extended_hdr.parsed_pkt.offset.eth_offset + 16]) * 256
	+ pkt[hdr->extended_hdr.parsed_pkt.offset.eth_offset + 17];
      displ = 4;
    }
  else
    {
      displ = 0;
      hdr->extended_hdr.parsed_pkt.vlan_id = 0; /* Any VLAN */
    }

  if(hdr->extended_hdr.parsed_pkt.eth_type == 0x0800 /* IP */) {
    hdr->extended_hdr.parsed_pkt.offset.l3_offset = hdr->extended_hdr.parsed_pkt.offset.eth_offset+displ+sizeof(struct eth_hdr);
    ip = (struct iphdr*)(pkt+hdr->extended_hdr.parsed_pkt.offset.l3_offset);

    hdr->extended_hdr.parsed_pkt.ipv4_src = ntohl(ip->saddr), hdr->extended_hdr.parsed_pkt.ipv4_dst = ntohl(ip->daddr), hdr->extended_hdr.parsed_pkt.l3_proto = ip->protocol;
    hdr->extended_hdr.parsed_pkt.ipv4_tos = ip->tos;

    if((ip->protocol == IPPROTO_TCP) || (ip->protocol == IPPROTO_UDP)) {
      u_int16_t ip_len = ip->ihl*4;

      hdr->extended_hdr.parsed_pkt.offset.l4_offset = hdr->extended_hdr.parsed_pkt.offset.l3_offset+ip_len;

      if(ip->protocol == IPPROTO_TCP) {
	struct tcphdr *tcp = (struct tcphdr*)(pkt+hdr->extended_hdr.parsed_pkt.offset.l4_offset);
	hdr->extended_hdr.parsed_pkt.l4_src_port = ntohs(tcp->source), hdr->extended_hdr.parsed_pkt.l4_dst_port = ntohs(tcp->dest);
	hdr->extended_hdr.parsed_pkt.offset.payload_offset = hdr->extended_hdr.parsed_pkt.offset.l4_offset+(tcp->doff * 4);
	hdr->extended_hdr.parsed_pkt.tcp.seq_num = ntohl(tcp->seq);
	hdr->extended_hdr.parsed_pkt.tcp.ack_num = ntohl(tcp->ack_seq);
	hdr->extended_hdr.parsed_pkt.tcp.flags = (tcp->fin * TH_FIN_MULTIPLIER) + (tcp->syn * TH_SYN_MULTIPLIER) + (tcp->rst * TH_RST_MULTIPLIER) +
	  (tcp->psh * TH_PUSH_MULTIPLIER) + (tcp->ack * TH_ACK_MULTIPLIER) + (tcp->urg * TH_URG_MULTIPLIER);
      } else if(ip->protocol == IPPROTO_UDP) {
	struct udphdr *udp = (struct udphdr*)(pkt+hdr->extended_hdr.parsed_pkt.offset.l4_offset);
	hdr->extended_hdr.parsed_pkt.l4_src_port = ntohs(udp->source), hdr->extended_hdr.parsed_pkt.l4_dst_port = ntohs(udp->dest);
	hdr->extended_hdr.parsed_pkt.offset.payload_offset = hdr->extended_hdr.parsed_pkt.offset.l4_offset+sizeof(struct udphdr);
      } else
	hdr->extended_hdr.parsed_pkt.offset.payload_offset = hdr->extended_hdr.parsed_pkt.offset.l4_offset;
    } else
      hdr->extended_hdr.parsed_pkt.l4_src_port = hdr->extended_hdr.parsed_pkt.l4_dst_port = 0;

    return(1); /* IP */
  } /* TODO: handle IPv6 */

  return(0); /* No IP */
}


static int32_t thiszone;
void looper(const struct pfring_pkthdr *h, const u_char *p, const u_char *user_bytes) {
  long threadId = (long)user_bytes;
   int verbose=1;	
  if(verbose) {
    struct ether_header ehdr;
    u_short eth_type, vlan_id;
    char buf1[32], buf2[32];
    struct ip ip;
    int s;
    uint usec;
    uint nsec=0;

    if(h->ts.tv_sec == 0) {
      gettimeofday((struct timeval*)&h->ts, NULL);
      parse_pkt((u_char*)p, (struct pfring_pkthdr*)h);
    }
 
    if(h->extended_hdr.timestamp_ns) {
      s = ((h->extended_hdr.timestamp_ns / 1000000000) + thiszone) % 86400;
      usec = (h->extended_hdr.timestamp_ns / 1000) % 1000000;
      nsec = h->extended_hdr.timestamp_ns % 1000;
    } else {
      s = (h->ts.tv_sec + thiszone) % 86400;
      usec = h->ts.tv_usec;
    }

    printf("%02d:%02d:%02d.%06u%03u ",
	   s / 3600, (s % 3600) / 60, s % 60,
	   usec, nsec);

#if 0
    for(i=0; i<32; i++) printf("%02X ", p[i]);
    printf("\n");
#endif

    if(h->extended_hdr.parsed_header_len > 0) {
      printf("[eth_type=0x%04X]", 
	     h->extended_hdr.parsed_pkt.eth_type);
      printf("[l3_proto=%u]", (unsigned int)h->extended_hdr.parsed_pkt.l3_proto);

      printf("[%s:%d -> ", (h->extended_hdr.parsed_pkt.eth_type == 0x86DD) ?
	     in6toa(h->extended_hdr.parsed_pkt.ipv6_src) : intoa(h->extended_hdr.parsed_pkt.ipv4_src),
	     h->extended_hdr.parsed_pkt.l4_src_port);
      printf("%s:%d] ", (h->extended_hdr.parsed_pkt.eth_type == 0x86DD) ?
	     in6toa(h->extended_hdr.parsed_pkt.ipv6_dst) : intoa(h->extended_hdr.parsed_pkt.ipv4_dst),
	     h->extended_hdr.parsed_pkt.l4_dst_port);

      printf("[%s -> %s] ",
	     etheraddr_string(h->extended_hdr.parsed_pkt.smac, buf1),
	     etheraddr_string(h->extended_hdr.parsed_pkt.dmac, buf2));
    }

    memcpy(&ehdr, p+h->extended_hdr.parsed_header_len, sizeof(struct ether_header));
    eth_type = ntohs(ehdr.ether_type);

    printf("[%s][%s -> %s][eth_type=0x%04X] ",
	   h->extended_hdr.rx_direction ? "RX" : "TX",
	   etheraddr_string(ehdr.ether_shost, buf1),
	   etheraddr_string(ehdr.ether_dhost, buf2), eth_type);


    if(eth_type == 0x8100) {
      vlan_id = (p[14] & 15)*256 + p[15];
      eth_type = (p[16])*256 + p[17];
      printf("[vlan %u] ", vlan_id);
      p+=4;
    }

    if(eth_type == 0x0800) {
      memcpy(&ip, p+h->extended_hdr.parsed_header_len+sizeof(ehdr), sizeof(struct ip));
      printf("[%s]", proto2str(ip.ip_p));
      printf("[%s:%d ", intoa(ntohl(ip.ip_src.s_addr)), h->extended_hdr.parsed_pkt.l4_src_port);
      printf("-> %s:%d] ", intoa(ntohl(ip.ip_dst.s_addr)), h->extended_hdr.parsed_pkt.l4_dst_port);

      printf("[hash=%u][tos=%d][tcp_seq_num=%u][caplen=%d][len=%d][parsed_header_len=%d]"
	     "[eth_offset=%d][l3_offset=%d][l4_offset=%d][payload_offset=%d]\n",
	     h->extended_hdr.pkt_hash,
	     h->extended_hdr.parsed_pkt.ipv4_tos, h->extended_hdr.parsed_pkt.tcp.seq_num,
	     h->caplen, h->len, h->extended_hdr.parsed_header_len,
	     h->extended_hdr.parsed_pkt.offset.eth_offset,
	     h->extended_hdr.parsed_pkt.offset.l3_offset,
	     h->extended_hdr.parsed_pkt.offset.l4_offset,
	     h->extended_hdr.parsed_pkt.offset.payload_offset);

    } else {
      if(eth_type == 0x0806)
	printf("[ARP]");
      else
	printf("[eth_type=0x%04X]", eth_type);

      printf("[caplen=%d][len=%d][parsed_header_len=%d]"
	     "[eth_offset=%d][l3_offset=%d][l4_offset=%d][payload_offset=%d]\n",
	     h->caplen, h->len, h->extended_hdr.parsed_header_len,
	     h->extended_hdr.parsed_pkt.offset.eth_offset,
	     h->extended_hdr.parsed_pkt.offset.l3_offset,
	     h->extended_hdr.parsed_pkt.offset.l4_offset,
	     h->extended_hdr.parsed_pkt.offset.payload_offset);
    }
  }
}



int main()
{
	int rc,caplen=128;
	char *buffer;
	u_char *actual_buffer = NULL;
	struct sockaddr sa;
	char device_name[]="eth0";
	u_int memSlotsLen;
	socklen_t len = sizeof(slot_header_len);
	packet_direction direction=0;
	char dummy;
	struct pfring_pkthdr hdr;
	u_int8_t wait_for_packet = 1;
	

	int sock_raw=socket(PF_RING, SOCK_RAW, htons(ETH_P_ALL));
	if(sock_raw < 0)
	{
		printf("Socket Error\n");
		return 1;
	}


	thiszone = gmt2local(0);

	rc = setsockopt(sock_raw, 0, SO_RING_BUCKET_LEN, &caplen, sizeof(caplen));

	if (rc < 0) {
		printf("setsockopt error");
		close(sock_raw);
	    	return -1;
	}




	memset(&sa, 0, sizeof(sa));
	sa.sa_family = PF_RING;
  	snprintf(sa.sa_data, sizeof(sa.sa_data), "%s", device_name);

  	rc = bind(sock_raw, (struct sockaddr *)&sa, sizeof(sa));
  	if(rc != 0) {
		printf("failed to bind....");
		close(sock_raw);
		return -1;
	}



	buffer = (char *)mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE,
			      MAP_SHARED, sock_raw, 0);
	if(buffer == MAP_FAILED) {
    		printf("mmap() failed: try with a smaller snaplen\n");
    		close(sock_raw);
    		return -1;
  	}

	slots_info = (FlowSlotInfo *)buffer;
	if(slots_info->version != RING_FLOWSLOT_VERSION) {
	printf("Wrong RING version: "
	   "kernel is %i, libpfring was compiled with %i\n",
	   slots_info->version, RING_FLOWSLOT_VERSION);
	close(sock_raw);
	return -1;
	}
	memSlotsLen =slots_info->tot_mem;
	munmap(buffer, PAGE_SIZE);

	buffer = (char *)mmap(NULL, memSlotsLen,
			      PROT_READ|PROT_WRITE,
			      MAP_SHARED, sock_raw, 0);

	/* printf("mmap len %u\n", memSlotsLen); */

	if(buffer == MAP_FAILED) {
	printf("mmap() failed");
	close(sock_raw);
	return -1;
	}

	slots_info = (FlowSlotInfo *)buffer;
	slots = (char *)(buffer+sizeof(FlowSlotInfo));

	rc = getsockopt(sock_raw, 0, SO_GET_PKT_HEADER_LEN, &slot_header_len, &len);
	if(rc!=0)
	{
		printf("error in getting socket header len");
		close(sock_raw);
		munmap(buffer,memSlotsLen);
		return -1;
	}


	//getting mac address of bounded device
	if(1)
	{
	 	socklen_t len1 = 6;
		u_char mac_address[6];
		char buf[32];
		if(getsockopt(sock_raw, 0, SO_GET_BOUND_DEVICE_ADDRESS, mac_address, &len1) != 0)
    			printf("Impossible to know the device address\n");
  		else
    			printf("Capturing from %s [%s]\n", device_name, etheraddr_string(mac_address, buf));

	  	
	}


	setsockopt(sock_raw, 0, SO_SET_PACKET_DIRECTION, &direction, sizeof(direction));


	dummy = 0;
  	rc=setsockopt(sock_raw, 0, SO_ACTIVATE_RING, &dummy, sizeof(dummy));
	if(rc!=0)
	{
		printf("Error in enableing ring");
		munmap(buffer,memSlotsLen);	
		close(sock_raw);
	}




	while(1) {
	    rc = pfring_recv(sock_raw, &actual_buffer, 0, &hdr, wait_for_packet);
	    if(rc < 0)
	      break;
	    else if(rc > 0)
	      looper(&hdr, actual_buffer,  (u_char*)NULL);
	    else {
	      sleep(1);
	    }
	  }


	munmap(buffer,memSlotsLen);	
	close(sock_raw);
	return 0;
}
