/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l3Proto;

/**
 *
 * @author nitish
 */
public class Udphdr
{
    public short source;
    public short dest;
    public short len;
    public short check;    

    public String get_udp_packet()
     {
        String udp_pkt_str=new String();
        udp_pkt_str="source::"+source+"\t"+"dest::"+dest+"\t"+"len::"+len+"\t"+"check::"+check;  
        return(udp_pkt_str);
     }    
}
