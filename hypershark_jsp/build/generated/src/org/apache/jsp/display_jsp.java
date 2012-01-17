package org.apache.jsp;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.jsp.*;
import Core.func2;
import Utils.*;

public final class display_jsp extends org.apache.jasper.runtime.HttpJspBase
    implements org.apache.jasper.runtime.JspSourceDependent {

  private static final JspFactory _jspxFactory = JspFactory.getDefaultFactory();

  private static java.util.Vector _jspx_dependants;

  private org.glassfish.jsp.api.ResourceInjector _jspx_resourceInjector;

  public Object getDependants() {
    return _jspx_dependants;
  }

  public void _jspService(HttpServletRequest request, HttpServletResponse response)
        throws java.io.IOException, ServletException {

    PageContext pageContext = null;
    HttpSession session = null;
    ServletContext application = null;
    ServletConfig config = null;
    JspWriter out = null;
    Object page = this;
    JspWriter _jspx_out = null;
    PageContext _jspx_page_context = null;

    try {
      response.setContentType("text/html;charset=UTF-8");
      pageContext = _jspxFactory.getPageContext(this, request, response,
      			null, true, 8192, true);
      _jspx_page_context = pageContext;
      application = pageContext.getServletContext();
      config = pageContext.getServletConfig();
      session = pageContext.getSession();
      out = pageContext.getOut();
      _jspx_out = out;
      _jspx_resourceInjector = (org.glassfish.jsp.api.ResourceInjector) application.getAttribute("com.sun.appserv.jsp.resource.injector");

      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("<!DOCTYPE html>\n");
      out.write("<html>\n");
      out.write("    <head>\n");
      out.write("        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n");
      out.write("        <title>JSP Page</title>\n");
      out.write("    </head>\n");
      out.write("    <body>\n");
      out.write("    ");
 
        int j=0;
        int i;
        //data d[]=new data[10];
        func2 f=new func2(); 
        f.fillpackets();
        
      out.write("    \n");
      out.write("        <table border=\"1\">\n");
      out.write("            <tr>\n");
      out.write("            <td width=\"20%\"> <b> SOURCE MAC </b>\n");
      out.write("            <td width=\"20%\"> <b> DESTINATION MAC</b>\n");
      out.write("            <td width=\"20%\"> <b> PROTOCOL </b>\n");
      out.write("        </tr>\n");
      out.write("        ");
while(j<4)
        { 
      out.write("\n");
      out.write("        <tr>\n");
      out.write("            ");
 String str= new String();
            str=f.packets[j].l1_eth_packet.get_eth_packet();
            out.println(str);   
            out.println("\n");
            
      out.write("\n");
      out.write("        </tr>\n");
      out.write("            \n");
      out.write("        <tr>    \n");
      out.write("        <td width=\"20%\">   \n");
      out.write("        ");
for(i=0; i<f.packets[j].l1_eth_packet.h_source.length; i++)
                    { 
                          out.print(Integer.toString((f.packets[j].l1_eth_packet.h_source[i] & 0xff) + 0x100, 16).substring(1));
                          if(i!= f.packets[j].l1_eth_packet.h_source.length-1)
                          out.print("-");
                    }
        
      out.write("\n");
      out.write("        </td>\n");
      out.write("        <td width=\"20%\">\n");
      out.write("            ");
for (i=0; i < f.packets[j].l1_eth_packet.h_dest.length; i++)
                    { 
                          out.print(Integer.toString((f.packets[j].l1_eth_packet.h_dest[i] & 0xff) + 0x100, 16).substring(1));
                          if(i!= f.packets[j].l1_eth_packet.h_dest.length-1)
                          out.print("-");
                    }
            
      out.write("\n");
      out.write("        </td> \n");
      out.write("        <td width=\"20%\">\n");
      out.write("                ");
out.println(f.packets[j].l1_eth_packet.h_proto);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        </tr>\n");
      out.write("         ");
 j++; } 
      out.write("\n");
      out.write("        </table>\n");
      out.write("\n");
      out.write("        <table border=\"1\">\n");
      out.write("            <tr>\n");
      out.write("            <td width=\"10%\"> <b> IHL </b>\n");
      out.write("            <td width=\"10%\"> <b> VERSION</b>\n");
      out.write("            <td width=\"10%\"> <b> TOS</b>\n");
      out.write("            <td width=\"10%\"> <b> TOTAL LENGTH </b>\n");
      out.write("            <td width=\"10%\"> <b> ID</b>\n");
      out.write("            <td width=\"10%\"> <b> TTL</b>\n");
      out.write("            <td width=\"10%\"> <b> PROTOCOL </b>\n");
      out.write("            <td width=\"10%\"> <b> SOURCE ADD</b>\n");
      out.write("            <td width=\"10%\"> <b> DESTINATION ADD</b>\n");
      out.write("                \n");
      out.write("        </tr>\n");
      out.write("        ");
 j=0;
        while(j<4)
        { 
      out.write("\n");
      out.write("        \n");
      out.write("        <tr>    \n");
      out.write("        <td width=\"10%\">   \n");
      out.write("        ");
out.println((f.packets[j].l2_ip_packet.ihl*4));
      out.write("\n");
      out.write("        </td>\n");
      out.write("        <td width=\"10%\">\n");
      out.write("            ");
out.println(f.packets[j].l2_ip_packet.version);
      out.write("\n");
      out.write("        </td> \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].l2_ip_packet.tos);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("            ");
out.println(Conversions.shortToUnsigned(f.packets[j].l2_ip_packet.tot_len));
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].l2_ip_packet.id);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].l2_ip_packet.ttl);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].l2_ip_packet.protocol);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(((f.packets[j].l2_ip_packet.saddr >> 24) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.saddr>> 16) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.saddr>> 8) & 0xFF) +"."+ ((f.packets[j].l2_ip_packet.saddr) & 0xFF));
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(((f.packets[j].l2_ip_packet.daddr >> 24) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.daddr>> 16) & 0xFF) + "." + ((f.packets[j].l2_ip_packet.daddr>> 8) & 0xFF) +"."+ ((f.packets[j].l2_ip_packet.daddr) & 0xFF));
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        \n");
      out.write("        </tr>\n");
      out.write("         ");
 j++; } 
      out.write("\n");
      out.write("        </table>\n");
      out.write("\n");
      out.write("        <table border=\"1\">\n");
      out.write("            <tr>\n");
      out.write("            <td width=\"10%\"> <b> SOURCE</b>\n");
      out.write("            <td width=\"10%\"> <b> DESTINATION</b>\n");
      out.write("            <td width=\"10%\"> <b> SEQ </b>\n");
      out.write("            <td width=\"10%\"> <b> ACK_SEQ </b>\n");
      out.write("            <td width=\"10%\"> <b> FLAGS</b>\n");
      out.write("            <td width=\"10%\"> <b> WINDOW <b>\n");
      out.write("            <td width=\"10%\"> <b> CHECK </b>\n");
      out.write("            <td width=\"10%\"> <b> URGENT POINTER</b>\n");
      out.write("                \n");
      out.write("        </tr>\n");
      out.write("        ");
 j=0;
        while(j<4)
        { 
      out.write("\n");
      out.write("        \n");
      out.write("        <tr>    \n");
      out.write("        <td width=\"10%\">   \n");
      out.write("        ");
out.println(Conversions.shortToUnsigned(f.packets[j].l3_tcp_packet.source));
      out.write("\n");
      out.write("        </td>\n");
      out.write("        <td width=\"10%\">\n");
      out.write("            ");
out.println(Conversions.shortToUnsigned(f.packets[j].l3_tcp_packet.dest));
      out.write("\n");
      out.write("        </td> \n");
      out.write("        <td width=\"10%\">\n");
      out.write("            ");
out.println(Conversions.intToUnsigned(f.packets[j].l3_tcp_packet.seq));
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("            ");
out.println(Conversions.intToUnsigned(f.packets[j].l3_tcp_packet.ack_seq));
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].l3_tcp_packet.flags);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].l3_tcp_packet.window);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].l3_tcp_packet.check);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].l3_tcp_packet.urg_ptr);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        \n");
      out.write("        </tr>\n");
      out.write("         ");
 j++; } 
      out.write("\n");
      out.write("        </table>\n");
      out.write("\n");
      out.write("        <table border=\"1\">\n");
      out.write("            <tr>\n");
      out.write("            ");
 String str= new String();
            str=f.packets[j].l1_eth_packet.get_eth_packet();
            out.println(str);   
            
      out.write("\n");
      out.write("            </tr>\n");
      out.write("            <tr>\n");
      out.write("            <td width=\"10%\"> <b> SOURCE MAC</b>\n");
      out.write("            <td width=\"10%\"> <b> DESTINATION MAC</b>\n");
      out.write("            <td width=\"10%\"> <b> IP SRC </b>\n");
      out.write("            <td width=\"10%\"> <b> IP DEST </b>\n");
      out.write("            <td width=\"10%\"> <b> CAPLEN</b>\n");
      out.write("            <td width=\"10%\"> <b>  LEN <b>\n");
      out.write("            <td width=\"10%\"> <b> TIMESTAMP </b>\n");
      out.write("            <td width=\"10%\"> <b> RX_DIRECTION</b>\n");
      out.write("                \n");
      out.write("        </tr>\n");
      out.write("        ");
 j=0;
        while(j<4)
        { 
      out.write("\n");
      out.write("        \n");
      out.write("        <tr>    \n");
      out.write("        <td width=\"10%\">   \n");
      out.write("        ");
for (i=0;i<f.packets[j].pfpacket.smac.length; i++)
            { 
                out.print(Integer.toString((f.packets[j].pfpacket.smac[i] & 0xff) + 0x100, 16).substring(1));
                if(i!=f.packets[j].pfpacket.smac.length-1)
                out.print("-");
            }
      out.write("\n");
      out.write("        </td>\n");
      out.write("        <td width=\"10%\">\n");
      out.write("            ");
for (i=0;i<f.packets[j].pfpacket.dmac.length; i++)
            { 
                out.print(Integer.toString((f.packets[j].pfpacket.dmac[i] & 0xff) + 0x100, 16).substring(1));
                if(i!=f.packets[j].pfpacket.dmac.length-1)
                out.print("-");
            }
      out.write("\n");
      out.write("        </td> \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(((f.packets[j].pfpacket.ip_src) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_src >> 8) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_src >> 16) &0xFF)+"."+((f.packets[j].pfpacket.ip_src >> 24) & 0xFF));
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(((f.packets[j].pfpacket.ip_dst) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_dst >> 8) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_dst >> 16) &0xFF)+"."+ ((f.packets[j].pfpacket.ip_dst >> 24) & 0xFF));
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].pfpacket.caplen);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].pfpacket.len);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].pfpacket.timestamp_ns);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].pfpacket.rx_direction);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        \n");
      out.write("        </tr>\n");
      out.write("         ");
 j++; } 
      out.write("\n");
      out.write("        </table>\n");
      out.write("\n");
      out.write("        </body>\n");
      out.write("</html>\n");
    } catch (Throwable t) {
      if (!(t instanceof SkipPageException)){
        out = _jspx_out;
        if (out != null && out.getBufferSize() != 0)
          out.clearBuffer();
        if (_jspx_page_context != null) _jspx_page_context.handlePageException(t);
        else throw new ServletException(t);
      }
    } finally {
      _jspxFactory.releasePageContext(_jspx_page_context);
    }
  }
}
