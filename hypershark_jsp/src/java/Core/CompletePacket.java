/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;
import Core.Protocols.l1Proto.Ethhdr;
import Core.Protocols.l2Proto.Iphdr;
import Core.Protocols.l3Proto.*;
/**
 *
 * @author nitish
 */
public class CompletePacket
{
public Ethhdr l1_eth_packet= new Ethhdr();
public Iphdr  l2_ip_packet= new Iphdr();
public Tcphdr l3_tcp_packet= new Tcphdr();
public Udphdr l3_udp_packet=new Udphdr();
public Pfring_Pkthdr pfpacket = new Pfring_Pkthdr();
}
