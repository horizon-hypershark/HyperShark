package org.apache.jsp.WebPages;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.jsp.*;
import Beans.UserDataBean;

public final class register_jsp extends org.apache.jasper.runtime.HttpJspBase
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
      out.write("        <title>HyperShark-Sign Up</title>\n");
      out.write("        <link href=\"style.css\" rel=\"stylesheet\" type=\"text/css\" />\n");
      out.write("        <style type=\"text/css\">\n");
      out.write("            body\n");
      out.write("            {\n");
      out.write("                background-image:url('background.png');\n");
      out.write("            }\n");
      out.write("        </style>\n");
      out.write("        <script type=\"text/javascript\">\n");
      out.write("            function doClick(objRad){\n");
      out.write("            if (objRad.value==\"0\"){\n");
      out.write("            document.getElementById(\"textbox\").style.display='none'; //hide textbox\n");
      out.write("             //show other options\n");
      out.write("            }\n");
      out.write("            else{\n");
      out.write("             //hide other options\n");
      out.write("            document.getElementById(\"textbox\").style.display='block'; //show textbox\n");
      out.write("            }\n");
      out.write("            }\n");
      out.write("        </script>   \n");
      out.write("            \n");
      out.write("    </head>\n");
      out.write("    <body>\n");
      out.write("        <div class=\"section\" id=\"page\">\n");
      out.write("            <div class=\"title\">\n");
      out.write("                <h1><font color=\"#52A300\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>\n");
      out.write("            </div>\n");
      out.write("            <div class=\"header\">\n");
      out.write("                <h3><font color=\"#FFFFFF\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h3>\n");
      out.write("            </div>\n");
      out.write("\n");
      out.write("            <br/><br/>\n");
      out.write("            <div class=\"articleBody clear\" id=\"articles\">\n");
      out.write("                <div class=\"figure\"><img src=\"Shark.JPG\"/></div> \n");
      out.write("                <div class=\"login\">\n");
      out.write("\n");
      out.write("                    <!-- write data here-->\n");
      out.write("\n");
      out.write("                </div>\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("            </div>\n");
      out.write("            <br/><br/><br/>\n");
      out.write("\n");
      out.write("            <!--table for vm image and buttons-->\n");
      out.write("\n");
      out.write("            <table>\n");
      out.write("                ");
int i=0;
                while(i<5)
                {
                
      out.write("\n");
      out.write("                <tr>\n");
      out.write("                    <td width=\"20%\">\n");
      out.write("                        <!--get image here-->\n");
      out.write("                    </td>    \n");
      out.write("                    <td width=\"20%\">\n");
      out.write("                        <form>\n");
      out.write("                            \n");
      out.write("                           <input type=\"radio\" name=\"sevice\" value=\"1\" onclick=\"doClick(this)\">Subscribe\n");
      out.write("                           \n");
      out.write("                           <input type=\"radio\" name=\"service\" value=\"Unsucscribe\"/>Unsubscribe\n");
      out.write("                           \n");
      out.write("                        </form> \n");
      out.write("                    </td> \n");
      out.write("                    <td width=\"20%\">\n");
      out.write("                            <div id=\"textbox\" style=\"display:none\">\n");
      out.write("                            <input type=\"text\" name=\"storage\">\n");
      out.write("                            </div>\n");
      out.write("\n");
      out.write("                    </td>\n");
      out.write("                </tr>\n");
      out.write("                \n");
      out.write("                ");
i++;
                }
                
      out.write("\n");
      out.write("            </table>\n");
      out.write("            <br/><br/>\n");
      out.write("            <br/><br/>\t\t\t\n");
      out.write("            <div class=\"footer\">\n");
      out.write("                <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->\n");
      out.write("                \n");
      out.write("            </div>\n");
      out.write("        </div>\n");
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
