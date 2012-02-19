/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l3Proto;

import Utils.Conversions;

/**
 *
 * @author nitish
 */
public class Iphdr extends L3proto
{
     public byte ihl;
     public byte version;
     public byte tos;
     public short tot_len;//check
     public short id;//check
     public short frag_off;//check
     public byte ttl;
     public byte protocol;
     public short check;//check
     public int saddr;
     public int daddr;  
     
    @Override
     public String getPacket()
     {
        String ip_pkt_str=new String();
        ip_pkt_str="<b>IP HEADER</b>"+"<br>"+"<b>ihl&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+ihl+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>version&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+version+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>tos&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+tos+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>tot_len&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(tot_len)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>id&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(id)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>frag_off&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+frag_off+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>ttl&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+ttl+"<b>protocol&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+protocol+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>check&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(check)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>saddr&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+(saddr & 0xFF) + "." + ((saddr>> 8) & 0xFF) + "." + ((saddr>> 16) & 0xFF) +"."+ ((saddr>> 24) & 0xFF)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>daddr&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+(daddr & 0xFF) + "." + ((daddr>> 8) & 0xFF) + "." + ((daddr>> 16) & 0xFF) +"."+ ((daddr>> 24) & 0xFF)+"<br/>"+"<br/>";
        return(ip_pkt_str);
     }
}
