<%-- 
    Document   : editfilters
    Created on : 30 Jan, 2012, 1:59:57 PM
    Author     : varun
--%>

<%@page import="java.sql.ResultSet"%>
<%@page import="util.ExecuteQuery"%>
<%@page import="Utils.ConnectionDb"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>HyperShark - EDITFILTERS</title>
        <link href="style.css" rel="stylesheet" type="text/css" />
        <style type="text/css">
            body
            {
                background-image:url('articleback.JPG');
            }
        </style>

    </head>
    <body>
        <% ConnectionDb conn = new ConnectionDb();
               ExecuteQuery eq = new ExecuteQuery();
               conn.getConnection();%>

        <div class="section" id="page">

            <div class="header">
                <h1><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
                <h2><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h2>
            </div>
            
            <br/><br/>
            
            <div class="articleBody clear" id="articles">
                <%
                int vm_id=101;
                String query = "select * from vm_filters where vm_id='" +vm_id+ "'";
                ResultSet valUser = eq.execQuery(conn, query);
                /*if (valUser.next())
                {
                    cust_id= valUser.getInt("cust_id");
                    cust_name= valUser.getString("cust_name");
                    cust_info = valUser.getString("cust_info");
                    vm_count= valUser.getInt("vm_count");
                }*/


                %>
            
            
            </div>

            <br/><br/>			
            <div class="footer">
	   <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->
	   <a href="#" class="up">Go UP</a>
       	    </div>
            </div>

    </body>
</html>
