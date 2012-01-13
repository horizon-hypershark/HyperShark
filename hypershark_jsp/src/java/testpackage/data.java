/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package testpackage;

/**
 *
 * @author varun
 */
public class data 
{
    // total 35 variables
	// hs_pkt_hdr
	private int pkt_id; 
	public String buffer;
	public short buffer_len;
	// pfring_pkthdr
	public int caplen; 
	public int len;
	// pfring_extended_pkthdr 
	public long timestamp_ns;
	public short rx_direction;
	public int if_index;
	public int pkt_hash;
	public short parsed_header_len;
	// pkt_parsing_info
	public short eth_type,vlan_id,ip_version,l3_proto,ip_tos;
	public int ip_src,ip_dst;  // ipv4 addresses
	public short l4_src_port,l4_dst_port,flags,last_matched_plugin_id,last_matched_rule_id;
	public int seq_num,ack_num;       
	// pkt_offset
	public short eth_offset,vlan_offset,l3_offset,l4_offset,payload_offset;
	// pkt_flow_info
	public int in_iface,out_iface,samplingPopulation,flow_sequence;
	public byte dmac[]=new byte[6];
	public byte smac[]=new byte[6];
}	    

    
    
