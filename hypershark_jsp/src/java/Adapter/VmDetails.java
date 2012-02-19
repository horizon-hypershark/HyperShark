/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Adapter;

import AccessDb.VmDetailsDAO;
import Core.VirtualMachine;
import java.util.ArrayList;

/**
 *
 * @author varun
 */
public class VmDetails {
    public ArrayList<VirtualMachine> getVmDetails(String userId)
    {
        VmDetailsDAO vmDao=new VmDetailsDAO();
        return vmDao.getVmDetails(userId);
    }
    
    public ArrayList<String> getVifs(String vmId)
    {
        VmDetailsDAO vmDao=new VmDetailsDAO();
        return vmDao.getVifs(vmId);
    }
    public boolean updateMonitoringStatus(String vmId,boolean status)
    {
        VmDetailsDAO vmDao=new VmDetailsDAO();
        return vmDao.updateMonitoringStatus(vmId,status);
    }
    public boolean updateHash(int hash,String vmId)
    {
        VmDetailsDAO vmDao=new VmDetailsDAO();
        return vmDao.updateHash(hash, vmId);        
    }
    public boolean updateGFL(int gfl,String vmId)
    {
        VmDetailsDAO vmDao=new VmDetailsDAO();
        return vmDao.updateGFL(gfl, vmId); 
    }
}
