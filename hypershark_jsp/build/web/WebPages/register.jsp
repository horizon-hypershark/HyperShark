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
        <script type="text/javascript">
            function doClick(objRad,i){
            if (objRad.value=="0"){
            document.getElementById("textbox"+i).style.display='none'; //hide textbox
             //show other options
            }
            else{
             //hide other options
            document.getElementById("textbox"+i).style.display='block'; //show textbox
            }
            }
        </script>   
            
    </head>
    <body>
        <div class="section" id="page">
            <div class="title">
                <h1><font color="#52A300">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
            </div>
            <div class="header">
                <h3><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h3>
            </div>

            <br/><br/>
            <div class="articleBody clear" id="articles">
                <div class="figure"><img src="Shark.JPG"/></div> 
                <div class="login">

                    <p><b>HyperShark lets u see what u pay for
                    
                    </p>    <!-- write data here-->

                </div>



            </div>
            <br/><br/><br/>

            <!--table for vm image and buttons-->

            <div class="registerform">
            <table>
                <%int i=0;
                while(i<5)
                {
                %>
                <tr>
                    <td width="25%">
                       <img src="CentOS.png" width="100px" height="70px"></img>
                    </td>    
                    <td width="30%">
                        <form>
                            
                           <input type="radio" name="service" value="1" onclick="doClick(this,<%=i%>)"><font face="37px/1 DNLTM" color="#4B4B4B" size="3">Subscribe</font>
                           
                           <input type="radio" name="service" value="0" onclick="doClick(this,<%=i%>)"/><font face="37px/1 DNLTM" color="#4B4B4B" size="3">Unsubscribe</font>
                           
                        </form> 
                    </td> 
                    <td width="40%">
                            <div id="textbox<%=i%>" style="display:none">
                            <font face="37px/1 DNLTM" color="#4B4B4B" size="3">Enter storage limit:</font><input type="text" name="storage">
                            </div>

                    </td>
                </tr>
                
                <%i++;
                }
                %>
            </table>
            </div>
            <br/><br/>
            <br/><br/>			
            <div class="footer">
                <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->
                
            </div>
        </div>


    </body>
</html>
