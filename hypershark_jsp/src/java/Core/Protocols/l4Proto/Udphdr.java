/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l4Proto;

/**
 *
 * @author nitish
 */
public class Udphdr extends L4proto
{
    public static final short UDP = 17;
    public short source;
    public short dest;
    public short len;
    public short check;    

    @Override
    public String getPacket()
     {
        String udp_pkt_str=new String();
        udp_pkt_str="UDP HEADER"+"<br>"+"<br>"+"<b>source&nbsp;&nbsp;::&nbsp;&nbsp;"+source+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>dest&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+dest+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>len&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+len+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>check&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+check;  
        return(udp_pkt_str);
     }    
}
