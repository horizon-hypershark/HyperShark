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
        <link href="style.css" rel="stylesheet" type="text/css" />

        <style type="text/css">
        body
            {
             background-image:url('background.png');
            }
        </style>
    

        
        
        <script type="text/javascript">
        function show_packets(index)
        {
                window.location.href="packets.jsp?index="+index;
        }
</script>

        
            </head>
    <body>
        <div class="section" id="page">

	<div class="header">
		<h1><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
		<h2><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;
                <a href="logout.jsp"><font color="#FFFFFF" size="4px">Logout</font></a>
                    
                    
                    </font></h2>
	</div>
	
        <br/><br/>
        

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
        
        /*System.out.println("day is"+f.getCpt().day);
        System.out.println("port is"+f.getRules().lowPort);
        System.out.println("dest port is"+f.getRules().highPort);
        System.out.println("ip is"+f.getRules().getSrcHost()[0]);
        System.out.println("ip is"+f.getRules().getDstHost()[0]);
        */
        
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
        <div class="articleBody clear" id="articles">
        <%
            if(f.flow.size()==0)
        {
            out.println("Oops! sorry no data for the given query found");
        
        }
        else
                 {
            
        
        System.out.println("no of flowrecords is::"+f.flow.size());
        
        
        %>
            
            <table border="0">
            <tr>
            <td>
            <h><b>FLOWRECORDS</b></h>
            
            </td>    
            </tr>
            <tr>
                <td>
                     <hr class="hor_line"/>
                </td>     
            </tr>
            <tr align="center">
            <td width="10%"> <font size="2px"><b>Flowrecord No.</b></font></td>   
            <td width="10%"> <font size="2px"><b>Source Port</b></font></td>
            <td width="10%"> <font size="2px"><b>Dest Port</b></font></td>
            <td width="10%"> <font size="2px"><b>Source IP</b></font></td>
            <td width="10%"> <font size="2px"><b>Destination IP</b></font></td>
            <td width="10%"> <font size="2px"><b>Protocol</b></font></td>    
            <td width="10%"> <font size="2px"><b>No Of Packets</b></font></td>
               
            </tr>
            
        <%int k=0,m=1; 
        j=0;
        FlowRecord recArray[]=new FlowRecord[f.flow.size()];
        for(FlowRecord flowRec:f.flow)
        {
            recArray[j++]=flowRec;
        }    
        j=0;
        for(FlowRecord flowRec:f.flow)
        { %>
        
        <tr onclick="show_packets(<%=j%>)" bgcolor="white"  align="center" onmouseout="this.style.background='white';" onmouseover="this.style.background='#EEEEEE';this.style.cursor='pointer';">   
            <td width="10%">
                <%out.println(m);
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
         <% j++; } 
               }%>
        </table>
        </div>
        
        <br/><br/>			
	<div class="footer">
	   <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->
	   <a href="#" class="up">Go UP</a>
	</div>
        </div>  

        
        
        <div id="Div">
        </div>    
        <br/>
        <br/>
        <br/>
    
        
    </body>
</html>
