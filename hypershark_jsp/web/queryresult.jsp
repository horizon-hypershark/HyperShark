<%-- 
    Document   : packetsdisplay
    Created on : 26 Jan, 2012, 7:47:58 PM
    Author     : varun
--%>

<%@page import="Core.FlowRecord"%>
<%@page import="FileAccess.GetPacket"%>
<%@page import="Core.DisplayPktRule"%>
<%@page import="Core.Rules"%>
<%@page import="Core.CaptureTime"%>
<%@page import="Core.func"%>
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<jsp:useBean id="dispFilter" class="Beans.DisplayTimeFilterBean" scope="page">
    <jsp:setProperty name="dispFilter" property="*"/>
</jsp:useBean>

<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>HyperShark- Packets</title>
            </head>
    <body>
        

    <%  DisplayPktRule dispRule=new DisplayPktRule();
        CaptureTime timRule=new CaptureTime();
        dispFilter.createDisplayRule(dispRule,timRule);
    %>
    <% 
        int j=0;
        int i=0;
        //data d[]=new data[10];
        GetPacket f=new GetPacket(); 
        
        f.setCpt(timRule);
        f.setRules(dispRule);
        
        System.out.println("day is"+f.getCpt().day);
        System.out.println("port is"+f.getRules().lowPort);
        System.out.println("dest port is"+f.getRules().highPort);
        System.out.println("ip is"+f.getRules().getSrcHost()[0]);
        System.out.println("ip is"+f.getRules().getDstHost()[0]);
        
        
        /*f.cpt.day=31;
        f.cpt.month=0;
        f.cpt.end_hr=9;
        f.cpt.end_min=29;
        f.cpt.start_hr=9;
        f.cpt.start_min=20;
        
        f.rules.sip_oct[0]=74;
        f.rules.sip_oct[1]=-1;
        f.rules.sip_oct[2]=-1;
        f.rules.sip_oct[3]=-1;
        
        f.rules.dip_oct[0]=-1;
        f.rules.dip_oct[1]=-1;
        f.rules.dip_oct[2]=-1;
        f.rules.dip_oct[3]=-1;
        
        f.rules.src_port=80;
        f.rules.dst_port=-1;
        f.rules.protocol=-1;*/
                
        f.fillpackets();//call native method
        if(session.getAttribute("flows")!=null)
            session.removeAttribute("flows");
        session.setAttribute("flows",f.flow);
%> 
        <table border="0">
            <tr>
            <td>
            <h><b>FLOWRECORDS</b></h>
            </td>    
            </tr>
            
            <tr>
            <td width="10%"> <b> FLOWRECORD NO.</b></td>   
            <td width="10%"> <b> SOURCE PORT</b></td>
            <td width="10%"> <b> DESTINATION PORT</b></td>
            <td width="10%"> <b> IP SRC </b></td>
            <td width="10%"> <b> IP DEST </b></td>
            <td width="10%"> <b> PROTOCOL</b></td>    
            <td width="10%"> <b> NO OF PACKETS</b></td>
               
            </tr>
        <%int k=0,m=1; 
        j=0;
        System.out.println("no of flowrecords is::"+f.flow.size());
        
        for(FlowRecord flowRec:f.flow)
        { %>
        
        <tr>   
            <td width="10%">
                <a href="packets.jsp?index=<%=j%>"><%out.println(m);
                m++;
                %></a>
            </td>    
            
        <td width="10%">   
        <%out.println(Conversions.shortToUnsigned(flowRec.src_port));%>
        </td>
        <td width="10%">
            <%out.println(Conversions.shortToUnsigned(flowRec.dst_port));%>
        </td> 
        <td width="10%">
                <%out.println(((flowRec.ip_src>>24) & 0xFF) + "." + ((flowRec.ip_src >> 16) & 0xFF) + "." + ((flowRec.ip_src>> 8) & 0xFF) +"."+ ((flowRec.ip_src) & 0xFF));%>
        </td>  
        
        <td width="10%">
                <%out.println(((flowRec.ip_dst >> 24) & 0xFF) + "." + ((flowRec.ip_dst >> 16) & 0xFF) + "." + ((flowRec.ip_dst>> 8) & 0xFF) +"."+ ((flowRec.ip_dst) & 0xFF));
            %>
        </td>  
        <td width="10%">
                <%out.println(flowRec.protocol);%>
        </td>
        
        <td width="10%">
                <%out.println(flowRec.nop);%>  
        </td>
        
        </tr>
         <% j++; } %>
        </table>
        <div id="Div">
        </div>    
        <br/>
        <br/>
        <br/>
    
        
    </body>
</html>
