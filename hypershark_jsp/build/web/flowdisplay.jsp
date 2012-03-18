<%-- 
    Document   : flowdisplay
    Created on : 16 Jan, 2012, 6:00:21 PM
    Author     : varun
--%>

<%@page import="java.util.ArrayList"%>
<%@page import="Core.FlowRecord"%>
<%@page import="Beans.UserDataBean"%>
<%@page import="FileAccess.ReadFlow"%>
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
        <title>JSP Page</title>
    </head>
    <body>
        <%ArrayList<FlowRecord> flowList = (ArrayList<FlowRecord>) session.getAttribute("flows");
        ReadFlow flow_obj=new ReadFlow(); 
         flow_obj.readflow();
        %>
        <table border="0">
        <tr>
            <td width="15%"> <b> SOURCE PORT</b></td>
            <td width="15%"> <b> DESTINATION PORT</b></td>
            <td width="15%"> <b> SOURCE IP </b></td>
            <td width="15%"> <b> DESTINATION IP </b></td>
            <td width="15%"> <b> PROTOCOL </b></td>
            <td width="15%"> <b> NUMBER OF PACKETS </b></td>
        </tr>
         <%
         int j=0;
        for(FlowRecord flowRec:flowList)
        { 
         %>
        
         <tr>    
        <td width="15%">   
        <% out.println(Conversions.shortToUnsigned(flowRec.getSrc_port()));%>
        </td>
        <td width="15%">
        <% out.println(Conversions.shortToUnsigned(flowRec.getDst_port()));%>
        </td>
        <td width="15%">
        <%out.println(((flowRec.ip_src) & 0xFF) + "." + ((flowRec.ip_src >> 8) & 0xFF) + "." + ((flowRec.ip_src>> 16) & 0xFF) +"."+ ((flowRec.ip_src>> 24) & 0xFF));%>
        </td>
        <td width="15%">
        <%out.println(((flowRec.ip_dst) & 0xFF) + "." + ((flowRec.ip_dst >> 8) & 0xFF) + "." + ((flowRec.ip_dst>> 16) & 0xFF) +"."+ ((flowRec.ip_dst>> 24) & 0xFF));%>
        </td>
        <td width="15%">   
        <% out.println(flowRec.protocol);%>
        </td>
        <td width="15%">   
        <% out.println(flowRec.nop);%>
        </td>
        </tr>
        
        
        <% j++; } %>
       
        </table>
 
</body>
</html>
