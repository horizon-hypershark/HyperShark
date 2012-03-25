/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

import java.util.ArrayList;

/**
 *
 * @author varun
 */
public class FlowRecord
{
    public ArrayList<CompletePacket> packets;
    //public CompletePacket packets[]= new CompletePacket[50];
    /*public CaptureTime cpt=new CaptureTime();
    public Rules rules= new Rules();*/
 
    public int flow_id; 
    public short src_port;
    private String src_port_str;
    private String dst_port_str;
    public short dst_port;
    public int ip_src;
    public int ip_dst;
    public byte protocol;
    private String protocol_str;
    public int nop;
    public int start_pkt_no;
    public int nxtfr;    
    public long bytes_transfer;
    public long start_tv_sec;
    public long start_tv_usec;
    public long end_tv_sec;
    public long end_tv_usec;

    public String getProtocol_str() {
        return protocol_str;
    }

    public void setProtocol_str(String protocol_str) {
        this.protocol_str = protocol_str;
    }
    public FlowRecord() {       
        packets=new ArrayList<CompletePacket>();        
    }

    public String getDst_port_str() {
        return dst_port_str;
    }

    public void setDst_port_str(String dst_port_str) {
        this.dst_port_str = dst_port_str;
    }

    public String getSrc_port_str() {
        return src_port_str;
    }

    public void setSrc_port_str(String src_port_str) {
        this.src_port_str = src_port_str;
    }
    
    
    public void addPacket(CompletePacket pkt)
    {
        packets.add(pkt);      
    }

    public long getBytes_transfer() {
        return bytes_transfer;
    }

    public void setBytes_transfer(long bytes_transfer) {
        this.bytes_transfer = bytes_transfer;
    }

    public short getDst_port() {
        return dst_port;
    }

    public void setDst_port(short dst_port) {
        this.dst_port = dst_port;
    }

    public long getEnd_tv_sec() {
        return end_tv_sec;
    }

    public void setEnd_tv_sec(long end_tv_sec) {
        this.end_tv_sec = end_tv_sec;
    }

    public long getEnd_tv_usec() {
        return end_tv_usec;
    }

    public void setEnd_tv_usec(long end_tv_usec) {
        this.end_tv_usec = end_tv_usec;
    }

    public int getFlow_id() {
        return flow_id;
    }

    public void setFlow_id(int flow_id) {
        this.flow_id = flow_id;
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

    public int getNop() {
        return nop;
    }

    public void setNop(int nop) {
        this.nop = nop;
    }

    public int getNxtfr() {
        return nxtfr;
    }

    public void setNxtfr(int nxtfr) {
        this.nxtfr = nxtfr;
    }

    public ArrayList<CompletePacket> getPackets() {
        return packets;
    }

    public void setPackets(ArrayList<CompletePacket> packets) {
        this.packets = packets;
    }

    public byte getProtocol() {
        return protocol;
    }

    public void setProtocol(byte protocol) {
        this.protocol = protocol;
    }

    public short getSrc_port() {
        return src_port;
    }

    public void setSrc_port(short src_port) {
        this.src_port = src_port;
    }

    public int getStart_pkt_no() {
        return start_pkt_no;
    }

    public void setStart_pkt_no(int start_pkt_no) {
        this.start_pkt_no = start_pkt_no;
    }

    public long getStart_tv_sec() {
        return start_tv_sec;
    }

    public void setStart_tv_sec(long start_tv_sec) {
        this.start_tv_sec = start_tv_sec;
    }

    public long getStart_tv_usec() {
        return start_tv_usec;
    }

    public void setStart_tv_usec(long start_tv_usec) {
        this.start_tv_usec = start_tv_usec;
    }
    
    
}
