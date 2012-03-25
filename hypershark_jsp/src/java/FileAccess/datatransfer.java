/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

import Core.CaptureTime;

/**
 *
 * @author root
 */
public class datatransfer
{
    private long bytes_transfer[]=new long[24];
    private CaptureTime cpt=new CaptureTime();
    private String path=new String();

    public long[] getBytes_transfer() {
        return bytes_transfer;
    }

    public void setBytes_transfer(long[] bytes_transfer) {
        this.bytes_transfer = bytes_transfer;
    }

    public CaptureTime getCpt() {
        return cpt;
    }

    public void setCpt(CaptureTime cpt) {
        this.cpt = cpt;
    }

    public String getPath() {
        return path;
    }

    public void setPath(String path) {
        this.path = path;
    }
        
    public native void fillData();
    
    static
    {
        System.loadLibrary("bytestransfer");
        
    }
    
    public static void main(String args[])
    {
     datatransfer transfer=new datatransfer();
     transfer.cpt.setDay(21);
     transfer.cpt.setEnd_hr(0);
     transfer.cpt.setEnd_min(0);
     transfer.cpt.setMonth(1);
     transfer.cpt.setStart_hr(0);
     transfer.cpt.setStart_min(0);
     transfer.setPath("/storage/hs1234/");
     System.out.println("before native call");
     transfer.fillData();
     System.out.println("after native call");
     for(int i=0;i<transfer.bytes_transfer.length;i++)
     {
         System.out.println(transfer.getBytes_transfer()[i]);
     }
     
    }
}
