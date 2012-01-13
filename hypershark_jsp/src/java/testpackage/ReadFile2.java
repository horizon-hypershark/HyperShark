/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package testpackage;

/**
 *
 * @author varun
 */
public class ReadFile2
{
    public data d[] = new data[10];
    public native void loadFile();
    static
    {
	System.loadLibrary("ReadFile");
    }
    public static void main(String args[])
    {
    
    }
}
