<%@page import="Beans.UserDataBean"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%UserDataBean userData = (UserDataBean) session.getAttribute("userData");
    if (userData != null) {
        response.sendRedirect("vmList.jsp");
    }
%>
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
        <title>HyperShark - Home</title>
        <link href="style.css" rel="stylesheet" type="text/css" />
        <script type="text/javascript">
            function sethighlight(x)
            {
                document.getElementbyId.style.border-width="thick"
                document.getElementbyId.style.border-color="blue"
            }
        </script>
        
        <!--change-->
        <script type="text/javascript">
        function setFocus()
        {
             document.getElementById("username").focus();
        }
        </script>
        
        <!--change ends-->
        <style type="text/css">
            body
            {
                background-image:url('background.png');
            }
        </style>

        <style type="text/css">

            a.ovalbutton{
                background: transparent url('oval-green-left.gif') no-repeat top left;
                display: block;
                float: left;
                font: normal 13px Tahoma; /* Change 13px as desired */
                line-height: 16px; /* This value + 4px + 4px (top and bottom padding of SPAN) must equal height of button background (default is 24px) */
                height: 24px; /* Height of button background height */
                padding-left: 11px; /* Width of left menu image */
                text-decoration: none;
            }

            a:link.ovalbutton, b:visited.ovalbutton, b:active.ovalbutton{
                color: #494949; /*button text color*/
            }

            a.ovalbutton span{
                background: transparent url('oval-green-right.gif') no-repeat top right;
                display: block;
                padding: 4px 11px 4px 0; /*Set 11px below to match value of 'padding-left' value above*/
            }

            a.ovalbutton:hover{ /* Hover state CSS */
                background-position: bottom left;
            }

            a.ovalbutton:hover span{ /* Hover state CSS */
                background-position: bottom right;
                color: black;
            }

            .buttonwrapper{ /* Container you can use to surround a CSS button to clear float */
                overflow: hidden; /*See: http://www.quirksmode.org/css/clearing.html */
                width: 100%;
            }

        </style>







    </head>
    <body onload="setFocus()">
        <div class="section" id="page">
            <div class="title">
                <h1><font color="#52A300">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Lucid</font></h1>
            </div>
            <div class="header">
                <h3><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h3>
            </div>

            <br/><br/>
            <div class="articleBody clear" id="articles">
                <div class="figure"><img src="Shark.JPG"/></div> 

                <div class="login">
                    <h3><font face="Times New Roman" color="#52A300"><b>&nbsp;SIGN IN</b></font></h3>
                    <form action="../authenticate.jsp" method="post">
                        <label><font face="Times New Roman" color="#000000"><b>Username :</b></font></label>
                        <input name="username" type="text" id="username" onfocus="sethighlight(username)" style="height:25px;width:300px" />

                        <br/><br/><label><b>Password :</b></label>
                        <input name="password" type="password" id="password" onfocus="sethighlight(password)" style="height:25px;width:300px" />

                        <br/>
                        <% String str = request.getParameter("error");
                            if (str != null) {
                                out.println("*Invalid username or password");
                            }
                        %> 
                        <br/>

                        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="submit" name="submit" id="submit" onClick="#" value="SIGN IN" style="height:30px;width:100px"/>
                        <br/>

                    </form>


                </div> 
            </div>
            <br/><br/><br/>            
            <div class="signup"  onmouseout="this.style.background='#DEDEDE';" onmouseover="this.style.background='rgb(255,224,61)';this.style.cursor='pointer';">
            SIGNUP FOR HYPERSHARK&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            <input type="text" name="Custid" onmouseout="this.value='Enter ID here'" onclick="this.value=' '" value="Enter ID here"/>    
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="submit" name="submit" id="submit" value="REGISTER" style="height:20px;width:100px" onclick="window.location='register.jsp'"/>
       
            </div>    
            <br/><br/>
            <div class="footer">
                &copy HyperShark.com<!-- Change the copyright notice -->

            </div>
        </div>

    </body>
</html>
