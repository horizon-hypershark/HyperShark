/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l3Proto;

/**
 *
 * @author nitish
 */
public class Tcphdr
{
    public short source;
    public short dest;
    public int seq;
    public int ack_seq;
    public short flags;//tcp flags: urgent,syn,fin    each field defined in terms of bits
    public short window;
    public short check;
    public short urg_ptr;
    
    public String get_tcp_packet()
     {
        String tcp_pkt_str=new String();
        tcp_pkt_str="source::"+source+"\t"+"dest::"+dest+"\t"+"seq::"+seq+"\t"+"ack_seq::"+ack_seq+"\t"+"flags::"+flags+"\n"+"window::"+window+"\t"+"check::"+check+"\t"+"urg_ptr::"+urg_ptr;  
        return(tcp_pkt_str);
     }
}
