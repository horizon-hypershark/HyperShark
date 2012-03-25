/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

import Core.Protocols.l3Proto.L3ProtocolMap;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author root
 */
public class L3ProtocolMapping
{
    private static ArrayList<L3ProtocolMap> list=new ArrayList<L3ProtocolMap>();

    public static void fillProtocolMap()
      {
          FileInputStream fstream = null;
        try {
            fstream = new FileInputStream("PORTRES");
        } catch (FileNotFoundException ex) {
            Logger.getLogger(L3ProtocolMapping.class.getName()).log(Level.SEVERE, null, ex);
        }
          // Get the object of DataInputStream
          DataInputStream in = new DataInputStream(fstream);
          BufferedReader br = new BufferedReader(new InputStreamReader(in));
          String strLine;
        try {
            while ((strLine = br.readLine()) != null)
            {
                String result[];
                result=strLine.split(" ");
                L3ProtocolMap temp=new L3ProtocolMap();
                temp.setProtocolName(result[0]);
                temp.setProtocolNo(Integer.parseInt(result[1]));
                L3ProtocolMapping.list.add(temp);
                //System.out.println(result[0]+"  "+result[1]);
              
            }
        } catch (IOException ex) {
            Logger.getLogger(L3ProtocolMapping.class.getName()).log(Level.SEVERE, null, ex);
        }
      }

    static
    {
        fillProtocolMap();
    }
    
     public static String getProtoByNumber(int protoNo)
     {
         int i;
         for(i=0;i<L3ProtocolMapping.list.size();i++)
         {
            if(protoNo==L3ProtocolMapping.list.get(i).getProtocolNo())
            {
                    return(L3ProtocolMapping.list.get(i).getProtocolName());
            }
         }
         return(null);
     }
     
     public static void main(String args[])
     {
         String protoName;
         int protoNo;
         protoName=L3ProtocolMapping.getProtoByNumber(17);
         System.out.println(protoName);
         
     }
     
}
    
