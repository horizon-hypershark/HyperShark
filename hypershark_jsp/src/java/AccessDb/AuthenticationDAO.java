/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package AccessDb;

import Utils.ConnectionDb;
import java.sql.*;

/**
 *
 * @author varun
 */
public class AuthenticationDAO
{
    private ConnectionDb conn=new ConnectionDb();

    public AuthenticationDAO()
    {
        conn.getConnection();
    } 
    public boolean validateUser(String username,String password) throws SQLException
    {
        if(username==null || password==null)
            return false;
        String selectPassword ="select password from cust_details where cust_name=?";
        PreparedStatement ps = null;
        ResultSet userPass=null;

        ps=conn.getConn().prepareStatement(selectPassword);
        ps.setString(1, username);
        userPass=ps.executeQuery();
        if(userPass.next())
        {
            if(password.equals(userPass.getString("password")))
            {
                conn.closeConnection();
                return true;
            }    
        }
        conn.closeConnection();
        return false;
    }
    /*public static void main(String args[]) throws SQLException
    {
         boolean ret;
         Authenticate auth=new Authenticate();
         ret=auth.validateUser("1234","1234234534564567");
         if(ret==true)
                System.out.println(ret);
         else
                System.out.println(ret);
    }*/
    
}
