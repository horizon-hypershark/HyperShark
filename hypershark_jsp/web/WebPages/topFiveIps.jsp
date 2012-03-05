<%-- 
    Document   : topFiveIps
    Created on : 2 Mar, 2012, 7:55:36 AM
    Author     : root
--%>

<%@page import="FileAccess.graph"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>TOP 5 IP ADDRESSES</title>
        <link href="layout.css" rel="stylesheet" type="text/css">
    <!--[if lte IE 8]><script language="javascript" type="text/javascript" src="../excanvas.min.js"></script><![endif]-->
    
    <link href="style.css" rel="stylesheet" type="text/css" />
    <script language="javascript" type="text/javascript" src="jquery.js"></script>
    <script language="javascript" type="text/javascript" src="jquery.flot.js"></script>
    <script language="javascript" type="text/javascript" src="jquery.flot.pie.js"></script>  

    <style type="text/css">
    body {
                font-family:verdana;
                font-size:15px;
                background-image:url('background.png');
            }
     </style>       
    
    </head>
    <body>
        <% 
            graph graphobj=new graph();
            graphobj.fillips();//call native method
       
        %>
        <div class="section" id="page"> 
            <div class="header"> 
                <h1><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
                <h2><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h2>

            </div>
            <br/>
            <br/>
            <br/>
    
            
        <div class="footer"> 
            <div class="line"></div>

            <p>&copy HyperShark.com</p> 

            <a href="#" class="up">Go UP</a>
        </div>
    </div>
        <br/>
        <br/>
        <br/>
        <br/>
        
   </body>
</html>
