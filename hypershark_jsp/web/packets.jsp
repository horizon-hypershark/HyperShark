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

            ArrayList<FlowRecord> flowList = (ArrayList<FlowRecord>) session.getAttribute("flows");
            if (session.getAttribute("flowpackets") != null) {
                session.removeAttribute("flowpackets");
            }
            session.setAttribute("flowpackets", flowList.get(index).getPackets());
            int i = 0, j = 0, k = 1;%>

        <div class="section" id="page">

            <div class="title">
                <h1><font color="#52A300">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
                <br/>
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
             <!--change-->
             <div class="filters">
             <ul id="menu">
            <li class="menu_right"><a href="#" class="drop">Filter More</a><!-- Begin 3 columns Item -->
                <div class="dropdown_3columns align_right"><!-- Begin 3 columns container -->
                    <div class="col_3">
                        <h2>Specify Filters</h2>
                    </div>
                    <div class="col_1">
                        <ul>			
                            <li>				
                                <form name="filter" action="capturefilter.jsp" method="get">
                                                                
                                Source Mac:<input type="text"/>
                                    <br/>
                                    Dest Mac:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="text"/>
                                    <br/>
                                    Source Ip:&nbsp;&nbsp;&nbsp;&nbsp;	
                                    <%for (int f = 1; f <= 4; f++) {%>
                                    <select name="srcOctet<%=f%>" id="srcOctet<%=f%>">
                                        <option>Select</option>
                                        <% int m = 0;
                                        while (m < 5) 
                                        {%>
                                        <option><%out.print(m);%></option>
                                        <% m++;
                                        }%>
                                    </select>
                                    <%if (f != 4)
                                    {
                                        out.print(":");
                                    }
                                  }%>                                                                                                                                       <br/>
                                  
                                  Dest Ip:&nbsp;&nbsp;&nbsp;&nbsp;	
                                    <%for (int f = 5; f <= 8; f++) {%>
                                    <select name="srcOctet<%=f%>" id="srcOctet<%=f%>">
                                        <option>Select</option>
                                        <% int m = 0;
                                        while (m < 255) 
                                        {%>
                                        <option><%out.print(m);%></option>
                                        <% m++;
                                        }%>
                                    </select>
                                    <%if (f != 8)
                                    {
                                        out.print(":");
                                    }
                                  }%>                                                                                                                                       <br/>
                                    Protocol:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="text"/>
                                  <br/>
                                    Size:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="text"/>
                                  <br/>
                                  <br/>
                                  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                  <input type="submit" value="Filter Results" align="right"/>
                                                                    
                                  
                                </form>
                            </li>		
                        </ul>                
                           
                    </div>
                </div><!-- End 3 columns container -->
            </li><!-- End 3 columns Item -->
        </ul>
        </div>     
             <!--change ends-->
            
            
            <br/><br/>
            <div class="articleBody clear" id="articles">
                
                    <table border="0">
                    
                    <tr align="center" width="100%">
                        <td width="22%"><font size="2px"> <b>Source Mac</b></font></td>
                        <td width="22%"><font size="2px"> <b> Destination Mac</b></font></td>
                        <td width="22%"><font size="2px"><b> Source IP </b></font></td>
                        <td width="22%"><font size="2px"><b> Dest IP </b></font></td>
                        <td width="6%"><font size="2px"><b> Protocol</b></font></td>    
                        <td width="6%"> <font size="2px"><b> Size</b></font></td>

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
