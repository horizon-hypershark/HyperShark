<%-- 
    Document   : signup
    Created on : 17 Feb, 2012, 1:11:32 PM
    Author     : nitish
--%>

<%@page import="Core.VirtualMachine"%>
<%@page import="java.util.ArrayList"%>
<%@page import="Beans.UserDataBean"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<jsp:useBean id="register" class="Beans.RegisterFormBean" scope="session">
    <jsp:setProperty name="register" property="*"/>
</jsp:useBean>
<%register.processRequest();%>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>LUCID-Sign Up</title>
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
                <h1><font color="#52A300">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Lucid</font></h1>
            </div>
            <div class="header">
            <h3><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font>
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
               <a href="../logout.jsp"><font color="#FFFFFF" size="4px">Logout</font></a>
             </h3>       
                
            </div>

            
            <br/><br/>
            <div class="articleBody clear" id="articles">
                <div class="figure"><img src="LUCID.jpg"width="380px"/></div> 
                <div class="login">

                    <p><b>Lucid enables you to see every network packet that comes on your machine without any effort,resources and 
                          maintenance on your part. Now you do not need to worry about the justification of the charges that you pay,
                          no more network monitoring from your side, maintaining huge collection of data or installing applications and 
                          most importantly no process on your machine gets affected.
                          
                    
                    </p>    <!-- write data here-->

                </div>



            </div>
            <br/><br/><br/>

            <!--table for vm image and buttons-->

            <div class="registerform">
            <form method="post" action="../record_reg_details.jsp">        
                <table>
                <%int i=0;
                ArrayList<VirtualMachine> virtList=register.getVirtList();                
                for(VirtualMachine virMach : virtList)
                {
                %>
                <tr>
                    <td width="25%">`
                       <img src="CentOS.png" width="100px" height="70px"></img>
                    </td>    
                    <td width="30%">
                        
                            
                           <input type="radio" name="service_<%=i%>" id="service_<%=i%>" value="1" onclick="doClick(this,<%=i%>)"><font face="37px/1 DNLTM" color="#4B4B4B" size="3">Subscribe</font>
                           
                           <input type="radio" name="service_<%=i%>" value="0" onclick="doClick(this,<%=i%>)"/><font face="37px/1 DNLTM" color="#4B4B4B" size="3">Unsubscribe</font>
                                                   
                    </td> 
                    <td width="45%">
                            <div id="textbox<%=i%>" style="display:none">
                            <font face="Times New Roman" color="#4B4B4B" size="3">&nbsp;&nbsp;&nbsp;&nbsp;Storage limit:</font><input type="text" name="storage_<%=i%>" id="storage_<%=i%>" />&nbsp;GB
                            </div>

                    </td>
                    </form> 
                </tr>
                
                <%i++;
                 }
                %>
            </table>
            <input type="submit" name="submit" value="Save Changes"/>
            </form> 
            </div>
            <br/><br/>
            <br/><br/>			
            <div class="footer">
                <p>&copy Lucid.com</p> <!-- Change the copyright notice -->
                
            </div>
        </div>


    </body>
</html>
