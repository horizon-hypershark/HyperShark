package org.apache.jsp.WebPages;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.jsp.*;

public final class query_jsp extends org.apache.jasper.runtime.HttpJspBase
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
      out.write("<!DOCTYPE html>\n");
      out.write("<html>\n");
      out.write("    <head>\n");
      out.write("        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n");
      out.write("        <title>JSP Page</title>\n");
      out.write("\n");
      out.write("        <link href=\"style.css\" rel=\"stylesheet\" type=\"text/css\" />\n");
      out.write("\n");
      out.write("        \n");
      out.write("        <!--change for time-->\n");
      out.write("        <link rel=\"stylesheet\" href=\"timepicker/include/jquery-ui-1.8.14.custom.css\" type=\"text/css\" />\n");
      out.write("    <link rel=\"stylesheet\" href=\"timepicker/jquery.ui.timepicker.css?v=0.2.9\" type=\"text/css\" />\n");
      out.write("\n");
      out.write("\n");
      out.write("    <script type=\"text/javascript\" src=\"timepicker/include/jquery-1.5.1.min.js\"></script>\n");
      out.write("    <script type=\"text/javascript\" src=\"timepicker/include/jquery.ui.core.min.js\"></script>\n");
      out.write("    <script type=\"text/javascript\" src=\"timepicker/include/jquery.ui.widget.min.js\"></script>\n");
      out.write("    <script type=\"text/javascript\" src=\"timepicker/include/jquery.ui.tabs.min.js\"></script>\n");
      out.write("    <script type=\"text/javascript\" src=\"timepicker/include/jquery.ui.position.min.js\"></script>\n");
      out.write("    <script type=\"text/javascript\" src=\"timepicker/jquery.ui.timepicker.js?v=0.2.9\"></script>\n");
      out.write("\n");
      out.write("    <style type=\"text/css\">\n");
      out.write("        body { font-size: 10px;}\n");
      out.write("        #content { font-size: 1.2em; \n");
      out.write("                   font-family: \"Lucida Sans Unicode\", \"Lucida Grande\", Verdana, Arial, Helvetica, sans-serif;\n");
      out.write("                   width: 950px; margin: auto;\n");
      out.write("        }\n");
      out.write("        .box { border: 1px solid #888; padding: 15px; margin:12px; }\n");
      out.write("        .code { margin: 6px; padding: 9px; background-color: #fdf5ce; border: 1px solid #c77405; }\n");
      out.write("        fieldset { padding: 0.5em 2em }\n");
      out.write("        hr { margin: 0.5em 0; clear: both }\n");
      out.write("        a { cursor: pointer; }\n");
      out.write("        #requirements li { line-height: 1.6em; \n");
      out.write("    </style>\n");
      out.write("\n");
      out.write("    \n");
      out.write("        <script type=\"text/javascript\">\n");
      out.write("            function showstarttime(){\n");
      out.write("\t\t//alert(\"time called\");\n");
      out.write("               $('#timepicker_customrange').timepicker({\n");
      out.write("                   hours: { starts: 0, ends: 24 },\n");
      out.write("                   minutes: { interval: 5 },\n");
      out.write("                   rows: 4,\n");
      out.write("                   showPeriodLabels: true,\n");
      out.write("                   minuteText: 'Min'\n");
      out.write("               })\n");
      out.write("            }\n");
      out.write("\n");
      out.write("        </script>\n");
      out.write("\t\n");
      out.write("\t\n");
      out.write("        <script type=\"text/javascript\">\n");
      out.write("            function validateStime(){\n");
      out.write("\t\t//alert(\"validate time called\");\n");
      out.write("                var rex = /^[0-1]\\d:[0-5]\\d|2[0-3]:[0-5]\\d/;\n");
      out.write("                //var rex1= RegExp(\"[0-1]\\d:[0-5]\\d|2[0-3]:[0-5]\\d\");\n");
      out.write("                //var re5digit=/^\\d{5}$/;\n");
      out.write("               //var st1= document.getElementById(\"stime\").value;\n");
      out.write("               //var st2= document.getElementById(\"etime\").value;\n");
      out.write("               if (document.getElementById(\"stime\").value.search(rex)==-1 ) //if match failed\n");
      out.write("                {\n");
      out.write("                  alert(\"Please enter valid Start time\");\n");
      out.write("                  document.getElementById(\"stime\").value=\"\";\n");
      out.write("              }\n");
      out.write("            }\n");
      out.write("\n");
      out.write("        </script>\n");
      out.write("\t\n");
      out.write("            <script type=\"text/javascript\">\n");
      out.write("            function validateEtime(){\n");
      out.write("\t\t//alert(\"validate time called\");\n");
      out.write("                var rex = /^[0-1]\\d:[0-5]\\d|2[0-3]:[0-5]\\d/;\n");
      out.write("                //var rex1= RegExp(\"[0-1]\\d:[0-5]\\d|2[0-3]:[0-5]\\d\");\n");
      out.write("                //var re5digit=/^\\d{5}$/;\n");
      out.write("               //var st1= document.getElementById(\"stime\").value;\n");
      out.write("               //var st2= document.getElementById(\"etime\").value;\n");
      out.write("               if (document.getElementById(\"etime\").value.search(rex)==-1 ) //if match failed\n");
      out.write("                   {\n");
      out.write("                  alert(\"Please enter valid End time\");\n");
      out.write("                  document.getElementById(\"etime\").value=\"\";\n");
      out.write("              }\n");
      out.write("            }\n");
      out.write("\n");
      out.write("        </script>\n");
      out.write("        \n");
      out.write("        <!--change for time ends-->\n");
      out.write("        <script>\n");
      out.write("        function verify_port(input)\n");
      out.write("            {\n");
      out.write("                var i;\n");
      out.write("                                                                           \n");
      out.write("                                                                            \n");
      out.write("                if(input==\"\")\n");
      out.write("                {\n");
      out.write("                    return(0);\n");
      out.write("                }\n");
      out.write("                if(input>65535)\n");
      out.write("                {\n");
      out.write("                    return(2);\n");
      out.write("                }\n");
      out.write("                else\n");
      out.write("                {\n");
      out.write("                    for(i=0;i<input.length;i++)\n");
      out.write("                    {\n");
      out.write("                        ch=input.charCodeAt(i);\n");
      out.write("                        if(!(ch>47&&ch<59))\n");
      out.write("                        {\n");
      out.write("                            return(3);//different return codes based on error \n");
      out.write("                        }    \n");
      out.write("                    }    \n");
      out.write("                }\n");
      out.write("                                                                            \n");
      out.write("                return(1);\n");
      out.write("                                                                            \n");
      out.write("            }\n");
      out.write("                                                                        \n");
      out.write("            function check_port(str)\n");
      out.write("            {\n");
      out.write("                var ret;   \n");
      out.write("                 alert(\"check called\");\n");
      out.write("                \n");
      out.write("                ret=verify_port(str);\n");
      out.write("                switch(ret)\n");
      out.write("                {\n");
      out.write("                    case 0:\n");
      out.write("                        alert(\"Port address empty\");\n");
      out.write("                        break;\n");
      out.write("                    case 2:\n");
      out.write("                        alert(\"value out of range\");\n");
      out.write("                        break;\n");
      out.write("                    case 3:\n");
      out.write("                        alert(\"Value invalid, Only digits allowed \");\n");
      out.write("                        break;\n");
      out.write("                    default:\n");
      out.write("                        return;\n");
      out.write(" \n");
      out.write("                }\n");
      out.write("                if(ret!=1)\n");
      out.write("                    document.getElementById(\"ChoiceFromPort\").value=\" \";\n");
      out.write("             \n");
      out.write("            }\n");
      out.write("            \n");
      out.write("        </script>\n");
      out.write("\n");
      out.write("        \n");
      out.write("        <script type=\"text/javascript\">\n");
      out.write("                      function retdate()\n");
      out.write("                      {\n");
      out.write("                      //alert(\"date called\");\n");
      out.write("                      var date=document.getElementById(\"inputField\").value;\n");
      out.write("                      return(date);\n");
      out.write("                      }\n");
      out.write("\n");
      out.write("\n");
      out.write("        </script>\n");
      out.write("        <style>\n");
      out.write("\n");
      out.write("            body {\n");
      out.write("                /*font-family:verdana;\n");
      out.write("                font-size:15px;*/\n");
      out.write("                background-image:url('background.png');\n");
      out.write("            }\n");
      out.write("            div#left { float: left; width: 300px }\n");
      out.write("            div#right { float: right; width: 300px }\n");
      out.write("\n");
      out.write("        </style>\n");
      out.write("\n");
      out.write("    </head>\n");
      out.write("    <body>\n");
      out.write("        <!--change for date-->\n");
      out.write("        <script src=\"datepicker/jquery.ui.datepicker.js\"></script>\n");
      out.write("\t<script>\n");
      out.write("        function showdate()\n");
      out.write("        {\n");
      out.write("\t\talert(\"date called\");\n");
      out.write("                $( \"#datepicker\" ).datepicker({ maxDate: \"+0M +0D\" });\n");
      out.write("\t}\n");
      out.write("\t</script>\n");
      out.write("        <!--change for date ends-->\n");
      out.write("        \n");
      out.write("        <!--<div class=\"section\" id=\"page\"> \n");
      out.write("            <div class=\"header\"> \n");
      out.write("                <h1><font color=\"#FFFFFF\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>\n");
      out.write("                <h2><font color=\"#FFFFFF\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h2>\n");
      out.write("\n");
      out.write("            </div>\n");
      out.write("        </div>\n");
      out.write("        <div class=\"section\" id=\"articles\"> \n");
      out.write("\n");
      out.write("           \n");
      out.write("            <div class=\"article\" id=\"article1\"> -->\n");
      out.write("        \n");
      out.write("        <form action=\"../queryresult.jsp\" method=\"GET\">\n");
      out.write("        <table height=\"100%\" width=\"100%\" border=\"0\">    \n");
      out.write("            <tr>\n");
      out.write("                <td>\n");
      out.write("\n");
      out.write("                    <table border=\"0\">\n");
      out.write("                        <tr>\n");
      out.write("                            <td width=\"400px\" align=\"center\">\n");
      out.write("                                <p>Enter Date: <input type=\"text\" id=\"datepicker\" name=\"date\" onclick=\"showdate()\"></p><!--change-->\n");
      out.write("                              \n");
      out.write("                            </td>\n");
      out.write("                            <td width=\"300px\" align=\"center\">\n");
      out.write("                                Start time:<input type=\"text\" name=\"startTime\" id=\"stime\" style=\"width: 70px\" id=\"timepicker_customrange\" onclick=\"showstarttime()\" onchange=\"validateStime()\"/>\n");
      out.write("        \n");
      out.write("                            </td>\n");
      out.write("                            <td width=\"300px\" align=\"center\">\n");
      out.write("                                End time:<input type=\"text\" name=\"endTime\" id=\"etime\" style=\"width: 70px\" id=\"timepicker_customrangeend\" onclick=\"showendtime()\" onchange=\"validateEtime()\"/>\n");
      out.write("                            </td>   \n");
      out.write("                        </tr>    \n");
      out.write("                    </table>\n");
      out.write("                </td>    \n");
      out.write("            </tr> \n");
      out.write("            <tr>\n");
      out.write("                <td>\n");
      out.write("                    <table border=\"0\">\n");
      out.write("                        <tr>\n");
      out.write("                            <td>\n");
      out.write("\n");
      out.write("                                Source IP:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
      out.write("                                <select name=\"srcOctet1\" id=\"oct1\" default-value=\"\">\n");
      out.write("                                    <option selected>Select</option>\n");
      out.write("                                    ");
 int m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                    <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                    ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                </select>\n");
      out.write("                                :<select name=\"srcOctet2\" id=\"oct2\">\n");
      out.write("                                    <option selected>Select</option>\n");
      out.write("                                    ");
 m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                    <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                    ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                </select>\n");
      out.write("                                :<select name=\"srcOctet3\" id=\"oct3\">                                \n");
      out.write("                                    <option selected>Select</option>\n");
      out.write("                                    ");
 m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                    <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                    ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                </select>\n");
      out.write("                                :<select name=\"srcOctet4\" id=\"oct4\">\n");
      out.write("                                    <option selected>Select</option>\n");
      out.write("                                    ");
 m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                    <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                    ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                </select>\n");
      out.write("                                <br/>\n");
      out.write("                                Destination IP :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<select name=\"dstOctet1\" id=\"oct5\">\n");
      out.write("                                    <option selected>Select</option>\n");
      out.write("                                    ");
m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                    <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                    ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                </select>\n");
      out.write("                                :<select name=\"dstOctet2\" id=\"oct6\">\n");
      out.write("                                    <option selected>Select</option>\n");
      out.write("                                    ");
 m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                    <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                    ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                </select>\n");
      out.write("                                :<select name=\"dstOctet3\" id=\"oct7\">\n");
      out.write("                                    <option selected>Select</option>\n");
      out.write("                                    ");
 m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                    <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                    ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                </select>\n");
      out.write("                                :<select name=\"dstOctet4\" id=\"oct8\">\n");
      out.write("                                    <option selected>Select</option>\n");
      out.write("                                    ");
 m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                    <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                    ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                </select>\n");
      out.write("                                <br/> \n");
      out.write("                                <br/>\n");
      out.write("                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Port:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Source:&nbsp;&nbsp;:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"text\" name=\"lowPort\" id=\"ChoiceFromPort\" title=\"Any positive integer between 0-65535\" onchange=\"check_port(document.getElementById('ChoiceFromPort').value)\"/>\n");
      out.write("                                &nbsp;&nbsp;&nbsp;&nbsp;Destination:&nbsp;&nbsp;:&nbsp;&nbsp;<input type=\"text\" id=\"ChoiceToPort\" name=\"highPort\" Title=\"Any positive integer between 0-65535\" onchange=\"check_port()\" onchange=\"check_port(document.getElementById('ChoiceToPort').value)\" />\n");
      out.write("                                <br/>\n");
      out.write("                                <br/>\n");
      out.write("                                &nbsp;&nbsp;&nbsp;&nbsp;Protocol&nbsp;&nbsp;:&nbsp;&nbsp;&nbsp;&nbsp;<select name=\"protocol\">\n");
      out.write("                                    <option selected>Select</option>\n");
      out.write("                                    <option>TCP</option>\n");
      out.write("                                    <option>UDP</option>\n");
      out.write("                                    <option>ICMP</option>\n");
      out.write("                                    <option>IGMP</option>\n");
      out.write("                                    <option>GRE</option>                                                                                                                        \n");
      out.write("                                    <option>SCTP</option>\n");
      out.write("                                </select>\n");
      out.write("                                <br/>\n");
      out.write("                                ");

                                String var=request.getParameter("msg");
      out.write("\n");
      out.write("                                \n");
      out.write("                                    \n");
      out.write("                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
      out.write("                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"submit\" value=\"Show Packets\"/>\n");
      out.write("                            </td>   \n");
      out.write("                        </tr>    \n");
      out.write("                    </table>     \n");
      out.write("                </td>\n");
      out.write("            </tr>   \n");
      out.write("        </table> \n");
      out.write("                                </form>\n");
      out.write("        <!--<div class=\"footer\"> \n");
      out.write("    <div class=\"line\"></div>\n");
      out.write("\n");
      out.write("    <p>&copy HyperShark.com</p> \n");
      out.write("\n");
      out.write("    <a href=\"#\" class=\"up\">Go UP</a>\n");
      out.write("</div>-->\n");
      out.write("    </body>\n");
      out.write("</html>\n");
      out.write("\n");
      out.write("\n");
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
