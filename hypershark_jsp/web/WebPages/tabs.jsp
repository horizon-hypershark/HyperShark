<%-- 
    Document   : tabs
    Created on : 19 Jan, 2012, 3:01:45 PM
    Author     : varun
--%>
<%@page import="FileAccess.ReadFlow"%>
<%@page import="Utils.*"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>TABS</title>
        <link href="style.css" rel="stylesheet" type="text/css" />
        <link rel="stylesheet" type="text/css" href="button_form.css" />

        <!--for tabs-->
        <style type="text/css" media="screen">

            .menu { padding: 0; clear: both; }
            .menu li { display: inline; }
            .menu li a { background: #97BE72; padding: 10px; float:left; border-right: 1px solid #ccf; border-bottom: none; text-decoration: none; color: #000; font-weight: bold;}
            .menu li.active a { background:#D6D6D6; }
            .content { float: left; clear: both; border: 1px solid #ccf; border-top: none; border-left: none; background: #EEEEEE; padding: 10px 20px 20px; width: 850px; height:450px; overflow-y:scroll; overflow-x: scroll;}
        </style>


        <script type="text/javascript">
            
            function sethighlight(x)
        {
            document.getElementbyId.style.border-width="thick"
            document.getElementbyId.style.border-color="#F0F8FF"
        }
        </script>

        <!--dor date and time-->

        <script>
            
        function verify(input)
        {
            var ret;
            ret=verify_mac(document.getElementById(input).value);
            switch(ret)
            {
                case 0:
                    alert("insufficient address length: example address AB:01:23:CD:EF:45");
                    break;
                case 2:
                    alert(" : missing ");
                    break;
                case 3:
                    alert("invalid characters : only 0=9 and A-F acceptable ");
                    break;
            }
            if(ret!=1)
                document.getElementById(input).value=" ";
        }
            
        function verify_mac(add)
        {
            var i,ch;
                
                
            if(add.length!=17)
                return(0);
            for(i=0;i<add.length;i++)
            {
                ch=add.charCodeAt(i);
                if(i==2||i==5||i==8||i==11||i==14)
                {
                    if(ch!=':')
                        return(2);    
                }
                else
                {
                    if(!((ch>47&&ch<59)||(ch>96&&ch<103)||(ch>64&&ch<71)))
                    {
                        return(3);
                    }    
                }    
            }
            return(1);
        }
              
             
        </script>



        <script type="text/javascript"> 
            
        function time()
        {
                
            Protoplasm.use('timepicker').transform('input.timepicker'); 
        }
        </script>

        <script type="text/javascript">
            
        function date()
        {
            
            new JsDatePick({
                useMode:2,
                target:"inputField",
                dateFormat:"%d-%M-%Y"
                    
                
            });
        }
        </script>
        <script language="javascript" src="time/protoplasm.js"></script> 
        <script language="javascript"> Protoplasm.use('timepicker').transform('input.timepicker'); </script> 
        <link rel="stylesheet" type="text/css" media="all" href="datepicker/jsDatePick_ltr.min.css" />
        <script type="text/javascript" src="datepicker/jsDatePick.min.1.3.js"></script>
        <script type="text/javascript">
        window.onload = function(){
            new JsDatePick({
                useMode:2,
                target:"inputField",
                dateFormat:"%d-%M-%Y"
                    
            });
        };
        </script>
        <!--pop up form-->
        <script type="text/javascript" src="http://code.jquery.com/jquery-latest.pack.js"></script>
        <script type="text/javascript" src="button_form.js"></script>


         <script type="text/javascript">
                                                                             
            function verify_port(port)
            {
                var ret;
                ret=checkport(document.getElementById(port).value);
                switch(ret)
                {
                    case 0:
                        alert("Port address empty");
                        break;
                    case 2:
                        alert("value out of range");
                        break;
                    case 3:
                        alert("Value invalid, Only digits allowed ");
                        break;
                    default:
                        return;
                                                                                        
                }
                if(ret!=1)
                   document.getElementById(port).value=" "; 
            }    
                                                                        
                                                                       
                                                                        
                                                                        
            function checkport(input)
            {
                var i,input;
                                                                           
                                                                            
                if(input=="")
                {
                    return(0);
                }
                if(input>65535)
                {
                    return(2);
                }
                else
                {
                    for(i=0;i<input.length;i++)
                    {
                        ch=input.charCodeAt(i);
                        if(!(ch>47&&ch<59))
                        {
                            return(3);//different return codes based on error 
                        }    
                    }    
                }
                                                                            
                return(1);
                                                                            
            }
                                                                        
                                                                                   
        </script>


        
        
        
        
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


            <!--<div class="articleBody clear" id="articles">-->
            <table width="100%" height="100%" border="0">
                <tr>
                    <td width="20%" height="80%" >
                        <div class="tabimage">
                            <!--image of virtual machine-->
                            <img border="0" src="screen.png" alt="screen.png" width="100px" height="80px"/>
                            <!--dynamic table displaying filters-->
                            <table height="100%" width="100%">
                                <tr>
                                    <td>
                                        <p> Filters for this VM</p>
                                    </td>
                                </tr> 
                                <!--next row contains a table for displaying filters which contains 2 columns (1 for attribute name and 2nd for attribute value)-->

                                <tr>
                                    <td>
                                        <table>
                                            <%int i = 0;
                                                int num_filters = 7;
                                                String filters[] = new String[20];//String which will evantually contain the filters
                                                filters[0] = "TCP PACKET";
                                                filters[1] = "UDP PACKET";
                                                filters[2] = "IP RANGE";
                                                filters[3] = "PORT RANGE";
                                                filters[4] = "PORT RANGE";
                                                filters[5] = "PORT RANGE";
                                                filters[6] = "PORT RANGE";
                                                filters[7] = "PORT RANGE";
                                                while (i < num_filters) {
                                            %>
                                            <tr>  
                                                <td>
                                                    <% out.println(filters[i]);%>    
                                                </td>
                                                <td>
                                                    <%out.println("10");%>
                                                </td>    
                                            </tr>
                                            <%i++;
                                                }
                                            %>   
                                        </table>
                                    </td>    
                                </tr>  

                                <tr>
                                    <td>
                                  
                                            <!--<input type="button" name="Set Filters" value="Set Filters" onclick="dialog"/>-->
                                            <!--<button align="center" onclick="#dialog" name="modal"> Set Filters</button>-->
                                            <a href="#dialog" name="modal"><button type="submit">Edit Filters</button></a>
                                    </td>
                                </tr>


                            </table>     
                        </div>  
                    </td>

                    <td width="70%" height="100%">
                        <div id="tabs">

                            <ul id="menu" class="menu">
                                <li class="active"><a href="#displayflows" onclick="displayFlows()">View Sessions</a></li>
                                <li><a href="#displaypackets" onclick="displayPackets()">View Packets</a></li>
                                <li><a href="#querypackets"  onclick="queryPackets()">Query Packets</a></li>
                                <li><a href="#viewgraph"  onclick="displayGraph()">Display Graph</a></li>
                            </ul>

                            <div id="displayflows" class="content">
                                <!-- content of tab1-->
                                <script type="text/javascript">
                                function displayFlows()
                                {
                                    //alert("flow called");
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
                                    xmlhttp.open("GET","../flowdisplay.jsp",true);
                                    xmlhttp.send();

                                        
                                }
                                </script>
                                <div id="myDiv"></div>
                            </div>
                            <!--second tab starts here>-->
                            <!--<div id="displaypackets" class="content">-->

                            <script type="text/javascript">
                            function displayGraph()
                            {
                                //alert("graph called");
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
                                xmlhttp.open("GET","graph.jsp",true);
                                xmlhttp.send();

                                        
                            }
                            </script>


                            <script type="text/javascript">
                            function getdata(index)
                            {
                              //alert("getdata called");
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
                                        document.getElementById("Div").innerHTML=xmlhttp.responseText;
                                    }
                                }
                                xmlhttp.open("GET","../display2.jsp?index="+index,true);
                                xmlhttp.send();

                                    
                            } 
                            </script>


                            <script type="text/javascript">
                            function displayPackets()
                            {
                                //alert("packets called");
                                
                                
                                
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
                                xmlhttp.open("GET","../packetsdisplay.jsp",true);
                                xmlhttp.send();

                                        
                            }
                            </script>
                            <script type="text/javascript">
                            function queryPackets()
                            {
                                //alert("query called");
                                
                                
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
                                xmlhttp.open("GET","query.jsp",true);
                                xmlhttp.send();

                                        
                            }
                            </script>



                        </div>
                        <!--</div>-->
                    </td>
                </tr> 
            </table>            
            <!--<script src="tab/jquery.js" type="text/javascript" charset="utf-8"></script>
            <script src="tab/jquery.tabify.js" type="text/javascript" charset="utf-8"></script>
            <script type="text/javascript">
                // <![CDATA[
                                
                $(document).ready(function () {
                    $('#menu').tabify();
                    $('#menu2').tabify();
                });
                                        
                // ]]>
            </script>-->


            <br/><br/>			
            <div class="footer">
                <p>&copy HyperShark.com</p> <!-- Change the copyright notice -->
                <a href="#" class="up">Go UP</a>
            </div>
        </div>

    </body>

</html>
