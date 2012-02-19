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
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
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
        String str=request.getParameter("index");
         int index=Integer.parseInt(str);
         
        ArrayList<FlowRecord> flowList= (ArrayList<FlowRecord>)session.getAttribute("flows");
        if(session.getAttribute("flowpackets")!=null)
            session.removeAttribute("flowpackets");
        session.setAttribute("flowpackets",flowList.get(index).getPackets());
        int i=0,j=0,k=1;%>
        <table border="0">
            <tr>
            <td>
            <h><b>PFRING HEADER</b></h>
            </td>    
            </tr>
            
            <tr>
            <td width="10%"> <b> PKT NO.</b></td>   
            <td width="10%"> <b> SOURCE MAC</b></td>
            <td width="10%"> <b> DESTINATION MAC</b></td>
            <td width="10%"> <b> IP SRC </b></td>
            <td width="10%"> <b> IP DEST </b></td>
            <td width="10%"> <b> PROTOCOL</b></td>    
            <td width="10%"> <b> CAPLEN</b></td>
               
            </tr>
      
            <%for(CompletePacket pkts : flowList.get(index).getPackets())
              {%>
                
              <tr>   
                        
            <td width="10%">
                <a href="javascript:getdata(<%=j%>)"><%out.println(k);
                k++;
                %></a>
            </td>    
            
        <td width="10%">   
        <%for (i=0;i<pkts.pfpacket.smac.length; i++)
            { 
                out.print(Integer.toString((pkts.pfpacket.smac[i] & 0xff) + 0x100, 16).substring(1));
                if(i!=pkts.pfpacket.smac.length-1)
                out.print("-");
            }%>
        </td>
        <td width="10%">
            <%for (i=0;i<pkts.pfpacket.dmac.length; i++)
            { 
                out.print(Integer.toString((pkts.pfpacket.dmac[i] & 0xff) + 0x100, 16).substring(1));
                if(i!=pkts.pfpacket.dmac.length-1)
                out.print("-");
            }%>
        </td> 
        <td width="10%">
                <%out.println(((pkts.pfpacket.ip_src>>24) & 0xFF) + "." + ((pkts.pfpacket.ip_src >> 16) & 0xFF) + "." + ((pkts.pfpacket.ip_src >> 8) &0xFF)+"."+((pkts.pfpacket.ip_src) & 0xFF));%></a>
        </td>  
        <td width="10%">
                <%out.println(((pkts.pfpacket.ip_dst>>24) & 0xFF) + "." + ((pkts.pfpacket.ip_dst >> 16) & 0xFF) + "." + ((pkts.pfpacket.ip_dst >> 8) &0xFF)+"."+ ((pkts.pfpacket.ip_dst) & 0xFF));%>
        </td>  
        <td width="10%">
                <%out.println(pkts.pfpacket.caplen);%>
        </td>  
        <td width="10%">
                <%out.println(pkts.pfpacket.l3_proto);%>
        </td>
            
        </tr>
         <% j++; } %>
        </table>
        <div id="Div">
        </div>    
        <br/>
        <br/>
        <br/>
        
    </body>
</html>
