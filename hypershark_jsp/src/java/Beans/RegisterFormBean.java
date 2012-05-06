/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Beans;

import Controller.FlowController;
import Core.VirtualMachine;
import java.util.ArrayList;

/**
 *
 * @author root
 */
public class RegisterFormBean {
    private String userId;
    private ArrayList<VirtualMachine> virtList;
    public void processRequest()
    {
        FlowController flowControl=new FlowController();
        virtList=flowControl.getSubscriptionList(userId);
    }

    public String getUserId() {
        return userId;
    }

    public void setUserId(String userId) {
        this.userId = userId;
    }

    public ArrayList<VirtualMachine> getVirtList() {
        return virtList;
    }

    public void setVirtList(ArrayList<VirtualMachine> virtList) {
        this.virtList = virtList;
    }
    
    
}
