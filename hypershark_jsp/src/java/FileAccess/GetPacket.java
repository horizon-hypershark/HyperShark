/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

import Core.CompletePacket;
import Core.func;

/**
 *
 * @author nitish
 */
public class GetPacket
{
CompletePacket packets[] = new CompletePacket[10];
 public native void fillpackets();
    static
    {
	System.loadLibrary("packetlib");
    } 
    public static void main(String args[])
    {
        func f= new func();
        f.fillpackets();
        String str=new String();
        int i,j=0;
        while(j<4)
        {
            System.out.println("\nETHERNET HEADER");
            System.out.println("\nEthernet header in string form");
            str=f.packets[j].l1_eth_packet.get_eth_packet();
            System.out.println(str);
            System.out.println("\nSource mac");
            for (i=0; i<f.packets[j].l1_eth_packet.h_source.length; i++)
                    { 
                          System.out.print(Integer.toString((f.packets[j].l1_eth_packet.h_source[i] & 0xff) + 0x100, 16).substring(1));
                          if(i!= f.packets[j].l1_eth_packet.h_source.length-1)
                          System.out.print("-");
                    }
            System.out.println("\nDest mac");
            for (i=0; i < f.packets[j].l1_eth_packet.h_dest.length; i++)
                    { 
                          System.out.print(Integer.toString((f.packets[j].l1_eth_packet.h_dest[i] & 0xff) + 0x100, 16).substring(1));
                          if(i!= f.packets[j].l1_eth_packet.h_dest.length-1)
                          System.out.print("-");
                    }
            System.out.println("\nProtocol");
            System.out.println(f.packets[j].l1_eth_packet.h_proto);
            System.out.println("\n");
            
        
        
            System.out.println("\nIP HEADER");
            System.out.println("\nIP header in string form");
            str=f.packets[j].l2_ip_packet.get_ip_packet();
            System.out.println(str);
            System.out.println("\n IHL"+f.packets[j].l2_ip_packet.ihl);
            System.out.println("\n version"+f.packets[j].l2_ip_packet.version);
            System.out.println("\n tos"+f.packets[j].l2_ip_packet.tos);
            System.out.println("\n tot_len"+f.packets[j].l2_ip_packet.tot_len);
            System.out.println("\n packet id"+f.packets[j].l2_ip_packet.id);
            System.out.println("\n frag_off"+f.packets[j].l2_ip_packet.frag_off);
            System.out.println("\n ttl"+f.packets[j].l2_ip_packet.ttl);
            System.out.println("\n protocol"+f.packets[j].l2_ip_packet.protocol);
            System.out.println("\n check"+f.packets[j].l2_ip_packet.check);
            System.out.println("\n source address"+((f.packets[j].l2_ip_packet.saddr) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.saddr>> 8) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.saddr>> 16) & 0xFF) +"."+ ((f.packets[j].l2_ip_packet.saddr>> 24) & 0xFF));
            System.out.println("\n dest addr"+((f.packets[j].l2_ip_packet.daddr) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.daddr>> 8) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.daddr>> 16) & 0xFF) +"."+ ((f.packets[j].l2_ip_packet.daddr>> 24) & 0xFF));
                    
            System.out.println("\n TCP HEADER");
            System.out.println("\nTCP header in string form");
            str=f.packets[j].l3_tcp_packet.get_tcp_packet();
            System.out.println(str);
            
            System.out.println("\n source"+f.packets[j].l3_tcp_packet.source);
            System.out.println("\n dest"+f.packets[j].l3_tcp_packet.dest);
            System.out.println("\n seq"+f.packets[j].l3_tcp_packet.seq);
            System.out.println("\n ack_seq"+f.packets[j].l3_tcp_packet.ack_seq);
            System.out.println("\n flags"+f.packets[j].l3_tcp_packet.flags);
            System.out.println("\n window"+f.packets[j].l3_tcp_packet.window);
            System.out.println("\n check"+f.packets[j].l3_tcp_packet.check);
            System.out.println("\n urg_pointer"+f.packets[j].l3_tcp_packet.urg_ptr);
            
            
            System.out.println("\nUDP HEADER");
            System.out.println("\nUDP header in string form");
            str=f.packets[j].l3_udp_packet.get_udp_packet();
            System.out.println(str);
            System.out.println("\n source"+f.packets[j].l3_udp_packet.source);
            System.out.println("\n dest"+f.packets[j].l3_udp_packet.dest);
            System.out.println("\n len"+f.packets[j].l3_udp_packet.len);
            System.out.println("\n check"+f.packets[j].l3_udp_packet.check);
            System.out.println("\nPFRING HEADER");
            System.out.println("packet_id"+f.packets[j].pfpacket.pkt_id);
            System.out.print("\nSMAC = ");

            for (i=0;i<f.packets[j].pfpacket.smac.length; i++)
            { 
                System.out.print(Integer.toString((f.packets[j].pfpacket.smac[i] & 0xff) + 0x100, 16).substring(1));
                if(i!=f.packets[j].pfpacket.smac.length-1)
                System.out.print("-");
            }
            System.out.print("\nDMAC = ");
            for (i=0;i<f.packets[j].pfpacket.dmac.length; i++)
            { 
                System.out.print(Integer.toString((f.packets[j].pfpacket.dmac[i] & 0xff) + 0x100, 16).substring(1));
                if(i!=f.packets[j].pfpacket.dmac.length-1)
                System.out.print("-");
            }	
            System.out.print("\nSRC IP= ");
            System.out.println(((f.packets[j].pfpacket.ip_src) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_src >> 8) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_src >> 16) &0xFF)+"."+((f.packets[j].pfpacket.ip_src >> 24) & 0xFF));
            System.out.print("DST IP= ");
            System.out.println(((f.packets[j].pfpacket.ip_dst) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_dst >> 8) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_dst >> 16) &0xFF)+"."+ ((f.packets[j].pfpacket.ip_dst >> 24) & 0xFF));
            //rest of the fields 			
            System.out.println(f.packets[j].pfpacket.caplen);
            System.out.println(f.packets[j].pfpacket.len);
            System.out.println(f.packets[j].pfpacket.timestamp_ns);
            System.out.println(f.packets[j].pfpacket.rx_direction);
            System.out.println(f.packets[j].pfpacket.if_index);
            System.out.println(f.packets[j].pfpacket.pkt_hash);
            System.out.println(f.packets[j].pfpacket.parsed_header_len);
            System.out.println(f.packets[j].pfpacket.eth_type);
            System.out.println(f.packets[j].pfpacket.vlan_id);
            System.out.println(f.packets[j].pfpacket.ip_version);
            System.out.println(f.packets[j].pfpacket.l3_proto);
            System.out.println(f.packets[j].pfpacket.ip_tos);
            System.out.println(f.packets[j].pfpacket.l4_src_port);
            System.out.println(f.packets[j].pfpacket.l4_dst_port);
            //System.out.println(f.packets.pfpacket.flags);
            System.out.println(f.packets[j].pfpacket.seq_num);
            System.out.println(f.packets[j].pfpacket.ack_num);
            System.out.println(f.packets[j].pfpacket.last_matched_plugin_id);
            System.out.println(f.packets[j].pfpacket.last_matched_rule_id);
            System.out.println(f.packets[j].pfpacket.eth_offset);
            System.out.println(f.packets[j].pfpacket.vlan_offset);
            System.out.println(f.packets[j].pfpacket.l3_offset);
            System.out.println(f.packets[j].pfpacket.l4_offset);
            System.out.println(f.packets[j].pfpacket.payload_offset);
            System.out.println(f.packets[j].pfpacket.in_iface);
            System.out.println(f.packets[j].pfpacket.out_iface);
            System.out.println(f.packets[j].pfpacket.samplingPopulation);
            System.out.println(f.packets[j].pfpacket.flow_sequence);
            System.out.println(f.packets[j].pfpacket.buffer);
            System.out.println(f.packets[j].pfpacket.buffer_len);
            j++;
        }
    }    
}
