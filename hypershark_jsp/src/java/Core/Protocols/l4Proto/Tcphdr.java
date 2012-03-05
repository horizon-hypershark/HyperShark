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
public class Tcphdr extends L4proto
{
    public static final short TCP = 6;
    public short source;
    public short dest;
    public int seq;
    public int ack_seq;
    public short flags;//tcp flags: urgent,syn,fin    each field defined in terms of bits
    public short window;
    public short check;
    public short urg_ptr;
    
    @Override
    public String getPacket()
     {
        String tcp_pkt_str=new String();
        tcp_pkt_str="<b>TCP HEADER</b>"+"<br>"+"<b>SOURCE&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(source)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>DEST&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(dest)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>SEQ&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.intToUnsigned(seq)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>ACK_SEQ&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.intToUnsigned(ack_seq)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>FLAGS&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+flags+"<br/>"+"<b>WINDOW&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(window)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>CHECK&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(check)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>URG_PTR&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+urg_ptr+"<br/>"+"<br/>";  
        return(tcp_pkt_str);
     }
}
