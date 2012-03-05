
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
 <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>Flot Pie Examples</title>
	<!--[if lte IE 8]><script language="javascript" type="text/javascript" src="../excanvas.min.js"></script><![endif]-->
    <script language="javascript" type="text/javascript" src="jquery.js"></script>
	<script language="javascript" type="text/javascript" src="jquery.flot.js"></script>
    <script language="javascript" type="text/javascript" src="jquery.flot.pie.js"></script>
	
<script type="text/javascript">
$(function () {
	// data
	/*var data = [
		{ label: "Series1",  data: 10},
		{ label: "Series2",  data: 30},
		{ label: "Series3",  data: 90},
		{ label: "Series4",  data: 70},
		{ label: "Series5",  data: 80},
		{ label: "Series6",  data: 110}
	];*/
	/*var data = [
		{ label: "Series1",  data: [[1,10]]},
		{ label: "Series2",  data: [[1,30]]},
		{ label: "Series3",  data: [[1,90]]},
		{ label: "Series4",  data: [[1,70]]},
		{ label: "Series5",  data: [[1,80]]},
		{ label: "Series6",  data: [[1,0]]}
	];*/
	var data = [];
	var series = Math.floor(Math.random()*10)+1;
	for( var i = 0; i<series; i++)
	{
		data[i] = { label: "Series"+(i+1), data: Math.floor(Math.random()*100)+1 }
	}

	// DEFAULT
    $.plot($("#default"), data, 
	{
		series: {
			pie: { 
				show: true
			}
		}
	});
	
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
	
	// GRAPH 2
	$.plot($("#graph2"), data, 
	{
		series: {
			pie: { 
				show: true,
				radius: 1,
				label: {
					show: true,
					radius: 1,
					formatter: function(label, series){
						return '<div style="font-size:8pt;text-align:center;padding:2px;color:white;">'+label+'<br/>'+Math.round(series.percent)+'%</div>';
					},
					background: { opacity: 0.8 }
				}
			}
		},
		legend: {
			show: false
		}
	});
	
	// GRAPH 3
	$.plot($("#graph3"), data, 
	{
		series: {
			pie: { 
				show: true,
				radius: 1,
				label: {
					show: true,
					radius: 3/4,
					formatter: function(label, series){
						return '<div style="font-size:8pt;text-align:center;padding:2px;color:white;">'+label+'<br/>'+Math.round(series.percent)+'%</div>';
					},
					background: { opacity: 0.5 }
				}
			}
		},
		legend: {
			show: false
		}
	});
	
	// GRAPH 4
	$.plot($("#graph4"), data, 
	{
		series: {
			pie: { 
				show: true,
				radius: 1,
				label: {
					show: true,
					radius: 3/4,
					formatter: function(label, series){
						return '<div style="font-size:8pt;text-align:center;padding:2px;color:white;">'+label+'<br/>'+Math.round(series.percent)+'%</div>';
					},
					background: { 
						opacity: 0.5,
						color: '#000'
					}
				}
			}
		},
		legend: {
			show: false
		}
	});
	
	// GRAPH 5
	$.plot($("#graph5"), data, 
	{
		series: {
			pie: { 
				show: true,
				radius: 3/4,
				label: {
					show: true,
					radius: 3/4,
					formatter: function(label, series){
						return '<div style="font-size:8pt;text-align:center;padding:2px;color:white;">'+label+'<br/>'+Math.round(series.percent)+'%</div>';
					},
					background: { 
						opacity: 0.5,
						color: '#000'
					}
				}
			}
		},
		legend: {
			show: false
		}
	});
	
	// GRAPH 6
	$.plot($("#graph6"), data, 
	{
		series: {
			pie: { 
				show: true,
				radius: 1,
				label: {
					show: true,
					radius: 2/3,
					formatter: function(label, series){
						return '<div style="font-size:8pt;text-align:center;padding:2px;color:white;">'+label+'<br/>'+Math.round(series.percent)+'%</div>';
					},
					threshold: 0.1
				}
			}
		},
		legend: {
			show: false
		}
	});
	
	// GRAPH 7
	$.plot($("#graph7"), data, 
	{
		series: {
			pie: { 
				show: true,
				combine: {
					color: '#999',
					threshold: 0.1
				}
			}
		},
		legend: {
			show: false
		}
	});
	
	// GRAPH 8
	$.plot($("#graph8"), data, 
	{
		series: {
			pie: { 
				show: true,
				radius:300,
				label: {
					show: true,
					formatter: function(label, series){
						return '<div style="font-size:8pt;text-align:center;padding:2px;color:white;">'+label+'<br/>'+Math.round(series.percent)+'%</div>';
					},
					threshold: 0.1
				}
			}
		},
		legend: {
			show: false
		}
	});
	
	// GRAPH 9
	$.plot($("#graph9"), data, 
	{
		series: {
			pie: { 
				show: true,
				radius: 1,
				tilt: 0.5,
				label: {
					show: true,
					radius: 1,
					formatter: function(label, series){
						return '<div style="font-size:8pt;text-align:center;padding:2px;color:white;">'+label+'<br/>'+Math.round(series.percent)+'%</div>';
					},
					background: { opacity: 0.8 }
				},
				combine: {
					color: '#999',
					threshold: 0.1
				}
			}
		},
		legend: {
			show: false
		}
	});
	
	// DONUT
    $.plot($("#donut"), data, 
	{
		series: {
			pie: { 
				innerRadius: 0.5,
				show: true
			}
		}
	});

	// INTERACTIVE
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
    <h1>Flot Pie Examples</h1>

	<h2>Default with Legend</h2>
    <div id="default" class="graph"></div>
	<label for="default">
		Default pie graph with no options set.
		<code>
$.plot($("#default"), data,<br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
});<br/>
		</code>
	</label>

	<h2>Default without Legend</h2>
    <div id="graph1" class="graph"></div>
	<label for="graph1">
		Default pie graph when legend is disabled. Since the labels would normally be outside the container, the graph is resized to fit.
		<code>
$.plot($("#graph1"), data, <br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>legend: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: false<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}</b><br/>
});<br/>
		</code>
	</label>
	
	<h2>Graph2</h2>
    <div id="graph2" class="graph"></div>
	<label for="graph2">
		Added a semi-transparent background to the labels and a custom labelFormatter function.
		<code>
$.plot($("#graph2"), data, <br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;radius: 1,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>label: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;radius: 1,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;formatter: function(label, series){<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return '&lt;div style="font-size:8pt;text-align:center;padding:2px;color:white;"&gt;'+label+'&lt;br/&gt;'+Math.round(series.percent)+'%&lt;/div&gt;';<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;background: { opacity: 0.8 }<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;legend: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: false<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
});<br/>
		</code>
	</label>
	
	<h2>Graph3</h2>
    <div id="graph3" class="graph"></div>
	<label for="graph3">
		Slightly more transparent label backgrounds and adjusted the radius values to place them within the pie.
		<code>
$.plot($("#graph3"), data, <br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;radius: 1,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;label: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>radius: 3/4,</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;formatter: function(label, series){<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return '&lt;div style="font-size:8pt;text-align:center;padding:2px;color:white;"&gt;'+label+'&lt;br/&gt;'+Math.round(series.percent)+'%&lt;/div&gt;';<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>background: { opacity: 0.5 }</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;legend: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: false<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
});<br/>
		</code>
	</label>
	
	<h2>Graph4</h2>
    <div id="graph4" class="graph"></div>
	<label for="graph4">
		Semi-transparent, black-colored label background.
		<code>
$.plot($("#graph4"), data, <br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;radius: 1,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;label: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;radius: 3/4,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;formatter: function(label, series){<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return '&lt;div style="font-size:8pt;text-align:center;padding:2px;color:white;"&gt;'+label+'&lt;br/&gt;'+Math.round(series.percent)+'%&lt;/div&gt;';<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;background: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;opacity: 0.5,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>color: '#000'</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;legend: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: false<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
});<br/>
		</code>
	</label>
	
	<h2>Graph5</h2>
    <div id="graph5" class="graph"></div>
	<label for="graph5">
		Semi-transparent, black-colored label background placed at pie edge.
		<code>
$.plot($("#graph5"), data, <br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>radius: 3/4,</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;label: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;radius: 3/4,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;formatter: function(label, series){<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return '&lt;div style="font-size:8pt;text-align:center;padding:2px;color:white;"&gt;'+label+'&lt;br/&gt;'+Math.round(series.percent)+'%&lt;/div&gt;';<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;background: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;opacity: 0.5,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;color: '#000'<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;legend: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: false<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
});<br/>
		</code>
	</label>
	
	<h2>Graph6</h2>
    <div id="graph6" class="graph"></div>
	<label for="graph6">
		Labels can be hidden if the slice is less than a given percentage of the pie (10% in this case).
		<br><span style="color: red">Note: you may need to refresh the page to see this effect.</span>
		<code>
$.plot($("#graph6"), data, <br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>radius: 1,</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;label: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>radius: 2/3,</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;formatter: function(label, series){<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return '&lt;div style="font-size:8pt;text-align:center;padding:2px;color:white;"&gt;'+label+'&lt;br/&gt;'+Math.round(series.percent)+'%&lt;/div&gt;';<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>threshold: 0.1</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;legend: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: false<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
});<br/>
		</code>
	</label>
	
	<h2>Graph7</h2>
    <div id="graph7" class="graph"></div>
	<label for="graph7">
		All slices less than a given percentage of the pie can be combined into a single, larger slice (10% in this case).
		<br><span style="color: red">Note: you may need to refresh the page to see this effect.</span>
		<code>
$.plot($("#graph7"), data, <br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>combine: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;color: '#999',<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;threshold: 0.1<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;legend: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: false<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
});<br/>
		</code>
	</label>
	
	<h2>Graph8</h2>
    <div id="graph8" class="graph"></div>
	<label for="graph8">
		The radius can also be set to a specific size (even larger than the container itself).
		<code>
$.plot($("#graph8"), data, <br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>radius:300,</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;label: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;formatter: function(label, series){<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return '&lt;div style="font-size:8pt;text-align:center;padding:2px;color:white;"&gt;'+label+'&lt;br/&gt;'+Math.round(series.percent)+'%&lt;/div&gt;';<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;threshold: 0.1<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;legend: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: false<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
});<br/>
		</code>
	</label>

	<h2>Graph9</h2>
    <div id="graph9" class="graph" style="height: 250px;"></div>
	<label for="graph9">
		The pie can be tilted at an angle.
		<code>
$.plot($("#graph9"), data, <br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;radius: 1,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>tilt: 0.5,</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;label: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;radius: 1,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;formatter: function(label, series){<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return '&lt;div style="font-size:8pt;text-align:center;padding:2px;color:white;"&gt;'+label+'&lt;br/&gt;'+Math.round(series.percent)+'%&lt;/div&gt;';<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;background: { opacity: 0.8 }<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;combine: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;color: '#999',<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;threshold: 0.1<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;legend: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: false<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
});<br/>
		</code>
	</label>
	
	<h2>Donut</h2>
    <div id="donut" class="graph"></div>
	<label for="donut">
		A donut hole can be added.
		<code>
$.plot($("#donut"), data,<br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>innerRadius: 0.5,</b><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
});<br/>
		</code>
	</label>
	
	<h2>Interactive</h2>
    <div id="interactive" class="graph"></div>
	<label for="interactive">
		The pie can be made interactive with hover and click events.
		<code>
$.plot($("#interactive"), data,<br/>
{<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;series: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pie: { <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;show: true<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>grid: {<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;hoverable: true,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;clickable: true<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}</b><br/>
});<br/>
<b>$("#interactive").bind("plothover", pieHover);<br/>
$("#interactive").bind("plotclick", pieClick);</b><br/>
		</code>
		<div id="hover"></div>
	</label>
		
	<h2>Pie Options</h2>
	<ul class="options">
		<li style="border-bottom: 1px dotted #ccc;"><b>option:</b> <i>default value</i> - Description of option</li>
		<li><b>show:</b> <i>false</i> - Enable the plugin and draw as a pie.</li>
		<li><b>radius:</b> <i>'auto'</i> - Sets the radius of the pie. If value is between 0 and 1 (inclusive) then it will use that as a percentage of the available space (size of the container), otherwise it will use the value as a direct pixel length. If set to 'auto', it will be set to 1 if the legend is enabled and 3/4 if not.</li>
		<li><b>innerRadius:</b> <i>0</i> - Sets the radius of the donut hole. If value is between 0 and 1 (inclusive) then it will use that as a percentage of the radius, otherwise it will use the value as a direct pixel length.</li>
		<li><b>startAngle:</b> <i>3/2</i> - Factor of PI used for the starting angle (in radians) It can range between 0 and 2 (where 0 and 2 have the same result).</li>
		<li><b>tilt:</b> <i>1</i> - Percentage of tilt ranging from 0 and 1, where 1 has no change (fully vertical) and 0 is completely flat (fully horizontal -- in which case nothing actually gets drawn).</li>
		<li><b>offset:</b> <ul>
			<li><b>top:</b> <i>0</i> - Pixel distance to move the pie up and down (relative to the center).</li>
			<li><b>left:</b> <i>'auto'</i> - Pixel distance to move the pie left and right (relative to the center).</li>
		</ul>
		<li><b>stroke:</b> <ul>
			<li><b>color:</b> <i>'#FFF'</i> - Color of the border of each slice. Hexadecimal color definitions are prefered (other formats may or may not work).</li>
			<li><b>width:</b> <i>1</i> - Pixel width of the border of each slice.</li>
		</ul>
		<li><b>label:</b> <ul>
			<li><b>show:</b> <i>'auto'</i> - Enable/Disable the labels. This can be set to true, false, or 'auto'. When set to 'auto', it will be set to false if the legend is enabled and true if not.</li>
			<li><b>radius:</b> <i>1</i> - Sets the radius at which to place the labels. If value is between 0 and 1 (inclusive) then it will use that as a percentage of the available space (size of the container), otherwise it will use the value as a direct pixel length.</li>
			<li><b>threshold:</b> <i>0</i> - Hides the labels of any pie slice that is smaller than the specified percentage (ranging from 0 to 1) i.e. a value of '0.03' will hide all slices 3% or less of the total.</li>
			<li><b>formatter:</b> <i>[function]</i> - This function specifies how the positioned labels should be formatted, and is applied after the legend's labelFormatter function. The labels can also still be styled using the class "pieLabel" (i.e. ".pieLabel" or "#graph1 .pieLabel").</li>
			<li><b>radius:</b> <i>1</i> - Sets the radius at which to place the labels. If value is between 0 and 1 (inclusive) then it will use that as a percentage of the available space (size of the container), otherwise it will use the value as a direct pixel length.</li>
			<li><b>background:</b> <ul>
				<li><b>color:</b> <i>null</i> - Backgound color of the positioned labels. If null, the plugin will automatically use the color of the slice.</li>
				<li><b>opacity:</b> <i>0</i> - Opacity of the background for the positioned labels. Acceptable values range from 0 to 1, where 0 is completely transparent and 1 is completely opaque.</li>
			</ul>
		</ul>
		<li><b>combine:</b> <ul>
			<li><b>threshold:</b> <i>0</i> - Combines all slices that are smaller than the specified percentage (ranging from 0 to 1) i.e. a value of '0.03' will combine all slices 3% or less into one slice).</li>
			<li><b>color:</b> <i>null</i> - Backgound color of the positioned labels. If null, the plugin will automatically use the color of the first slice to be combined.</li>
			<li><b>label:</b> <i>'Other'</i> - Label text for the combined slice.</li>
		</ul>
		<li><b>highlight:</b> <ul>
			<li><b>opacity:</b> <i>0.5</i> - Opacity of the highlight overlay on top of the current pie slice. Currently this just uses a white overlay, but support for changing the color of the overlay will also be added at a later date.
		</ul>
	</ul>
	
	<h2>Changes/Features</h2>
	<ul>
		<li style="list-style: none;"><i>v1.0 - November 20th, 2009 - Brian Medendorp</i></li>
		<li>The pie plug-in is now part of the Flot repository! This should make it a lot easier to deal with.</li>
		<li>Added a new option (innerRadius) to add a "donut hole" to the center of the pie, based on comtributions from Anthony Aragues. I was a little reluctant to add this feature because it doesn't work very well with the shadow created for the tilted pie, but figured it was worthwhile for non-tilted pies. Also, excanvas apparently doesn't support compositing, so it will fall back to using the stroke color to fill in the center (but I recommend setting the stroke color to the background color anyway).</li>
		<li>Changed the lineJoin for the border of the pie slices to use the 'round' option. This should make the center of the pie look better, particularly when there are numerous thin slices.</li>
		<li>Included a bug fix submitted by btburnett3 to display a slightly smaller slice in the event that the slice is 100% and being rendered with Internet Explorer. I haven't experienced this bug myself, but it doesn't seem to hurt anything so I've included it.</li>
		<li>The tilt value is now used when calculating the maximum radius of the pie in relation to the height of the container. This should prevent the pie from being smaller than it needed to in some cases, as well as reducing the amount of extra white space generated above and below the pie.</li>
		<li><b>Hover and Click functionality are now availabe!</b><ul>
			<li>Thanks to btburnett3 for the original hover functionality and Anthony Aragues for the modification that makes it compatable with excanvas, this was a huge help!</li>
			<li>Added a new option (highlight opacity) to modify the highlight created when mousing over a slice. Currently this just uses a white overlay, but an option to change the hightlight color will be added when the appropriate functionality becomes available.
			<li>I had a major setback that required me to practically rebuild the hover/click events from scratch one piece at a time (I discovered that it only worked with a single pie on a page at a time), but the end result ended up being virtually identical to the original, so I'm not quite sure what exactly made it work.</li>
			<li><span style="color: red;">Warning:</span> There are some minor issues with using this functionality in conjuction with some of the other more advanced features (tilt and donut). When using a donut hole, the inner portion still triggers the events even though that portion of the pie is no longer visible. When tilted, the interactive portions still use the original, untilted version of the pie when determining mouse position (this is because the isPointInPath function apparently doesn't work with transformations), however hover and click both work this way, so the appropriate slice is still highlighted when clicking, and it isn't as noticable of a problem.</li>
		</ul></li>
		<li>Included a bug fix submitted by Xavi Ivars to fix array issues when other javascript libraries are included in addition to jQuery</li>
		<br/>
		<li style="list-style: none;"><i>v0.4 - July 1st, 2009 - Brian Medendorp</i></li>
		<li>Each series will now be shown in the legend, even if it's value is zero. The series will not get a positioned label because it will overlap with the other labels present and often makes them unreadable.</li>
		<li>Data can now be passed in using the standard Flot method using an array of datapoints, the pie plugin will simply use the first y-value that it finds for each series in this case. The plugin uses this datastructure internally, but you can still use the old method of passing in a single numerical value for each series (the plugin will convert it as necessary). This should make it easier to transition from other types of graphs (such as a stacked bar graph) to a pie.</li>
		<li>The pie can now be tilted at an angle with a new "tilt" option. Acceptable values range from 0-1, where 1 has no change (fully vertical) and 0 is completely flat (fully horizontal -- in which case nothing actually gets drawn). If the plugin determines that it will fit within the canvas, a drop shadow will be drawn under the tilted pie (this also requires a tilt value of 0.8 or less).</li>
		<br/>
		<li style="list-style: none;"><i>v0.3.2 - June 25th, 2009 - Brian Medendorp</i></li>
		<li>Fixed a bug that was causing the pie to be shifted too far left or right when the legend is showing in some cases.</li>
		<br/>
		<li style="list-style: none;"><i>v0.3.1 - June 24th, 2009 - Brian Medendorp</i></li>
		<li>Fixed a bug that was causing nothing to be drawn and generating a javascript error if any of the data values were set to zero.</li>
		<br/>
		<li style="list-style: none;"><i>v0.3 - June 23rd, 2009 - Brian Medendorp</i></li>
		<li>The legend now works without any modifications! Because of changes made to flot and the plugin system (thanks Ole Laursen!) I was able to simplify a number of things and am now able to use the legend without the direct access hack that was required in the previous version.</li>
		<br/>
		<li style="list-style: none;"><i>v0.2 - June 22nd, 2009 - Brian Medendorp</i></li>
		<li>The legend now works but only if you make the necessary changes to jquery.flot.js. Because of this, I changed the default values for pie.radius and pie.label.show to new 'auto' settings that change the default behavior of the size and labels depending on whether the legend functionality is available or not.</li>
		<br/>
		<li style="list-style: none;"><i>v0.1 - June 18th, 2009 - Brian Medendorp</i></li>
		<li>Rewrote the entire pie code into a flot plugin (since that is now an option), so it should be much easier to use and the code is cleaned up a bit. However, the (standard flot) legend is no longer available because the only way to prevent the grid lines from being displayed also prevents the legend from being displayed. Hopefully this can be fixed at a later date.</li>
		<li>Restructured and combined some of the options. It should be much easier to deal with now.</li>
		<li>Added the ability to change the starting point of the pie (still defaults to the top).</li>
		<li>Modified the default options to show the labels to compensate for the lack of a legend.</li>
		<li>Modified this page to use a random dataset. <span style="color: red">Note: you may need to refresh the page to see the effects of some of the examples.</span></li>
		<br/>
		<li style="list-style: none;"><i>May 21st, 2009 - Brian Medendorp</i></li>
		<li>Merged original pie modifications by Sergey Nosenko into the latest SVN version <i>(as of May 15th, 2009)</i> so that it will work with ie8.</li>
		<li>Pie graph will now be centered in the canvas unless moved because of the legend or manually via the options. Additionally it prevents the pie from being moved beyond the edge of the canvas.</li>
		<li>Modified the code related to the labelFormatter option to apply flot's legend labelFormatter first. This is so that the labels will be consistent, but still provide extra formatting for the positioned labels (such as adding the percentage value).</li>
		<li>Positioned labels now have their backgrounds applied as a seperate element (much like the legend background) so that the opacity value can be set independently from the label itself (foreground). Additionally, the background color defaults to that of the matching slice.</li>
		<li>As long as the labelOffset and radiusLimit are not set to hard values, the pie will be shrunk if the labels will extend outside the edge of the canvas</li>
		<li>Added new options "radiusLimitFactor" and "radiusLimit" which limits how large the (visual) radius of the pie is in relation to the full radius (as calculated from the canvas dimensions) or a hard-pixel value (respectively). This allows for pushing the labels "outside" the pie.</li>
		<li>Added a new option "labelHidePercent" that does not show the positioned labels of slices smaller than the specified percentage. This is to help prevent a bunch of overlapping labels from small slices.</li>
		<li>Added a new option "sliceCombinePercent" that combines all slices smaller than the specified percentage into one larger slice. This is to help make the pie more attractive when there are a number of tiny slices. The options "sliceCombineColor" and "sliceCombineLabel" have also been added to change the color and name of the new slice if desired.</li>
		<li>Tested in Firefox (3.0.10, 3.5b4), Internet Explorer (6.0.2900, 7.0.5730, 8.0.6001), Chrome (1.0.154), Opera (9.64), and Safari (3.1.1, 4 beta 5528.16).
	</ul>

	
 </body>
</html>

