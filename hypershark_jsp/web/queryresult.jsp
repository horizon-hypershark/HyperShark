<%-- 
    Document   : packetsdisplay
    Created on : 26 Jan, 2012, 7:47:58 PM
    Author     : varun
--%>

<%@page import="Beans.UserDataBean"%>
<%@page import="java.util.ArrayList"%>
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
<%UserDataBean userData=(UserDataBean)session.getAttribute("userData");
if(userData==null){
    response.sendRedirect("WebPages/homepage.jsp");    
}    
else 
{    
%>

<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>HyperShark- Packets</title>
        <link href="style.css" rel="stylesheet" type="text/css" />

        <script type="text/javascript">
        function sortResult(param)
        {
           //alert("script working"+param); 
           window.location.href="queryresult.jsp?sortingField="+param;
        }
        
        </script>
        
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
            <div class="title">
                <h1><font color="#52A300">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
            </div>
            <div class="header">
            <h3><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font>
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
               <a href="../logout.jsp"><font color="#FFFFFF" size="4px">Logout</font></a>
             </h3>       
                
            </div>

	<br/><br/>
        

        <%ArrayList<FlowRecord> flowRec=(ArrayList<FlowRecord>)session.getAttribute("flows");  
        int j=0,i=0;
        if(dispFilter.getSortingField()!=null && flowRec==null){
                response.sendRedirect("WebPages/vmList.jsp");
        }
        else{
            
        if(dispFilter.getSortingField()==null)
        {            
        DisplayPktRule dispRule=new DisplayPktRule();
        CaptureTime timRule=new CaptureTime();
        dispFilter.createDisplayRule(dispRule,timRule);        
        flowRec=dispFilter.queryPackets(dispRule, timRule);        
        if(session.getAttribute("flows")!=null)
            session.removeAttribute("flows");
        session.setAttribute("flows",flowRec);
        }    
%> 
        <div class="articleBody clear" id="articles">
        <%
        if(flowRec.size()==0)
        {
            out.println("Oops! sorry no data for the given query found");
        
        }
        else
        {
            
        
        System.out.println("no of flowrecords is::"+flowRec.size());
               
        
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
            <td width="10%"><button style="width:175px;height:25px" onclick="sortResult('recNo')">Flowrecord No.</button></td>    
            <!--<td width="10%" onmouseover="this.style.cursor='pointer';" onclick="sortResult('recNo')"> <font size="2px"><b>Flowrecord No.</b></font></td>-->   
            <td width="10%"><button style="width:120px;height:25px" onclick="sortResult('sourcePort')">Source Port</button></td>
            <td width="10%"><button style="width:120px;height:25px" onclick="sortResult('destPort')">Dest Port</button></td>
            <td width="10%"><button style="width:140px;height:25px" onclick="sortResult('sourceIp')">Source IP</button></td>
            <td width="10%"><button style="width:145px;height:25px" onclick="sortResult('destIp')">Destination IP</button></td>
            <td width="10%"><button style="width:130px;height:25px" onclick="sortResult('protocol')">Protocol</button></td>    
            <td width="10%"><button style="width:140px;height:25px" onclick="sortResult('numPackets')">No Of Packets</button></td>
               
            </tr>
            
        <%int k=0,m=1; 
        j=0;
        FlowRecord recArray[]=new FlowRecord[flowRec.size()];
        for(FlowRecord flowRecord:flowRec)
        {
            recArray[j++]=flowRecord;
        }
        if(dispFilter.getSortingField()!=null && (!dispFilter.getSortingField().equals("recNo")))
        {
            dispFilter.sortFlowRecord(recArray);
        }
        for(j=0;j<recArray.length;j++)
        { %>
        
        <tr onclick="show_packets(<%=j%>)" bgcolor="white"  align="center" onmouseout="this.style.background='white';" onmouseover="this.style.background='#EEEEEE';this.style.cursor='pointer';">   
            <td width="10%">
                <%out.println(m);
                m++;
                %></a>
            </td>    
            
        <td width="10%">   
        <%out.println(Conversions.shortToUnsigned(recArray[j].src_port));%>
        </td>
        <td width="10%">
            <%out.println(Conversions.shortToUnsigned(recArray[j].dst_port));%>
        </td> 
        <td width="10%">
                <%out.println(((recArray[j].ip_src>>24) & 0xFF) + "." + ((recArray[j].ip_src >> 16) & 0xFF) + "." + ((recArray[j].ip_src>> 8) & 0xFF) +"."+ ((recArray[j].ip_src) & 0xFF));%>
        </td>  
        
        <td width="10%">
                <%out.println(((recArray[j].ip_dst >> 24) & 0xFF) + "." + ((recArray[j].ip_dst >> 16) & 0xFF) + "." + ((recArray[j].ip_dst>> 8) & 0xFF) +"."+ ((recArray[j].ip_dst) & 0xFF));
            %>
        </td>  
        <td width="10%">
                <%out.println(recArray[j].protocol);%>
        </td>
        
        <td width="10%">
                <%out.println(recArray[j].nop);%>  
        </td>
        
        </tr>
         <%  } 
               }
        %>
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
<%}
}%>
