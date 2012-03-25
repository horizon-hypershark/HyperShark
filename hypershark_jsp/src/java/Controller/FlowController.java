/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Controller;

import Adapter.*;
import Beans.UserDataBean;
import Core.CaptureTime;
import Core.CompletePacket;
import Core.DisplayPktRule;
import Core.FlowRecord;
import Core.Graph.Statistics;
import Core.Monitoring;
import Core.VirtualMachine;
import GeoIP.GeoInfoLookup;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author varun
 */
public class FlowController {
    public ArrayList<FlowRecord> queryPackets(DisplayPktRule dispRule,CaptureTime timRule,String path)
    {
        String []temp = null;
        temp = path.split("/");      
        path="/"+temp[temp.length-2]+"/"+temp[temp.length-1]+"/";    
        //System.out.println("resultant string is"+path);        
        GetData getPkts=new GetData();
        ArrayList<FlowRecord> flowList = getPkts.queryPackets(dispRule, timRule,path);
        
        for(FlowRecord flowRec : flowList)
        {
            PortMapping portMap= new PortMapping();
            ProtoMapping protocolMap = new ProtoMapping();
            flowRec.setSrc_port_str(portMap.getPortByNumber(flowRec.getSrc_port()));
            flowRec.setDst_port_str(portMap.getPortByNumber(flowRec.getDst_port()));
            flowRec.setProtocol_str(protocolMap.getProtoByNumber(flowRec.getProtocol()));
            
            for(CompletePacket pkts :flowRec.packets)
            {
                pkts.getPfpacket().setL4_src_port_str(portMap.getPortByNumber(pkts.getPfpacket().getL4_src_port()));
                pkts.getPfpacket().setL4_dst_port_str(portMap.getPortByNumber(pkts.getPfpacket().getL4_dst_port()));
                pkts.getPfpacket().setL3_proto_str(protocolMap.getProtoByNumber(pkts.getPfpacket().getL3_proto()));
                
            }
        }    
        return flowList;
    }
    public boolean getUserData(UserDataBean userData)
    {
        Authentication authUser = new Authentication();
        if(authUser.authenticateUser(userData.getUsername(),userData.getPassword()))
        {
            Monitoring.mapVmToVif();            
            VmDetails vmDetails=new VmDetails();
            ArrayList<VirtualMachine> updatedVmList=vmDetails.getUpdatedVmDetails();
            for(VirtualMachine vm :updatedVmList)
            {
                vmDetails.updateVmState(vm.getState(), vm.getVmId());
                vmDetails.updateVmVifs(vm);
            }   
            UserDetails usrDetails=new UserDetails();
            userData.setUserDetails(usrDetails.getUserDetails(userData.getUsername()));
            userData.getUserDetails().setVirMachineList(new VmDetails().getVmDetails(userData.getUserDetails().getUserId()));
            for(VirtualMachine vm : userData.getUserDetails().getVirMachineList())
            {
                vm.setVifs(new VmDetails().getVifs(vm.getVmId()));
            }
            for(VirtualMachine vm : userData.getUserDetails().getVirMachineList())
            {
                vm.setCaptureRules(new CaptureFilter().retrieveCaptureFilter(vm.getVmId()));
            }
            
            return true;
        }
        return false;
    }
    public ArrayList<Statistics> getGraphData(String type,int no,String ordinateLabel,String abscissaLabel) 
    {
        ArrayList<Statistics> stringStat=new ArrayList<Statistics>();        
        if(type.equals("srcIp"))
        {            
            Graph graph=new Graph();
            for(Statistics stat : graph.getTopSrcIps(no))
            {
                String info="";
                Statistics temp=new Statistics();
                temp.setOrdinate(Integer.toString((Integer)stat.getOrdinate()));
                int abscissa=(Integer)stat.getAbscissa();
                String strAbscissa=(((abscissa>>24) & 0xFF) + "." + ((abscissa >> 16) & 0xFF) + "." + ((abscissa>> 8) & 0xFF) +"."+ (abscissa & 0xFF));
                temp.setAbscissa(strAbscissa);
                String country=GeoInfoLookup.getCity(strAbscissa).countryName;
                String city=GeoInfoLookup.getCity(strAbscissa).city;
                //change
                info="<font size='1px'>"+"IP : "+temp.getAbscissa()+"<br/>";
                
                //change ends
                if(country!=null)
                    info+="Country:"+country;
                if(city!=null)
                    info+="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"City:"+city;
                info+="<br/></font>";
                temp.setInfo(info);
                
                stringStat.add(temp);
            }                
            ordinateLabel="#Packets";
            abscissaLabel="Source IP";
        }    
        else if(type.equals("srcPort"))
        {
            Graph graph=new Graph();
            PortMapping map=new PortMapping();
            for(Statistics stat : graph.getTopSrcPorts(no))
            {
                Statistics temp=new Statistics();
                temp.setAbscissa(Integer.toString((Integer)stat.getAbscissa()));
                temp.setOrdinate(Integer.toString((Integer)stat.getOrdinate()));
                String portname=map.getPortByNumber((Integer)stat.getAbscissa());
                String info = null;
                
                if(portname!=null)
                {
                    info="<font size='1px'>"+"Port no : "+temp.getAbscissa();
                    info+="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"Port : "+portname;
                    
                }
                temp.setInfo(info);
                stringStat.add(temp);
            }
            
            ordinateLabel="#Packets";
            abscissaLabel="Source Port";
        }    
        return stringStat;
    }        
    public void recordCaptureFilter(UserDataBean userData,int index)
    {
        CaptureFilter captFilter=new CaptureFilter();
        VirtualMachine vm=userData.getUserDetails().getVirMachineList().get(index);
        int vmNo=vm.getCaptureRules().size()-1;
        vm.getCaptureRules().get(vmNo).setRuleId(captFilter.recordCaptureFilter(vm.getCaptureRules().get(vmNo), vm.getVmId()));
         //System.out.println("RuleId is ::"+vm.getCaptureRules().get(vmNo).getRuleId());
    }
    public void changeMonitoringStatus(UserDataBean userData,int index,int packets)
    {
        ChangeVmStatus changStat=new ChangeVmStatus();
        VmDetails vmDet=new VmDetails();
        VirtualMachine vm=userData.getUserDetails().getVirMachineList().get(index);
        vm.setPackets(packets);        
        if(vm.isMonitoringStatus())
        {  //monnitoring going on
            Monitoring.stopDaemon(vm.getDirPath(), userData.getUserDetails().getUserId());
            changStat.stop(vm);            
            vmDet.updateGFL(vm.getGlobalFlowCount(), vm.getVmId());
        }    
        else 
        {    
            Monitoring.startDaemon(vm.getDirPath(), vm.getMemAlloc(), userData.getUserDetails().getUserId());
            changStat.start(userData.getUserDetails().getUserId(), vm);
            vmDet.updateHash(vm.getHashVal(), vm.getVmId());
        }    
        new VmDetails().updateMonitoringStatus(vm.getVmId(), !vm.isMonitoringStatus());
        vm.setMonitoringStatus(!vm.isMonitoringStatus());
    }        
    public void startCapture(UserDataBean userData,int index)
    {
        ChangeVmStatus changStat=new ChangeVmStatus();
        changStat.start(userData.getUserDetails().getUserId(), userData.getUserDetails().getVirMachineList().get(index));
    }  
    public void stopCapture(UserDataBean userData,int index)
    {
        ChangeVmStatus changStat=new ChangeVmStatus();
        changStat.stop(userData.getUserDetails().getVirMachineList().get(index));
    }
}
