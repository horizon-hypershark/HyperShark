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

    public String getL3_proto_str() {
        return l3_proto_str;
    }

    public void setL3_proto_str(String l3_proto_str) {
        this.l3_proto_str = l3_proto_str;
    }
    public byte ip_version;
    public byte l3_proto;
    private String l3_proto_str;

    public int getAck_num() {
        return ack_num;
    }

    public void setAck_num(int ack_num) {
        this.ack_num = ack_num;
    }

    public String getBuffer() {
        return buffer;
    }

    public void setBuffer(String buffer) {
        this.buffer = buffer;
    }

    public int getCaplen() {
        return caplen;
    }

    public void setCaplen(int caplen) {
        this.caplen = caplen;
    }

    public byte[] getDmac() {
        return dmac;
    }

    public void setDmac(byte[] dmac) {
        this.dmac = dmac;
    }

    public short getEth_offset() {
        return eth_offset;
    }

    public void setEth_offset(short eth_offset) {
        this.eth_offset = eth_offset;
    }

    public short getEth_type() {
        return eth_type;
    }

    public void setEth_type(short eth_type) {
        this.eth_type = eth_type;
    }

    public byte getFlags() {
        return flags;
    }

    public void setFlags(byte flags) {
        this.flags = flags;
    }

    public int getFlow_sequence() {
        return flow_sequence;
    }

    public void setFlow_sequence(int flow_sequence) {
        this.flow_sequence = flow_sequence;
    }

    public int getIf_index() {
        return if_index;
    }

    public void setIf_index(int if_index) {
        this.if_index = if_index;
    }

    public int getIn_iface() {
        return in_iface;
    }

    public void setIn_iface(int in_iface) {
        this.in_iface = in_iface;
    }

    public int getIp_dst() {
        return ip_dst;
    }

    public void setIp_dst(int ip_dst) {
        this.ip_dst = ip_dst;
    }

    public int getIp_src() {
        return ip_src;
    }

    public void setIp_src(int ip_src) {
        this.ip_src = ip_src;
    }

    public byte getIp_tos() {
        return ip_tos;
    }

    public void setIp_tos(byte ip_tos) {
        this.ip_tos = ip_tos;
    }

    public byte getIp_version() {
        return ip_version;
    }

    public void setIp_version(byte ip_version) {
        this.ip_version = ip_version;
    }

    public short getL3_offset() {
        return l3_offset;
    }

    public void setL3_offset(short l3_offset) {
        this.l3_offset = l3_offset;
    }

    public byte getL3_proto() {
        return l3_proto;
    }

    public void setL3_proto(byte l3_proto) {
        this.l3_proto = l3_proto;
    }

    public short getL4_dst_port() {
        return l4_dst_port;
    }

    public void setL4_dst_port(short l4_dst_port) {
        this.l4_dst_port = l4_dst_port;
    }

    public String getL4_dst_port_str() {
        return l4_dst_port_str;
    }

    public void setL4_dst_port_str(String l4_dst_port_str) {
        this.l4_dst_port_str = l4_dst_port_str;
    }

    public short getL4_offset() {
        return l4_offset;
    }

    public void setL4_offset(short l4_offset) {
        this.l4_offset = l4_offset;
    }

    public short getL4_src_port() {
        return l4_src_port;
    }

    public void setL4_src_port(short l4_src_port) {
        this.l4_src_port = l4_src_port;
    }

    public String getL4_src_port_str() {
        return l4_src_port_str;
    }

    public void setL4_src_port_str(String l4_src_port_str) {
        this.l4_src_port_str = l4_src_port_str;
    }

    public short getLast_matched_plugin_id() {
        return last_matched_plugin_id;
    }

    public void setLast_matched_plugin_id(short last_matched_plugin_id) {
        this.last_matched_plugin_id = last_matched_plugin_id;
    }

    public short getLast_matched_rule_id() {
        return last_matched_rule_id;
    }

    public void setLast_matched_rule_id(short last_matched_rule_id) {
        this.last_matched_rule_id = last_matched_rule_id;
    }

    public int getLen() {
        return len;
    }

    public void setLen(int len) {
        this.len = len;
    }

    public int getNum_bytes() {
        return num_bytes;
    }

    public void setNum_bytes(int num_bytes) {
        this.num_bytes = num_bytes;
    }

    public int getNum_pkts() {
        return num_pkts;
    }

    public void setNum_pkts(int num_pkts) {
        this.num_pkts = num_pkts;
    }

    public int getOut_iface() {
        return out_iface;
    }

    public void setOut_iface(int out_iface) {
        this.out_iface = out_iface;
    }

    public short getParsed_header_len() {
        return parsed_header_len;
    }

    public void setParsed_header_len(short parsed_header_len) {
        this.parsed_header_len = parsed_header_len;
    }

    public short getPayload_offset() {
        return payload_offset;
    }

    public void setPayload_offset(short payload_offset) {
        this.payload_offset = payload_offset;
    }

    public int getPkt_hash() {
        return pkt_hash;
    }

    public void setPkt_hash(int pkt_hash) {
        this.pkt_hash = pkt_hash;
    }

    public byte getRx_direction() {
        return rx_direction;
    }

    public void setRx_direction(byte rx_direction) {
        this.rx_direction = rx_direction;
    }

    public int getSamplingPopulation() {
        return samplingPopulation;
    }

    public void setSamplingPopulation(int samplingPopulation) {
        this.samplingPopulation = samplingPopulation;
    }

    public int getSeq_num() {
        return seq_num;
    }

    public void setSeq_num(int seq_num) {
        this.seq_num = seq_num;
    }

    public byte[] getSmac() {
        return smac;
    }

    public void setSmac(byte[] smac) {
        this.smac = smac;
    }

    public long getTimestamp_ns() {
        return timestamp_ns;
    }

    public void setTimestamp_ns(long timestamp_ns) {
        this.timestamp_ns = timestamp_ns;
    }

    public short getVlan_id() {
        return vlan_id;
    }

    public void setVlan_id(short vlan_id) {
        this.vlan_id = vlan_id;
    }

    public short getVlan_offset() {
        return vlan_offset;
    }

    public void setVlan_offset(short vlan_offset) {
        this.vlan_offset = vlan_offset;
    }
    public byte ip_tos;
    public int ip_src;
    public int ip_dst;
    public short l4_src_port;
    private String l4_src_port_str;
    public short l4_dst_port;
    private String l4_dst_port_str;
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
    public String getPacket(String appProto)
    {
        String pfring_pkt_str=new String();
        //pfring_pkt_str="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+BuildString.mac_string(smac)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+BuildString.mac_string(dmac)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+(ip_src & 0xFF) + "." + ((ip_src>> 8) & 0xFF) + "." + ((ip_src>> 16) & 0xFF) +"."+ ((ip_src>> 24) & 0xFF)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+(ip_dst & 0xFF) + "." + ((ip_dst>> 8) & 0xFF) + "." + ((ip_dst>> 16) & 0xFF) +"."+ ((ip_dst>> 24) & 0xFF)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+l3_proto+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+caplen;
        pfring_pkt_str="<table width=100%><tr width=100% align=\"center\"><td width=22%>"+BuildString.mac_string(smac)+"</td><td width=22%>"+BuildString.mac_string(dmac)+"</td><td width=22%>"+((ip_src>>24) & 0xFF) + "." + ((ip_src>> 16) & 0xFF) + "." + ((ip_src>> 8) & 0xFF) +"."+ ((ip_src) & 0xFF)+"</td><td width=22%>"+
                ((ip_dst>>24) & 0xFF) + "." + ((ip_dst>> 16) & 0xFF) + "." + ((ip_dst>> 8) & 0xFF) +"."+ ((ip_dst) & 0xFF)+"</td><td width=6%>";
        //l3_proto+"</td><td width=6%>"+caplen+"</td></tr></table>";
        //eth_pkt_str=eth_pkt_str+;
        
        
        if(appProto!=null)
            pfring_pkt_str+=appProto;
        else if(l3_proto_str!=null)
            pfring_pkt_str+=l3_proto_str;
        else
            pfring_pkt_str+=l3_proto;
       
            pfring_pkt_str+="</td><td width=6%>"+caplen+"</td></tr></table>";
        
        return(pfring_pkt_str);
    }

}
