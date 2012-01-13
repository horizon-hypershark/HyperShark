<%-- 
    Document   : index
    Created on : 6 Jan, 2012, 2:15:03 PM
    Author     : varun
--%>

<%@page import="testpackage.testing"%>

<%@page import="testpackage.ReadFile2"%>
<%@page import="testpackage.data"%>

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
        //data d[]=new data[10];
        ReadFile2 rf=new ReadFile2(); 
        rf.loadFile();
        %>
        <table border="1">
        <tr>
            <td width="20%"> <b> PACKET ID </b>
            <td width="20%"> <b> SOURCE IP </b>
            <td width="20%"> <b> DESTINATION IP </b>
            <td width="20%"> <b> SOURCE MAC </b>
            <td width="20%"> <b> DESTINATION MAC </b>
        </tr>
       <%while(j<5)
        { %>
        <tr>    
        <td width="20%">   
        <% out.println(rf.d[j].pkt_id);%>
        <td width="20%">
        
        <% out.println(((rf.d[j].ip_src) & 0xFF) + "." + ((rf.d[j].ip_src >> 8) & 0xFF) + "." + ((rf.d[j].ip_src >> 16) & 0xFF) +"."+ ((rf.d[j].ip_src >> 24) & 0xFF));%>
			    
        <td width="20%">
                
        <%out.println(((rf.d[j].ip_dst) & 0xFF) + "." + ((rf.d[j].ip_dst >> 8) & 0xFF) + "." + ((rf.d[j].ip_dst >> 16) & 0xFF) +"."+ ((rf.d[j].ip_dst >> 24) & 0xFF));%>

        <td width="20%">
	<%	for (int i = 0;i< rf.d[j].smac.length; i++)
		{ 
		      out.println(Integer.toString((rf.d[j].smac[i] & 0xff) + 0x100, 16).substring(1));
		      if(i!= rf.d[j].smac.length-1)
                      out.print("-");
		}
        %>
	<td width="20%">		
        <% for (int i = 0; i < rf.d[j].dmac.length; i++)
		{ 
		      out.print(Integer.toString((rf.d[j].dmac[i] & 0xff) + 0x100, 16).substring(1));
		      if(i!=rf.d[j].dmac.length-1)
                      out.print("-");
		}	
	
        %>
        </td>
        </tr>
        <% j++; } %>
        </table>
    </body>
</html>
