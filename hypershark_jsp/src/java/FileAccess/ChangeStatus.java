/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

import Core.VirtualMachine;
import Utils.Conversions;
import Utils.Hash;
import java.util.ArrayList;

/**
 *
 * @author varun
 */
public class ChangeStatus
{
    VirtualMachine vm=new VirtualMachine();
    public native void startCapture();
    public native void stopCapture();

    public void start(String userId,VirtualMachine virtMach)
    {
        vm=virtMach;
        vm.setHashVal(Hash.getDirHash(vm.getVmId(), userId));
        System.out.println("Initial hash value is:"+vm.getHashVal());
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

    
    public static void main(String args[])
    {
        ChangeStatus cngStat=new ChangeStatus();
        VirtualMachine vm=new VirtualMachine();
        vm.setDirPath("storage/hs1234/");
        vm.setVmId("000004d20000092900000d80000011d7");
        vm.setVifs(new ArrayList<String>());
        vm.getVifs().add("eth0");
        vm.setGlobalFlowCount(0);
        vm.setHashVal(36);
        //cngStat.start("1234",vm);
        cngStat.stop(vm);
       // System.out.println("in main flowcount is "+vm.getGlobalFlowCount());
    }
}
