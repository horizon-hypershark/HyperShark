/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

import Core.CaptureRule;
import Core.VirtualMachine;
import Utils.Hash;
import java.util.ArrayList;

/**
 *
 * @author varun
 */
public class ChangeStatus
{
    VirtualMachine vm=new VirtualMachine();
    public CaptureRule cpt=new CaptureRule();
        
    public native void startCapture();
    public native void stopCapture();
        
    
    public void start(String userId,VirtualMachine virtMach)
    {
        vm=virtMach;     
        vm.setHashVal(Hash.getDirHash(vm.getVmId(), userId));
       /* System.out.println("Initial hash value is:"+vm.getHashVal());
       System.out.println("in java size is"+vm.getCaptureRules().size());
        System.out.println("in java final start low src port is "+vm.getCaptureRules().get(0).getLowSrcPort());
        System.out.println("in java final start high src port is "+vm.getCaptureRules().get(0).getHighSrcPort());
        System.out.println("in java final start src ip is "+vm.getCaptureRules().get(0).getSrcHost()[0]+"."+vm.getCaptureRules().get(0).getSrcHost()[1]+"."+vm.getCaptureRules().get(0).getSrcHost()[2]);
        */
     
        
        
        startCapture();
        System.out.println("Recomputed hash is:"+virtMach.getHashVal());
    }
    
    public void stop(VirtualMachine virtMach)
    {
        vm=virtMach;
        System.out.println("Hash in java is:"+vm.getHashVal());
        stopCapture();
        System.out.println("in java stop flowcount is "+vm.getGlobalFlowCount());
    }
    static
    {
        System.loadLibrary("status");
    }
    public CaptureRule getCaptureRule(int index)
    {
        /*System.out.println("index is"+index);
        System.out.println("in getcapture low src port is"+vm.getCaptureRules().get(0).getLowSrcPort());
        System.out.println("in getcapture high src port is"+vm.getCaptureRules().get(0).getHighSrcPort());
        System.out.println("in getcapture dmac is"+vm.getCaptureRules().get(0).getDmac());
        System.out.println("in getcapture dest host add is"+vm.getCaptureRules().get(0).getDstHost()[0]+"."+vm.getCaptureRules().get(0).getDstHost()[1]+"."+vm.getCaptureRules().get(0).getDstHost()[2]+"."+vm.getCaptureRules().get(0).getDstHost()[3]);
        System.out.println("in getcapture smac is"+vm.getCaptureRules().get(0).getSmac());
        System.out.println("in getcapture src host add is"+vm.getCaptureRules().get(0).getSrcHost()[0]+"."+vm.getCaptureRules().get(0).getSrcHost()[1]+"."+vm.getCaptureRules().get(0).getSrcHost()[2]+"."+vm.getCaptureRules().get(0).getSrcHost()[3]);
        System.out.println("in getcapture low protocol is"+vm.getCaptureRules().get(0).getProtocol());
        */
        
        
        
        
        return vm.getCaptureRules().get(index);
    }

    
    public CaptureRule setCaptureRule()
    {
        CaptureRule cpt=new CaptureRule();
        short shost[]=new short[4];
        short dhost[]=new short[4];
        short smac[]=new short[6];
        short dmac[]=new short[6];

        /*shost[0]=-1;    
        shost[1]=11;
        shost[2]=12;    
        shost[3]=13;
        
        dhost[0]=-1;    
        dhost[1]=21;
        dhost[2]=22;    
        dhost[3]=23;
        
        smac[0]=30;    
        smac[1]=31;
        smac[2]=32;    
        smac[3]=33;
        smac[4]=34;    
        smac[5]=35;
        
        
        dmac[0]=40;    
        dmac[1]=41;
        dmac[2]=42;    
        dmac[3]=43;
        dmac[4]=44;    
        dmac[5]=45;*/
        cpt.setHighSrcPort(20);
        cpt.setLowSrcPort(10);
        cpt.setProtocol((short)100);
        cpt.setDmac(dmac);
        cpt.setSmac(smac);
        cpt.setSrcHost(shost);
        cpt.setDstHost(dhost);
        
        return cpt;
        }
    public static void main(String args[])
    {
        ChangeStatus cngStat=new ChangeStatus();
        VirtualMachine vm=new VirtualMachine();
        CaptureRule rule_obj=new CaptureRule();
        vm.setDirPath("/storage/hs1234/");
        vm.setVmId("000004d20000092900000d80000011d7");
        vm.setVifs(new ArrayList<String>());
        vm.getVifs().add("wlan0");
        vm.setGlobalFlowCount(0);
        vm.setHashVal(36);
        vm.getCaptureRules().add(cngStat.setCaptureRule());
        //rule_obj=cngStat.getCaptureRule(0);
        
        cngStat.start("1234",vm);
        //cngStat.stop(vm);
        
        //for capturefilter
        //cngStat.cpt.setHighSrcPort(10);
        //cngStat.cpt.setLowSrcPort(5);
        
       // System.out.println("in main flowcount is "+vm.getGlobalFlowCount());
    }
}
