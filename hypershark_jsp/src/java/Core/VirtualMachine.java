/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

import java.util.ArrayList;

/**
 *
 * @author varun
 */
public class VirtualMachine {
    private String vmName;
    private String vmId;
    private int globalFlowCount;
    private String dirPath;
    private int hashVal;
    private int memAlloc;
    private int memUsed;
    private boolean monitoringStatus;
    private String vmImage;
    private int packets;
    private String state;
    private String vmType;
    private ArrayList<CaptureRule> captureRules;
    private ArrayList<String> vifs;
    private int serviceActive;

    public VirtualMachine() 
    {
      
        vifs=new ArrayList<String>();
    }
    
    
    public void addVif(String vif)
    {
        System.out.println("vifs is"+vifs);
        System.out.println("vif is"+vif);
        vifs.add(vif);      
    }

    public int getServiceActive() {
        return serviceActive;
    }

    public void setServiceActive(int serviceActive) {
        this.serviceActive = serviceActive;
    }
    
    
    public int getPackets() {
        return packets;
    }

    public String getState() {
        return state;
    }

    public void setState(String state) {
        this.state = state;
    }

    public String getVmType() {
        return vmType;
    }

    public void setVmType(String vmType) {
        this.vmType = vmType;
    }

    public void setPackets(int packets) {
        this.packets = packets;
    }
    
    
    public String getVmName() {
        return vmName;
    }
    public void setVmName(String vmName) {
        this.vmName = vmName;
    }

    public String getVmImage() {
        return vmImage;
    }

    public void setVmImage(String vmImage) {
        this.vmImage = vmImage;
    }
    
    public String getVif(int index)
    {
        
        return vifs.get(index);
    }

    public boolean isMonitoringStatus() {
        return monitoringStatus;
    }

    public void setMonitoringStatus(boolean monitoringStatus) {
        this.monitoringStatus = monitoringStatus;
    }
    
    
    public ArrayList<CaptureRule> getCaptureRules() {
        return captureRules;
    }

    public void setCaptureRules(ArrayList<CaptureRule> captureRules) {
        this.captureRules = captureRules;
    }

    public String getDirPath() {
        return dirPath;
    }

    public void setDirPath(String dirPath) {
        this.dirPath = dirPath;
    }

    public int getGlobalFlowCount() {
        return globalFlowCount;
    }

    public void setGlobalFlowCount(int globalFlowCount) {
        this.globalFlowCount = globalFlowCount;
    }

    public int getHashVal() {
        return hashVal;
    }

    public void setHashVal(int hashVal) {
        this.hashVal = hashVal;
    }

    public int getMemAlloc() {
        return memAlloc;
    }

    public void setMemAlloc(int memAlloc) {
        this.memAlloc = memAlloc;
    }

    public int getMemUsed() {
        return memUsed;
    }

    public void setMemUsed(int memUsed) {
        this.memUsed = memUsed;
    }

    public ArrayList<String> getVifs() {
        return vifs;
    }

    public void setVifs(ArrayList<String> vifs) {
        this.vifs = vifs;
    }

    public String getVmId() {
        return vmId;
    }

    public void setVmId(String vmId) {
        this.vmId = vmId;
    }
    
}
