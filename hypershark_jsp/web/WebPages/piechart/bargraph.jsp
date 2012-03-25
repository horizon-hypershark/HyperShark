<%-- 
    Document   : bargraph
    Created on : 23 Mar, 2012, 11:10:45 AM
    Author     : root
--%>

<%@page import="FileAccess.datatransfer"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<title>bar graph</title>
<head>
<script language="javascript" type="text/javascript" src="jquery.js"></script>
<script language="javascript" type="text/javascript" src="jquery.flot.js"></script>
</head>
<body>
<div id="placeholder" style="width: 775px; height: 300px;"></div>
<% datatransfer transfer=new datatransfer();
     
     String date=new String();
     date=request.getParameter("date_selected");
     String day = date.substring(3, 5);
     transfer.getCpt().setDay(Integer.parseInt(day));
     transfer.getCpt().setEnd_hr(0);
     transfer.getCpt().setEnd_min(0);
     String month = date.substring(0, 2);
     transfer.getCpt().setMonth(Integer.parseInt(month)-1);
     transfer.getCpt().setStart_hr(0);
     transfer.getCpt().setStart_min(0);
     transfer.setPath("/storage/hs1234/");
       
    transfer.fillData();
%>
<script id="source">
$(function () {
	  
          var ms_data = [{"label":"FOO","data":[
                    <%for(int i=0;i<24;i++){%>[<%=i%>,<%=transfer.getBytes_transfer()[i]%>],<%}%>]}];
          
	  var ms_ticks = [ <%for(int i=0;i<24;i++){%>[<%=i%>,<%=i%>],<%}%>];
	     
	    function plotWithOptions() {
	      $.plot($("#placeholder"), ms_data, {
	        bars: { show: true, barWidth: 0.6, series_spread: true, align: "center" },
	        xaxis: { ticks: ms_ticks, autoscaleMargin: .10 },
	        grid: { hoverable: true, clickable: true }
	      });
	    }
	     
	        function showTooltip(x, y, contents) {
	        $('<div id="tooltip">' + contents + '</div>').css( {
	            position: 'absolute',
	            display: 'none',
	            top: y + 5,
	            left: x + 5,
	            border: '1px solid #fdd',
	            padding: '2px',
	            'background-color': '#fee',
	            opacity: 0.80
	        }).appendTo("body").show();
	    }
	 
	    plotWithOptions();
	 
	        $("#placeholder").bind("plothover", function (event, pos, item) {
	          $("#x").text(pos.x.toFixed(2));
	      $("#y").text(pos.y.toFixed(2));
	        if (item) {
	            if (previousPoint != item.datapoint) {
	                previousPoint = item.datapoint;
	               
	                $("#tooltip").remove();
	                var x = item.datapoint[0].toFixed(2),
	                    y = item.datapoint[1].toFixed(2);
	               
	                showTooltip(item.pageX, item.pageY,
	                            item.series.label + " Group id: " + Math.floor(x) + ", y = " + y + ", seriesIndex: " + item.seriesIndex);
	            }
	        }
	        else {
	            $("#tooltip").remove();
	            previousPoint = null;           
	        }
	        });
	 
	    $("#placeholder").bind("plotclick", function (event, pos, item) {
	        if (item) {
	            $("#clickdata").text("You clicked bar " + item.dataIndex + " in " + item.series.label + ".");
	        }
	    });
	});
	</script>
</body>
</html>