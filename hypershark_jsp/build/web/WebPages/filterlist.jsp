
<%@page import="Beans.UserDataBean"%>
<%@page import="java.sql.ResultSet"%>
<%@page import="util.ExecuteQuery"%>
<%@page import="Utils.ConnectionDb"%>
<%@page import="Core.CaptureRule"%>

<%@page import="Utils.*"%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<%UserDataBean userData = (UserDataBean) session.getAttribute("userData");
    if (userData == null) {
        response.sendRedirect("homepage.jsp");
//    return;
    } else {
        int index = 0;
        if (request.getParameter("index") != null) {
            index = Integer.parseInt(request.getParameter("index"));
        }
%>


<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
    </head>
    <body>

        FILTERS SET FOR VM:<br/>
        <table>
            <tr>
                <td width ="8%"> SOURCE IP </td>
                <td width ="8%"> DEST. IP</td>
                <td width ="9%"> SOURCE MAC</td>
                <td width ="9%"> DEST. MAC</d>
                <td width ="7%"> SOURCE PORT LOW</d>
                <td width ="7%"> SOURCE PORT HIGH</d>
                <td width ="8%"> PROTOCOL</d>    

            </tr>


            
                <%                                        
                                    int j=0; 
                                    for(VirtualMachine virMach : userData.getUserDetails().getVirMachineFilters()) {
                %>

            <tr>
                <td width ="8%"><% out.print(cpt.srcHost[0]);
                                                            out.print(".");
                                                            out.print(cpt.srcHost[1]);
                                                            out.print(".");
                                                            out.print(cpt.srcHost[2]);
                                                            out.print(".");
                                                            out.print(cpt.srcHost[3]);%></td>
                <td width ="8%"><% out.print(cpt.dstHost[0]);
                                                            out.print(".");
                                                            out.print(cpt.dstHost[1]);
                                                            out.print(".");
                                                            out.print(cpt.dstHost[2]);
                                                            out.print(".");
                                                            out.print(cpt.dstHost[3]);%></td>
                <td width ="9%"><% out.print(cpt.smac[0]);
                                                            out.print(":");
                                                            out.print(cpt.smac[1]);
                                                            out.print(":");
                                                            out.print(cpt.smac[2]);
                                                            out.print(":");
                                                            out.print(cpt.smac[3]);
                                                            out.print(":");
                                                            out.print(cpt.smac[4]);
                                                            out.print(":");
                                                            out.print(cpt.smac[5]);%></td>
                <td width ="9%"><%out.print(cpt.dmac[0]);
                                                            out.print(":");
                                                            out.print(cpt.dmac[1]);
                                                            out.print(":");
                                                            out.print(cpt.dmac[2]);
                                                            out.print(":");
                                                            out.print(cpt.dmac[3]);
                                                            out.print(":");
                                                            out.print(cpt.dmac[4]);
                                                            out.print(":");
                                                            out.print(cpt.dmac[5]);%></d>
                <td width ="7%"><%out.print(cpt.lowSrcPort);%></d>
                <td width ="7%"><%out.print(cpt.highSrcPort);%></d>
                <td width ="7%"><%out.print(cpt.lowDstPort);%></d> 
                <td width ="7%"><%out.print(cpt.highDstPort);%></d>
                <td width ="8%"><%out.print(cpt.srcHostMask[0]);
                                                            out.print(".");
                                                            out.print(cpt.srcHostMask[1]);
                                                            out.print(".");
                                                            out.print(cpt.srcHostMask[2]);
                                                            out.print(".");
                                                            out.print(cpt.srcHostMask[3]);%></td>
                <td width ="8%"><%out.print(cpt.dstHostMask[0]);
                                                            out.print(".");
                                                            out.print(cpt.dstHostMask[1]);
                                                            out.print(".");
                                                            out.print(cpt.dstHostMask[2]);
                                                            out.print(".");
                                                            out.print(cpt.dstHostMask[3]);%> </td>
                <td width ="8%"><%out.print(cpt.protocol);%> </td>    

            </tr>
            <% }%>  <!-- end of while loop -->      
    </body>
</html>
