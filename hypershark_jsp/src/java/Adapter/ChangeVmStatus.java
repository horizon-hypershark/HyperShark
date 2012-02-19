/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Adapter;

import Core.VirtualMachine;
import FileAccess.ChangeStatus;

/**
 *
 * @author varun
 */
public class ChangeVmStatus {
    public void start(String userId,VirtualMachine virtMach)
    {
        ChangeStatus changStat=new ChangeStatus();
        changStat.start(userId, virtMach);
    }        
    public void stop(VirtualMachine virtMach)
    {
        ChangeStatus changStat=new ChangeStatus();
        changStat.stop(virtMach);
    }
    
}
