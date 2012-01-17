#include<linux/net.h>
#include<linux/in6.h>
#include<linux/ip.h>
#include<linux/if_ether.h>
#include<linux/tcp.h>
#include<linux/udp.h>

struct eth_hdr {
  unsigned char   h_dest[ETH_ALEN];       /* destination eth addr */
  unsigned char   h_source[ETH_ALEN];     /* source ether addr    */
  u_int16_t       h_proto;                /* packet type ID field */
};

#define TH_FIN_MULTIPLIER	0x01
#define TH_SYN_MULTIPLIER	0x02
#define TH_RST_MULTIPLIER	0x04
#define TH_PUSH_MULTIPLIER	0x08
#define TH_ACK_MULTIPLIER	0x10
#define TH_URG_MULTIPLIER	0x20



struct pkt_offset {
  int16_t eth_offset; /* This offset *must* be added to all offsets below */
  int16_t vlan_offset;
  int16_t l3_offset;
  int16_t l4_offset;
  int16_t payload_offset;
};

struct pkt_flow_info {
  u_int32_t in_iface, out_iface, samplingPopulation, flow_sequence;
};

typedef union {
  struct in6_addr v6;  /* IPv6 src/dst IP addresses (Network byte order) */
  u_int32_t v4;        /* IPv4 src/dst IP addresses */
} ip_addr;

typedef union {
  struct pkt_flow_info flow; /* Flow Information */
  //struct pkt_aggregation_info aggregation; /* Future or plugin use */
} packet_user_detail;

struct pkt_parsing_info {
  /* Core fields (also used by NetFlow) */
  u_int8_t dmac[ETH_ALEN], smac[ETH_ALEN];  /* MAC src/dst addresses */
  u_int16_t eth_type;   /* Ethernet type */
  u_int16_t vlan_id;    /* VLAN Id or NO_VLAN */
  u_int8_t  ip_version;
  u_int8_t  l3_proto, ip_tos; /* Layer 3 protocol/TOS */
  ip_addr   ip_src, ip_dst;   /* IPv4 src/dst IP addresses */
  u_int16_t l4_src_port, l4_dst_port; /* Layer 4 src/dst ports */
  struct {
    u_int8_t flags;   /* TCP flags (0 if not available) */
    u_int32_t seq_num, ack_num; /* TCP sequence number */
  } tcp;
  u_int16_t last_matched_plugin_id; /* If > 0 identifies a plugin to that matched the packet */
  u_int16_t last_matched_rule_id; /* If > 0 identifies a rule that matched the packet */
  struct pkt_offset offset; /* Offsets of L3/L4/payload elements */

  /* Leave it at the end of the structure */
  packet_user_detail pkt_detail;
};


struct pfring_extended_pkthdr {
  u_int64_t timestamp_ns; /* Packet timestamp at ns precision. Note that if your NIC supports
			     hardware timestamp, this is the place to read timestamp from */
  u_int8_t rx_direction;  /* 1=RX: packet received by the NIC, 0=TX: packet transmitted by the NIC */
  int if_index;           /* index of the interface on which the packet has been received.
                             It can be also used to report other information */
  u_int32_t pkt_hash;     /* Hash based on the packet header */
  u_int16_t parsed_header_len; /* Extra parsing data before packet */

  /* NOTE: leave it as last field of the memset on parse_pkt() will fail */
  struct pkt_parsing_info parsed_pkt; /* packet parsing info */
};

/* NOTE

   Keep 'struct pfring_pkthdr' in sync with 'struct pcap_pkthdr'
*/

struct pfring_pkthdr {
  /* pcap header */
  struct timeval ts;    /* time stamp */
  u_int32_t caplen;     /* length of portion present */
  u_int32_t len;        /* length this packet (off wire) */
  struct pfring_extended_pkthdr extended_hdr; /* PF_RING extended header */
};

/*struct hs_pkt_hdr{
	u_int32_t pkt_id;
	struct pfring_pkthdr *pkf;
	char *buffer;
	int buffer_len;
};*/

typedef struct hs_pkt_hdr{
	u_int32_t pkt_id;
	struct pfring_pkthdr pkf;
        char buffer[1500];//Has to be changed
	u_int16_t buffer_len;
}hs_pkt_hdr;

typedef struct flow_record{
  u_int32_t flow_id;
  u_int16_t src_port, dst_port;
  ip_addr   ip_src, ip_dst;
  u_int8_t  protocol;
  u_int64_t timestamp_ns;
  u_int32_t nop;
  u_int32_t start_pkt_no;
  //u_int32_t *offset_table;
  u_int32_t nxtfr;
}flow_record;

typedef struct offset_node{
  loff_t offset;
  struct offset_node *next;
}offset_node;

typedef struct offset_table{
  struct offset_node *start;
  struct offset_node *end;
}offset_table;

typedef struct vm_id
{
  u_int32_t  v_id[4];
}vm_id;
//Customer Records Mapping Structure

typedef struct ip_bits
{
	char octet_1[13];//for 100 bits
	char octet_2[13];
	char octet_3[13];
	char octet_4[13];
}ip_bits;

typedef struct port_bits
{
	char octet_1[13];
}port_bits;

typedef struct protcol_bits
{
	char octet_1[13];
}protocol_bits;

typedef struct bitmap
{
	ip_bits *src_ip,*dst_ip;//255 instances 
	port_bits *src_port,*dst_port;//65536 instances
	protocol_bits *protocol;//255 instances
}bitmap;

typedef struct maprecord{
  u_int32_t  cust_id;
  vm_id vmid;
  char path[15];
  u_int16_t  link;
  u_int16_t GFL;//global flow location
  u_int32_t GPC;//global packet count	
  loff_t pkt_offset;
  offset_table *off_table;
  flow_record *flow_start;
  bitmap *bit_map;
}maprecord;

void lookupcreate(void);
int crc32(u_int32_t reg,u_int32_t flowkey);
int calc_hash(struct pfring_pkthdr *pfr);
