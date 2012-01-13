/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package testpackage;

/**
 *
 * @author varun
 */
public class test2 
{
    private  native void loadFile();
    int a;
    float b;
    char c;
    static
  	{
            System.loadLibrary("testing");
        }
        public int main1()
        {    
            test2 test=new test2();
            test.loadFile();
                //System.out.println(this.s);
            return test.a;
        }
        public static void main(String args[])
        {
            
        }
}