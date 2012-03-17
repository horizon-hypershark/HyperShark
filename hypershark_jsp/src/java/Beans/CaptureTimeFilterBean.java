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
    private String lowSrcPort,highSrcPort;
    private String protocol;

    public void recordCaptureFilter(UserDataBean userData,int index)
    {
        FlowController controller=new FlowController();
        controller.recordCaptureFilter(userData, index);
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
         if(highSrcPort!=null)
           
             captRule.setHighSrcPort(Integer.parseInt(highSrcPort));
        else
            captRule.setHighSrcPort(-1);
        
        if(lowSrcPort!=null)
            
        captRule.setLowSrcPort(Integer.parseInt(lowSrcPort));
        else
            captRule.setLowSrcPort(-1);
        
        if(!protocol.equals("Select"))
            captRule.setProtocol(Conversions.stringToProtocol(protocol));
        else
            captRule.setProtocol((short)-1);
        
        //verify data in rule
        
        /*System.out.println("capture time filter bean starts");
       System.out.println("high src port is"+captRule.getHighSrcPort());
        System.out.println("low src port is"+captRule.getLowSrcPort());
        System.out.println("low ip address is");
         for(int i=0;i<4;i++)
        {
            System.out.println(captRule.getSrcHost()[i]);
        }
         System.out.println("high ip address is");
         for(int i=0;i<4;i++)
        {
            System.out.println(captRule.getDstHost()[i]);
        }
        
         System.out.println("source mac is");
        for(int i=0;i<6;i++)
        {
            System.out.print(captRule.getSmac()[i]);
        }
        System.out.println("destination mac is");
        for(int i=0;i<6;i++)
        {
            System.out.print(captRule.getDmac()[i]);
        }
        
        System.out.println("protocol is"+captRule.getProtocol());
        System.out.println("capture timefilterbean ends");
        
        */
        
        userData.getUserDetails().getVirMachineList().get(index).getCaptureRules().add(captRule);        
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

    
    public String getHighSrcPort() {
        return highSrcPort;
    }

    public void setHighSrcPort(String highSrcPort) {
        this.highSrcPort = highSrcPort;
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
