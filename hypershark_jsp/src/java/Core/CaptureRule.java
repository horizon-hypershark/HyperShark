/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

/**
 *
 * @author varun
 */
public class CaptureRule {
    public short smac[];
    public short dmac[];
    public short srcHost[];//src ip address
    public short dstHost[];//dest ip address
    public short srcHostMask[];
    public short dstHostMask[];
    public int lowSrcPort;
    public int highSrcPort;
    public int lowDstPort;
    public int highDstPort;
    public short protocol;

   
    public short[] getDmac() {
        return dmac;
    }

    public void setDmac(short[] dmac) {
        this.dmac = dmac;
    }

    public short[] getDstHost() {
        return dstHost;
    }

    public void setDstHost(short[] dstHost) {
        this.dstHost = dstHost;
    }

    public short[] getDstHostMask() {
        return dstHostMask;
    }

    public void setDstHostMask(short[] dstHostMask) {
        this.dstHostMask = dstHostMask;
    }

    public int getHighDstPort() {
        return highDstPort;
    }

    public void setHighDstPort(int highDstPort) {
        this.highDstPort = highDstPort;
    }

    public int getHighSrcPort() {
        return highSrcPort;
    }

    public void setHighSrcPort(int highSrcPort) {
        this.highSrcPort = highSrcPort;
    }

    public int getLowDstPort() {
        return lowDstPort;
    }

    public void setLowDstPort(int lowDstPort) {
        this.lowDstPort = lowDstPort;
    }

    public int getLowSrcPort() {
        return lowSrcPort;
    }

    public void setLowSrcPort(int lowSrcPort) {
        this.lowSrcPort = lowSrcPort;
    }

    public short getProtocol() {
        return protocol;
    }

    public void setProtocol(short protocol) {
        this.protocol = protocol;
    }

    public short[] getSmac() {
        return smac;
    }

    public void setSmac(short[] smac) {
        this.smac = smac;
    }

    public short[] getSrcHost() {
        return srcHost;
    }

    public void setSrcHost(short[] srcHost) {
        this.srcHost = srcHost;
    }

    public short[] getSrcHostMask() {
        return srcHostMask;
    }

    public void setSrcHostMask(short[] srcHostMask) {
        this.srcHostMask = srcHostMask;
    }
    
    
}
