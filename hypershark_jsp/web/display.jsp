<%-- 
    Document   : display
    Created on : 16 Jan, 2012, 11:04:23 AM
    Author     : varun
--%>

<%@page import="Core.func2"%>
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
    </head>
    <body>
    <% 
        int j=0;
        int i;
        //data d[]=new data[10];
        func2 f=new func2(); 
        f.fillpackets();
        %>    
        <table border="1">
            <tr>
            <td width="20%"> <b> SOURCE MAC </b>
            <td width="20%"> <b> DESTINATION MAC</b>
            <td width="20%"> <b> PROTOCOL </b>
        </tr>
        <%while(j<4)
        { %>
        <tr>
            <% String str= new String();
            str=f.packets[j].l1_eth_packet.get_eth_packet();
            out.println(str);   
            out.println("\n");
            %>
        </tr>
            
        <tr>    
        <td width="20%">   
        <%for(i=0; i<f.packets[j].l1_eth_packet.h_source.length; i++)
                    { 
                          out.print(Integer.toString((f.packets[j].l1_eth_packet.h_source[i] & 0xff) + 0x100, 16).substring(1));
                          if(i!= f.packets[j].l1_eth_packet.h_source.length-1)
                          out.print("-");
                    }
        %>
        </td>
        <td width="20%">
            <%for (i=0; i < f.packets[j].l1_eth_packet.h_dest.length; i++)
                    { 
                          out.print(Integer.toString((f.packets[j].l1_eth_packet.h_dest[i] & 0xff) + 0x100, 16).substring(1));
                          if(i!= f.packets[j].l1_eth_packet.h_dest.length-1)
                          out.print("-");
                    }
            %>
        </td> 
        <td width="20%">
                <%out.println(f.packets[j].l1_eth_packet.h_proto);%>
        </td>  
        </tr>
         <% j++; } %>
        </table>

        <table border="1">
            <tr>
            <td width="10%"> <b> IHL </b>
            <td width="10%"> <b> VERSION</b>
            <td width="10%"> <b> TOS</b>
            <td width="10%"> <b> TOTAL LENGTH </b>
            <td width="10%"> <b> ID</b>
            <td width="10%"> <b> TTL</b>
            <td width="10%"> <b> PROTOCOL </b>
            <td width="10%"> <b> SOURCE ADD</b>
            <td width="10%"> <b> DESTINATION ADD</b>
                
        </tr>
        <% j=0;
        while(j<4)
        { %>
        
        <tr>    
        <td width="10%">   
        <%out.println((f.packets[j].l2_ip_packet.ihl*4));%>
        </td>
        <td width="10%">
            <%out.println(f.packets[j].l2_ip_packet.version);%>
        </td> 
        <td width="10%">
                <%out.println(f.packets[j].l2_ip_packet.tos);%>
        </td>  
        <td width="10%">
            <%out.println(Conversions.shortToUnsigned(f.packets[j].l2_ip_packet.tot_len));%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].l2_ip_packet.id);%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].l2_ip_packet.ttl);%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].l2_ip_packet.protocol);%>
        </td>  
        <td width="10%">
                <%out.println(((f.packets[j].l2_ip_packet.saddr >> 24) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.saddr>> 16) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.saddr>> 8) & 0xFF) +"."+ ((f.packets[j].l2_ip_packet.saddr) & 0xFF));%>
        </td>  
        <td width="10%">
                <%out.println(((f.packets[j].l2_ip_packet.daddr >> 24) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.daddr>> 16) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.daddr>> 8) & 0xFF) +"."+ ((f.packets[j].l2_ip_packet.daddr) & 0xFF));%>
        </td>  
        
        </tr>
         <% j++; } %>
        </table>

        <table border="1">
            <tr>
            <td width="10%"> <b> SOURCE</b>
            <td width="10%"> <b> DESTINATION</b>
            <td width="10%"> <b> SEQ </b>
            <td width="10%"> <b> ACK_SEQ </b>
            <td width="10%"> <b> FLAGS</b>
            <td width="10%"> <b> WINDOW <b>
            <td width="10%"> <b> CHECK </b>
            <td width="10%"> <b> URGENT POINTER</b>
                
        </tr>
        <% j=0;
        while(j<4)
        { %>
        
        <tr>    
        <td width="10%">   
        <%out.println(Conversions.shortToUnsigned(f.packets[j].l3_tcp_packet.source));%>
        </td>
        <td width="10%">
            <%out.println(Conversions.shortToUnsigned(f.packets[j].l3_tcp_packet.dest));%>
        </td> 
        <td width="10%">
            <%out.println(Conversions.intToUnsigned(f.packets[j].l3_tcp_packet.seq));%>
        </td>  
        <td width="10%">
            <%out.println(Conversions.intToUnsigned(f.packets[j].l3_tcp_packet.ack_seq));%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].l3_tcp_packet.flags);%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].l3_tcp_packet.window);%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].l3_tcp_packet.check);%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].l3_tcp_packet.urg_ptr);%>
        </td>  
        
        </tr>
         <% j++; } %>
        </table>

        <table border="1">
            <tr>
            <% String str= new String();
            str=f.packets[j].l1_eth_packet.get_eth_packet();
            out.println(str);   
            %>
            </tr>
            <tr>
            <td width="10%"> <b> SOURCE MAC</b>
            <td width="10%"> <b> DESTINATION MAC</b>
            <td width="10%"> <b> IP SRC </b>
            <td width="10%"> <b> IP DEST </b>
            <td width="10%"> <b> CAPLEN</b>
            <td width="10%"> <b>  LEN <b>
            <td width="10%"> <b> TIMESTAMP </b>
            <td width="10%"> <b> RX_DIRECTION</b>
                
        </tr>
        <% j=0;
        while(j<4)
        { %>
        
        <tr>    
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
                <%out.println(((f.packets[j].pfpacket.ip_src) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_src >> 8) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_src >> 16) &0xFF)+"."+((f.packets[j].pfpacket.ip_src >> 24) & 0xFF));%>
        </td>  
        <td width="10%">
                <%out.println(((f.packets[j].pfpacket.ip_dst) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_dst >> 8) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_dst >> 16) &0xFF)+"."+ ((f.packets[j].pfpacket.ip_dst >> 24) & 0xFF));%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].pfpacket.caplen);%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].pfpacket.len);%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].pfpacket.timestamp_ns);%>
        </td>  
        <td width="10%">
                <%out.println(f.packets[j].pfpacket.rx_direction);%>
        </td>  
        
        </tr>
         <% j++; } %>
        </table>

        </body>
</html>
