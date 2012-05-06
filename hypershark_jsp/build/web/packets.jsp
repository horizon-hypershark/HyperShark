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
        <title>Lucid-Packets</title>
        <link rel="stylesheet" href="accordion/style.css" type="text/css" />
        <link href="style.css" rel="stylesheet" type="text/css" />
        <link rel="stylesheet" href="menu.css" type="text/css" media="screen" /><!--for dropdown filters-->
        

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

           // ArrayList<FlowRecord> flowList = (ArrayList<FlowRecord>) session.getAttribute("flows");
            if (session.getAttribute("flowpackets") != null) {
                session.removeAttribute("flowpackets");
            }
            FlowRecord flowArr[]=(FlowRecord [])session.getAttribute("flowArr");
            //session.setAttribute("flowpackets", flowList.get(index).getPackets());
            session.setAttribute("flowpackets", flowArr[index].getPackets());
            int i = 0, j = 0, k = 1;%>

        <div class="section" id="page">

            <div class="title">
                <br/>
                <h1><font color="#52A300">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Lucid</font></h1>
                <br/>
            </div>
            <div class="header">
            <h2><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font>
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
               <a href="logout.jsp"><font color="#FFFFFF" size="4px">Logout</font></a>
             </h2>       
                
            </div>
            <br/><br/>
             <!--change for more filters is here-->
                  
             <!--change ends-->
            
            
            <br/><br/>
            <div class="articleBody clear" id="articles">
                
                    <table border="0">
                    
                    <tr align="center" width="100%">
                        <td width="23%"><font size="2px"> <b>Source Mac</b></font></td>
                        <td width="23%"><font size="2px"> <b> Destination Mac</b></font></td>
                        <td width="20%"><font size="2px"><b> Source IP </b></font></td>
                        <td width="22%"><font size="2px"><b> Dest IP </b></font></td>
                        <td width="6%"><font size="2px"><b>Protocol</b></font></td>    
                        <td width="6%"> <font size="2px"><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Size</b></font></td>

                    </tr>
                </table>
                   
                <div id="accordion">
                    <dl class="accordion" id="slider">


                         
                        <%--  <%for (CompletePacket pkts : flowList.get(index).getPackets()) {%>--%>


                        <%for (CompletePacket pkts : flowArr[index].getPackets()) {%>

                        <dt><%out.print(pkts.getPfpacket().getPacket(pkts.getAppProto()));%></dt>
                        <dd>
                            <span>
                                <%out.print(pkts.getL2Packet().getPacket());%>
                                <br/>
                                <%if(pkts.getL3Packet()!=null)
                                out.print(pkts.getL3Packet().getPacket());%>
                                <br/>
                                <%if(pkts.getL4Packet()!=null)
                                out.print(pkts.getL4Packet().getPacket());%>
                                <br/>
                                <%if(pkts.getBuffer()!=null)     
                                {
                                    String value = new String(pkts.getBuffer());
                                    System.out.println("Start::"+value+"::End");
                                    out.print("<b>APPLICATION DATA</b>"+"<br/><b>PROTOCOL</b>:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
                                    out.print(pkts.getAppProto());
                                    out.print("<br/><b>DETAILS</b>:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
                                    out.print(value);
                                }
                                %>
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
	   <p>&copy Lucid.com</p> <!-- Change the copyright notice -->
	   <a href="#" class="up">Go UP</a>
	</div>
        </div>

            
            <br/>
            <br/>
            <br/>

    </body>
</html>
