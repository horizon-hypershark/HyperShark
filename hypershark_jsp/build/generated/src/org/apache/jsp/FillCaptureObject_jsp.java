package org.apache.jsp;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.jsp.*;
import Utils.Conversions;
import Core.func;
import Core.Rules;
import Core.CaptureTime;

public final class FillCaptureObject_jsp extends org.apache.jasper.runtime.HttpJspBase
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
      out.write("\n");
      out.write("\n");
      out.write("<!DOCTYPE html>\n");
      out.write("<html>\n");
      out.write("    <head>\n");
      out.write("        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n");
      out.write("        <title>JSP Page</title>\n");
      out.write("    </head>\n");
      out.write("    <body>\n");
      out.write(" ");

 func f= new func();
 
 f.cpt.day=31;
        f.cpt.month=0;
        f.cpt.end_hr=9;
        f.cpt.end_min=29;
        f.cpt.start_hr=9;
        f.cpt.start_min=20;
        
        f.rules.sip_oct[0]=74;
        f.rules.sip_oct[1]=-1;
        f.rules.sip_oct[2]=-1;
        f.rules.sip_oct[3]=-1;
        
        f.rules.dip_oct[0]=-1;
        f.rules.dip_oct[1]=-1;
        f.rules.dip_oct[2]=-1;
        f.rules.dip_oct[3]=-1;
        
        f.rules.src_port=80;
        f.rules.dst_port=-1;
        f.rules.protocol=-1;
        
        
/*f.setCpt(cpt);
f.setRules(rules);*/
f.fillpackets();

      out.write("\n");
      out.write("<br/>\n");
      out.write("\n");
      out.write("<!--display fields-->\n");
      out.write("  <table border=\"0\">\n");
      out.write("            <tr>\n");
      out.write("            <td>\n");
      out.write("            <h><b>SESSIONS:</b></h>\n");
      out.write("            \n");
      out.write("            </td>    \n");
      out.write("            </tr>\n");
      out.write("            \n");
      out.write("            <tr>\n");
      out.write("                <td width=\"10%\"> <b> S.NO</b></td>\n");
      out.write("            <td width=\"10%\"> <b> SOURCE PORT</b></td>   \n");
      out.write("            <td width=\"10%\"> <b> DESTINATION PORT</b></td>\n");
      out.write("            <td width=\"10%\"> <b> SOURCE IP</b></td>\n");
      out.write("            <td width=\"10%\"> <b> DESTINATION IP </b></td>\n");
      out.write("            \n");
      out.write("            <td width=\"10%\"> <b> PROTOCOL</b></td>    \n");
      out.write("            <td width=\"10%\"> <b> NO OF PACKETS</b></td>\n");
      out.write("               \n");
      out.write("            </tr>\n");
      out.write("            \n");
      out.write("            \n");
      out.write("            ");
int k=1,j,l=0; 
                j=0;
                while(f.flow[l]!=null)
               { 
      out.write("\n");
      out.write("        \n");
      out.write("              <tr>   \n");
      out.write("            <td width=\"10%\">\n");
      out.write("                <a>");
out.println(k);
                k++;
                
      out.write("</a>\n");
      out.write("            </td>    \n");
      out.write("            \n");
      out.write("        <td width=\"10%\">   \n");
      out.write("        ");
out.println(Conversions.shortToUnsigned(f.flow[l].src_port));
      out.write("\n");
      out.write("        </td>\n");
      out.write("        <td width=\"10%\">\n");
      out.write("            ");
out.println(Conversions.shortToUnsigned(f.flow[l].dst_port));
      out.write("\n");
      out.write("        </td> \n");
      out.write("        <td width=\"10%\">\n");
      out.write("          ");
out.println(((f.flow[l].ip_src) & 0xFF) + "." + ((f.flow[l].ip_src >> 8) & 0xFF) + "." + ((f.flow[l].ip_src>> 16) & 0xFF) +"."+ ((f.flow[l].ip_src>> 24) & 0xFF));
      out.write("\n");
      out.write("           \n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("               ");
out.println(((f.flow[l].ip_dst) & 0xFF) + "." + ((f.flow[l].ip_dst >> 8) & 0xFF) + "." + ((f.flow[l].ip_dst>> 16) & 0xFF) +"."+ ((f.flow[l].ip_dst>> 24) & 0xFF));
      out.write("\n");
      out.write("        </td>  \n");
      out.write("        <td width=\"10%\">\n");
      out.write("           ");
out.println(f.flow[l].protocol);
      out.write("\n");
      out.write("        </td> \n");
      out.write("         \n");
      out.write("        <td width=\"10%\">\n");
      out.write("           ");
out.println(f.flow[l].nop);
      out.write("\n");
      out.write("        </td>\n");
      out.write("        \n");
      out.write("        </tr>\n");
      out.write("         ");
 l++; } 
      out.write("\n");
      out.write("        </table>\n");
      out.write("        \n");
      out.write("               \n");
      out.write("               \n");
      out.write("      \n");
      out.write("\n");
      out.write("\n");
      out.write("    </body>\n");
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
