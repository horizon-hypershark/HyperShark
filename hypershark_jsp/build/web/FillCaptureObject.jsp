<%-- 
    Document   : FillCaptureObject
    Created on : 31 Jan, 2012, 1:15:52 PM
    Author     : varun
--%>

<%@page import="Utils.Conversions"%>
<%@page import="Core.func"%>
<%@page import="Core.Rules"%>
<%@page import="Core.CaptureTime"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
    </head>
    <body>
 <%
        CaptureTime cpt=new CaptureTime();
        String date=new String();
        date=request.getParameter("inputField");
        
        
        int i;
        
        String day=new String();
        day=date.substring(0,2);
        cpt.day=Integer.parseInt(day);
        
        String month=date.substring(3,6);
        if(month.matches("JAN"))
            cpt.month=0;
        if(month.matches("FEB"))
            cpt.month=1;
        if(month.matches("MAR"))
            cpt.month=2;
        if(month.matches("APR"))
            cpt.month=3;
        if(month.matches("MAY"))
            cpt.month=4;
        if(month.matches("JUN"))
            cpt.month=5;
        if(month.matches("JUL"))
            cpt.month=6;
        if(month.matches("AUG"))
            cpt.month=7;
        if(month.matches("SEP"))
            cpt.month=8;
        if(month.matches("OCT"))
            cpt.month=9;
        if(month.matches("NOV"))
            cpt.month=10;
        if(month.matches("DEC"))
            cpt.month=11;
        
        String start_time=request.getParameter("start_time");
        String hr =start_time.substring(0,2);
        cpt.start_hr=Integer.parseInt(hr);
        
        String min =start_time.substring(3,5);
        cpt.start_min=Integer.parseInt(min);
        
        String end_time=request.getParameter("end_time");
        hr =end_time.substring(0,2);
        cpt.end_hr=Integer.parseInt(hr);
        
        min = end_time.substring(3,5);
        cpt.end_min=Integer.parseInt(min);
        
        
 %>
 
 <!--fill rules-->
 <%
 Rules rules=new Rules();
 String port=request.getParameter("ChoiceFromPort");
 int p=Integer.parseInt(port);
 rules.src_port=p;
 
 port=request.getParameter("ChoiceToPort");
 p=Integer.parseInt(port);
 rules.dst_port=p;
 
 String octet=request.getParameter("octet1");
 int oct=Integer.parseInt(octet);
 rules.sip_oct[0]=oct;
 
 octet=request.getParameter("octet2");
 oct=Integer.parseInt(octet);
 rules.sip_oct[1]=oct;
 
 octet=request.getParameter("octet3");
 oct=Integer.parseInt(octet);
 rules.sip_oct[2]=oct;
 
 octet=request.getParameter("octet4");
 oct=Integer.parseInt(octet);
 rules.sip_oct[3]=oct;
 
 octet=request.getParameter("octet5");
 oct=Integer.parseInt(octet);
 rules.dip_oct[0]=oct;
 
 octet=request.getParameter("octet6");
 oct=Integer.parseInt(octet);
 rules.dip_oct[1]=oct;
 
 octet=request.getParameter("octet7");
 oct=Integer.parseInt(octet);
 rules.dip_oct[2]=oct;
 
 octet=request.getParameter("octet8");
 oct=Integer.parseInt(octet);
 rules.dip_oct[3]=oct;

 String protocol=request.getParameter("Proto");
 
 if(protocol.matches("TCP"))
 rules.protocol=6;
 
 if(protocol.matches("UDP"))
 rules.protocol=17;
 
 if(protocol.matches("SCTP"))
 rules.protocol=132;
 
 if(protocol.matches("GRE"))
 rules.protocol=47;
 
 if(protocol.matches("ICMP"))
 rules.protocol=1;
 
 if(protocol.matches("IGMP"))
 rules.protocol=2;
 
%>
    </body>
</html>
