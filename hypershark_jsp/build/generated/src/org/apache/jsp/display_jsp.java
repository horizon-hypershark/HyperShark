package org.apache.jsp;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.jsp.*;
import Core.func;
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
      out.write("        <title>HyperShark- Packets</title>\n");
      out.write("        <script type=\"text/javascript\">\n");
      out.write("function loadXMLDoc(index)\n");
      out.write("{\n");
      out.write("var xmlhttp;\n");
      out.write("\n");
      out.write("/*alert(\"i loadxml\");*/\n");
      out.write("if (window.XMLHttpRequest)\n");
      out.write("  {// code for IE7+, Firefox, Chrome, Opera, Safari\n");
      out.write("  xmlhttp=new XMLHttpRequest();\n");
      out.write("  }\n");
      out.write("else\n");
      out.write("  {// code for IE6, IE5\n");
      out.write("  xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");\n");
      out.write("  }\n");
      out.write("xmlhttp.onreadystatechange=function()\n");
      out.write("  {\n");
      out.write("  if (xmlhttp.readyState==4 && xmlhttp.status==200)\n");
      out.write("    {\n");
      out.write("\t   \n");
      out.write("\tdocument.getElementById(\"myDiv\").innerHTML=xmlhttp.responseText;\n");
      out.write("    }\n");
      out.write("  }\n");
      out.write("xmlhttp.open(\"GET\",\"display2.jsp?index=\"+index,true);\n");
      out.write("xmlhttp.send();\n");
      out.write("}\n");
      out.write("\n");
      out.write("\n");
      out.write("</script>\n");
      out.write("        </head>\n");
      out.write("    <body>\n");
      out.write("    ");
 
        int j=0;
        int i;
        //data d[]=new data[10];
        func f=new func(); 
        f.fillpackets();
        
      out.write(" \n");
      out.write("               <table border=\"0\">\n");
      out.write("            <tr>\n");
      out.write("            <td>\n");
      out.write("            <h><b>PFRING HEADER</b></h>\n");
      out.write("            </td>    \n");
      out.write("            </tr>\n");
      out.write("            \n");
      out.write("            <tr>\n");
      out.write("            <td width=\"10%\"> <b> PKT NO.</b></td>   \n");
      out.write("            <td width=\"10%\"> <b> SOURCE MAC</b></td>\n");
      out.write("            <td width=\"10%\"> <b> DESTINATION MAC</b></td>\n");
      out.write("            <td width=\"10%\"> <b> IP SRC </b></td>\n");
      out.write("            <td width=\"10%\"> <b> IP DEST </b></td>\n");
      out.write("            <td width=\"10%\"> <b> PROTOCOL</b></td>    \n");
      out.write("            <td width=\"10%\"> <b> CAPLEN</b></td>\n");
      out.write("               \n");
      out.write("            </tr>\n");
      out.write("        ");
int k=1; 
        j=0;
        while(j<3)
        { 
      out.write("\n");
      out.write("        \n");
      out.write("        <tr>    \n");
      out.write("            <script type=\"text/javascript\">\n");
      out.write("            var str_pkts= new Array(");

        for(int l=0;l<3;l++)
        {
                       out.print("\""+f.packets[l].l2Packet.getPacket()+f.packets[l].l3Packet.getPacket()+f.packets[l].l4Packet.getPacket()+"\",");
        }   
        out.print("\"\"");
        
      out.write(");\n");
      out.write("           function getdata(index)\n");
      out.write("           {\n");
      out.write("               alert(\"in function \"+index);\n");
      out.write("               document.getElementById(\"myDiv\").innerHTML=str_pkts[index];\n");
      out.write("           }\n");
      out.write("            \n");
      out.write("        </script>\n");
      out.write("\n");
      out.write("            <td width=\"10%\">\n");
      out.write("                <a href=\"javascript:getdata(");
      out.print(j);
      out.write(")\">");
out.println(k);
                k++;
                
      out.write("</a>\n");
      out.write("            </td>    \n");
      out.write("                \n");
      out.write("            \n");
      out.write("            <td width=\"10%\">   \n");
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
out.println(((f.packets[j].pfpacket.ip_src>>24) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_src >> 16) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_src >> 8) &0xFF)+"."+((f.packets[j].pfpacket.ip_src) & 0xFF));
      out.write("</a>\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(((f.packets[j].pfpacket.ip_dst>>24) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_dst >> 16) & 0xFF) + "." + ((f.packets[j].pfpacket.ip_dst >> 8) &0xFF)+"."+ ((f.packets[j].pfpacket.ip_dst) & 0xFF));
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].pfpacket.caplen);
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("                ");
out.println(f.packets[j].pfpacket.l3_proto);
      out.write("\n");
      out.write("        </td>\n");
      out.write("            \n");
      out.write("        </tr>\n");
      out.write("         ");
 j++; } 
      out.write("\n");
      out.write("        </table>\n");
      out.write("        <p id=\"myDiv\">\n");
      out.write("        </p>    \n");
      out.write("        <br/>\n");
      out.write("        <br/>\n");
      out.write("        <br/>\n");
      out.write("        \n");
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
