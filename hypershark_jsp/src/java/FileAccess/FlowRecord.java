/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

/**
 *
 * @author varun
 */
public class FlowRecord
{
    public int flow_id; 
    public short src_port;
    public short dst_port;
    public int ip_src;
    public int ip_dst;
    public byte protocol;
    public long timestamp_ns;
    public int nop;
    public int start_pkt_no;
    public int nxtfr;    
}
