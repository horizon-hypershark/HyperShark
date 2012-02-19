/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Utils;

/**
 *
 * @author varun
 */
public class BuildString
{
    public static String mac_string(byte mac_arr[])
    {
        String str=new String();
        str= Integer.toString((mac_arr[0] & 0xff) + 0x100, 16).substring(1)+"-"+Integer.toString((mac_arr[1] & 0xff) + 0x100, 16).substring(1)+"-"+Integer.toString((mac_arr[2] & 0xff) + 0x100, 16).substring(1)+"-"+Integer.toString((mac_arr[3] & 0xff) + 0x100, 16).substring(1)+"-"+Integer.toString((mac_arr[4] & 0xff) + 0x100, 16).substring(1)+"-"+Integer.toString((mac_arr[5] & 0xff) + 0x100, 16).substring(1);
        return(str);
    }
    public static void main(String args[])
    {
        Conversions c= new Conversions();
        short num=-10;
        int num2=-10;
        System.out.println("num is"+Conversions.shortToUnsigned(num));
        System.out.println("num is"+Conversions.intToUnsigned(num2));
        System.out.println(0xEFFFFFFF);
        
    }
    
}
