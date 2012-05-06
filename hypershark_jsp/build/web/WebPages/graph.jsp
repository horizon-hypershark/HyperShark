<%-- 
    Document   : graph
    Created on : 27 Jan, 2012, 9:06:17 PM
    Author     : varun
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
        <script type="text/javascript">
            function doClick()
            {               
                //document.getElementById("date").style.display="block";
                //document.getElementById("date").style.visibility="visible";
                var str="<form method=\"get\" action=\"piechart/bargraph.jsp\"> <input type=\"text\" name=\"date_selected\" id=\"datepicker\" onmouseover=\"showdate()\" onmouseout=\"this.value='Enter Date'\" value=\"Enter Date\"/>     <input type=\"submit\" value=\"Show Graph\" name=\"showGraph\" \"/></form>";
                document.getElementById("show_textbox").innerHTML=str;
            }
        </script>   
        
        <script type="text/javascript">
            function hidetext()
            {
                document.getElementById("date").style.display="none";
                document.getElementById("date").style.visibility="hidden";
            }
        </script>   
        
        
    </head>
    <body>
              
        <br/><br/><br/>       
        <b><a href="#" onclick="doClick()">Bytes Transferred per day</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b><div id="show_textbox"></div><br/><br/>                                      
        <b><p><a href="piechart/piechart.jsp?param=srcIp">Most Frequent Ips</a></p><br/></b>
        <b><p><a href="piechart/piechart.jsp?param=srcPort">Most Frequent Ports</a></p><br/></b>
        <!--<b><p><a href="newlinechart.jsp">No Of Packets per IP</a></p><br/></b> 
        <b><p><a href="newlinechart.jsp">No Of Packets against Time</a></p><br/></b>-->
        
                
    </body>
</html>
