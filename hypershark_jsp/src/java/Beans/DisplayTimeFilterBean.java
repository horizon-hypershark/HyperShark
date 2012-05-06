/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Beans;

import Controller.FlowController;
import Core.CaptureTime;
import Core.DisplayPktRule;
import Core.FlowRecord;
import Utils.Conversions;
import java.util.ArrayList;

/**
 *
 * @author varun
 */
        
public class DisplayTimeFilterBean {

    public enum fieldToSort 
        {
            recNo,sourcePort,destPort,sourceIp,destIp,protocol,numPackets;
        }

    private String date;
    private String startTime, endTime;
    private String srcOctet1, srcOctet2, srcOctet3, srcOctet4;
    private String sortingField;  
    
    public ArrayList<FlowRecord> queryPackets(DisplayPktRule dispRule,CaptureTime timRule,String path)
    {
        FlowController contr=new FlowController();
        return contr.queryPackets(dispRule, timRule,path);
    }

    public String getSortingField() {
        return sortingField;
    }

    public void setSortingField(String sortingField) {
        this.sortingField = sortingField;
    }
    
    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
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

    public String getEndTime() {
        return endTime;
    }

    public void setEndTime(String endTime) {
        this.endTime = endTime;
    }

    public String getHighPort() {
        return highPort;
    }

    public void setHighPort(String highPort) {
        this.highPort = highPort;
    }

    public String getLowPort() {
        return lowPort;
    }

    public void setLowPort(String lowPort) {
        this.lowPort = lowPort;
    }

    public String getProtocol() {
        return protocol;
    }

    public void setProtocol(String protocol) {
        this.protocol = protocol;
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

    public String getStartTime() {
        return startTime;
    }

    public void setStartTime(String startTime) {
        this.startTime = startTime;
    }
    private String dstOctet1, dstOctet2, dstOctet3, dstOctet4;
    private String lowPort, highPort;
    private String protocol;

    public void swap(FlowRecord recArr[],int i,int j)
    {
        FlowRecord temp;
        temp=recArr[i];
        recArr[i]=recArr[j];
        recArr[j]=temp;
        
    }
    
    public void sortFlowRecord(FlowRecord recArr[])
    {
        
        for(int i=0;i<(recArr.length-1);i++)
        {
                for(int j=i+1;j<recArr.length;j++)
                {
             
                    if(sortingField.equals("sourcePort"))
                    {
                        if(Conversions.shortToUnsigned(recArr[i].src_port)>Conversions.shortToUnsigned(recArr[j].src_port))
                            swap(recArr,i,j);
                    }
                    else if(sortingField.equals("destPort"))
                    {
                        if(Conversions.shortToUnsigned(recArr[i].dst_port)>Conversions.shortToUnsigned(recArr[j].dst_port))
                            swap(recArr,i,j);
                    }
                    else if(sortingField.equals("sourceIp"))
                    {
                        if((Conversions.intToUnsigned(recArr[i].ip_src))>(Conversions.intToUnsigned(recArr[j].ip_src)))
                            swap(recArr,i,j);
                    }
                    else if(sortingField.equals("protocol"))
                    {
                        if(recArr[i].getProtocol()>recArr[j].getProtocol())
                            swap(recArr,i,j);
                    }
                    else if(sortingField.equals("destIp"))
                    {
                        if((Conversions.intToUnsigned(recArr[i].getIp_dst()))>(Conversions.intToUnsigned(recArr[j].getIp_dst())))
                            swap(recArr,i,j);
                    }
                    else if(sortingField.equals("numPackets"))
                    {
                        if((Conversions.intToUnsigned(recArr[i].getNop()))>(Conversions.intToUnsigned(recArr[j].getNop())))
                            swap(recArr,i,j);
                    }
                }
        }        
    }

    public ArrayList<FlowRecord> getFlowRec(DisplayPktRule dispRule,CaptureTime timRule)
    {
        
        return null;
    }        
    public void createDisplayRule(DisplayPktRule dispRule,CaptureTime timRule) { 
        String octet[]=new String[4];
        octet[0]=srcOctet1;
        octet[1]=srcOctet2;
        octet[2]=srcOctet3;
        octet[3]=srcOctet4;         
        dispRule.setSrcHost(Conversions.incompleteStringToIp(octet,"Select"));

        octet[0]=dstOctet1;
        octet[1]=dstOctet2;
        octet[2]=dstOctet3;
        octet[3]=dstOctet4;         
        dispRule.setDstHost(Conversions.incompleteStringToIp(octet,"Select"));
        
        if (highPort != null) {
            dispRule.setHighPort(Integer.parseInt(highPort));
        } else {
            dispRule.setHighPort(-1);
        }

        if (lowPort != null) {
            dispRule.setLowPort(Integer.parseInt(lowPort));
        } else {
            dispRule.setLowPort(-1);
        }

        if (!protocol.equals("Select")) {
            dispRule.setProtocol(Conversions.stringToProtocol(protocol));
        } else {
            dispRule.setProtocol((short) -1);
        }
        //display time object
        if (date != null) {
            String day = date.substring(3, 5);
            timRule.setDay(Integer.parseInt(day));
            String month = date.substring(0, 2);
            timRule.setMonth(Integer.parseInt(month)-1);
           //timRule.setMonth(Conversions.stringToMonth(month));
        } else {
            timRule.setDay(-1);
            timRule.setMonth(-1);
        }



        if (startTime != null) {
            String hr = startTime.substring(0, 2);
            timRule.setStart_hr(Integer.parseInt(hr));

            String min = startTime.substring(3, 5);
            timRule.setStart_min(Integer.parseInt(min));
        } else {
            timRule.setStart_hr(-1);
            timRule.setStart_min(-1);
        }

        if (endTime != null) {
            String hr = endTime.substring(0, 2);
            timRule.setEnd_hr(Integer.parseInt(hr));

            String min = endTime.substring(3, 5);
            timRule.setEnd_min(Integer.parseInt(min));

        } else {
            timRule.setEnd_hr(-1);
            timRule.setEnd_min(-1);
        }
    }
}
