<%-- 
    Document   : query
    Created on : 20 Jan, 2012, 2:53:34 PM
    Author     : varun
--%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>

        <link href="style.css" rel="stylesheet" type="text/css" />

        
        <!--change for time-->
        <link rel="stylesheet" href="timepicker/include/jquery-ui-1.8.14.custom.css" type="text/css" />
    <link rel="stylesheet" href="timepicker/jquery.ui.timepicker.css?v=0.2.9" type="text/css" />


    <script type="text/javascript" src="timepicker/include/jquery-1.5.1.min.js"></script>
    <script type="text/javascript" src="timepicker/include/jquery.ui.core.min.js"></script>
    <script type="text/javascript" src="timepicker/include/jquery.ui.widget.min.js"></script>
    <script type="text/javascript" src="timepicker/include/jquery.ui.tabs.min.js"></script>
    <script type="text/javascript" src="timepicker/include/jquery.ui.position.min.js"></script>
    <script type="text/javascript" src="timepicker/jquery.ui.timepicker.js?v=0.2.9"></script>

    <style type="text/css">
        body { font-size: 14px;}
        #content { font-size: 1.2em; 
                   font-family: "Lucida Sans Unicode", "Lucida Grande", Verdana, Arial, Helvetica, sans-serif;
                   width: 950px; margin: auto;
        }
        .box { border: 1px solid #888; padding: 15px; margin:12px; }
        .code { margin: 6px; padding: 9px; background-color: #fdf5ce; border: 1px solid #c77405; }
        fieldset { padding: 0.5em 2em }
        hr { margin: 0.5em 0; clear: both }
        a { cursor: pointer; }
        #requirements li { line-height: 1.6em; 
    </style>

    
        <script type="text/javascript">
            function showstarttime(){
		//alert("time called");
               $('#timepicker_customrange').timepicker({
                   hours: { starts: 0, ends: 24 },
                   minutes: { interval: 3 },
                   rows: 4,
                   showPeriodLabels: true,
                   minuteText: 'Min'
               })
            }

        </script>
	
	<script type="text/javascript">
            function showendtime(){
		//alert("time called");
               $('#timepicker_customrangeend').timepicker({
                   hours: { starts: 0, ends: 24 },
                   minutes: { interval: 3 },
                   rows: 4,
                   showPeriodLabels: true,
                   minuteText: 'Min'
               })
            }

        </script>
	
        
        
        <!--change for time ends-->
        <script>
        function verify_port(input)
            {
                var i;
                
                if(input>65535)
                {
                    return(2);
                }
                else
                {
                    for(i=0;i<input.length;i++)
                    {
                        ch=input.charCodeAt(i);
                        if(!(ch>47&&ch<58))
                        {
                            alert("u entered"+input.charCodeAt(i));
                            return(3);//different return codes based on error 
                        }    
                    }    
                }
                                                                            
                return(1);
                                                                            
            }
                                                                        
            function check_port(input)
            {
                var ret;   
                                                                                       
                ret=verify_port(document.getElementById(input).value);
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
                    document.getElementById(input).value=" ";
             
            }
            
        </script>

        
        <script type="text/javascript">
                      function retdate()
                      {
                      //alert("date called");
                      var date=document.getElementById("inputField").value;
                      return(date);
                      }


        </script>
        <style>

            body {
                /*font-family:verdana;
                font-size:15px;*/
                background-image:url('background.png');
            }
            div#left { float: left; width: 300px }
            div#right { float: right; width: 300px }

        </style>

    </head>
    <body>
        <!--change for date-->
        <script src="datepicker/jquery.ui.datepicker.js"></script>
	<script>
        $(function() {
		$( "#datepicker" ).datepicker({ maxDate: "+0M +0D" });
	});
	</script>
        <!--change for date ends-->
        
        <!--<div class="section" id="page"> 
            <div class="header"> 
                <h1><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Lucid</font></h1>
                <h2><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h2>

            </div>
        </div>
        <div class="section" id="articles"> 

           
            <div class="article" id="article1"> -->
        
        <form action="../queryresult.jsp?index=<%=request.getParameter("index")%>" method="POST">
        <table height="100%" width="100%" border="0">    
            <tr>
                <td>

                    <table border="0">
                        <tr>
                            <td width="400px" align="center">
                                <p>Enter Date: <input type="text" id="datepicker" name="date" onmouseover="showdate()"></p><!--change-->
                              
                            </td>
                            <td width="300px" align="center">
                                Start time:<input type="text" name="startTime" style="width: 70px" id="timepicker_customrange" onmouseover="showstarttime()"/>
        
                            </td>
                            <td width="300px" align="center">
                                End time:<input type="text" name="endTime"  style="width: 70px" id="timepicker_customrangeend" onmouseover="showendtime()"/>
                            </td>   
                        </tr>    
                    </table>
                </td>    
            </tr> 
            <tr>
                <td>
                    <table border="0">
                        <tr>
                            <td>

                                Source IP:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                <select name="srcOctet1" id="oct1" default-value="">
                                    <option selected>Select</option>
                                    <% int m = 0;
                                                                            while (m < 255) {%>
                                    <option><%out.print(m);%></option>
                                    <% m++;
                                                                            }%>
                                </select>
                                :<select name="srcOctet2" id="oct2">
                                    <option selected>Select</option>
                                    <% m = 0;
                                                                            while (m < 255) {%>
                                    <option><%out.print(m);%></option>
                                    <% m++;
                                                                            }%>
                                </select>
                                :<select name="srcOctet3" id="oct3">                                
                                    <option selected>Select</option>
                                    <% m = 0;
                                                                            while (m < 255) {%>
                                    <option><%out.print(m);%></option>
                                    <% m++;
                                                                            }%>
                                </select>
                                :<select name="srcOctet4" id="oct4">
                                    <option selected>Select</option>
                                    <% m = 0;
                                                                            while (m < 255) {%>
                                    <option><%out.print(m);%></option>
                                    <% m++;
                                                                            }%>
                                </select>
                                <br/>
                                Destination IP :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<select name="dstOctet1" id="oct5">
                                    <option selected>Select</option>
                                    <%m = 0;
                                                                            while (m < 255) {%>
                                    <option><%out.print(m);%></option>
                                    <% m++;
                                                                            }%>
                                </select>
                                :<select name="dstOctet2" id="oct6">
                                    <option selected>Select</option>
                                    <% m = 0;
                                                                            while (m < 255) {%>
                                    <option><%out.print(m);%></option>
                                    <% m++;
                                                                            }%>
                                </select>
                                :<select name="dstOctet3" id="oct7">
                                    <option selected>Select</option>
                                    <% m = 0;
                                                                            while (m < 255) {%>
                                    <option><%out.print(m);%></option>
                                    <% m++;
                                                                            }%>
                                </select>
                                :<select name="dstOctet4" id="oct8">
                                    <option selected>Select</option>
                                    <% m = 0;
                                                                            while (m < 255) {%>
                                    <option><%out.print(m);%></option>
                                    <% m++;
                                                                            }%>
                                </select>
                                <br/> 
                                <br/>
                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Port:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Source:&nbsp;&nbsp;:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="text" name="lowPort" id="ChoiceFromPort" title="Any positive integer between 0-65535" onchange="check_port('ChoiceFromPort')"/>
                                &nbsp;&nbsp;&nbsp;&nbsp;Destination:&nbsp;&nbsp;:&nbsp;&nbsp;<input type="text" id="ChoiceToPort" name="highPort" Title="Any positive integer between 0-65535" onchange="check_port('ChoiceToPort')" />
                                <br/>
                                <br/>
                                &nbsp;&nbsp;&nbsp;&nbsp;Protocol&nbsp;&nbsp;:&nbsp;&nbsp;&nbsp;&nbsp;<select name="protocol">
                                    <option selected>Select</option>
                                    <option>TCP</option>
                                    <option>UDP</option>
                                    <option>ICMP</option>
                                    <option>IGMP</option>
                                    <option>GRE</option>                                                                                                                        
                                    <option>SCTP</option>
                                </select>
                                <br/>
                                <%
                                String var=request.getParameter("msg");%>
                                
                                    
                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="submit" value="Show Packets"/>
                            </td>   
                        </tr>    
                    </table>     
                </td>
            </tr>   
        </table> 
                                </form>
        <!--<div class="footer"> 
    <div class="line"></div>

    <p>&copy Lucid.com</p> 

    <a href="#" class="up">Go UP</a>
</div>-->
    </body>
</html>


