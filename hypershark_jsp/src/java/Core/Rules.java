/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

/**
 *
 * @author nitish
 */
public class Rules
{
    public int sip_oct[]=new int[4];
    public int dip_oct[]=new int[4];

    public int[] getDip_oct() {
        return dip_oct;
    }

    public void setDip_oct(int[] dip_oct) {
        this.dip_oct = dip_oct;
    }

    public long getDst_port() {
        return dst_port;
    }

    public void setDst_port(long dst_port) {
        this.dst_port = dst_port;
    }

    public int getProtocol() {
        return protocol;
    }

    public void setProtocol(int protocol) {
        this.protocol = protocol;
    }

    public int[] getSip_oct() {
        return sip_oct;
    }

    public void setSip_oct(int[] sip_oct) {
        this.sip_oct = sip_oct;
    }

    public long getSrc_port() {
        return src_port;
    }

    public void setSrc_port(long src_port) {
        this.src_port = src_port;
    }
    public long src_port;
    public long dst_port;
    public int protocol;
    
    /*public byte srcOctet1[]=new byte[30];
    public byte srcOctet2[]=new byte[100];
    public byte srcOctet3[]=new byte[100];
    public byte srcOctet4[]=new byte[100];
    public byte dstOctet1[]=new byte[100];
    public byte dstOctet2[]=new byte[100];
    public byte dstOctet3[]=new byte[100];
    public byte dstOctet4[]=new byte[100];
    public short srcPort[]=new short[100];
    public short dstPort[]=new short[100];
    public byte protocol[]=new byte[100];
    //time startTime;
    //time endTime;*/
    
}
