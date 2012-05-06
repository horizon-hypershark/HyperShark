<%-- 
    Document   : record_reg_details
    Created on : 26 Mar, 2012, 2:47:41 PM
    Author     : root
--%>

<%@page import="Controller.FlowController"%>
<%@page import="java.util.ArrayList"%>
<%@page import="Core.VirtualMachine"%>
<%@page import="Beans.RegisterFormBean"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
    </head>
    <body>
        
        <%           
        RegisterFormBean regDetails=(RegisterFormBean)(session.getAttribute("register"));
        ArrayList<Integer> serv=new ArrayList<Integer>();
        ArrayList<Integer> limit=new ArrayList<Integer>();    
        for(int i=0;i<regDetails.getVirtList().size();i++)
        {
            Integer temp=Integer.parseInt(request.getParameter("service_"+i));            
            if(temp==null)
                temp=0;
            serv.add(temp);
            temp=Integer.parseInt(request.getParameter("storage_"+i));
            if(temp==null)
                temp=0;
            limit.add(temp);                

        }
        
        new FlowController().updateRegDetails(regDetails, serv, limit);
        /*for(int i=0;i<RegDetails.getVirtList().size();i++)
        {
            out.println("service:"+serv.get(i));
            out.println("storage:"+limit.get(i));
        }*/
        response.sendRedirect("WebPages/register.jsp");
        %>
    </body>
</html>
