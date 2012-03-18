

<%@page import="Core.Graph.Statistics"%>
<%@page import="java.util.ArrayList"%>
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<jsp:useBean id="graphData" class="Beans.GraphBean" scope="page">
    <jsp:setProperty name="graphData" property="*"/>
</jsp:useBean>
<html>

 <head>

    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">

    <title>Pie Chart</title>

	<!--[if lte IE 8]><script language="javascript" type="text/javascript" src="../excanvas.min.js"></script><![endif]-->

    <!--change-->
    <link href="style.css" rel="stylesheet" type="text/css"/>
    <style type="text/css">
            body
            {
                background-image:url('background.png');
            }
        </style>
    <!--change ends-->    
      
    
    
    <script language="javascript" type="text/javascript" src="jquery.js"></script>

    <script language="javascript" type="text/javascript" src="jquery.flot.js"></script>

    <script language="javascript" type="text/javascript" src="jquery.flot.pie.js"></script>  

    
	

	<style type="text/css">

		* {

		  font-family: sans-serif;

		}

		

		body

		{

			padding: 0 1em 1em 1em;

		}

		

		div.graph

		{

			width: 600px;

			height: 320px;

			float: left;

			border: 1px dashed gainsboro;

		}

		

		label

		{

			display: block;

			margin-left: 400px;

			padding-left: 1em;

		}

		

		h2

		{

			padding-top: 1em;

			margin-bottom: 0;

			clear: both;

			color: #ccc;

		}

		

		code

		{

			display: block;

			background-color: #eee;

			border: 1px dashed #999;

			padding: 0.5em;

			margin: 0.5em;

			color: #666;

			font-size: 10pt;

		}

		

		code b

		{

			color: black;

		}

		

		ul

		{

			font-size: 10pt;

		}

		

		ul li

		{

			margin-bottom: 0.5em;

		}

		

		ul.options li

		{

			list-style: none;

			margin-bottom: 1em;

		}

		

		ul li i

		{

			color: #999;

		}

	</style>

 </head>

    <body>
        <% 
        ArrayList<Statistics> statList=graphData.getDataToPlot(5);
        %>
        <!--change-->
        <div class="section" id="page">
            <div class="title">
                <h1><font color="#52A300">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
            </div>
            <div class="header">
                <h3><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h3>
            </div>

            <br/><br/>
            <div class="articleBody clear" id="articles">
        <!--change ends-->
            <div id="interactive" class="graph">
        
        
            </div> 
    
        
        <script type="text/javascript">

$(function () {

	// data
	var count =6;
        //var ip1 = [["111.111.111.111"],["182.123.221.122"],["122.122.21.11"],["11.21.33.11"],["111.21.1.1"],["122.323.122.122"]];
	//var freq = [20,30,40,90,1,12];
	var data = [];
        <%
            int i=0;            
            for(Statistics stat :statList)
            {%>    
               data[<%=i++%>] = { label: "<%=stat.getAbscissa()%>", data: <%=stat.getOrdinate()%> }
            <%}           
         %>       
        	// GRAPH 1

	$.plot($("#interactive"), data,
{
        series: {
            pie: {
                show: true
            }
        },
        grid: {
            hoverable: true,
            clickable: true
        }
});
$("#interactive").bind("plothover", pieHover);
$("#interactive").bind("plotclick", pieClick);	

});

function pieHover(event, pos, obj) 
{
	if (!obj)
                return;
	percent = parseFloat(obj.series.percent).toFixed(2);
	$("#hover").html('<span style="font-weight: bold; color: '+obj.series.color+'">'+obj.series.label+' ('+percent+'%)</span>');
        
}

function pieClick(event, pos, obj) 
{
	if (!obj)
                return;
	percent = parseFloat(obj.series.percent).toFixed(2);
	alert(''+obj.series.label+': '+percent+'%');
}



</script>

        <body>
            
            <div id="hover"></div>
            <div class="info">
                <h4>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Details</h4>
                <%for(Statistics stat :statList)
                    {    
                        
                        out.println(stat.getInfo());%>
                        <br/>
                    <%}%>
                </font>
            </div>    
        </body>    
  </div>
    <br/><br/>
            <div class="footer">
                &copy HyperShark.com<!-- Change the copyright notice -->

            </div>
        </div>
  
 </body> 

</html>


