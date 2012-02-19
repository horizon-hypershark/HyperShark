/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Beans;

import Controller.FlowController;
import Core.CaptureRule;
import Utils.Conversions;

/**
 *
 * @author varun
 */
public class CaptureTimeFilterBean {
    private String srcMac;
    private String dstMac;
    private String srcOctet1,srcOctet2,srcOctet3,srcOctet4;
    private String dstOctet1,dstOctet2,dstOctet3,dstOctet4;
    private String srcMaskOctet1,srcMaskOctet2,srcMaskOctet3,srcMaskOctet4;
    private String dstMaskOctet1,dstMaskOctet2,dstMaskOctet3,dstMaskOctet4;
    private String lowSrcPort,highSrcPort;
    private String lowDestPort,highDestPort;
    private String protocol;

    public void recordCaptureFilter(UserDataBean userData)
    {
        FlowController controller=new FlowController();
        controller.recordCaptureFilter(userData, userData.getUserDetails().getVirMachineList().size()-1);
    }
    public void createCaptureRule(UserDataBean userData,int index)
    {
        CaptureRule captRule=new CaptureRule();
        if(srcMac!=null)        
            captRule.setSmac(Conversions.stringToMac(srcMac));
        if(dstMac!=null)        
            captRule.setDmac(Conversions.stringToMac(dstMac));
        
        if(!srcOctet1.equals("Select"))
            captRule.setSrcHost(Conversions.stringToIp(srcOctet1, srcOctet2, srcOctet3, srcOctet4));
        
        if(!dstOctet1.equals("Select"))            
            captRule.setDstHost(Conversions.stringToIp(dstOctet1, dstOctet2, dstOctet3, dstOctet4));
        if(!srcMaskOctet1.equals("Select"))            
            captRule.setSrcHostMask(Conversions.stringToIp(srcMaskOctet1, srcMaskOctet2, srcMaskOctet3, srcMaskOctet4));
        if(!dstMaskOctet1.equals("Select"))
            captRule.setDstHostMask(Conversions.stringToIp(dstMaskOctet1, dstMaskOctet2, dstMaskOctet3, dstMaskOctet4));
        if(highSrcPort!=null)
           captRule.setHighSrcPort(Integer.parseInt(highSrcPort));
        else
            captRule.setHighSrcPort(-1);
        
        if(lowSrcPort!=null)
            captRule.setLowSrcPort(Integer.parseInt(lowSrcPort));
        else
            captRule.setLowSrcPort(-1);
        
        if(lowDestPort!=null)
            captRule.setLowDstPort(Integer.parseInt(lowDestPort));
        else
            captRule.setLowDstPort(-1);
                    
        if(highDestPort!=null)
            captRule.setHighDstPort(Integer.parseInt(highDestPort));
        else
            captRule.setHighDstPort(-1);
            
        if(!protocol.equals("Select"))
            captRule.setProtocol(Conversions.stringToProtocol(protocol));
        else
            captRule.setProtocol((short)-1);
        
        userData.getUserDetails().getVirMachineList().get(index).getCaptureRules().add(captRule);
    }
    
    public String getDstMaskOctet1() {
        return dstMaskOctet1;
    }

    public void setDstMaskOctet1(String dstMaskOctet1) {
        this.dstMaskOctet1 = dstMaskOctet1;
    }

    public String getDstMaskOctet2() {
        return dstMaskOctet2;
    }

    public void setDstMaskOctet2(String dstMaskOctet2) {
        this.dstMaskOctet2 = dstMaskOctet2;
    }

    public String getDstMaskOctet3() {
        return dstMaskOctet3;
    }

    public void setDstMaskOctet3(String dstMaskOctet3) {
        this.dstMaskOctet3 = dstMaskOctet3;
    }

    public String getDstMaskOctet4() {
        return dstMaskOctet4;
    }

    public void setDstMaskOctet4(String dstMaskOctet4) {
        this.dstMaskOctet4 = dstMaskOctet4;
    }

    public String getSrcMaskOctet1() {
        return srcMaskOctet1;
    }

    public void setSrcMaskOctet1(String srcMaskOctet1) {
        this.srcMaskOctet1 = srcMaskOctet1;
    }

    public String getSrcMaskOctet2() {
        return srcMaskOctet2;
    }

    public void setSrcMaskOctet2(String srcMaskOctet2) {
        this.srcMaskOctet2 = srcMaskOctet2;
    }

    public String getSrcMaskOctet3() {
        return srcMaskOctet3;
    }

    public void setSrcMaskOctet3(String srcMaskOctet3) {
        this.srcMaskOctet3 = srcMaskOctet3;
    }

    public String getSrcMaskOctet4() {
        return srcMaskOctet4;
    }

    public void setSrcMaskOctet4(String srcMaskOctet4) {
        this.srcMaskOctet4 = srcMaskOctet4;
    }
           
    public String getDstMac() {
        return dstMac;
    }

    public void setDstMac(String dstMac) {
        this.dstMac = dstMac;
    }


    public String getDstOctet1() {
        return dstOctet1;
    }

    public void setDstOctet1(String dstOctet1) {
        this.dstOctet1 = dstOctet1;
    }

    public String getDstOctet2() {
        return dstOctet2;
    }

    public void setDstOctet2(String dstOctet2) {
        this.dstOctet2 = dstOctet2;
    }

    public String getDstOctet3() {
        return dstOctet3;
    }

    public void setDstOctet3(String dstOctet3) {
        this.dstOctet3 = dstOctet3;
    }

    public String getDstOctet4() {
        return dstOctet4;
    }

    public void setDstOctet4(String dstOctet4) {
        this.dstOctet4 = dstOctet4;
    }

    public String getSrcMac() {
        return srcMac;
    }

    public void setSrcMac(String srcMac) {
        this.srcMac = srcMac;
    }

    public String getSrcOctet1() {
        return srcOctet1;
    }

    public void setSrcOctet1(String srcOctet1) {
        this.srcOctet1 = srcOctet1;
    }

    public String getSrcOctet2() {
        return srcOctet2;
    }

    public void setSrcOctet2(String srcOctet2) {
        this.srcOctet2 = srcOctet2;
    }

    public String getSrcOctet3() {
        return srcOctet3;
    }

    public void setSrcOctet3(String srcOctet3) {
        this.srcOctet3 = srcOctet3;
    }

    public String getSrcOctet4() {
        return srcOctet4;
    }

    public void setSrcOctet4(String srcOctet4) {
        this.srcOctet4 = srcOctet4;
    }

    public String getHighDestPort() {
        return highDestPort;
    }

    public void setHighDestPort(String highDestPort) {
        this.highDestPort = highDestPort;
    }

    public String getHighSrcPort() {
        return highSrcPort;
    }

    public void setHighSrcPort(String highSrcPort) {
        this.highSrcPort = highSrcPort;
    }

    public String getLowDestPort() {
        return lowDestPort;
    }

    public void setLowDestPort(String lowDestPort) {
        this.lowDestPort = lowDestPort;
    }

    public String getLowSrcPort() {
        return lowSrcPort;
    }

    public void setLowSrcPort(String lowSrcPort) {
        this.lowSrcPort = lowSrcPort;
    }

    public String getProtocol() {
        return protocol;
    }

    public void setProtocol(String protocol) {
        this.protocol = protocol;
    }
    
}
