/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

import Utils.BuildString;

/**
 *
 * @author nitish
 */
public class PfringPktHdr
{
    //public int pkt_id;
    public int caplen; 
    public int len;
    public long timestamp_ns;
    public byte rx_direction;
    public int if_index;	
    public int pkt_hash;
    public short parsed_header_len;
    public byte dmac[]=new byte[6];
    public byte smac[]=new byte[6];
    public short eth_type;
    public short vlan_id;
    public byte ip_version;
    public byte l3_proto;
    public byte ip_tos;
    public int ip_src;
    public int ip_dst;
    public short l4_src_port;
    public short l4_dst_port;
    public byte flags;
    public int seq_num;
    public int ack_num;
    public short last_matched_plugin_id;
    public short last_matched_rule_id;
    public short eth_offset;
    public short vlan_offset;
    public short l3_offset;
    public short l4_offset;
    public short payload_offset;
    public int in_iface;
    public int out_iface;
    public int samplingPopulation;
    public int flow_sequence;
    public String buffer = new String();
    //public short buffer_len;
    public int num_pkts;
    public int num_bytes;
    
    //struct timeval 2 members;
    public String getPacket()
    {
        String pfring_pkt_str=new String();
        //pfring_pkt_str="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+BuildString.mac_string(smac)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+BuildString.mac_string(dmac)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+(ip_src & 0xFF) + "." + ((ip_src>> 8) & 0xFF) + "." + ((ip_src>> 16) & 0xFF) +"."+ ((ip_src>> 24) & 0xFF)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+(ip_dst & 0xFF) + "." + ((ip_dst>> 8) & 0xFF) + "." + ((ip_dst>> 16) & 0xFF) +"."+ ((ip_dst>> 24) & 0xFF)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+l3_proto+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+caplen;
        pfring_pkt_str="<table width=100%><tr width=100% align=\"center\"><td width=22%>"+BuildString.mac_string(smac)+"</td><td width=22%>"+BuildString.mac_string(dmac)+"</td><td width=22%>"+((ip_src>>24) & 0xFF) + "." + ((ip_src>> 16) & 0xFF) + "." + ((ip_src>> 8) & 0xFF) +"."+ ((ip_src) & 0xFF)+"</td><td width=22%>"+
                ((ip_dst>>24) & 0xFF) + "." + ((ip_dst>> 16) & 0xFF) + "." + ((ip_dst>> 8) & 0xFF) +"."+ ((ip_dst) & 0xFF)+"</td><td width=6%>"+l3_proto+"</td><td width=6%>"+caplen+"</td></tr></table>";
        //eth_pkt_str=eth_pkt_str+;
        return(pfring_pkt_str);
    }
}
