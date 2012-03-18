<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

        <link rel="stylesheet" href="menu.css" type="text/css" media="screen" />
        <title>Mega Drop Down Menu</title>
        <!--[if IE 6]>
        <style>
        body {behavior: url("csshover3.htc");}
        #menu li .drop {background:url("img/drop.gif") no-repeat right 8px; 
        </style>
        <![endif]-->
        <style type="text/css">
            body
            {
                background-image:url('background.png');
            }
        </style>
        <script>
        function disp()
        {
        document.getElementById("filter").style.display="none";
        document.getElementById("filter").style.visibility="hidden";
        }
        </script>
        
        <script>
        /*function disp()
        {        
        alert("script called");
        document.getElementById("filter").style.display="block";
        document.getElementById("filter").style.visibility="visible";
        }*/
        </script>
        --
        <link href="style.css" rel="stylesheet" type="text/css" />

    </head>
    <body>
        <ul id="menu">
            <li id ="filter" class="menu_right" ><a href="#" class="drop" onclick="disp()">Filter More</a><!-- Begin 3 columns Item -->
                <div class="dropdown_3columns align_right" id="filter"><!-- Begin 3 columns container -->
                    <div class="col_3">
                        <h2>Specify Filters</h2>
                    </div>
                    <div class="col_1">
                        <ul>			
                            <li>				
                                <form name="filter" action="capturefilter.jsp" method="get">
                                                                
                                Source Mac:<input type="text"/>
                                    <br/>
                                    Dest Mac:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="text"/>
                                    <br/>
                                    Source Ip:&nbsp;&nbsp;&nbsp;&nbsp;	
                                    <%for (int i = 1; i <= 4; i++) {%>
                                    <select name="srcOctet<%=i%>" id="srcOctet<%=i%>">
                                        <option>Select</option>
                                        <% int m = 0;
                                        while (m < 255) 
                                        {%>
                                        <option><%out.print(m);%></option>
                                        <% m++;
                                        }%>
                                    </select>
                                    <%if (i != 4)
                                    {
                                        out.print(":");
                                    }
                                  }%>                                                                                                                                       <br/>
                                  
                                  Dest Ip:&nbsp;&nbsp;&nbsp;&nbsp;	
                                    <%for (int i = 5; i <= 8; i++) {%>
                                    <select name="srcOctet<%=i%>" id="srcOctet<%=i%>">
                                        <option>Select</option>
                                        <% int m = 0;
                                        while (m < 255) 
                                        {%>
                                        <option><%out.print(m);%></option>
                                        <% m++;
                                        }%>
                                    </select>
                                    <%if (i != 8)
                                    {
                                        out.print(":");
                                    }
                                  }%>                                                                                                                                       <br/>
                                    Protocol:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="text"/>
                                  <br/>
                                    Size:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="text"/>
                                  <br/>
                                  <input type="checkbox" name="protocol" value="TCP" />TCP
                                  <input type="checkbox" name="protocol" value="UDP" />UDP 
                                  <br/>
                                  <br/>
                                  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                  <input type="submit" value="Filter Results" align="right"/>
                                                                    
                                  
                                </form>
                            </li>		
                        </ul>                
                           
                    </div>
                </div><!-- End 3 columns container -->
            </li><!-- End 3 columns Item -->
        </ul>
    </body>
</html>