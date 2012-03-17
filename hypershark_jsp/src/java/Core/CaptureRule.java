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
    private int ruleId;
    public short smac[];
    public short dmac[];
    public short srcHost[];//src ip address
    public short dstHost[];//dest ip address
    public int lowSrcPort;
    public int highSrcPort;
    public short protocol;

    public int getRuleId() {
        return ruleId;
    }

    public void setRuleId(int ruleId) {
        this.ruleId = ruleId;
    }

   
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
 
    
    public int getHighSrcPort() {
        return highSrcPort;
    }

    public void setHighSrcPort(int highSrcPort) {
        this.highSrcPort = highSrcPort;
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

}
