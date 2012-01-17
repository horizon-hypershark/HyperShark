/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

/**
 *
 * @author varun
 */
public class ReadFlow 
{
        public FlowRecord flow[] =new FlowRecord[10];	
	public native void readflow();
	static
	{
		System.loadLibrary("flowrecord");
	}
	public static void main(String args[])
	{
		
        }
}
