/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package util;
import Utils.ConnectionDb;
import java.sql.*;


/**
 *
 * @author pankaj
 */
public class ExecuteQuery {
    Statement stat;
    ResultSet rs;
    public ResultSet execQuery(ConnectionDb connect,String query)throws java.sql.SQLException
    {
        stat=connect.getConn().createStatement(ResultSet.TYPE_SCROLL_SENSITIVE,ResultSet.CONCUR_READ_ONLY);
        rs=stat.executeQuery(query);
        return rs;
    }
    public void close() throws SQLException
    {
        if(stat!=null && !stat.isClosed())
            stat.close();
        if(rs !=null && !rs.isClosed())
            rs.close();
    }


}
