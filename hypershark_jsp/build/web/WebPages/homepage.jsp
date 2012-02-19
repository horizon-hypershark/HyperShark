<%@page import="Beans.UserDataBean"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%UserDataBean userData=(UserDataBean)session.getAttribute("userData");
if(userData!=null)
    response.sendRedirect("form.jsp");
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
<style type="text/css">
        body
            {
             background-image:url('articleback.JPG');
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
<body>
<div class="section" id="page">

	<div class="header">
		<h1><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
		<h2><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h2>
	</div>
	
<br/><br/>
<div class="articleBody clear" id="articles">
<div class="figure"><img src="Shark.JPG" /></div> 

<div class="login">
    <fieldset style="width:350x;height:375px">
        <legend style="color:#529214<h2><font face="Times New Roman"><b>SIGN IN</b></font></h2><br/>
        <form action="../authenticate.jsp" method="post">
                <label><b>Username :</b></label><br/><br/>
                <input name="username" type="text" id="username" onfocus="sethighlight(username)" style="height:35px;width:300px" />

                <br/><br/><label><b>Password :</b></label><br/><br/>
                <input name="password" type="password" id="password" onfocus="sethighlight(password)" style="height:35px;width:300px" />

                <br/>
                <% String str=request.getParameter("error");
                  if(str!=null)
                  out.println("*Invalid username or password");    
               %> 
               <br/><br/><br/>
                <input type="checkbox" id="loggedin" onfocus="sethighlight(password)"/><b>Keep me logged in</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<br/><br/>
                
                
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="submit" name="submit" id="submit" onClick="#" value="SIGN IN" style="height:30px;width:100px"/>
                <br/>&nbsp;


                
                                <a href="forgotpassword.html">I cannot access my account</a>
       </form>
      </legend>
    </fieldset>
</div> 

</div><br/><br/>			
	<div class="footer">
	   <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->
	   <a href="#" class="up">Go UP</a>
	</div>
</div>

</body>
</html>
