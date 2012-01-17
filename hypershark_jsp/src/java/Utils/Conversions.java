/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Utils;

/**
 *
 * @author varun
 */
public class Conversions 
{
    public static int shortToUnsigned(short num)
    {
        return(num&0xFFFF);
    }
    public static long intToUnsigned(int num)
    {
        return(num&(0x0FFFFFFFFL));
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
