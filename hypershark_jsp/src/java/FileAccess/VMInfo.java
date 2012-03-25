/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

import AccessDb.VmDetailsDAO;
import Core.VirtualMachine;
import java.util.ArrayList;

/**
 *
 * @author root
 */
public class VMInfo
{
    
    
    private ArrayList<VirtualMachine> vmList=new ArrayList<VirtualMachine>();
    
    public void addVM(VirtualMachine vm)
    {
        vmList.add(vm);      
    }
    
    
    public native void fillInfo();
    
    static
    {
        System.loadLibrary("vminfo");
        
    }
    
    public ArrayList<VirtualMachine> getVmList() {
        return vmList;
    }

    public void setVmList(ArrayList<VirtualMachine> vmList) {
        this.vmList = vmList;
    }
    
    public static void main(String args[])
    {
     VMInfo info=new VMInfo();
     info.fillInfo();
     VmDetailsDAO vmDAO=new VmDetailsDAO();
     for(int i=0;i<info.getVmList().size();i++)
     {
        System.out.println("in java vm name is"+info.getVmList().get(i).getVmName());
        System.out.println("in java vm type is"+info.getVmList().get(i).getVmType());
        System.out.println("in java vm state is"+info.getVmList().get(i).getState());
        System.out.println("in java vm id is"+info.getVmList().get(i).getVmId());
        for(int j=0;j<info.getVmList().get(i).getVifs().size();j++)
            System.out.println("in java vif is"+info.getVmList().get(i).getVif(j));
        vmDAO.updateVmState(info.getVmList().get(i).getState(), info.getVmList().get(i).getVmId());
        vmDAO.updateVmVifs(info.getVmList().get(i));
     }     
     
    }

    
}

