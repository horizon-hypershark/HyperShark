<%-- 
    Document   : display
    Created on : 16 Jan, 2012, 11:04:23 AM
    Author     : varun
--%>

<%@page import="Core.func"%>
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>HyperShark- Packets</title>
        <script type="text/javascript">
function loadXMLDoc(index)
{
var xmlhttp;

/*alert("i loadxml");*/
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
	   
	document.getElementById("myDiv").innerHTML=xmlhttp.responseText;
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
        func f=new func(); 
        f.fillpackets();
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
            <script type="text/javascript">
            var str_pkts= new Array(<%
        for(int l=0;l<3;l++)
        {
                       out.print("\""+f.packets[l].l2Packet.getPacket()+f.packets[l].l3Packet.getPacket()+f.packets[l].l4Packet.getPacket()+"\",");
        }   
        out.print("\"\"");
        %>);
           function getdata(index)
           {
               alert("in function "+index);
               document.getElementById("myDiv").innerHTML=str_pkts[index];
           }
            
        </script>

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
                <%out.println(f.packets[j].pfpacket.l3_proto);%>
        </td>
            
        </tr>
         <% j++; } %>
        </table>
        <p id="myDiv">
        </p>    
        <br/>
        <br/>
        <br/>
        
        </body>
</html>
