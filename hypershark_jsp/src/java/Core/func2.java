/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

/**
 *
 * @author varun
 */
public class func2
{
 public CompletePacket packets[] = new CompletePacket[10];
 public native void fillpackets();
    static
    {
	System.loadLibrary("packetlib");
    } 
    public static void main(String args[])
    {
    }
}
