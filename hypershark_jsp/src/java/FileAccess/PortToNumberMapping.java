/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package FileAccess;

import Core.Portlist;
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
public class PortToNumberMapping 
{
    private static ArrayList<Portlist> list=new ArrayList<Portlist>();

    public static void fillPortMap()
      {
          /*map
          map.portobj.setPortNo(443);*/
          FileInputStream fstream = null;
        try {
            fstream = new FileInputStream("PORTLIST");
        } catch (FileNotFoundException ex) {
            Logger.getLogger(PortToNumberMapping.class.getName()).log(Level.SEVERE, null, ex);
        }
          // Get the object of DataInputStream
          DataInputStream in = new DataInputStream(fstream);
          BufferedReader br = new BufferedReader(new InputStreamReader(in));
          String strLine;
        try {
            while ((strLine = br.readLine()) != null)
            {
                String result[],searchport;
                result=strLine.split(" ");
                Portlist temp=new Portlist();
                temp.setPortName(result[0]);
                temp.setPortNo(Integer.parseInt(result[1]));
                PortToNumberMapping.list.add(temp);
                /*searchport=Integer.toString(portobj.getPortNo());
                if(searchport.equals(result[1]))
                {
                    portobj.setPortName(result[0]);
                    System.out.println(portobj.getPortName());
                    break;
                }*/
                
            }
        } catch (IOException ex) {
            Logger.getLogger(PortToNumberMapping.class.getName()).log(Level.SEVERE, null, ex);
        }
      }

    public static ArrayList<Portlist> getList() {
        return list;
    }

    public static void setList(ArrayList<Portlist> list) {
        PortToNumberMapping.list = list;
    }
     
    static
    {
        fillPortMap();
    }
    
     public static int getPortByName(String portName)
     {
         int i;
         for(i=0;i<PortToNumberMapping.list.size();i++)
         {
            if(portName.equals(PortToNumberMapping.list.get(i).getPortName()))
            {
                    return(PortToNumberMapping.list.get(i).getPortNo());
            }
         }
         return(0);
     }
     public static String getPortByNumber(int portNo)
     {
         int i;
         for(i=0;i<PortToNumberMapping.list.size();i++)
         {
            if(portNo==PortToNumberMapping.list.get(i).getPortNo())
            {
                    return(PortToNumberMapping.list.get(i).getPortName());
            }
         }
         return(null);
     }
     
     public static void main(String args[])
     {
         String portname;
         int portno;
         portname=PortToNumberMapping.getPortByNumber(443);
         System.out.println(portname);
         portno=PortToNumberMapping.getPortByName("https");
         System.out.println(portno);
        
     }
     
}