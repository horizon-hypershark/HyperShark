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


        <script language="javascript" src="time/protoplasm.js"></script> 
        <script type="text/javascript"> 
            
            function time()
            {
                //alert("function called");
                Protoplasm.use('timepicker').transform('input.timepicker'); 
            }
        </script> 
        <link rel="stylesheet" type="text/css" media="all" href="datepicker/jsDatePick_ltr.min.css" />
        <script type="text/javascript" src="datepicker/jsDatePick.min.1.3.js"></script>
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

        
        <script>
        function verify_port(input)
            {
                var i;
                                                                           
                                                                            
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
                      alert("date called");
                      var date=document.getElementById("inputField").value;
                      return(date);
                      }


        </script>
        <style>

            body {
                font-family:verdana;
                font-size:15px;
                background-image:url('articleback.JPG');
            }
            div#left { float: left; width: 300px }
            div#right { float: right; width: 300px }

        </style>

    </head>
    <body>
        <!--<div class="section" id="page"> 
            <div class="header"> 
                <h1><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HyperShark</font></h1>
                <h2><font color="#FFFFFF">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;See what you Pay for</font></h2>

            </div>
        </div>
        <div class="section" id="articles"> 

           
            <div class="article" id="article1"> -->
        
        <form action="../queryresult.jsp" method="GET">
        <table height="100%" width="100%" border="0">    
            <tr>
                <td>

                    <table border="0">
                        <tr>
                            <td width="400px" align="center">
                                Enter date:<input type="text" size="12" align="center" name="date" id="inputField" onclick="date()" />

                            </td>
                            <td width="300px" align="center">
                                Start time:<input type="text" name="startTime" class="timepicker" /> 

                            </td>
                            <td width="300px" align="center">
                                End time:<input type="text" name="endTime" class="timepicker" />
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
                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Port:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Source:&nbsp;&nbsp;:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="text" name="lowPort" id="ChoiceFromPort" title="Any positive integer between 0-65535"/>
                                &nbsp;&nbsp;&nbsp;&nbsp;Destination:&nbsp;&nbsp;:&nbsp;&nbsp;<input type="text" id="ChoiceToPort" name="highPort" Title="Any positive integer between 0-65535" />
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

    <p>&copy HyperShark.com</p> 

    <a href="#" class="up">Go UP</a>
</div>-->
    </body>
</html>


