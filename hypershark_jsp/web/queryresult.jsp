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
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<jsp:useBean id="dispFilter" class="Beans.DisplayTimeFilterBean" scope="page">
    <jsp:setProperty name="dispFilter" property="*"/>
</jsp:useBean>
<%UserDataBean userData=(UserDataBean)session.getAttribute("userData");
    int index;
if(userData==null){
    response.sendRedirect("WebPages/homepage.jsp");    
}    
else 
{    
    index=Integer.parseInt(request.getParameter("index"));
%>

<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Lucid- FlowRecords</title>
        <link href="style.css" rel="stylesheet" type="text/css" />

        <script type="text/javascript">
        function sortResult(param)
        {
           //alert("script working"+param); 
           window.location.href="queryresult.jsp?index=<%=index%>&sortingField="+param;
       
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
                <h1><font color="#52A300">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Lucid</font></h1>
            </div>
            <div class="header">
            <h3><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font>
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
               <a href="logout.jsp"><font color="#FFFFFF" size="4px">Logout</font></a>
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
        flowRec=dispFilter.queryPackets(dispRule,timRule,userData.getUserDetails().getVirMachineList().get(index).getDirPath());        
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
        if(session.getAttribute("flowArr")!=null)
            session.removeAttribute("flowArr");
        session.setAttribute("flowArr",recArray);
        if(dispFilter.getSortingField()!=null && (!dispFilter.getSortingField().equals("recNo")))
        {
            dispFilter.sortFlowRecord(recArray);
            
        }
        for(j=0;j<recArray.length;j++)
        { 
            if(recArray[j].getProtocol()==6)
                               {
        %>
        
        <tr onclick="show_packets(<%=j%>)" bgcolor="#00FFFF"  align="center" onmouseout="this.style.background='#00FFFF';" onmouseover="this.style.background='#EEEEEE';this.style.cursor='pointer';">   
            <%  }
            else 
            if(recArray[j].getProtocol()==17)    
                  {
            %>
            <tr onclick="show_packets(<%=j%>)" bgcolor="#FFFF5F"  align="center" onmouseout="this.style.background='#FFFF5F';" onmouseover="this.style.background='#EEEEEE';this.style.cursor='pointer';">   
                <%}
            else        
            if(recArray[j].getProtocol()==1)    
                  {
                  
                %>
            <tr onclick="show_packets(<%=j%>)" bgcolor="#D5F5F5"  align="center" onmouseout="this.style.background='#D5F5F5';" onmouseover="this.style.background='#EEEEEE';this.style.cursor='pointer';">   
                <%}
                else
                  {
                %>
            <tr onclick="show_packets(<%=j%>)" bgcolor="white"  align="center" onmouseout="this.style.background='white';" onmouseover="this.style.background='#EEEEEE';this.style.cursor='pointer';">   
                <%}
                %>
            
                
                <td width="10%">
                <%out.println(m);
                m++;
                %></a>
            </td>    
            
        <td width="10%">   
            <%if(recArray[j].getSrc_port_str()==null)
              out.println(Conversions.shortToUnsigned(recArray[j].src_port));
              else
              out.println(recArray[j].getSrc_port_str());
                   
            %>
        </td>
        <td width="10%">
            <%if(recArray[j].getDst_port_str()==null)
              out.println(Conversions.shortToUnsigned(recArray[j].dst_port));
              else
              out.println(recArray[j].getDst_port_str());
                   
            %>
        </td> 
        <td width="10%">
                <%out.println(((recArray[j].ip_src>>24) & 0xFF) + "." + ((recArray[j].ip_src >> 16) & 0xFF) + "." + ((recArray[j].ip_src>> 8) & 0xFF) +"."+ ((recArray[j].ip_src) & 0xFF));%>
        </td>  
        
        <td width="10%">
                <%out.println(((recArray[j].ip_dst >> 24) & 0xFF) + "." + ((recArray[j].ip_dst >> 16) & 0xFF) + "." + ((recArray[j].ip_dst>> 8) & 0xFF) +"."+ ((recArray[j].ip_dst) & 0xFF));
            %>
        </td>  
        <td width="10%">
                <%if(recArray[j].getProtocol_str()== null)
              out.println(recArray[j].getProtocol());
              else
              out.println(recArray[j].getProtocol_str());
                   
            %>
        </td>
        
        <td width="10%">
                <%out.println(recArray[j].nop);%>  
        </td>
        
        </tr>
             <%
        } 
               }
               
        %>
        </table>
        </div>
        
        <br/><br/>			
	<div class="footer">
	   <p>&copy Lucid.com</p> <!-- Change the copyright notice -->
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
