/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Utils;

/**
 *
 * @author varun
 */
public class Hash {
    public static int getDirHash(String vmId,String userId)
    {
        long hash;
        hash=Long.parseLong(userId);
        long vmIds[]=Conversions.stringToVmids(vmId);
        for(int i=0;i<4;i++)
            hash+=vmIds[i];
        return ((int)(hash%256));
    }
    public static void main(String args[])
    {
        System.out.println(getDirHash("000004d20000092900000d80000011d7", "1234"));
    }        
    
}
