<%-- 
    Document   : form
    Created on : 17 Jan, 2012, 4:52:03 PM
    Author     : varun
--%>

<%@page import="Core.VirtualMachine"%>
<%@page import="Beans.UserDataBean"%>
<%@page import="java.sql.ResultSet"%>
<%@page import="util.ExecuteQuery"%>
<%@page import="Utils.ConnectionDb"%>
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
        <title>Lucid-VMList</title>
        <link rel="stylesheet" type="text/css" href="style.css" />
        <!--<link href="styles.css" rel="stylesheet" type="text/css" />-->
        <!-- pop up form-->  
        <link rel="stylesheet" type="text/css" href="button_form.css" />
        <script type="text/javascript" src="http://code.jquery.com/jquery-latest.pack.js"></script>

        <script type="text/javascript" src="button_form.js"></script>
        <script type="text/javascript" src="validate_form.js"></script>
        
        <script type="text/javascript">
            var index;                            
            function setIndex(temp)
            {
                index=temp;
                
            }
        
        </script>
        
        <!--change-->
        <!--change -->
        <script><%int count=userData.getUserDetails().getVirMachineList().size();%>
        var store_packets=new Array(<%=count%>);
        <%
            for(int i=0;i<count;i++){                   
        %>
                store_packets[<%=i%>]=0;
        <%}%>
            function markcheck(index)
            {                
                if(document.getElementById("packets"+index).checked==true)
                   store_packets[index]=1;//return(1);
                else
                  store_packets[index]=0;//return(0);
                //alert("store packets is"+store_packets[index]);
            }
        </script>
        <!--change ends-->
        <style type="text/css">
            body
            {
                background-image:url('background.png');
            }
        </style>
        
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
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
               <a href="../logout.jsp"><font color="#FFFFFF" size="4px">Logout</font></a>
             </h3>       
                
            </div>

            <br/><br/>
        
        
        
        <div class="section" id="articles"> <!-- A new section with the articles -->

                <!-- Article 1 start -->

                <div class="article" id="article1"> <!-- The new article tag. The id is supplied so it can be scrolled into view. -->
                   
                    <!-- main code starts from here-->
                    <table border="0" height="100%" width="100%" cellspacing="40px">
                        <tr>
                            <td width="20%" valign="top">
                                <table border="0" width="100%">
                                    <tr  width="100%" bgcolor="">
                                        <td valign="bottom"><!-- image-->
                                            <img border="0" src="user.png" alt="Hypershark" width="100px" height="80px" align="center" margin-top="-10px"/>

                                        </td>
                                    </tr>    
                                    <tr>
                                        <td>    
                                            <table border="0" width="100%" height="20px">
                                                <!-- customer information-->   
                                                <tr height="3px">
                                                    <td><p>ID&nbsp;&nbsp;:&nbsp;&nbsp;<%out.println(userData.getUserDetails().getUserId());%></p>
                                                    </td>
                                                </tr>
                                                <tr height="3px">
                                                    <td><p>Name&nbsp;&nbsp;:&nbsp;&nbsp;<%out.println(userData.getUsername());%></p>
                                                    </td>
                                                </tr>
                                                <tr height="3px">
                                                    <td><p>Information&nbsp;&nbsp;:&nbsp;&nbsp;<%out.println(userData.getUserDetails().getUserInfo());%></p>
                                                    </td>
                                                </tr>
                                                <tr height="3px">
                                                    <td><p>VMs assigned&nbsp;&nbsp;:&nbsp;&nbsp;<%out.println(userData.getUserDetails().getVirMachineList().size());%></p>
                                                    </td>
                                                </tr>
                                            </table>
                                        </td>
                                    </tr>

                                </table>    
                            </td>  

                            <!-- draw a vertical line-->
                            <td width="10" bgcolor="#DEDEDE">
                            
                            </td>
                            <td width="80%"><!-- 2nd column of main table -->
                                <table height="100%" border="0">

                                    <%                                        
                                    int j=0; 
                                    for(VirtualMachine virMach : userData.getUserDetails().getVirMachineList()) {
                                    %>

                                    <tr width="300px">
                                        <td width="100%" height="5%">
                                            <table height="100%" border="0" width="100%"> 
                                                <tr>
                                                    <td  width="600px" height="100%">
                                                        <img border="0" src="<%=virMach.getVmImage()%>" alt="Hypershark" width="100px" height="80px"/><br/>
                                                        <% 
                                                            out.println("VM Name  :    "+virMach.getVmName());
                                                        %>
                                                    </td>
                                                    <td width="30%">
                                                       
                                                           <%int packet=0;
                                                           
                                                             %>
                                                          <a href="../toggle_status.jsp?index=<%=j%>" onclick="location.href=this.href+'&packets='+store_packets[<%=j%>];return false;"> <button background-color="#000000" <%=(virMach.getState().equals("shut"))?"disabled=\"disabled\"":""%>><%=(virMach.isMonitoringStatus())?"STOP":"START"%></button></a>
                                                           &nbsp;&nbsp;&nbsp;
                                                           <input type="checkbox" id="packets<%=j%>" name="packets<%=j%>" value="<%=j%>" onchange="markcheck(<%=j%>)"/> Keep Packets<br />
 

                                                    </td>    
                                                    <td width="15%">
                                                     
                                                            <!--<input type="button" name="Set Filters" value="Set Filters" onclick="dialog"/>-->
                                                            <!--<button align="center" onclick="#dialog" name="modal"> Set Filters</button>-->
                                                            <a href="#dialog" name="modal"><button value="Add Filter" onclick="setIndex(<%=j%>)">Add Filter</button></a>
                                                                                                                                                                        
                                                    </td>   
                                                   
                                                    
                                                    <td width="15%">
                                                                                                                 <!--<input type="button" name="Set Filters" value="Set Filters" onclick="dialog"/>-->
                                                            <!--<button align="center" onclick="#dialog" name="modal"> Set Filters</button>-->
                                                            
                                                            <a href="tabs.jsp?index=<%=j%>"><button width="50px"/>Show history</button></a>
                                                    </td>

                                                    <td width="15%">
                                                        <img src="<%if(virMach.getState().equals("shut")){ out.print("grayscale.png");%> " width="25px" height="25px" <% }else if(virMach.isMonitoringStatus()){ out.print("greensignal.png");}else {out.print("redsignal.png");}%>" width="25px" height="25px" align="right"/>
                                                        
                                                    </td>
                                                    
                                                    
                                                </tr>    
                                            </table>    
                                        </td>
                                    </tr>
                                    <tr>
                                    <td>
                                        <hr class="hor_line"/>
                                    </td>
                                    </tr>
                                    <%
                                        j++;
                                        }%>
                                </table>
                            </td>
                        </tr>    
                    </table>
                </div>
            </div> 
                                                         <div id="boxes">
                                                            <div id="dialog" class="window">
                                                                <p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SET FILTERS FOR SELECTED VM&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#"class="close" margin-left="120px"float="right"/>CLOSE&nbsp;&nbsp;<b>X</b></a></p>
                                                                <!--close link-->



                                                                <form name="input" action="capturefilter.jsp?index="+index method="get">
                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Source Mac:&nbsp;&nbsp;&nbsp;<input type="text" id ="srcMac" name="srcMac" title="Example: 12:23:34:ab:cd:ef" onblur="verify('src_mac')"/>   

                                                                    <br/>
                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Destination Mac:&nbsp;&nbsp;&nbsp; <input type="text" id ="dstMac" name="dstMac" title="Example: 12:23:34:ab:cd:ef" onblur="verify('dst_mac')"/>   


                                                                    <br/>
                                                                    <br/>
                                                                    <!-- for ip address-->                                                                    
                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IP Address:&nbsp;&nbsp;&nbsp;<br/>
                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Low:
                                                                    <%for(int i=1;i<=4;i++)
                                                                    {%>
                                                                    <select name="srcOctet<%=i%>" id="srcOctet<%=i%>">
                                                                        <option>Select</option>
                                                                        <% int m = 0;
                                                                            while (m < 255) {%>
                                                                        <option><%out.print(m);%></option>
                                                                        <% m++;
                                                                            }%>
                                                                    </select>
                                                                            <%if(i!=4)out.print(":");}%>                                                                                                                                       <br/>

                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;High :&nbsp;&nbsp;&nbsp;
                                                                    <%for(int i=1;i<=4;i++)
                                                                    {%>
                                                                    <select name="dstOctet<%=i%>" id="dstOctet<%=i%>">
                                                                        <option>Select</option>
                                                                        <%int m = 0;
                                                                            while (m < 255) {%>
                                                                        <option><%out.print(m);%></option>
                                                                        <% m++;
                                                                            }%>
                                                                    </select>
                                                                    <%if(i!=4)out.print(":");}%>
                                                                    
                                                                    <br/>
                                                                    <br/>
                                                                    
                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Port:&nbsp;&nbsp;&nbsp;&nbsp;

                                                                    Low<input type="text" name="lowSrcPort" id="lowSrcPort" onblur="verify_port('ChoiceFrom')"/>
                                                                    High<input type="text" id="highSrcPort" name="highSrcPort" onblur="verify_port('ChoiceTo')"/>
                                                                    <br/>
                                                                    <br/>

                                                                    <br/>
                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Protocol:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                                                    <select name="protocol">
                                                                        <option value="Select">Select</option>
                                                                        <option value="TCP">TCP</option>
                                                                        <option value="UDP">UDP</option>
                                                                        <option value="ICMP">ICMP</option>
                                                                        <option value="IGMP">IGMP</option>
                                                                        <option value="GRE">GRE</option>
                                                                        <option value="SCTP">SCTP</option>

                                                                    </select>
                                                                    <br/>
                                                                    <br/>
                                                                    
                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                                                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
               
                                                                    <input type="submit" value="SUBMIT"/>
                                                                    
                                                                    <br/>
                                                                    <br/>
                                                                    <br/>
                                                                    
                                                                    <label for="info">Note:&nbsp;&nbsp;All the fields are optional and the traffic generated from specified values will not be monitored</label>
                                                                    </form>
                                                                    
                                                                    
                                                            </div>

                                                        </div>
                                                                    
                                                                    
                                                        <!-- Mask to cover the whole screen -->
                                                        <div id="mask"></div>
                                                        </div>
                                
        <!-- make entry in database-->
                                 <div id="makeentry">
                                                        <%
                                                         /* int src_octet1,src_octet2,src_octet3,src_octet4,dst_octet1;
                                                          int dst_octet2,dst_octet3,dst_octet4,shost_mask_octet1;
                                                          int shost_mask_octet2,shost_mask_octet3,shost_mask_octet4;
                                                          int dhost_mask_octet1,dhost_mask_octet2,dhost_mask_octet3;
                                                          int dhost_mask_octet4,proto,sport_low,sport_high,dport_low,dport_high;
                                                          
                                                          String octet=request.getParameter("octet1");
                                                          if(checknull(octet))
                                                          src_octet1=-1;
                                                          else
                                                          src_octet1=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet2");
                                                          if(checknull(octet))
                                                          src_octet2=-1;
                                                          else
                                                          src_octet2=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet3");
                                                          if(checknull(octet))
                                                          src_octet3=-1;
                                                          else
                                                          src_octet3=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet4");
                                                          if(checknull(octet))
                                                          src_octet4=-1;
                                                          else
                                                          src_octet4=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet5");
                                                          if(checknull(octet))
                                                          dst_octet1=-1;
                                                          else
                                                          dst_octet1=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet6");
                                                          if(checknull(octet))
                                                          dst_octet2=-1;
                                                          else
                                                          dst_octet2=Integer.parseInt(octet);
                                                          
                                                          
                                                          octet=request.getParameter("octet7");
                                                          if(checknull(octet))
                                                          dst_octet3=-1;
                                                          else
                                                          dst_octet3=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet8");
                                                          if(checknull(octet))
                                                          dst_octet4=-1;
                                                          else
                                                          dst_octet4=Integer.parseInt(octet);
                                                          
                                                          
                                                          
                                                          octet=request.getParameter("octet9");
                                                          if(checknull(octet))
                                                          shost_mask_octet1=-1;
                                                          else
                                                          shost_mask_octet1=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet10");
                                                          if(checknull(octet))
                                                          shost_mask_octet2=-1;
                                                          else
                                                          shost_mask_octet2=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet11");
                                                          if(checknull(octet))
                                                          shost_mask_octet3=-1;
                                                          else
                                                          shost_mask_octet3=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet12");
                                                          if(checknull(octet))
                                                          shost_mask_octet4=-1;
                                                          else
                                                          shost_mask_octet4=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet13");
                                                          if(checknull(octet))
                                                          dhost_mask_octet1=-1;
                                                          else
                                                          dhost_mask_octet1=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet14");
                                                          if(checknull(octet))
                                                          dhost_mask_octet2=-1;
                                                          else
                                                          dhost_mask_octet2=Integer.parseInt(octet);
                                                          
                                                          
                                                          octet=request.getParameter("octet15");
                                                          if(checknull(octet))
                                                          dhost_mask_octet3=-1;
                                                          else
                                                          dhost_mask_octet3=Integer.parseInt(octet);
                                                          
                                                          octet=request.getParameter("octet16");
                                                          if(checknull(octet))
                                                          dhost_mask_octet4=-1;
                                                          else
                                                          dhost_mask_octet4=Integer.parseInt(octet);
                                                          
                                                          String port=request.getParameter("ChoiceFrom");
                                                          if(checknull(port))
                                                          sport_low=-1;
                                                          else
                                                          sport_low=Integer.parseInt(port);
                                                          
                                                          
                                                          port=request.getParameter("ChoiceTo");
                                                          if(checknull(port))
                                                          sport_high=-1;
                                                          else
                                                          sport_high=Integer.parseInt(port);
                                                          
                                                          port=request.getParameter("ChoiceFromDest");
                                                          if(checknull(port))
                                                          dport_low=-1;
                                                          else
                                                          dport_low=Integer.parseInt(port);
                                                          
                                                          
                                                          port=request.getParameter("ChoiceToDest");
                                                          if(checknull(port))
                                                          dport_high=-1;
                                                          else
                                                          dport_high=Integer.parseInt(port);
                                                          
                                                          
                                                          String protocol=request.getParameter("protocol");
                                                          if(checknull(protocol))
                                                          proto=-1;
                                                          else
                                                          proto=Integer.parseInt(protocol);
                                                          
                                                          
                                                         query = "insert into vm_filters values('" + vm_id + "','" + proto + "','" + src_octet1 + "','" + src_octet2 + "','" + src_octet3 + "','" + src_octet4 + "','" + dst_octet1 + "','" + dst_octet2 + "','" + dst_octet3 + "',,'" + dst_octet4 + "',,'" + shost_mask_octet1 + "','" + shost_mask_octet2 + "','" + shost_mask_octet3 + "','" + shost_mask_octet4 + "','" + dhost_mask_octet1 + "','" + dhost_mask_octet2 + "','" + dhost_mask_octet3 + "','" + dhost_mask_octet4 + "','" + sport_low + "','" + sport_high + "','" + dport_low + "','" + dport_high + "','" + smac + "','" + dmac + "'" ;
                                                         eq.execQuery(conn, query);*/%>
                                                         
                                                          
                                                         <%!
                                                         /*boolean checknull(String value)
                                                           {
                                                               if(value.equals(null))
                                                                   return true;
                                                               return false;
                                                           }*/
                                                        %>
                                                    </div>    
                                                   
                                
                                
            <div class="footer"> <!-- Marking the footer section -->
                <div class="line"></div>

                <p>&copy Lucid.com</p> <!-- Change the copyright notice -->

                <a href="#" class="up">Go UP</a>
            </div>
        </div>

    </body>
</html>
<%}%>