/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l2Proto;

/**
 *
 * @author nitish
 */
public class Iphdr
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
     
     public String get_ip_packet()
     {
        String ip_pkt_str=new String();
        ip_pkt_str="ihl::"+ihl+"\t"+"version::"+version+"\t"+"tos::"+tos+"\t"+"tot_len::"+tot_len+"\t"+"id::"+id+"\t"+"frag_off::"+frag_off+"\t"+"ttl::"+ttl+"\n"+"protocol::"+protocol+"\t"+"check::"+check+"\t"+"saddr::"+(saddr & 0xFF) + "." + ((saddr>> 8) & 0xFF) + "." + ((saddr>> 16) & 0xFF) +"."+ ((saddr>> 24) & 0xFF)+"\t"+"daddr::"+(daddr & 0xFF) + "." + ((daddr>> 8) & 0xFF) + "." + ((daddr>> 16) & 0xFF) +"."+ ((daddr>> 24) & 0xFF);
        return(ip_pkt_str);
     }
}
