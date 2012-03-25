#define MAX_FLOW_REC 100
#define MAX_FLOW_BYTE 13

struct eth_hdr {
  unsigned char   h_dest[ETH_ALEN];       /* destination eth addr */
  unsigned char   h_source[ETH_ALEN];     /* source ether addr    */
  u_int16_t       h_proto;                /* packet type ID field */
};

typedef struct hs_pkt_hdr{
	struct pfring_pkthdr pkf;
        unsigned char *buffer;
	struct hs_pkt_hdr *next;
}hs_pkt_hdr;

typedef struct flow_record{
  u_int32_t flow_id;
  u_int16_t src_port, dst_port;
  ip_addr   ip_src, ip_dst;
  u_int8_t  protocol;
  struct timeval start_time;
  struct timeval end_time;
  u_int64_t bytes_transfer;
  u_int32_t nop;
  u_int32_t start_pkt_no;
  u_int32_t nxtfr;
//Change Begins
  int pkt_file_no;
//Change Ends
}flow_record;

typedef struct ip_bits
{
	char octet_1[MAX_FLOW_BYTE];//for 100 bits
	char octet_2[MAX_FLOW_BYTE];
	char octet_3[MAX_FLOW_BYTE];
	char octet_4[MAX_FLOW_BYTE];		
}ip_bits;

typedef struct port_bits
{
	char octet_1[MAX_FLOW_BYTE];
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
	loff_t src_ip_octet[4];	
	loff_t dst_ip_octet[4];
	loff_t src_port_octet;
	loff_t dst_port_octet;
	loff_t protocol_octet;		
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
//Change
	u_int16_t GPKT;//global packet trace file number
//Change ends
	u_int32_t GPC;//global packet count	
	loff_t pkt_offset;
	char interface_name[15];
	offset_table *off_table;
	flow_record *flow_start;
	bitmap *bit_map;
	u_int8_t store_pkt;
	filtering_rule filter;
	struct task_struct *ts;
}maprecord;

void lookupcreate(void);
int crc32(u_int32_t reg,u_int32_t flowkey);
int calc_hash(struct pfring_pkthdr *pfr);
u_int32_t map_hash(u_int32_t cust_id,vm_id *vmid);

#define TH_FIN_MULTIPLIER	0x01
#define TH_SYN_MULTIPLIER	0x02
#define TH_RST_MULTIPLIER	0x04
#define TH_PUSH_MULTIPLIER	0x08
#define TH_ACK_MULTIPLIER	0x10
#define TH_URG_MULTIPLIER	0x20
