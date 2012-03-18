<%-- 
    Document   : authenticate
    Created on : 26 Jan, 2012, 4:05:00 PM
    Author     : varun
--%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html> 
<jsp:useBean id="userData" class="Beans.UserDataBean" scope="session">
    <jsp:setProperty name="userData" property="*"/>
</jsp:useBean>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
    </head>
    
    <body>
        
       <%
            if(userData.getData())
            {
                response.sendRedirect("WebPages/vmList.jsp");
                
            }
            else
            {
                session.invalidate();
                response.sendRedirect("WebPages/homepage.jsp?error=1");
                
            }
        %>
            
    </body>
</html>
