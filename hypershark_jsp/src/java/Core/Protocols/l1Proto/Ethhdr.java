/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l1Proto;

import Utils.BuildString;

/**
 *
 * @author nitish
 */
public class Ethhdr
{
    public byte h_dest[]= new byte[6];//ETH_ALEN value = 6 to be changed
    public byte h_source[]= new byte[6];//ETH_ALEN value = 6 to be changed
    public short h_proto;
    
   public String get_eth_packet()
    {
        String eth_pkt_str=new String();
        eth_pkt_str="<td>Destination mac::"+BuildString.mac_string(h_dest)+"</td>"+"<td>Source mac::"+BuildString.mac_string(h_source)+"</td>"+"<td>Protocol::"+h_proto+"</td>";
        eth_pkt_str=eth_pkt_str+"\n";
        return(eth_pkt_str);
    }
}