package org.apache.jsp.WebPages;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.jsp.*;
import Beans.UserDataBean;

public final class homepage_jsp extends org.apache.jasper.runtime.HttpJspBase
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
      response.setContentType("text/html");
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
      out.write("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");
UserDataBean userData=(UserDataBean)session.getAttribute("userData");
if(userData!=null)
    response.sendRedirect("form.jsp");

      out.write("\n");
      out.write("<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
      out.write("<head>\n");
      out.write("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />\n");
      out.write("<title>HyperShark - Home</title>\n");
      out.write("<link href=\"style.css\" rel=\"stylesheet\" type=\"text/css\" />\n");
      out.write("<script type=\"text/javascript\">\n");
      out.write("function sethighlight(x)\n");
      out.write("{\n");
      out.write("\tdocument.getElementbyId.style.border-width=\"thick\"\n");
      out.write("\tdocument.getElementbyId.style.border-color=\"blue\"\n");
      out.write("}\n");
      out.write("</script>\n");
      out.write("<style type=\"text/css\">\n");
      out.write("        body\n");
      out.write("            {\n");
      out.write("             background-image:url('articleback.JPG');\n");
      out.write("            }\n");
      out.write("    </style>\n");
      out.write("    \n");
      out.write(" <style type=\"text/css\">\n");
      out.write("\n");
      out.write("a.ovalbutton{\n");
      out.write("background: transparent url('oval-green-left.gif') no-repeat top left;\n");
      out.write("display: block;\n");
      out.write("float: left;\n");
      out.write("font: normal 13px Tahoma; /* Change 13px as desired */\n");
      out.write("line-height: 16px; /* This value + 4px + 4px (top and bottom padding of SPAN) must equal height of button background (default is 24px) */\n");
      out.write("height: 24px; /* Height of button background height */\n");
      out.write("padding-left: 11px; /* Width of left menu image */\n");
      out.write("text-decoration: none;\n");
      out.write("}\n");
      out.write("\n");
      out.write("a:link.ovalbutton, b:visited.ovalbutton, b:active.ovalbutton{\n");
      out.write("color: #494949; /*button text color*/\n");
      out.write("}\n");
      out.write("\n");
      out.write("a.ovalbutton span{\n");
      out.write("background: transparent url('oval-green-right.gif') no-repeat top right;\n");
      out.write("display: block;\n");
      out.write("padding: 4px 11px 4px 0; /*Set 11px below to match value of 'padding-left' value above*/\n");
      out.write("}\n");
      out.write("\n");
      out.write("a.ovalbutton:hover{ /* Hover state CSS */\n");
      out.write("background-position: bottom left;\n");
      out.write("}\n");
      out.write("\n");
      out.write("a.ovalbutton:hover span{ /* Hover state CSS */\n");
      out.write("background-position: bottom right;\n");
      out.write("color: black;\n");
      out.write("}\n");
      out.write("\n");
      out.write(".buttonwrapper{ /* Container you can use to surround a CSS button to clear float */\n");
      out.write("overflow: hidden; /*See: http://www.quirksmode.org/css/clearing.html */\n");
      out.write("width: 100%;\n");
      out.write("}\n");
      out.write("\n");
      out.write("</style>\n");
      out.write("\n");
      out.write("   \n");
      out.write("    \n");
      out.write("    \n");
      out.write("    \n");
      out.write("    \n");
      out.write("    \n");
      out.write("</head>\n");
      out.write("<body>\n");
      out.write("<div class=\"section\" id=\"page\">\n");
      out.write("\n");
      out.write("\t<div class=\"header\">\n");
      out.write("\t\t<h1><font color=\"#FFFFFF\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>\n");
      out.write("\t\t<h2><font color=\"#FFFFFF\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h2>\n");
      out.write("\t</div>\n");
      out.write("\t\n");
      out.write("<br/><br/>\n");
      out.write("<div class=\"articleBody clear\" id=\"articles\">\n");
      out.write("<div class=\"figure\"><img src=\"Shark.JPG\" /></div> \n");
      out.write("\n");
      out.write("<div class=\"login\">\n");
      out.write("    <fieldset style=\"width:350x;height:375px\">\n");
      out.write("        <legend style=\"color:#529214<h2><font face=\"Times New Roman\"><b>SIGN IN</b></font></h2><br/>\n");
      out.write("        <form action=\"../authenticate.jsp\" method=\"post\">\n");
      out.write("                <label><b>Username :</b></label><br/><br/>\n");
      out.write("                <input name=\"username\" type=\"text\" id=\"username\" onfocus=\"sethighlight(username)\" style=\"height:35px;width:300px\" />\n");
      out.write("\n");
      out.write("                <br/><br/><label><b>Password :</b></label><br/><br/>\n");
      out.write("                <input name=\"password\" type=\"password\" id=\"password\" onfocus=\"sethighlight(password)\" style=\"height:35px;width:300px\" />\n");
      out.write("\n");
      out.write("                <br/>\n");
      out.write("                ");
 String str=request.getParameter("error");
                  if(str!=null)
                  out.println("*Invalid username or password");    
               
      out.write(" \n");
      out.write("               <br/><br/><br/>\n");
      out.write("                <input type=\"checkbox\" id=\"loggedin\" onfocus=\"sethighlight(password)\"/><b>Keep me logged in</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<br/><br/>\n");
      out.write("                \n");
      out.write("                \n");
      out.write("                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"submit\" name=\"submit\" id=\"submit\" onClick=\"#\" value=\"SIGN IN\" style=\"height:30px;width:100px\"/>\n");
      out.write("                <br/>&nbsp;\n");
      out.write("\n");
      out.write("\n");
      out.write("                \n");
      out.write("                                <a href=\"forgotpassword.html\">I cannot access my account</a>\n");
      out.write("       </form>\n");
      out.write("      </legend>\n");
      out.write("    </fieldset>\n");
      out.write("</div>  \n");
      out.write("\n");
      out.write("</div><br/><br/>\t\t\t\n");
      out.write("\t<div class=\"footer\">\n");
      out.write("\t   <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->\n");
      out.write("\t   <a href=\"#\" class=\"up\">Go UP</a>\n");
      out.write("\t</div>\n");
      out.write("</div>\n");
      out.write("\n");
      out.write("</body>\n");
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
