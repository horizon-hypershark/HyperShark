
<%@page import="Core.FlowRecord"%>
<%@page import="java.util.ArrayList"%>
<%@page import="FileAccess.ReadFlow"%>
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>

 <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>line chart</title>
    <link href="layout.css" rel="stylesheet" type="text/css">
    <!--[if lte IE 8]><script language="javascript" type="text/javascript" src="../excanvas.min.js"></script><![endif]-->
    
    <link href="style.css" rel="stylesheet" type="text/css" />
    <script language="javascript" type="text/javascript" src="jquery.js"></script>
    <script language="javascript" type="text/javascript" src="jquery.flot.js"></script>
    
    <style type="text/css">
    body {
                font-family:verdana;
                font-size:15px;
                background-image:url('background.png');
            }
     </style>       
    
  </head>
    <body>
        <%ArrayList<FlowRecord> flowList = (ArrayList<FlowRecord>) session.getAttribute("flows");
          //ReadFlow flow_obj=new ReadFlow(); 
          //flow_obj.readflow();
        %> 
        <% 
            //ReadFlow flow_obj=new ReadFlow(); 
            //flow_obj.readflow();
        %>
    <h1></h1>

    <div class="section" id="page"> 
            <div class="header"> 
                <h1><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
                <h2><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h2>

            </div>
            <br/>
            <br/>
            <br/>
        
    <div id="placeholder" style="width:800px;height:300px;"></div>
   <div class="links">
       <a href="newlinechart.jsp">View Next</a>
       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="tabs.jsp">Go Back</a>
   
   </div>    
<script type="text/javascript">
$(function () {
    var d1 = [];
	
   
	 var opt = {

          
            yaxis: {tickSize: 4},
            xaxis: {tickSize: 5}  <!-- label on x axis; if not specified,it will auto-generate depending upon input.same case for yaxis -->-

        };
<% 
   int i=0;
  
	for(FlowRecord flowRec:flowList)
	{ 
            if(i==30)
                break;
            %>
        <% out.print(flowRec.flow_id);%>
            d1.push([<%= flowRec.flow_id%>,<%=flowRec.nop%>]);
 <%
            i++;
       } %>
  
    
    $.plot($("#placeholder"),[{label : "No. of Packets per flow record",data:d1}],opt);
});
</script>
        <br/>
        <br/>
        <br/>
        <br/>
        
<div class="footer"> 
            <div class="line"></div>

            <p>&copy HyperShark.com</p> 

            <a href="#" class="up">Go UP</a>
        </div>
    </div>
 </body>
</html>
