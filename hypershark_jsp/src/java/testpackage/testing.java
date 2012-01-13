/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package testpackage;

/**
 *
 * @author varun
 */
public class testing {
    private  native void loadFile();
    private String s;   
    static
  	{
            System.loadLibrary("testing");
        }
        public String main1()
        {    
            testing test=new testing();
            test.loadFile();
                //System.out.println(this.s);
            return test.s;
        }
        public static void main(String args[])
        {
            
        }
}
