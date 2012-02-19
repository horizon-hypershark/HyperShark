
<%@page import="java.sql.ResultSet"%>
<%@page import="util.ExecuteQuery"%>
<%@page import="Utils.ConnectionDb"%>
<%@page import="Core.CaptureRule"%>

<%@page import="Utils.*"%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
    </head>
    <body>
     
        <% ConnectionDb conn = new ConnectionDb();
                      ExecuteQuery eq = new ExecuteQuery();
                      conn.getConnection();              
                                                int vm_id=101;
                                                int vm_count=0;
                                                int cust_id=0;
                                                int i=1;
                                                String query = "select * from vm_filters where vm_id='" + vm_id + "'";
                                                String query1 = "select * from cust_vm where vm_id='" + vm_id + "'";
                                                ResultSet valUser = eq.execQuery(conn,query1);
                                                if(valUser.next())
                                                {
                                                    cust_id= valUser.getInt("cust_id");
                                                        
                                                }
                                                query1= "select *from cust_details where cust_id='" + cust_id + "'";
                                                valUser= eq.execQuery(conn,query1);
                                                
                                                
                                                    
                                                    
                                                if(valUser.next())
                                                {
                                                    vm_count = valUser.getInt("vm_count");
                                                
                                                }
                                                valUser = eq.execQuery(conn,query);
                                                CaptureRule cpt = new CaptureRule();
                                                
                                                String cust_name=null;
                                                String cust_info=null;
                                                %>
                                                <table>
                                                    <tr>
                                                        <td width ="8%"> SOURCE IP </td>
                                                        <td width ="8%"> DEST. IP</td>
                                                        <td width ="9%"> SOURCE MAC</td>
                                                        <td width ="9%"> DEST. MAC</d>
                                                        <td width ="7%"> SOURCE PORT1</d>
                                                        <td width ="7%"> SOURCE PORT2</d>
                                                        <td width ="7%"> DEST. PORT1</d> 
                                                        <td width ="7%"> DEST. PORT2</d>
                                                        <td width ="8%"> SOURCE SUBNET</d>
                                                        <td width ="8%"> DEST. SUBNET</d>
                                                        <td width ="8%"> PROTOCOL</d>    
                                                            
                                                    </tr>
                                                        

                                                    <%
                                                while(valUser.next())
                                                {    
                                                
                                                   
                                                   
                                                     
                                                     cpt.smac[0]=valUser.getInt("smac1");
                                                    cpt.smac[1]=valUser.getInt("smac2");
                                                    cpt.smac[2]=valUser.getInt("smac3");
                                                    cpt.smac[3]=valUser.getInt("smac4");
                                                    cpt.smac[4]=valUser.getInt("smac5");
                                                    cpt.smac[5]=valUser.getInt("smac6");
                                                    cpt.dmac[0]=valUser.getInt("dmac1");
                                                    cpt.dmac[1]=valUser.getInt("dmac2");
                                                    cpt.dmac[2]=valUser.getInt("dmac3");
                                                    cpt.dmac[3]=valUser.getInt("dmac4");
                                                    cpt.dmac[4]=valUser.getInt("dmac5");
                                                    cpt.dmac[5]=valUser.getInt("dmac6");
                                                    cpt.srcHost[0]=valUser.getInt("src_octet1");
                                                    cpt.srcHost[1]=valUser.getInt("src_octet2");
                                                    cpt.srcHost[2]=valUser.getInt("src_octet3");
                                                    cpt.srcHost[3]=valUser.getInt("src_octet4");
                                                    cpt.dstHost[0]=valUser.getInt("dst_octet1");
                                                    cpt.dstHost[1]=valUser.getInt("dst_octet2");
                                                    cpt.dstHost[2]=valUser.getInt("dst_octet3");
                                                    cpt.dstHost[3]=valUser.getInt("dst_octet4");
                                                    cpt.srcHostMask[0]=valUser.getInt("shost_mask_octet1");
                                                    cpt.srcHostMask[1]=valUser.getInt("shost_mask_octet2");
                                                    cpt.srcHostMask[2]=valUser.getInt("shost_mask_octet3");
                                                    cpt.srcHostMask[3]=valUser.getInt("shost_mask_octet4");
                                                    cpt.dstHostMask[0]=valUser.getInt("dhost_mask_octet1");
                                                    cpt.dstHostMask[1]=valUser.getInt("dhost_mask_octet2");
                                                    cpt.dstHostMask[2]=valUser.getInt("dhost_mask_octet3");
                                                    cpt.dstHostMask[3]=valUser.getInt("dhost_mask_octet4");
                                                    cpt.lowSrcPort = valUser.getShort("sport_low");
                                                    cpt.highSrcPort = valUser.getShort("sport_high");
                                                    cpt.lowDstPort = valUser.getShort("dport_low");
                                                    cpt.highDstPort = valUser.getShort("dport_high");
                                                    cpt.protocol = valUser.getInt("proto"); 
                                                 %>
                                                
                                                                                                                                             
                                               
                                                    <tr>
                                                        <td width ="8%"><% out.print(cpt.srcHost[0]);out.print(".");out.print(cpt.srcHost[1]);out.print(".");out.print(cpt.srcHost[2]);out.print(".");out.print(cpt.srcHost[3]);%></td>
                                                        <td width ="8%"><% out.print(cpt.dstHost[0]);out.print(".");out.print(cpt.dstHost[1]);out.print(".");out.print(cpt.dstHost[2]);out.print(".");out.print(cpt.dstHost[3]);%></td>
                                                        <td width ="9%"><% out.print(cpt.smac[0]);out.print(":");out.print(cpt.smac[1]);out.print(":");out.print(cpt.smac[2]);out.print(":");out.print(cpt.smac[3]);out.print(":");out.print(cpt.smac[4]);out.print(":");out.print(cpt.smac[5]);%></td>
                                                        <td width ="9%"><%out.print(cpt.dmac[0]);out.print(":");out.print(cpt.dmac[1]);out.print(":");out.print(cpt.dmac[2]);out.print(":");out.print(cpt.dmac[3]);out.print(":");out.print(cpt.dmac[4]);out.print(":");out.print(cpt.dmac[5]);%></d>
                                                        <td width ="7%"><%out.print(cpt.lowSrcPort);%></d>
                                                        <td width ="7%"><%out.print(cpt.highSrcPort);%></d>
                                                        <td width ="7%"><%out.print(cpt.lowDstPort);%></d> 
                                                        <td width ="7%"><%out.print(cpt.highDstPort);%></d>
                                                        <td width ="8%"><%out.print(cpt.srcHostMask[0]);out.print(".");out.print(cpt.srcHostMask[1]);out.print(".");out.print(cpt.srcHostMask[2]);out.print(".");out.print(cpt.srcHostMask[3]);%></td>
                                                        <td width ="8%"><%out.print(cpt.dstHostMask[0]);out.print(".");out.print(cpt.dstHostMask[1]);out.print(".");out.print(cpt.dstHostMask[2]);out.print(".");out.print(cpt.dstHostMask[3]);%> </td>
                                                        <td width ="8%"><%out.print(cpt.protocol);%> </td>    
                                                            
                                                    </tr>
                                                <% } %>  <!-- end of while loop -->      
    </body>
</html>
