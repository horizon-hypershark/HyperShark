/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Adapter;

import AccessDb.CaptureFilterDAO;
import Core.CaptureRule;
import java.util.ArrayList;

/**
 *
 * @author varun
 */
public class CaptureFilter {
    public int recordCaptureFilter(CaptureRule captRule,String vmId)
    {
        CaptureFilterDAO captDao=new CaptureFilterDAO();
        return captDao.insertCaptureFilter(captRule, vmId);
    }
    public ArrayList<CaptureRule> retrieveCaptureFilter(String vmId)
    {
        CaptureFilterDAO captDao=new CaptureFilterDAO();
        return captDao.retrieveCaptureFilter(vmId);
    }
}
