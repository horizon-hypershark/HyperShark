/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Adapter;

import AccessDb.AuthenticationDAO;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author varun
 */
public class Authentication {
    public boolean authenticateUser(String username,String password)
    {
        AuthenticationDAO auth= new AuthenticationDAO();
        try {
            if(auth.validateUser(username,password))
                return true;
        } catch (SQLException ex) {
            Logger.getLogger(Authentication.class.getName()).log(Level.SEVERE, null, ex);            
        }
        return false;
    }        
    
}
