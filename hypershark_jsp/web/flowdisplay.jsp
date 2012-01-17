<%-- 
    Document   : flowdisplay
    Created on : 16 Jan, 2012, 6:00:21 PM
    Author     : varun
--%>

<%@page import="FileAccess.ReadFlow"%>
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
    </head>
    <body>
        <% 
        int j=0;
         ReadFlow flow_obj=new ReadFlow(); 
         flow_obj.readflow();
        %>
        <table border="1">
        <tr>
        <td height="">    
        <table border="1">
        <tr>
            <td width="15%"> <b> SOURCE PORT</b>
            <td width="15%"> <b> DESTINATION PORT</b>
            <td width="15%"> <b> SOURCE IP </b>
            <td width="15%"> <b> DESTINATION IP </b>
            <td width="15%"> <b> PROTOCOL </b>
            <td width="15%"> <b> NUMBER OF PACKETS </b>
        </tr>
        <%while(j<5)
        { %>
        <tr>    
        <td width="15%">   
        <% out.println(Conversions.shortToUnsigned(flow_obj.flow[j].src_port));%>
        </td>
        <td width="15%">
        <% out.println(Conversions.shortToUnsigned(flow_obj.flow[j].dst_port));%>
        </td>
        <td width="15%">
        <%out.println(((flow_obj.flow[j].ip_src) & 0xFF) + "." + ((flow_obj.flow[j].ip_src >> 8) & 0xFF) + "." + ((flow_obj.flow[j].ip_src>> 16) & 0xFF) +"."+ ((flow_obj.flow[j].ip_src>> 24) & 0xFF));%>
        </td>
        <td width="15%">
        <%out.println(((flow_obj.flow[j].ip_dst) & 0xFF) + "." + ((flow_obj.flow[j].ip_dst >> 8) & 0xFF) + "." + ((flow_obj.flow[j].ip_dst>> 16) & 0xFF) +"."+ ((flow_obj.flow[j].ip_dst>> 24) & 0xFF));%>
        </td>
        <td width="15%">   
        <% out.println(flow_obj.flow[j].protocol);%>
        </td>
        <td width="15%">   
        <% out.println(flow_obj.flow[j].nop);%>
        </td>
        </tr>
        <% j++; } %>
        </table>
 
</body>
</html>
