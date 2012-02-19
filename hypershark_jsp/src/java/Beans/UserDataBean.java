/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Beans;

import Adapter.Authentication;
import Controller.FlowController;
import Core.User;

/**
 *
 * @author varun
 */
public class UserDataBean {
    private String username;
    private String password;
    private User userDetails;

    public User getUserDetails() {
        return userDetails;
    }

    public void setUserDetails(User userDetails) {
        this.userDetails = userDetails;
    }
    
    
    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }
    public boolean getData()
    {        
        FlowController flowCont = new FlowController();
        if(flowCont.getUserData(this))
            return true;
        return false;
    }        
}
