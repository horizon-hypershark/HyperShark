/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Adapter;

import AccessDb.UserDetailsDAO;
import Core.User;

/**
 *
 * @author varun
 */
public class UserDetails {
    public User getUserDetails(String username)
    {
        UserDetailsDAO usrDetails=new UserDetailsDAO();
        return usrDetails.getUserDetails(username);
    }        
    
}