<%-- 
    Document   : packetsdisplay
    Created on : 26 Jan, 2012, 7:47:58 PM
    Author     : varun
--%>

<%@page import="Core.pkts"%>
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>HyperShark- Packets</title>
        <link rel="stylesheet" href="accordion/style.css" type="text/css" />
        <script type="text/javascript" src="accordion/script.js"></script>

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
                                        xmlhttp.open("GET","display2.jsp?index="+index,true);
                                        xmlhttp.send();

                                    
           } 
        </script>

        
        </head>
    <body>
    <% 
        int j=0;
        int i;
        //data d[]=new data[10];
        pkts f=new pkts(); 
        f.fill();
        if(session.getAttribute("packets")!=null)
            session.removeAttribute("packets");
        session.setAttribute("packets",f.packets);

        %> 
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
        <%int k=1; 
        j=0;
        while(j<3)
        { %>
        
        <tr>   
            
           
            
            <td width="10%">
                <a href="javascript:getdata(<%=j%>)"><%out.println(k);
                k++;
                %></a>
            </td>    
            
        <td width="10%">   
        <%for (i=0;i<f.packets[j].pfpacket.smac.length; i++)
            { 
                out.print(Integer.toString((f.packets[j].pfpacket.smac[i] & 0xff) + 0x100, 16).substring(1));
                if(i!=f.packets[j].pfpacket.smac.length-1)
                out.print("-");
            }%>
        </td>
        <td width="10%">
            <%for (i=0;i<f.packets[j].pfpacket.dmac.length; i++)
            { 
                out.print(Integer.toString((f.packets[j].pfpacket.dmac[i] & 0xff) + 0x100, 16).substring(1));
                if(i!=f.packets[j].pfpacket.dmac.length-1)
                out.print("-");
            }%>
        </td> 
        <td width="10%">
                <%out.println(((f.packets[j].pfpacket.ip_src>>24) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_src >> 16) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_src >> 8) &0xFF)+"."+((f.packets[j].pfpacket.ip_src) & 0xFF));%></a>
        </td>  
        <td width="10%">
                <%out.println(((f.packets[j].pfpacket.ip_dst>>24) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_dst >> 16) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_dst >> 8) &0xFF)+"."+ ((f.packets[j].pfpacket.ip_dst) & 0xFF));%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].pfpacket.caplen);%>
        </td>  
        <td width="10%">
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
