<%-- 
    Document   : logout
    Created on : 13 Feb, 2012, 7:00:45 PM
    Author     : varun
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<%session.invalidate();
response.sendRedirect("WebPages/homepage.jsp");%>