<%-- 
    Document   : capturefilter
    Created on : 8 Feb, 2012, 9:05:45 PM
    Author     : varun
--%>

<%@page import="java.util.ArrayList"%>
<%@page import="Beans.UserDataBean"%>
<%@page import="Utils.Conversions"%>
<%@page import="Core.CaptureRule"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<jsp:useBean id="captFilter" class="Beans.CaptureTimeFilterBean" scope="page">
    <jsp:setProperty name="captFilter" property="*"/>
</jsp:useBean>
<%UserDataBean userData=(UserDataBean)session.getAttribute("userData");
if(userData==null){
    response.sendRedirect("homepage.jsp");    
//    return;
}    
else
{
%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
    </head>
    <body>
        <%
            int index;    
            System.out.println("in capturefilter.jsp");    
            String indexstr=request.getParameter("index");
            if(indexstr!=null)
                           {
                index=Integer.parseInt(indexstr);
                System.out.println("index passed is "+index);
                               }
            else
               System.out.println("value passed is null");
            //captFilter.createCaptureRule(userData,index);            
            //captFilter.recordCaptureFilter(userData);
        %>  
        
    </body>
</html>
<%}%>