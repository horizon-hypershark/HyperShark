
<%@page import="FileAccess.ReadFlow"%>
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<html>

 <head>

    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">

    <title>Pie Chart</title>

	<!--[if lte IE 8]><script language="javascript" type="text/javascript" src="../excanvas.min.js"></script><![endif]-->

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

			width: 400px;

			height: 300px;

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
        ReadFlow flow_obj=new ReadFlow(); 
         flow_obj.readflow();
        %>

        <script type="text/javascript">

$(function () {

	// data
	var count =6;
  //  var ip1 = [["111.111.111.111"],["182.123.221.122"],["122.122.21.11"],["11.21.33.11"],["111.21.1.1"],["122.323.122.122"]];
//	var freq = [20,30,40,90,1,12];
	var data = [];
        <% 
           int i;
	for(i = 0; i<6; i++)

	{ %>

		data[i] = { label: <%= flow_obj.flow[i].ip_src%>, data:<%= flow_obj.flow[i].nop %> }

	<% } %>


	// GRAPH 1

	$.plot($("#graph1"), data, 

	{

		series: {

			pie: { 

				show: true

			}

		},

		legend: {

			show: false

		}

	});

	




});




</script>


 
    <div id="graph1" class="graph"></div>

 
  <!--  <div id="donut" class="graph"></div>

    <div id="interactive" class="graph"></div>  -->

 </body> 

</html>


