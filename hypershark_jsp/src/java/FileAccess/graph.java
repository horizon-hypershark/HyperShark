/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

/**
 *
 * @author varun
 */
public class graph
{
    int Ips[]=new int[5];
    int frequency[]=new int[5];

    public int[] getIps() {
        return Ips;
    }

    public void setIps(int[] Ips) {
        this.Ips = Ips;
    }

    public int[] getFrequency() {
        return frequency;
    }

    public void setFrequency(int[] frequency) {
        this.frequency = frequency;
    }
    
    public native void fillips();

    static
    {
        System.loadLibrary("graph");
    }

    public static void main(String args[])
    {
    
        graph graphobj=new graph();
        graphobj.fillips();//call native method
        int i=0;
        while(i<5)
        {
            System.out.println("in java ip["+i+"]is"+((graphobj.Ips[i] >> 24) & 0xFF)+"."+((graphobj.Ips[i] >> 16) & 0xFF)+"."+((graphobj.Ips[i] >> 8) & 0xFF)+"."+((graphobj.Ips[i] & 0xFF)));
            System.out.println("in java frequency["+i+"]is"+graphobj.frequency[i]);
            i++;
        }
        }
        
}


    
    
    
    