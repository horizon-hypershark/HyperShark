package org.apache.jsp.WebPages;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.jsp.*;
import Core.VirtualMachine;
import Beans.UserDataBean;
import java.sql.ResultSet;
import util.ExecuteQuery;
import Utils.ConnectionDb;

public final class form_jsp extends org.apache.jasper.runtime.HttpJspBase
    implements org.apache.jasper.runtime.JspSourceDependent {


                                                         /*boolean checknull(String value)
                                                           {
                                                               if(value.equals(null))
                                                                   return true;
                                                               return false;
                                                           }*/
                                                        
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
      out.write("\n");
UserDataBean userData=(UserDataBean)session.getAttribute("userData");
if(userData==null){
    response.sendRedirect("homepage.jsp");    
//    return;
}    
else
{

      out.write("\n");
      out.write("<!DOCTYPE html>\n");
      out.write("<html>\n");
      out.write("    <head>\n");
      out.write("        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n");
      out.write("        <title>HyperShark-VMList</title>\n");
      out.write("        <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" />\n");
      out.write("        <!--<link href=\"styles.css\" rel=\"stylesheet\" type=\"text/css\" />-->\n");
      out.write("        <!-- pop up form-->  \n");
      out.write("        <script type=\"text/javascript\" src=\"http://code.jquery.com/jquery-latest.pack.js\"></script>\n");
      out.write("\n");
      out.write("        <link rel=\"stylesheet\" type=\"text/css\" href=\"button_form.css\" />\n");
      out.write("        <script type=\"text/javascript\" src=\"button_form.js\"></script>\n");
      out.write("        \n");
      out.write("        <script type=\"text/javascript\">\n");
      out.write("            function funct()\n");
      out.write("            {\n");
      out.write("                window.location.href=\"tabs.jsp\";\n");
      out.write("            }\n");
      out.write("\n");
      out.write("        </script>\n");
      out.write("    <script>\n");
      out.write("            \n");
      out.write("            function verify(input)\n");
      out.write("            {\n");
      out.write("                var ret;\n");
      out.write("                ret=verify_mac(document.getElementById(input).value);\n");
      out.write("                switch(ret)\n");
      out.write("                {\n");
      out.write("                    case 0:\n");
      out.write("                        alert(\"insufficient address length: example address AB:01:23:CD:EF:45\");\n");
      out.write("                        break;\n");
      out.write("                    case 2:\n");
      out.write("                        alert(\" : missing \");\n");
      out.write("                        break;\n");
      out.write("                    case 3:\n");
      out.write("                        alert(\"invalid characters : only 0-9 and A-F acceptable \");\n");
      out.write("                        break;\n");
      out.write("                }\n");
      out.write("            }\n");
      out.write("            \n");
      out.write("            function verify_mac(add)\n");
      out.write("            {\n");
      out.write("                var i,ch;\n");
      out.write("                \n");
      out.write("                \n");
      out.write("                if(add.length!=17)\n");
      out.write("                    return(0);\n");
      out.write("                for(i=0;i<add.length;i++)\n");
      out.write("                {\n");
      out.write("                    ch=add.charCodeAt(i);\n");
      out.write("                    if(i==2||i==5||i==8||i==11||i==14)\n");
      out.write("                    {\n");
      out.write("                        if(ch!=':')\n");
      out.write("                            return(2);    \n");
      out.write("                    }\n");
      out.write("                    else\n");
      out.write("                    {\n");
      out.write("                        if(!((ch>47&&ch<59)||(ch>96&&ch<103)||(ch>64&&ch<71)))\n");
      out.write("                        {\n");
      out.write("                            return(3);\n");
      out.write("                        }    \n");
      out.write("                    }    \n");
      out.write("                }\n");
      out.write("                return(1);\n");
      out.write("            }\n");
      out.write("              \n");
      out.write("             \n");
      out.write("        </script>\n");
      out.write("\n");
      out.write("\n");
      out.write("        <script type=\"text/javascript\">\n");
      out.write("                                                                             \n");
      out.write("            function verify_port(port)\n");
      out.write("            {\n");
      out.write("                var ret;\n");
      out.write("                ret=checkport(document.getElementById(port).value);\n");
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
      out.write("                                                                                        \n");
      out.write("                }\n");
      out.write("            }    \n");
      out.write("                                                                        \n");
      out.write("                                                                       \n");
      out.write("                                                                        \n");
      out.write("                                                                        \n");
      out.write("            function checkport(input)\n");
      out.write("            {\n");
      out.write("                var i,input;\n");
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
      out.write("                                                                                   \n");
      out.write("        </script>\n");
      out.write("\n");
      out.write("\n");
      out.write("    </head>\n");
      out.write("    <body>\n");
      out.write("        <div class=\"section\" id=\"page\"> <!-- Defining the #page section with the section tag -->\n");
      out.write("            <div class=\"header\"> <!-- Defining the header section of the page with the appropriate tag -->\n");
      out.write("                <h1><font color=\"#FFFFFF\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>\n");
      out.write("                <h2><font color=\"#FFFFFF\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h2>\n");
      out.write("\n");
      out.write("            </div>\n");
      out.write("\n");
      out.write("            <div class=\"section\" id=\"articles\"> <!-- A new section with the articles -->\n");
      out.write("\n");
      out.write("                <!-- Article 1 start -->\n");
      out.write("\n");
      out.write("                <div class=\"article\" id=\"article1\"> <!-- The new article tag. The id is supplied so it can be scrolled into view. -->\n");
      out.write("                   \n");
      out.write("                    <!-- main code starts from here-->\n");
      out.write("                    <table border=\"0\" height=\"100%\" width=\"100%\" cellspacing=\"40px\">\n");
      out.write("                        <tr>\n");
      out.write("                            <td width=\"20%\" valign=\"top\">\n");
      out.write("                                <table border=\"0\" width=\"100%\">\n");
      out.write("                                    <tr  width=\"100%\" bgcolor=\"\">\n");
      out.write("                                        <td valign=\"bottom\"><!-- image-->\n");
      out.write("                                            <img border=\"0\" src=\"user.png\" alt=\"Hypershark\" width=\"100px\" height=\"80px\" align=\"center\" margin-top=\"-10px\"/>\n");
      out.write("\n");
      out.write("                                        </td>\n");
      out.write("                                    </tr>    \n");
      out.write("                                    <tr>\n");
      out.write("                                        <td>    \n");
      out.write("                                            <table border=\"0\" width=\"100%\" height=\"20px\">\n");
      out.write("                                                <!-- customer information-->   \n");
      out.write("                                                <tr height=\"3px\">\n");
      out.write("                                                    <td><p>ID&nbsp;&nbsp;:&nbsp;&nbsp;");
out.println(userData.getUserDetails().getUserId());
      out.write("</p>\n");
      out.write("                                                    </td>\n");
      out.write("                                                </tr>\n");
      out.write("                                                <tr height=\"3px\">\n");
      out.write("                                                    <td><p>Name&nbsp;&nbsp;:&nbsp;&nbsp;");
out.println(userData.getUsername());
      out.write("</p>\n");
      out.write("                                                    </td>\n");
      out.write("                                                </tr>\n");
      out.write("                                                <tr height=\"3px\">\n");
      out.write("                                                    <td><p>Information&nbsp;&nbsp;:&nbsp;&nbsp;");
out.println(userData.getUserDetails().getUserInfo());
      out.write("</p>\n");
      out.write("                                                    </td>\n");
      out.write("                                                </tr>\n");
      out.write("                                                <tr height=\"3px\">\n");
      out.write("                                                    <td><p>VMs assigned&nbsp;&nbsp;:&nbsp;&nbsp;");
out.println(userData.getUserDetails().getNoOfVms());
      out.write("</p>\n");
      out.write("                                                    </td>\n");
      out.write("                                                </tr>\n");
      out.write("                                            </table>\n");
      out.write("                                        </td>\n");
      out.write("                                    </tr>\n");
      out.write("\n");
      out.write("                                </table>    \n");
      out.write("                            </td>  \n");
      out.write("\n");
      out.write("                            <!-- draw a vertical line-->\n");
      out.write("                            <td width=\"1\" bgcolor=\"#CBCBCB\">\n");
      out.write("                            \n");
      out.write("                            </td>\n");
      out.write("                            <td width=\"80%\"><!-- 2nd column of main table -->\n");
      out.write("                                <table height=\"100%\" border=\"0\">\n");
      out.write("\n");
      out.write("\n");
      out.write("                                    ");
                                        
                                     for(VirtualMachine virMach : userData.getUserDetails().getVirMachineList()) {
                                    
      out.write("\n");
      out.write("\n");
      out.write("                                    <tr width=\"300px\">\n");
      out.write("                                        <td width=\"100%\" height=\"5%\">\n");
      out.write("                                            <table height=\"100%\" border=\"0\" width=\"100%\"> \n");
      out.write("                                                <tr>\n");
      out.write("                                                    <td  width=\"600px\" height=\"100%\">\n");
      out.write("                                                        <img border=\"0\" src=\"screen.png\" alt=\"Hypershark\" width=\"100px\" height=\"80px\"/><br/>\n");
      out.write("                                                        ");
 
                                                            out.println("VM ID  :    "+virMach.getVmId());
                                                        
      out.write("\n");
      out.write("                                                    </td>\n");
      out.write("                                                    <td width=\"30%\">\n");
      out.write("\n");
      out.write("\n");
      out.write("                                                        \n");
      out.write("\n");
      out.write("\n");
      out.write("                                                           <a href=\"tabs.jsp\"> <button type=\"submit\" background-color=\"#000000\">Start Capturing</button></a>\n");
      out.write("                                                            \n");
      out.write("\n");
      out.write("                                                                                                            </td>    \n");
      out.write("                                                    <td width=\"20%\">\n");
      out.write("                                                     \n");
      out.write("                                                            <!--<input type=\"button\" name=\"Set Filters\" value=\"Set Filters\" onclick=\"dialog\"/>-->\n");
      out.write("                                                            <!--<button align=\"center\" onclick=\"#dialog\" name=\"modal\"> Set Filters</button>-->\n");
      out.write("                                                            <a href=\"#dialog\" name=\"modal\"><button type=\"submit\">Add Filter</button></a>\n");
      out.write("                                                            \n");
      out.write("                                                     \n");
      out.write("                                                       \n");
      out.write("                                                         <div id=\"boxes\">\n");
      out.write("                                                            <div id=\"dialog\" class=\"window\">\n");
      out.write("                                                                <p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SET FILTERS FOR SELECTED VM&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"#\"class=\"close\" margin-left=\"120px\"float=\"right\"/>CLOSE&nbsp;&nbsp;<b>X</b></a></p>\n");
      out.write("                                                                <!--close link-->\n");
      out.write("\n");
      out.write("\n");
      out.write("\n");
      out.write("                                                                <form name=\"input\" action=\"capturefilter.jsp\" method=\"get\">\n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Source Mac:&nbsp;&nbsp;&nbsp;<input type=\"text\" id =\"srcMac\" name=\"srcMac\" title=\"Example: 12:23:34:ab:cd:ef\" onblur=\"verify('src_mac')\"/>   \n");
      out.write("\n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Destination Mac:&nbsp;&nbsp;&nbsp; <input type=\"text\" id =\"dstMac\" name=\"dstMac\" title=\"Example: 12:23:34:ab:cd:ef\" onblur=\"verify('dst_mac')\"/>   \n");
      out.write("\n");
      out.write("\n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    <!-- for ip address-->                                                                    \n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Source IP:&nbsp;&nbsp;&nbsp;\n");
      out.write("                                                                    ");
for(int i=1;i<=4;i++)
                                                                    {
      out.write("\n");
      out.write("                                                                    <select name=\"srcOctet");
      out.print(i);
      out.write("\" id=\"srcOctet");
      out.print(i);
      out.write("\">\n");
      out.write("                                                                        <option>Select</option>\n");
      out.write("                                                                        ");
 int m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                                                        <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                                                        ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                                                    </select>\n");
      out.write("                                                                            ");
if(i!=4)out.print(":");}
      out.write("                                                                                                                                       <br/>\n");
      out.write("\n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Destination IP :&nbsp;&nbsp;&nbsp;\n");
      out.write("                                                                    ");
for(int i=1;i<=4;i++)
                                                                    {
      out.write("\n");
      out.write("                                                                    <select name=\"dstOctet");
      out.print(i);
      out.write("\" id=\"dstOctet");
      out.print(i);
      out.write("\">\n");
      out.write("                                                                        <option>Select</option>\n");
      out.write("                                                                        ");
int m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                                                        <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                                                        ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                                                    </select>\n");
      out.write("                                                                    ");
if(i!=4)out.print(":");}
      out.write("\n");
      out.write("                                                                    \n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    <!-- for source and destination mask-->\n");
      out.write("\n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Source Mask:&nbsp;&nbsp;&nbsp;\n");
      out.write("                                                                    ");
for(int i=1;i<=4;i++)
                                                                    {
      out.write("\n");
      out.write("                                                                    <select name=\"srcMaskOctet");
      out.print(i);
      out.write("\" id=\"srcMaskOctet");
      out.print(i);
      out.write("\">\n");
      out.write("                                                                        <option>Select</option>\n");
      out.write("                                                                        ");
 int m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                                                        <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                                                        ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                                                    </select>\n");
      out.write("                                                                    ");
if(i!=4)out.print(":");}
      out.write("                                                                    \n");
      out.write("                                                                    <br/>\n");
      out.write("\n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Destination Mask:&nbsp;&nbsp;&nbsp;\n");
      out.write("                                                                    ");
for(int i=1;i<=4;i++)
                                                                    {
      out.write("\n");
      out.write("                                                                    <select name=\"dstMaskOctet");
      out.print(i);
      out.write("\" id=\"dstMaskOctet");
      out.print(i);
      out.write("\">\n");
      out.write("                                                                        <option>Select</option>\n");
      out.write("                                                                        ");
int m = 0;
                                                                            while (m < 255) {
      out.write("\n");
      out.write("                                                                        <option>");
out.print(m);
      out.write("</option>\n");
      out.write("                                                                        ");
 m++;
                                                                            }
      out.write("\n");
      out.write("                                                                    </select>\n");
      out.write("                                                                    ");
if(i!=4)out.print(":");}
      out.write("                                                                    \n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    <br/>\n");
      out.write("\n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Source Port:&nbsp;&nbsp;&nbsp;&nbsp;\n");
      out.write("\n");
      out.write("                                                                    From<input type=\"text\" name=\"lowSrcPort\" id=\"lowSrcPort\" onblur=\"verify_port('ChoiceFrom')\"/>\n");
      out.write("                                                                    To<input type=\"text\" id=\"highSrcPort\" name=\"highSrcPort\" onblur=\"verify_port('ChoiceTo')\"/>\n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Destination Port:&nbsp;&nbsp;\n");
      out.write("                                                                    From<input type=\"text\" id=\"lowDestPort\" name=\"lowDestPort\" onblur=\"verify_port('ChoiceFromDest')\"/>\n");
      out.write("                                                                    To<input type=\"text\" id=\"highDestPort\" name=\"highDestPort\" onblur=\"verify_port('ChoiceToDest')\"/>\n");
      out.write("\n");
      out.write("                                                                    <br/>\n");
      out.write("\n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Protocol:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
      out.write("                                                                    <select name=\"protocol\">\n");
      out.write("                                                                        <option value=\"Select\">Select</option>\n");
      out.write("                                                                        <option value=\" TCP\">TCP</option>\n");
      out.write("                                                                        <option value=\" UDP\">UDP</option>\n");
      out.write("                                                                        <option value=\" ICMP\">ICMP</option>\n");
      out.write("                                                                        <option value=\" IGMP\">IGMP</option>\n");
      out.write("                                                                        <option value=\" GRE\">GRE</option>\n");
      out.write("                                                                        <option value=\" SCTP\">SCTP</option>\n");
      out.write("\n");
      out.write("                                                                    </select>\n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    \n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
      out.write("                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n");
      out.write("               \n");
      out.write("                                                                    <input type=\"submit\" value=\"SUBMIT\"/>\n");
      out.write("                                                                    \n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    <br/>\n");
      out.write("                                                                    \n");
      out.write("                                                                    <label for=\"info\">Note:&nbsp;&nbsp;All the fields are optional and the traffic generated from specified values will not be monitored</label>\n");
      out.write("                                                                    </form>\n");
      out.write("                                                                    \n");
      out.write("                                                                    \n");
      out.write("                                                            </div>\n");
      out.write("\n");
      out.write("                                                        </div>\n");
      out.write("                                                                    \n");
      out.write("                                                                    \n");
      out.write("                                                        <!-- Mask to cover the whole screen -->\n");
      out.write("                                                        <div id=\"mask\"></div>\n");
      out.write("                                                        </div>\n");
      out.write("\n");
      out.write("                                                    </td>   \n");
      out.write("                                                   \n");
      out.write("                                                    \n");
      out.write("                                                    <td width=\"20%\">\n");
      out.write("                                                                                                                 <!--<input type=\"button\" name=\"Set Filters\" value=\"Set Filters\" onclick=\"dialog\"/>-->\n");
      out.write("                                                            <!--<button align=\"center\" onclick=\"#dialog\" name=\"modal\"> Set Filters</button>-->\n");
      out.write("                                                            \n");
      out.write("                                                            <a href=\"editfilters.jsp\"><button type=\"submit\" width=\"50px\"/>Edit filters</a>\n");
      out.write("                                                                                                              </td>\n");
      out.write("\n");
      out.write("                                                    \n");
      out.write("                                                </tr>    \n");
      out.write("                                            </table>    \n");
      out.write("                                        </td>\n");
      out.write("                                    </tr>\n");
      out.write("                                    <tr>\n");
      out.write("                                    <td>\n");
      out.write("                                        <hr/>\n");
      out.write("                                        </td>\n");
      out.write("                                        </tr>\n");
      out.write("                                    ");

                                    
                                        }
      out.write("\n");
      out.write("                                </table>\n");
      out.write("                            </td>\n");
      out.write("                        </tr>    \n");
      out.write("                    </table>\n");
      out.write("                </div>\n");
      out.write("            </div> \n");
      out.write("        <!-- make entry in database-->\n");
      out.write("                                 <div id=\"makeentry\">\n");
      out.write("                                                        ");

                                                         /* int src_octet1,src_octet2,src_octet3,src_octet4,dst_octet1;
                                                          int dst_octet2,dst_octet3,dst_octet4,shost_mask_octet1;
                                                          int shost_mask_octet2,shost_mask_octet3,shost_mask_octet4;
                                                          int dhost_mask_octet1,dhost_mask_octet2,dhost_mask_octet3;
                                                          int dhost_mask_octet4,proto,sport_low,sport_high,dport_low,dport_high;
                                                          
                                                          String octet=request.getParameter("octet1");
                                                          if(checknull(octet))
                                                          src_octet1=-1;
                                                          else
                                                          src_octet1=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet2");
                                                          if(checknull(octet))
                                                          src_octet2=-1;
                                                          else
                                                          src_octet2=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet3");
                                                          if(checknull(octet))
                                                          src_octet3=-1;
                                                          else
                                                          src_octet3=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet4");
                                                          if(checknull(octet))
                                                          src_octet4=-1;
                                                          else
                                                          src_octet4=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet5");
                                                          if(checknull(octet))
                                                          dst_octet1=-1;
                                                          else
                                                          dst_octet1=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet6");
                                                          if(checknull(octet))
                                                          dst_octet2=-1;
                                                          else
                                                          dst_octet2=Integer.parseInt(octet);
                                                          
                                                          
                                                          octet=request.getParameter("octet7");
                                                          if(checknull(octet))
                                                          dst_octet3=-1;
                                                          else
                                                          dst_octet3=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet8");
                                                          if(checknull(octet))
                                                          dst_octet4=-1;
                                                          else
                                                          dst_octet4=Integer.parseInt(octet);
                                                          
                                                          
                                                          
                                                          octet=request.getParameter("octet9");
                                                          if(checknull(octet))
                                                          shost_mask_octet1=-1;
                                                          else
                                                          shost_mask_octet1=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet10");
                                                          if(checknull(octet))
                                                          shost_mask_octet2=-1;
                                                          else
                                                          shost_mask_octet2=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet11");
                                                          if(checknull(octet))
                                                          shost_mask_octet3=-1;
                                                          else
                                                          shost_mask_octet3=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet12");
                                                          if(checknull(octet))
                                                          shost_mask_octet4=-1;
                                                          else
                                                          shost_mask_octet4=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet13");
                                                          if(checknull(octet))
                                                          dhost_mask_octet1=-1;
                                                          else
                                                          dhost_mask_octet1=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet14");
                                                          if(checknull(octet))
                                                          dhost_mask_octet2=-1;
                                                          else
                                                          dhost_mask_octet2=Integer.parseInt(octet);
                                                          
                                                          
                                                          octet=request.getParameter("octet15");
                                                          if(checknull(octet))
                                                          dhost_mask_octet3=-1;
                                                          else
                                                          dhost_mask_octet3=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet16");
                                                          if(checknull(octet))
                                                          dhost_mask_octet4=-1;
                                                          else
                                                          dhost_mask_octet4=Integer.parseInt(octet);
                                                          
                                                          String port=request.getParameter("ChoiceFrom");
                                                          if(checknull(port))
                                                          sport_low=-1;
                                                          else
                                                          sport_low=Integer.parseInt(port);
                                                          
                                                          
                                                          port=request.getParameter("ChoiceTo");
                                                          if(checknull(port))
                                                          sport_high=-1;
                                                          else
                                                          sport_high=Integer.parseInt(port);
                                                          
                                                          port=request.getParameter("ChoiceFromDest");
                                                          if(checknull(port))
                                                          dport_low=-1;
                                                          else
                                                          dport_low=Integer.parseInt(port);
                                                          
                                                          
                                                          port=request.getParameter("ChoiceToDest");
                                                          if(checknull(port))
                                                          dport_high=-1;
                                                          else
                                                          dport_high=Integer.parseInt(port);
                                                          
                                                          
                                                          String protocol=request.getParameter("protocol");
                                                          if(checknull(protocol))
                                                          proto=-1;
                                                          else
                                                          proto=Integer.parseInt(protocol);
                                                          
                                                          
                                                         query = "insert into vm_filters values('" + vm_id + "','" + proto + "','" + src_octet1 + "','" + src_octet2 + "','" + src_octet3 + "','" + src_octet4 + "','" + dst_octet1 + "','" + dst_octet2 + "','" + dst_octet3 + "',,'" + dst_octet4 + "',,'" + shost_mask_octet1 + "','" + shost_mask_octet2 + "','" + shost_mask_octet3 + "','" + shost_mask_octet4 + "','" + dhost_mask_octet1 + "','" + dhost_mask_octet2 + "','" + dhost_mask_octet3 + "','" + dhost_mask_octet4 + "','" + sport_low + "','" + sport_high + "','" + dport_low + "','" + dport_high + "','" + smac + "','" + dmac + "'" ;
                                                         eq.execQuery(conn, query);*/
      out.write("\n");
      out.write("                                                         \n");
      out.write("                                                          \n");
      out.write("                                                         ");
      out.write("\n");
      out.write("                                                    </div>    \n");
      out.write("                                                   \n");
      out.write("                                \n");
      out.write("                                \n");
      out.write("            <div class=\"footer\"> <!-- Marking the footer section -->\n");
      out.write("                <div class=\"line\"></div>\n");
      out.write("\n");
      out.write("                <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->\n");
      out.write("\n");
      out.write("                <a href=\"#\" class=\"up\">Go UP</a>\n");
      out.write("            </div>\n");
      out.write("        </div>\n");
      out.write("\n");
      out.write("    </body>\n");
      out.write("</html>\n");
}
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
