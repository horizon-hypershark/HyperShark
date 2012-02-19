/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

/**
 *
 * @author varun
 */
public class DisplayPktRule 
{

    public short[] getDstHost() {
        return dstHost;
    }

    public void setDstHost(short[] dstHost) {
        this.dstHost = dstHost;
    }

    public int getHighPort() {
        return highPort;
    }

    public void setHighPort(int highPort) {
        this.highPort = highPort;
    }

    public int getLowPort() {
        return lowPort;
    }

    public void setLowPort(int lowPort) {
        this.lowPort = lowPort;
    }

    public short getProtocol() {
        return protocol;
    }

    public void setProtocol(short protocol) {
        this.protocol = protocol;
    }

    public short[] getSrcHost() {
        return srcHost;
    }

    public void setSrcHost(short[] srcHost) {
        this.srcHost = srcHost;
    }
    public short srcHost[];//src ip address
    public short dstHost[];//dest ip address
    public int lowPort;
    public int highPort;
    public short protocol;
    
}
