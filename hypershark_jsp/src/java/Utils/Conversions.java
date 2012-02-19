/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Utils;

import Core.Protocols.l4Proto.Icmphdr;
import Core.Protocols.l4Proto.Igmphdr;
import Core.Protocols.l4Proto.Tcphdr;
import Core.Protocols.l4Proto.Udphdr;
import javax.print.DocFlavor.BYTE_ARRAY;

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
    public static short stringToProtocol(String protocol)
    {
        short proto;
        if(protocol.equals("TCP"))
            proto=Tcphdr.TCP;
        else if(protocol.equals("UDP"))
            proto=Udphdr.UDP;
        else if(protocol.equals("ICMP"))
            proto=Icmphdr.ICMP;
        else if(protocol.equals("IGMP"))
            proto=Igmphdr.IGMP;
        else if(protocol.equals("SCTP"))
            proto=132;
        else if(protocol.equals("GRE"))
            proto=47;
        else
            proto=-1;
        return proto;
    }   
    public static int stringToMonth(String month)
    {
        int mnth;
        if(month.equals("JAN"))
            mnth=0;
        else if(month.equals("FEB"))
            mnth=1;
        else if(month.equals("MAR"))
            mnth=2;
        else if(month.equals("APR"))
           mnth=3;
        else if(month.equals("MAY"))
            mnth=4;
        else if(month.equals("JUN"))
            mnth=5;
        else if(month.equals("JUL"))
            mnth=6;
        else if(month.equals("AUG"))
            mnth=7;
        else if(month.equals("SEP"))
            mnth=8;
        else if(month.equals("OCT"))
            mnth=9;
        else if(month.equals("NOV"))
            mnth=10;
        else if(month.equals("DEC"))
            mnth=11;
        else 
            mnth=-1;
        return mnth;
    }        
    public static short[] stringToMac(String mac)
    {       
        String delims = ":";
        String[] tokens = mac.split(delims);
        short tokensVal[]=new short[tokens.length];
        for(int i=0;i<tokens.length;i++)
            tokensVal[i]=Short.parseShort(tokens[i], 16);
        return tokensVal;
    }        
    public static short[] stringToIp(String octet1,String octet2,String octet3,String octet4) 
    {
         short tokensVal[]=new short[4];
         tokensVal[0]=Short.parseShort(octet1);
         tokensVal[1]=Short.parseShort(octet2);
         tokensVal[2]=Short.parseShort(octet3);
         tokensVal[3]=Short.parseShort(octet4);
         return tokensVal;
    }
    public static short[] incompleteStringToIp(String octet[],String notSpecified) 
    {
         short tokensVal[]=new short[4];
         for(int i=0;i<4;i++)
         {    
             if(!octet[i].equals(notSpecified))             
                tokensVal[i]=Short.parseShort(octet[i]);
             else
                tokensVal[i]=-1;
         }   
         return tokensVal;
    }        
    public static long[] stringToVmids(String vmId)
    {
        long vmIdList[]=new long[4];
        String temp=vmId.substring(0, 8);
        vmIdList[0]=Long.parseLong(temp,16);
        temp=vmId.substring(8, 16);
        vmIdList[1]=Long.parseLong(temp,16);
        temp=vmId.substring(16, 24);
        vmIdList[2]=Long.parseLong(temp,16);
        temp=vmId.substring(24, 32);
        vmIdList[3]=Long.parseLong(temp,16);
        return vmIdList;
    }        
    public static void main(String args[])
    {
        /*Conversions c= new Conversions();
        short num=-10;
        int num2=-10;
        System.out.println("num is"+Conversions.shortToUnsigned(num));
        System.out.println("num is"+Conversions.intToUnsigned(num2));
        System.out.println(0xEFFFFFFF);*/
        //short []mac=stringToMac("11:af:56:34:ff:ae");
        /*short []ip=stringToIp("192","23","14","82");
        for(int i=0;i<ip.length;i++)
            System.out.println(ip[i]);*/
        long vmId[] =stringToVmids("0abc123456c1123c321412020abc1011");
        for(int i=0;i<vmId.length;i++)
            System.out.println(vmId[i]);
        
    }
}
