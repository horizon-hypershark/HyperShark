<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>HyperShark - Home</title>
<link href="style.css" rel="stylesheet" type="text/css" />
<!--<script type="text/javascript">
function sethighlight(x)
{
	document.getElementbyId.style.border-width="thick"
	document.getElementbyId.style.border-color="blue"
}
</script>-->
<style type="text/css">
        body
            {
             background-image:url('articleback.JPG');
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
        <legend style="color:#529214"><h2><font face="Times New Roman"><b>SIGN IN</b></font></h2><br/><br/>
        
        
       <form method="get" action="authenticate.jsp">

            
            
            <br/><label><b>Username :</b></label><br/><br/>
                <input name="username" type="text" id="username" onfocus="sethighlight(username)" style="height:35px;width:300px" />

               <br/><label><b>Password :</b></label><br/>
                <input name="password" type="password" id="password" onfocus="sethighlight(password)" style="height:35px;width:300px" />

                <script type="text/javascript">
                                function redirect()
                                {
                                    alert("graph called");
                                    var xmlhttp;
                                    if (window.XMLHttpRequest)
                                    {// code for IE7+, Firefox, Chrome, Opera, Safari
                                        xmlhttp=new XMLHttpRequest();
                                    }
                                    else
                                    {// code for IE6, IE5
                                        xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
                                    }
                                    xmlhttp.onreadystatechange=function()
                                    {
                                        if (xmlhttp.readyState==4 && xmlhttp.status==200)
                                        {
                                            document.getElementById("myDiv").innerHTML=xmlhttp.responseText;
                                        }
                                    }
                                    xmlhttp.open("GET","checkexample.jsp",true);
                                    alert("page called");
                                    xmlhttp.send();

                                        
                                }
                            </script>
               
 
                <br/><br/><br/>
                <input type="checkbox" id="loggedin" onfocus="sethighlight(password)"/><b>Keep me logged in</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp<input type="submit" name="submit" value="SIGN IN" style="height:30px;width:100px"/>SIGN IN<br/><br/>&nbsp;
               </form>
        </legend>
    </fieldset>        
    </div>     
<div id="myDiv"> pppppppppppppppppp
                    </div>
	
                
                
      


</div><br/><br/>			
	<div class="footer">
	   <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->
	   <a href="#" class="up">Go UP</a>
	</div>
</div>


</body>
</html>
