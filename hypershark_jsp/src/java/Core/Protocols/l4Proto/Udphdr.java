/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l4Proto;

import Utils.Conversions;

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
        udp_pkt_str="UDP HEADER"+"<br>"+"<br>"+"<b>source&nbsp;&nbsp;::&nbsp;&nbsp;"+Conversions.shortToUnsigned(source)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>dest&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(dest)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>len&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(len)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>check&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(check);  
        return(udp_pkt_str);
     }    
}
