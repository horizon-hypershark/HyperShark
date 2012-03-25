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

   
    public L2proto getL2Packet() {
        return l2Packet;
    }

    public void setL2Packet(L2proto l2Packet) {
        this.l2Packet = l2Packet;
    }

    public L3proto getL3Packet() {
        return l3Packet;
    }

    public void setL3Packet(L3proto l3Packet) {
        this.l3Packet = l3Packet;
    }

    public L4proto getL4Packet() {
        return l4Packet;
    }

    public byte[] getBuffer() {
        return buffer;
    }

    public void setBuffer(byte[] buffer) {
        this.buffer = buffer;
    }

    public void setL4Packet(L4proto l4Packet) {
        this.l4Packet = l4Packet;
    }

    public PfringPktHdr getPfpacket() {
        return pfpacket;
    }

    public void setPfpacket(PfringPktHdr pfpacket) {
        this.pfpacket = pfpacket;
    }
private  L2proto l2Packet;
private L3proto  l3Packet;

    public String getAppProto() {
        return appProto;
    }

    public void setAppProto(String appProto) {
        this.appProto = appProto;
    }
private L4proto l4Packet;
private PfringPktHdr pfpacket;
private String appProto;

private byte buffer[]=new byte[100];
}
