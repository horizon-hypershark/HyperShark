<%@page import="FileAccess.ReadFlow"%>
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
 <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>line chart</title>
    <link href="style2.css" rel="stylesheet" type="text/css">
   
    <script language="javascript" type="text/javascript" src="jquery.js"></script>
    <script language="javascript" type="text/javascript" src="jquery.flot.js"></script>
     <script language="javascript" type="text/javascript" src="jquery.flot.pie.js"></script>
 </head>
    <body>
    <h1></h1>
          

<script type="text/javascript">

$(function () {

	// data
	var count =6;
    var ip1 = [["111.111.111.111"],["182.123.221.122"],["122.122.21.11"],["11.21.33.11"],["111.21.1.1"],["122.323.122.122"]];
	var freq = [20,30,40,90,1,12];
/*	var data = [

		{ label: ip1[0],  data: 10},

		{ label: ip1[1],  data: 30},

		{ label: ip1[2],  data: 90},

		{ label: ip1[3],  data: 70},

		{ label: ip1[4],  data: 80},

		{ label: ip1[5],  data: 110}

	];  */

	/*var data = [

		{ label: "Series1",  data: [[1,10]]},

		{ label: "Series2",  data: [[1,30]]},

		{ label: "Series3",  data: [[1,90]]},

		{ label: "Series4",  data: [[1,70]]},

		{ label: "Series5",  data: [[1,80]]},

		{ label: "Series6",  data: [[1,0]]}

	];*/

	var data = [];

	for( var i = 0; i<count; i++)

	{

		data[i] = { label: ip1[i], data: freq[i] }

	}


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

 </body>
</html>
