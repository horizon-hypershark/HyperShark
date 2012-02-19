/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package AccessDb;

import Beans.UserDataBean;
import Core.User;
import Utils.ConnectionDb;
import java.sql.*;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author varun
 */
public class UserDetailsDAO {
    public User getUserDetails(String username)
    {      
            ConnectionDb conn = new ConnectionDb();
            String selectUserDetails ="select * from cust_details where cust_name=?";
            PreparedStatement ps = null;
            ResultSet userDetails=null;
            User usr=new User();
            conn.getConnection();
       try{    
            ps =conn.getConn().prepareStatement(selectUserDetails);
            ps.setString(1,username);
            userDetails = ps.executeQuery();
            if(userDetails.next())
            {
                usr.setUserId(userDetails.getString("cust_id"));
                usr.setUserName(username);
                usr.setUserInfo(userDetails.getString("cust_info"));
                usr.setNoOfVms(userDetails.getInt("vm_count"));
            }
            userDetails.close();
            ps.close();
            
            //still remaining setting capture time filters
            conn.closeConnection();
            return usr;
        } catch (SQLException ex) {
            Logger.getLogger(UserDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }        
}