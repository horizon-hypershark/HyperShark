<%-- 
    Document   : form
    Created on : 17 Jan, 2012, 4:52:03 PM
    Author     : varun
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
        <link rel="stylesheet" type="text/css" href="style.css" />
        <link href="styles.css" rel="stylesheet" type="text/css" />
        <script type="text/javascript" src="http://code.jquery.com/jquery-latest.pack.js"></script>
        <script>

            $(document).ready(function() {	

                //select all the a tag with name equal to modal
                $('a[name=modal]').click(function(e) {
                    //Cancel the link behavior
                    e.preventDefault();
		
                    //Get the A tag
                    var id = $(this).attr('href');
	
                    //Get the screen height and width
                    var maskHeight = $(document).height();
                    var maskWidth = $(window).width();
	
                    //Set heigth and width to mask to fill up the whole screen
                    $('#mask').css({'width':maskWidth,'height':maskHeight});
		
                    //transition effect		
                    $('#mask').fadeIn(1000);	
                    $('#mask').fadeTo("slow",0.8);	
	
                    //Get the window height and width
                    var winH = $(window).height();
                    var winW = $(window).width();
              
                    //Set the popup window to center
                    $(id).css('top',  winH/2-$(id).height()/2);
                    $(id).css('left', winW/2-$(id).width()/2);
	
                    //transition effect
                    $(id).fadeIn(2000); 
	
                });
	
                //if close button is clicked
                $('.window .close').click(function (e) {
                    //Cancel the link behavior
                    e.preventDefault();
		
                    $('#mask').hide();
                    $('.window').hide();
                });		
	
                //if mask is clicked
                $('#mask').click(function () {
                    $(this).hide();
                    $('.window').hide();
                });			
	
            });

        </script>
        <script>
                function add_result()
                {
                    var ret;   
                    ret=verify_mac(document.getElementById("choice").value);
                    alert(ret);
                   
                    if(ret==1)
                    {
                        document.getElementById("result").value=document.getElementById("result").value+document.getElementById("choice").value+",";
                        document.getElementById("choice").value="";
                    }
                    else
                        {
                            alert("address not valid");
                        }
                 }
                
                function verify_mac(add)
                {
                    var i,ch;
                    if(add.length!=17)
                    return 0;
                    for(i=0;i<17;i++)
                        {
                            ch=add.charCodeAt(i);
                            if(i==2||i==5||i==8||i==11||i==14)
                                {
                                   if(ch!=':')
                                    return(1);    
                                }
                                else
                                {
                                    if(!((ch>47&&ch<59)||(ch>96&&ch<103)||(ch>64&&ch<71)))
                                    {
                                        return(0);
                                    }    
                                }    
                        }
                        return(1);
        }
                
                
        </script>
        <style>
            
            body {
                font-family:verdana;
                font-size:15px;
                background-image:url('articleback.JPG');
            }

            a {color:#333; text-decoration:none}
            a:hover {color:#ccc; text-decoration:none}

            #mask {
                position:absolute;
                left:0;
                top:0;
                z-index:9000;
                background-color:#000;
                display:none;
            }

            #boxes .window {
                position:absolute;
                left:0;
                top:0;
                width:440px;
                height:200px;
                display:none;
                z-index:9999;
                padding:20px;
            }

            #boxes #dialog {
                width:700px; 
                height:450px;
                padding:10px;
                background-color:#ffffff;
            }

            
            .buttons a, .buttons button{
                display:block;
                float:left;
                margin:0 7px 0 0;
                background-color:#EDEDFF;
                border:1px solid #dedede;
                border-top:1px solid #eee;
                border-left:1px solid #eee;    font-family:"Lucida Grande", Tahoma, Arial, Verdana, sans-serif;
                font-size:100%;
                line-height:130%;
                text-decoration:none;
                font-weight:bold;
                color:#565656;
                cursor:pointer;
                padding:5px 10px 6px 7px; /* Links */
            }
            .buttons button{
                width:auto;
                overflow:visible;
                padding:4px 10px 3px 7px; /* IE6 */
            }
            .buttons button[type]{
                padding:5px 10px 5px 7px; /* Firefox */
                line-height:17px; /* Safari */
            }
            *:first-child+html button[type]{
                padding:4px 10px 3px 7px; /* IE7 */
            }
            .buttons button img, .buttons a img{
                margin:0 3px -3px 0 !important;
                padding:0;
                border:none;
                width:16px;
                height:16px;
            }
            button:hover, .buttons a:hover{
                background-color:#dff4ff;
                border:1px solid #c2e1ef;
                color:#336699;
            }
            .buttons a:active{
                background-color:#6299c5;
                border:1px solid #6299c5;
                color:#fff;
            }/* POSITIVE */button.positive, .buttons a.positive{
                color:#529214;
            }
            .buttons a.positive:hover, button.positive:hover{
                background-color:#E6EFC2;
                border:1px solid #C6D880;
                color:#529214;
            }
            .buttons a.positive:active{
                background-color:#529214;
                border:1px solid #529214;
                color:#fff;
            }/* NEGATIVE */.buttons a.negative, button.negative{
                color:#d12f19;
            }
            .buttons a.negative:hover, button.negative:hover{
                background:#fbe3e4;
                border:1px solid #fbc2c4;
                color:#d12f19;
            }
            .buttons a.negative:active{
                background-color:#000000;
                border:1px solid #d12f19;
                color:#000000;
            }






        </style>

    </head>
    <body>
        <div class="section" id="page"> <!-- Defining the #page section with the section tag -->
            <div class="header"> <!-- Defining the header section of the page with the appropriate tag -->
                <h1><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
                <h2><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h2>
           
            </div>

            <div class="section" id="articles"> <!-- A new section with the articles -->

                <!-- Article 1 start -->

            <div class="article" id="article1"> <!-- The new article tag. The id is supplied so it can be scrolled into view. -->

                    <!-- main code starts from here-->
                    <table border="0" height="100%" width="100%">
                        <tr height="100%" width="100%">
                            <td width="30%">
                                <table border="0" width="100%" height="100%">
                                    <tr height="30%" width="100%" bgcolor="">
                                        <td><!-- image-->
                                            <img border="0" src="user2.png" alt="Hypershark" width="100px" height="100px" align="center"/>

                                        </td>
                                    </tr>    
                                    <tr>
                                        <td>    
                                            <table border="0" width="100%" height="100%">

                                                <!-- customer information-->   
                                                <tr height="25%">
                                                    <td><p>VARUN WADHWA</p>
                                                    </td>
                                                </tr>
                                                <tr height="25%">
                                                    <td><p>RAVIRAJ JOSHI</p>
                                                    </td>
                                                </tr>
                                                <tr height="25%">
                                                    <td><p>SAGAR HOTCHANDANI</p>
                                                    </td>
                                                </tr>
                                                <tr height="25%">
                                                    <td><p>BHARGAV PATEL</p>
                                                    </td>
                                                </tr>
                                            </table>
                                        </td>
                                    </tr>

                                </table>    
                            </td>  

                            <td width="70%"><!-- 2nd column of main table -->
                                <table height="100%" border="0">


                                    <%
                                        int i = 1;
                                        int num_vm = 5;
                                        while (i <= num_vm) {
                                    %>

                                    <tr width="300px">
                                        <td width="100%" height="5%">
                                            <table height="100%" border="0" width="100%"> 
                                                <tr>
                                                    <td  width="600px" height="100%">
                                                        <img border="0" src="screen.png" alt="Hypershark" width="100px" height="80px"/>
                                                    </td>
                                                    <td width="30%">


                                                        <div class="buttons">


                                                            <button type="submit" bgcolor="#000000"><img src="start_icon.png" alt="start_icon" width="20px" height="24px" align="left"/> Start Capturing</button>
                                                        </div> 
                                                        <!--   <button type="button">Start</button> -->
                                                    </td>    
                                                    <td width="30%">
                                                        <button type="button" align="center" onClick="location.href='#dialog'" name="modal"> Set Filters</button>
                                                        <a href="#dialog" name="modal">Click here</a>
                                                        <div id="boxes">
                                                            <div id="dialog" class="window">
                                                                <p>SET FILTERS FOR SELECTED VM</p>
                                                                <form name="input" action="#dialog" method="get">
                                                                Source Mac <input type="text" id ="choice"/>   
                                                                <input type="button" value="Add" onclick="add_result()"/>
                                                                <input type="text" id="result" /><br/>
                                                                Destination Mac <input type="text" id ="choice"/>   
                                                                <input type="button" value="Add" onclick="add_result()"/>
                                                                <input type="text" id="result"/>
                                                                
                                                                <a href="#"class="close"/>Close it</a>
                                                            </div>
                                                            <!-- Mask to cover the whole screen -->
                                                            <div id="mask"></div>
                                                        </div>

                                                    </td>    
                                                </tr>    
                                            </table>    
                                        </td>
                                    </tr>
                                   
                                    <%i++;
              }%>
                                </table>
                            </td>
                        </tr>    
                    </table>
                </div>
            </div> 

            <div class="footer"> <!-- Marking the footer section -->
                <div class="line"></div>

                <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->

                <a href="#" class="up">Go UP</a>
            </div>
        </div>

    </body>
</html>
