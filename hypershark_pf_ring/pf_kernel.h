struct eth_hdr {
  unsigned char   h_dest[ETH_ALEN];       /* destination eth addr */
  unsigned char   h_source[ETH_ALEN];     /* source ether addr    */
  u_int16_t       h_proto;                /* packet type ID field */
};

typedef struct hs_pkt_hdr{
	struct pfring_pkthdr pkf;
        unsigned char *buffer;
}hs_pkt_hdr;

typedef struct flow_record{
  u_int32_t flow_id;
  u_int16_t src_port, dst_port;
  ip_addr   ip_src, ip_dst;
  u_int8_t  protocol;
  u_int64_t timestamp_ns;
  u_int64_t bytes_transfer;
  u_int32_t nop;
  u_int32_t start_pkt_no;
  u_int32_t nxtfr;
}flow_record;

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

typedef port_bits protocol_bits;
//typedef struct protcol_bits
//{
//	char octet_1[13];
//}protocol_bits;

typedef struct bitmap
{
	ip_bits *src_ip,*dst_ip;//255 instances 
	port_bits *src_port,*dst_port;//65536 instances
	protocol_bits *protocol;//255 instances
}bitmap;

typedef struct bitmap_start_offsets
{
	u_int16_t src_ip_octet[4];	
	u_int16_t dst_ip_octet[4];
	u_int16_t src_port_octet;
	u_int16_t dst_port_octet;
	u_int16_t protocol_octet;		
}bitmap_start_offsets;

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

typedef struct maprecord{
  u_int32_t  cust_id;
  vm_id vmid;
  char path[50];
  u_int16_t  link;
  u_int16_t GFL;//global flow location
  u_int32_t GPC;//global packet count	
  loff_t pkt_offset;
  char interface_name[15];
  offset_table *off_table;
  flow_record *flow_start;
  bitmap *bit_map;
}maprecord;

void lookupcreate(void);
int crc32(u_int32_t reg,u_int32_t flowkey);
int calc_hash(struct pfring_pkthdr *pfr);

#define TH_FIN_MULTIPLIER	0x01
#define TH_SYN_MULTIPLIER	0x02
#define TH_RST_MULTIPLIER	0x04
#define TH_PUSH_MULTIPLIER	0x08
#define TH_ACK_MULTIPLIER	0x10
#define TH_URG_MULTIPLIER	0x20