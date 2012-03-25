/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Adapter;

import Core.CaptureTime;
import Core.DisplayPktRule;
import Core.FlowRecord;
import FileAccess.GetPacket;
import java.util.ArrayList;

/**
 *
 * @author root
 */
public class GetData {
    public ArrayList<FlowRecord> queryPackets(DisplayPktRule dispRule,CaptureTime timRule,String path)
    {
        GetPacket getPkts=new GetPacket();
        return getPkts.getPackets(dispRule, timRule,path);
    }
    
}
