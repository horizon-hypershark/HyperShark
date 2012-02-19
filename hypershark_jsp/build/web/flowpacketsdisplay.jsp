<%-- 
    Document   : display2
    Created on : 20 Jan, 2012, 7:24:07 PM
    Author     : varun
--%>

<%@page import="java.util.ArrayList"%>
<%@page import="Core.CompletePacket"%>
<%@page import="Core.pkts"%>

<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
        
</head>
<body>
    
    <br/>
    <br/>
    <br/>
        
    <% 
        int j=0;
        int i;
        String str_hdr= new String();
        //data d[]=new data[10];
        String strr=request.getParameter("index");
        int index= Integer.parseInt(strr);
        //out.println(index);
        //int index=1;
        ArrayList<CompletePacket> pkts= (ArrayList<CompletePacket>)session.getAttribute("flowpackets");
        
        str_hdr=pkts.get(index).l2Packet.getPacket();
        out.println(str_hdr);%>
        <br/>
        <br/>
        <%str_hdr=pkts.get(index).l3Packet.getPacket();
        out.println(str_hdr);%>
        <br/>
        <br/>
        <%str_hdr=pkts.get(index).l4Packet.getPacket();
        out.println(str_hdr);%>
    
     </body>
</html>