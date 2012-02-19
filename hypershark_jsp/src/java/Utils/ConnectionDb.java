/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Utils;
import com.mysql.jdbc.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import util.ReadProperty;
/**
 *
 * @author varun
 */
public class ConnectionDb
{
     private static Connection conn;
     private String url ;
     private String dbName;
     private String driver;
     private String userName;
     private String password;
     public ConnectionDb()
   {
         url = ReadProperty.getInstance().getProperty("hypershark.db.url");
         dbName=ReadProperty.getInstance().getProperty("hypershark.db.dbName");          
         driver = ReadProperty.getInstance().getProperty("hypershark.db.driver");
         userName = ReadProperty.getInstance().getProperty("hypershark.db.userName");
         password = ReadProperty.getInstance().getProperty("hypershark.db.password");
        
   }
     
     public com.mysql.jdbc.Connection getConn()
     {
       return conn;
     }
     public void closeConnection()throws SQLException
     {
       conn.close();
     }
     public void getConnection()
     {
        try
        {
           Class.forName(driver).newInstance();
        }catch(Exception e){System.out.println(e);}
        try
        {
           //System.out.println(userName);
           conn = (com.mysql.jdbc.Connection) DriverManager.getConnection(url+dbName,userName,password);
        }catch(Exception e){System.out.println(e.toString());}
     }
     public void setDbName(String dbName) {
       this.dbName = dbName;
   }

   public void setDriver(String driver) {
       this.driver = driver;
   }

   public void setPassword(String password) {
       this.password = password;
   }

   public void setUrl(String url) {
       this.url = url;
   }

   public void setUserName(String userName) {
       this.userName = userName;
   }
}


