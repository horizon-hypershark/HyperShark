/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l4Proto;

import Utils.Conversions;

/**
 *
 * @author root
 */
public class Igmphdr extends L4proto
{
    public static final short IGMP = 2;
    byte type;
    byte code;		/* For newer IGMP */
    short csum;
    int group;    

        @Override
     public String getPacket()
     {
        String igmp_pkt_str=new String();
        igmp_pkt_str="<b>IGMP HEADER</b>"+"<br>"+"<b>type&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+type+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>code&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+code+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>csum&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(csum)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>group&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.intToUnsigned(group)+"<br/>";  
        return(igmp_pkt_str);
     }
}
