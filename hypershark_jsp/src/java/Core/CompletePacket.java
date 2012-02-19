/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;
import Core.Protocols.l2Proto.L2proto;
import Core.Protocols.l3Proto.L3proto;
import Core.Protocols.l4Proto.L4proto;
/**
 *
 * @author nitish
 */
public class CompletePacket
{
public L2proto l2Packet;
public L3proto  l3Packet;
public L4proto l4Packet;
public PfringPktHdr pfpacket;
}
