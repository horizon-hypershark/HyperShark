/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l3Proto;

import Utils.Conversions;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.logging.Level;
import java.util.logging.Logger;

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

    @Override
    public byte getProtocol() {
        return protocol;
    }

    public void setProtocol(byte protocol) {
        this.protocol = protocol;
    }
     public int saddr;
     public int daddr;  
     
    @Override
     public String getPacket()
     {
         String str = new Integer(saddr).toString();
         String name= new String();
         InetAddress ia = null;
            //name=ia.getHostName();
                
           String ip_pkt_str=new String();
           
           /*ip_pkt_str="<b>IP HEADER</b>"+"<table width=100%><tr width=100% align=\"left\"><td width=10%>"+"<b>IHL&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+ihl+"</td><td width=10%>"+"<b>VERSION&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+version+"</td><td width=10%>"+"<b>TOS&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+tos+"</td><td width=10%>"+
           "<b>TOT_LEN&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(tot_len)+"</td><td width=10%></td>"+
           "<td width=10%>"+"<b>ID&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(id)+"</td></tr></table>"+
           "<table width=100%><tr width=100% align=\"left\"><td width=10%>"+"<b>FRAG_OFF&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+frag_off+"</td>+<td width=10%>"+"<b>TTL&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+ttl+"</td>+<td width=10%>"+"<b>PROTOCOL&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+protocol+"</td><td width=10%>"+"<b>CHECK&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+check+"</td><td width=10%>"+"<b>SADDR&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+(saddr & 0xFF) + "." + ((saddr>> 8) & 0xFF) + "." + ((saddr>> 16) & 0xFF) +"."+ ((saddr>> 24) & 0xFF)+"</td></tr></table>";
            */
           ip_pkt_str="<b>IP HEADER</b>"+"<br>"+"<b>IHL&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+ihl+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>VERSION&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+version+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>TOS&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+tos+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>TOT_LEN&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(tot_len)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>ID&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(id)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>FRAG_OFF&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+frag_off+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<br/>"+"<b>TTL&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+ttl+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>PROTOCOL&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+protocol+"&nbsp;&nbsp;"+"<b>CHECK&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(check)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>SADDR&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+(saddr & 0xFF) + "." + ((saddr>> 8) & 0xFF) + "." + ((saddr>> 16) & 0xFF) +"."+ ((saddr>> 24) & 0xFF)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>DADDR&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+(daddr & 0xFF) + "." + ((daddr>> 8) & 0xFF) + "." + ((daddr>> 16) & 0xFF) +"."+ ((daddr>> 24) & 0xFF)+"<br/>"+"<br/>";
             
            return(ip_pkt_str);
     }
}
