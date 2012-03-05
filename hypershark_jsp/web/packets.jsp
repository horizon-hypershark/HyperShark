<%-- 
    Document   : packets
    Created on : 4 Feb, 2012, 7:43:01 AM
    Author     : root
--%>

<%@page import="Core.CompletePacket"%>
<%@page import="java.util.ArrayList"%>
<%@page import="Core.FlowRecord"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
        <title>HyperShark-Packets</title>
        <link rel="stylesheet" href="accordion/style.css" type="text/css" />
        <link href="style.css" rel="stylesheet" type="text/css" />

        <style type="text/css">
            body
            {
                background-image:url('background.png');
            }
        </style>


        <script type="text/javascript" src="accordion/script.js"></script>

    </head>
    <body>
        <script type="text/javascript">
            function getdata(index)
            {
                //alert("getdata called");
                var xmlhttp;
                if (window.XMLHttpRequest)
                {// code for IE7+, Firefox, Chrome, Opera, Safari
                    xmlhttp=new XMLHttpRequest();
                }
                else
                {// code for IE6, IE5
                    xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
                }
                xmlhttp.onreadystatechange=function()
                {
                    if (xmlhttp.readyState==4 && xmlhttp.status==200)
                    {
                        document.getElementById("Div").innerHTML=xmlhttp.responseText;
                    }
                }
                xmlhttp.open("GET","flowpacketsdisplay.jsp?index="+index,true);
                xmlhttp.send();

                                    
            } 
        </script>


        <%
            String str = request.getParameter("index");
            int index = Integer.parseInt(str);

            ArrayList<FlowRecord> flowList = (ArrayList<FlowRecord>) session.getAttribute("flows");
            if (session.getAttribute("flowpackets") != null) {
                session.removeAttribute("flowpackets");
            }
            session.setAttribute("flowpackets", flowList.get(index).getPackets());
            int i = 0, j = 0, k = 1;%>

        <div class="section" id="page">

            <div class="header">
               <h1><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
                <h2><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font>
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
               <a href="logout.jsp"><font color="#FFFFFF" size="4px">Logout</font></a>
                
                
                
                </h2>
 
                
                
                 </div>

            <br/><br/>
            <div class="articleBody clear" id="articles">
                
             
                <table border="0">
                    
                    <tr align="center" width="100%">
                        <td width="22%"><font size="2px"> <b>Source Mac</b></font></td>
                        <td width="22%"><font size="2px"> <b> Destination Mac</b></font></td>
                        <td width="22%"><font size="2px"><b> Source IP </b></font></td>
                        <td width="22%"><font size="2px"><b> Dest IP </b></font></td>
                        <td width="6%"><font size="2px"><b> Protocol</b></font></td>    
                        <td width="6%"> <font size="2px"><b> Caplen</b></font></td>

                    </tr>
                </table>
                   
                <div id="accordion">
                    <dl class="accordion" id="slider">


                         
                        <%for (CompletePacket pkts : flowList.get(index).getPackets()) {%>

                        <dt><%out.print(pkts.pfpacket.getPacket());%></dt>
                        <dd>
                            <span>
                                <%out.print(pkts.l2Packet.getPacket());%>
                                <br/>
                                <%out.print(pkts.l3Packet.getPacket());%>
                                <br/>
                                <%out.print(pkts.l4Packet.getPacket());%>
                                <br/>
                            </span>
                        </dd>



                        <%}%>
                    </dl>
                </div>
            </div>
            <script type="text/javascript">

                var slider1=new accordion.slider("slider1");
                slider1.init("slider");
            </script>
            <br/><br/>			
	<div class="footer">
	   <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->
	   <a href="#" class="up">Go UP</a>
	</div>
        </div>

            
            <br/>
            <br/>
            <br/>

    </body>
</html>
