/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l2Proto;

import Utils.BuildString;

/**
 *
 * @author nitish
 */
public class Ethhdr extends L2proto
{
    public byte h_dest[]= new byte[6];//ETH_ALEN value = 6 to be changed
    public byte h_source[]= new byte[6];//ETH_ALEN value = 6 to be changed
    public short h_proto;
    
    @Override
   public String getPacket()
    {
        String eth_pkt_str=new String();
        eth_pkt_str="<b>ETHERNET HEADER</b>"+"<br>"+"<b>Destination mac&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+BuildString.mac_string(h_dest)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>Source mac&nbsp;&nbsp::&nbsp;&nbsp;</b>"+BuildString.mac_string(h_source)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>Protocol&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+h_proto+"<br/>"+"<br/>";        
        //eth_pkt_str=eth_pkt_str+;
        return(eth_pkt_str);
    }
}