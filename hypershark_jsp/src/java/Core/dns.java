/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

/**
 *
 * @author varun
 */
import java.net.InetAddress;
import java.net.UnknownHostException;

public class dns
{

    public static void main(String args[])
    {
        try {
            InetAddress host;
            
            String str="192.168.1.12";
            String name= new String();
            if (args.length == 0) {
                InetAddress ia = InetAddress.getByName(str);
                name=ia.getHostName();
                System.out.println("name of host is"+name);
                //name = InetAddress.getHostName("120.0.1.1");
            } else {
                host = InetAddress.getByName(args[0]);
            }
            //System.out.println("Host:'" + host.getHostName()+ "' has address: " + host.getHostAddress());

        } catch (UnknownHostException e) {
        }
    }
}
