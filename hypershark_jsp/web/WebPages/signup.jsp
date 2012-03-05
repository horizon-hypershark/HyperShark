<%-- 
    Document   : signup
    Created on : 17 Feb, 2012, 1:11:32 PM
    Author     : nitish
--%>

<%@page import="Beans.UserDataBean"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%UserDataBean userData=(UserDataBean)session.getAttribute("userData");
if(userData==null){
    response.sendRedirect("homepage.jsp");    
//    return;
}    
else
{
%>

<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>HyperShark-Sign Up</title>
        <link href="style.css" rel="stylesheet" type="text/css" />
        <style type="text/css">
            body
            {
                background-image:url('background.png');
            }
        </style>

    </head>
    <body>
        <div class="section" id="page">
            <div class="header">
                <h1><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
                <h2><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h2>
            </div>
            <br/><br/>    
            <div class="articleBody clear" id="articles">
                <div class="signimage"><img src="Shark.JPG" /></div> 
                
                
            </div>
            
            
            <!--table for vm image and buttons-->
            
            <table>
            
            
            </table>
            <br/><br/>
            <br/><br/>			
            <div class="footer">
                <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->
                <a href="#" class="up">Go UP</a>
            </div>
        </div>


    </body>
</html>
