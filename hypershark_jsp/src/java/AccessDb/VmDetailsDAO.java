/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package AccessDb;

import Core.VirtualMachine;
import Utils.ConnectionDb;
import java.util.ArrayList;
import java.sql.*;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author varun
 */
public class VmDetailsDAO {
    public boolean updateMonitoringStatus(String vmId,boolean status)
    {
        ConnectionDb conn = new ConnectionDb();
        String updateMonitoring ="update vm_details set monitoring=? where vm_id=?";
        PreparedStatement ps = null;
        conn.getConnection();
       try{    
            ps =conn.getConn().prepareStatement(updateMonitoring);
            ps.setBoolean(1,status);
            ps.setString(2, vmId);
            int no = ps.executeUpdate();
            if(no>0)
                return true;
        } catch (SQLException ex) {
            Logger.getLogger(UserDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        try {
            conn.closeConnection();
        } catch (SQLException ex) {
            Logger.getLogger(VmDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }        
    public ArrayList<VirtualMachine> getVmDetails(String userId)
    {
        ConnectionDb conn = new ConnectionDb();
        String selectVmDetails ="select * from vm_details where vm_id in(select vm_id from cust_vm where cust_id=?)";
        PreparedStatement ps = null;
        ResultSet vmDetails=null;
        ArrayList<VirtualMachine> vmList=new ArrayList<VirtualMachine>();
        conn.getConnection();
       try{    
            ps =conn.getConn().prepareStatement(selectVmDetails);
            ps.setString(1,userId);
            vmDetails = ps.executeQuery();
            while(vmDetails.next())
            {
                VirtualMachine virtMach=new VirtualMachine();
                virtMach.setVmId(vmDetails.getString("vm_id"));
                virtMach.setVmName(vmDetails.getString("vm_name"));
                virtMach.setDirPath(vmDetails.getString("path_dir"));
                virtMach.setGlobalFlowCount(vmDetails.getInt("gb_flow_count"));
                virtMach.setHashVal(vmDetails.getInt("hashval"));
                virtMach.setMemAlloc(vmDetails.getInt("mem_alloc"));
                virtMach.setMemUsed(vmDetails.getInt("mem_used"));
                virtMach.setMonitoringStatus(vmDetails.getBoolean("monitoring"));
                virtMach.setVmImage(vmDetails.getString("vm_image"));
                virtMach.setState(vmDetails.getString("vm_state"));                
                vmList.add(virtMach);             
            }
        } catch (SQLException ex) {
            Logger.getLogger(UserDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        try {
            conn.closeConnection();
        } catch (SQLException ex) {
            Logger.getLogger(VmDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }        
        return vmList;
    }        
    public ArrayList<String> getVifs(String vmId)
    {
        ConnectionDb conn = new ConnectionDb();
        String selectVifs ="select vif_no from vm_vif where vm_id=?";
        PreparedStatement ps = null;
        ResultSet vifs=null;
        ArrayList<String> vifList=new ArrayList<String>();
        conn.getConnection();
       try{    
            ps =conn.getConn().prepareStatement(selectVifs);
            ps.setString(1,vmId);
            vifs = ps.executeQuery();
            while(vifs.next())
            {
                vifList.add(vifs.getString("vif_no"));
            }
        } catch (SQLException ex) {
            Logger.getLogger(UserDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        try {
            conn.closeConnection();
        } catch (SQLException ex) {
            Logger.getLogger(VmDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        return vifList;
    }
    public boolean updateGFL(int gfl,String vmId)
    {
        ConnectionDb conn = new ConnectionDb();
        String updateGfl ="update vm_details set gb_flow_count=? where vm_id=?";
        PreparedStatement ps = null;               
        conn.getConnection();
       try{    
            ps =conn.getConn().prepareStatement(updateGfl);
            ps.setInt(1, gfl);
            ps.setString(2, vmId);
            if(ps.executeUpdate()>0)
                return true;
        } catch (SQLException ex) {
            Logger.getLogger(UserDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        try {
            conn.closeConnection();
        } catch (SQLException ex) {
            Logger.getLogger(VmDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }        
    public boolean updateHash(int hash,String vmId)
    {
        ConnectionDb conn = new ConnectionDb();
        String updateHashVal ="update vm_details set hashval=? where vm_id=?";
        PreparedStatement ps = null;               
        conn.getConnection();
       try{    
            ps =conn.getConn().prepareStatement(updateHashVal);
            ps.setInt(1, hash);
            ps.setString(2, vmId);
            if(ps.executeUpdate()>0)
                return true;
        } catch (SQLException ex) {
            Logger.getLogger(UserDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        try {
            conn.closeConnection();
        } catch (SQLException ex) {
            Logger.getLogger(VmDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }
    public boolean updateVmState(String state,String vmId)
    {
        ConnectionDb conn = new ConnectionDb();
        String updateState ="update vm_details set vm_state=? where vm_id=?";
        PreparedStatement ps = null;
        //System.out.println("in updateVmState query string ::"+"update vm_details set vm_state="+state+" where vm_id="+vmId);
        conn.getConnection();
       try{    
            ps =conn.getConn().prepareStatement(updateState);
            ps.setString(1, state);
            ps.setString(2, vmId);
            if(ps.executeUpdate()>0)
                return true;
        } catch (SQLException ex) {
            Logger.getLogger(UserDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        try {
            conn.closeConnection();
        } catch (SQLException ex) {
            Logger.getLogger(VmDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }
    public boolean updateVmVifs(VirtualMachine virtMach)
    {
        ConnectionDb conn = new ConnectionDb();
        String updateState ="delete from vm_vif where vm_id=?";
        PreparedStatement ps = null;               
        conn.getConnection();
       try{    
            ps =conn.getConn().prepareStatement(updateState);
            ps.setString(1, virtMach.getVmId());            
            if(ps.executeUpdate()<0)
                return false;
        } catch (SQLException ex) {
            Logger.getLogger(UserDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        String insertVif="insert into vm_vif("
                    +"vm_id,"
                    +"vif_no)"
                    +"values(?,?)";        
        try{    
            ps =conn.getConn().prepareStatement(insertVif);
            for(String vif : virtMach.getVifs())
            {    
                ps.setString(1, virtMach.getVmId());            
                ps.setString(2, vif);
                if(ps.executeUpdate()<0)
                    return false;
            }    
        } catch (SQLException ex) {
            Logger.getLogger(UserDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        try {
            conn.closeConnection();
        } catch (SQLException ex) {
            Logger.getLogger(VmDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        return true;
    }
            
}
