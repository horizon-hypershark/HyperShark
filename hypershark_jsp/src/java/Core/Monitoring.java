package Core;

/**
 *
 * @author root
 */

import ch.ethz.ssh2.Connection;
import java.io.*;
import ch.ethz.ssh2.Session;
import ch.ethz.ssh2.StreamGobbler;
import java.util.logging.Level;
import java.util.logging.Logger;
import util.ReadProperty;

public class Monitoring {
    private static String hostName;
    private static String userName;
    private static String password;

    static {
        hostName=ReadProperty.getInstance().getProperty("hypershark.dom0.hostname");
        userName=ReadProperty.getInstance().getProperty("hypershark.dom0.username");
        password=ReadProperty.getInstance().getProperty("hypershark.dom0.password");
    }
    
    public static boolean executeCommand(String hostname,String username,String password,String command) throws IOException
    {
                
                try
                {
                    int stat;
                    Connection conn = new Connection(hostname);
                    conn.connect();
                    boolean isAuthenticated = conn.authenticateWithPassword(username, password);
                    if (isAuthenticated == false)
                        throw new IOException("Authentication failed.");
                    Session sess = conn.openSession();                    
                    sess.execCommand(command);
                    InputStream stdout = new StreamGobbler(sess.getStdout());
                    BufferedReader br = new BufferedReader(new InputStreamReader(stdout));
                    while(true){
                        String line=br.readLine();
                        if(line==null)
                            break;
                        System.out.println(line);
                    }                    
                    sess.close();
                    conn.close();
                    return true;                    
                }
                catch(IOException e){
                    throw e;
                }             
    }
    public static void mapVmToVif()
    {
        boolean look;
        try {
            look = executeCommand(hostName, userName, password, ReadProperty.getInstance().getProperty("hypershark.dom0.vmToVif.command"));
            System.out.println(look);
        } catch (IOException ex) {
            Logger.getLogger(Monitoring.class.getName()).log(Level.SEVERE, null, ex);
        }
    }        
    public static void startDaemon(String path,int maxMem,String userId)
    {        
        String command=ReadProperty.getInstance().getProperty("hypershark.dom0.startDaemon.command");
        command+=" "+path+" "+maxMem+" "+userId;
        boolean look;
        try {
            look = executeCommand(hostName, userName, password, command);
            System.out.println(look);
        } catch (IOException ex) {
            Logger.getLogger(Monitoring.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    public static void stopDaemon(String path,String userId)
    {        
        String command=ReadProperty.getInstance().getProperty("hypershark.dom0.stopDaemon.command");
        command+=" `cat /compress_"+userId+".lockd`";
        boolean look;
        try {
            look = executeCommand(hostName, userName, password, command);
            System.out.println(look);
        } catch (IOException ex) {
            Logger.getLogger(Monitoring.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
     public static void main(String ags[])
    {
       boolean look;
        try {
            look = executeCommand("192.168.1.6", "root", "seagate", ReadProperty.getInstance().getProperty("hypershark.vmToVif.command"));
            System.out.println(look);
        } catch (IOException ex) {
            Logger.getLogger(Monitoring.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}
