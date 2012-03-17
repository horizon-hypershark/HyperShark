<%-- 
    Document   : toggle_status
    Created on : 12 Feb, 2012, 6:13:14 PM
    Author     : varun
--%>

<%@page import="Controller.FlowController"%>
<%@page import="Beans.UserDataBean"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%UserDataBean userData=(UserDataBean)session.getAttribute("userData");
if(userData==null){
    response.sendRedirect("homepage.jsp");    
//    return;
}    
else
{   
    //System.out.println("Packets is::"+request.getParameter("packets"));
    FlowController flowControl=new FlowController();
    //System.out.println("In toogle_status.jsp");
    flowControl.changeMonitoringStatus(userData, Integer.parseInt(request.getParameter("index")),Integer.parseInt(request.getParameter("packets")));
   
    response.sendRedirect("WebPages/vmList.jsp");
}%>