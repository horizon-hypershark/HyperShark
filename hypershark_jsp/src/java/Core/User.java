/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

import java.util.ArrayList;

/**
 *
 * @author nitish
 */
public class User {
    private String userId;
    private String userName;
    private String userInfo;
    private int noOfVms;
    private ArrayList<VirtualMachine> virMachineList;

    public ArrayList<VirtualMachine> getVirMachineList() {
        return virMachineList;
    }

    public void setVirMachineList(ArrayList<VirtualMachine> virMachineList) {
        this.virMachineList = virMachineList;
    }
    
    
    public int getNoOfVms() {
        return noOfVms;
    }

    public void setNoOfVms(int noOfVms) {
        this.noOfVms = noOfVms;
    }

    public String getUserId() {
        return userId;
    }

    public void setUserId(String userId) {
        this.userId = userId;
    }

    public String getUserInfo() {
        return userInfo;
    }

    public void setUserInfo(String userInfo) {
        this.userInfo = userInfo;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }    
    
}
