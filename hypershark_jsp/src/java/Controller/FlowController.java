/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Controller;

import Adapter.*;
import Beans.UserDataBean;
import Core.VirtualMachine;

/**
 *
 * @author varun
 */
public class FlowController {
    public boolean getUserData(UserDataBean userData)
    {
        Authentication authUser = new Authentication();
        if(authUser.authenticateUser(userData.getUsername(),userData.getPassword()))
        {
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
            //System.out.println("In change status Flowcontrollor Stop");
            changStat.stop(vm);            
            vmDet.updateGFL(vm.getGlobalFlowCount(), vm.getVmId());
        }    
        else 
        {    
            //System.out.println("In change status Flowcontrollor Start");
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
