/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package AccessDb;

import Core.CaptureRule;
import Utils.ConnectionDb;
import java.sql.*;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Administrator
 */
public class CaptureFilterDAO {
    public void insertCaptureFilter(CaptureRule captRule,String vmId)
    {
            ConnectionDb conn = new ConnectionDb();
            conn.getConnection();            
            String insertCaptFilter ="insert into vm_filters("
                    +"vm_id,"
                    +"proto,"
                    +"src_octet1,"
                    +"src_octet2,"
                    +"src_octet3,"
                    +"src_octet4,"
                    +"dst_octet1,"
                    +"dst_octet2,"
                    +"dst_octet3,"
                    +"dst_octet4,"
                    +"shost_mask_octet1,"
                    +"shost_mask_octet2,"
                    +"shost_mask_octet3,"
                    +"shost_mask_octet4,"
                    +"dhost_mask_octet1,"
                    +"dhost_mask_octet2,"
                    +"dhost_mask_octet3,"
                    +"dhost_mask_octet4,"
                    +"sport_low,"
                    +"sport_high,"
                    +"dport_low,"
                    +"dport_high,"
                    +"smac1,"
                    +"smac2,"
                    +"smac3,"
                    +"smac4,"
                    +"smac5,"
                    +"smac6,"
                    +"dmac1,"
                    +"dmac2,"
                    +"dmac3,"
                    +"dmac4,"
                    +"dmac5,"
                    +"dmac6)"
                    +"values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
            PreparedStatement ps = null;
            try{
                ps =conn.getConn().prepareStatement(insertCaptFilter);
                ps.setString(1,vmId);
                ps.setInt(2,captRule.getProtocol());
                for(int i=0;i<4;i++)
                {                                    
                    ps.setInt(3+i,(captRule.getSrcHost()!=null)?captRule.getSrcHost()[i]:-1);                    
                    ps.setInt(7+i,(captRule.getDstHost()!=null)?captRule.getDstHost()[i]:-1);
                    ps.setInt(11+i,(captRule.getSrcHostMask()!=null)?captRule.getSrcHostMask()[i]:-1);
                    ps.setInt(15+i,(captRule.getDstHostMask()!=null)?captRule.getDstHostMask()[i]:-1);
                }
                ps.setInt(19,captRule.getLowSrcPort());
                ps.setInt(20,captRule.getHighSrcPort());
                ps.setInt(21,captRule.getLowDstPort());
                ps.setInt(22,captRule.getHighDstPort());
                //ps.setInt(23,captRule.);                   filter id not set.................
                for(int i=0;i<6;i++)
                {                    
                    ps.setInt(23+i,(captRule.getSmac()!=null)?captRule.getSmac()[i]:-1);
                    ps.setInt(29+i,(captRule.getDmac()!=null)?captRule.getDmac()[i]:-1);
                }
                int no=ps.executeUpdate();
                
            } catch (SQLException ex) {
            Logger.getLogger(UserDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
            }
        try {
            conn.closeConnection();
        } catch (SQLException ex) {
            Logger.getLogger(CaptureFilterDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public ArrayList<CaptureRule> retrieveCaptureFilter(String vmId)
    {
            ConnectionDb conn = new ConnectionDb();
            conn.getConnection();            
            
            String retrieveCaptFilter ="select * from vm_filters where vm_id=?";
            PreparedStatement ps = null;
            ResultSet CaptFilter=null;
            ArrayList<CaptureRule> captRuleList=new ArrayList<CaptureRule>();                         
            
            try{
                ps =conn.getConn().prepareStatement(retrieveCaptFilter);
                ps.setString(1,vmId);
                CaptFilter = ps.executeQuery();
                
                while(CaptFilter.next())
                {
                    CaptureRule rule=new CaptureRule();
                    short[] temp=new short[6];
                    temp[0]=CaptFilter.getShort("smac1");
                    temp[1]=CaptFilter.getShort("smac2");
                    temp[2]=CaptFilter.getShort("smac3");
                    temp[3]=CaptFilter.getShort("smac4");
                    temp[4]=CaptFilter.getShort("smac5");
                    temp[5]=CaptFilter.getShort("smac6");
                    rule.setSmac(temp);
                    
                    short[] destmac=new short[6];
                    destmac[0]=CaptFilter.getShort("dmac1");
                    destmac[1]=CaptFilter.getShort("dmac2");
                    destmac[2]=CaptFilter.getShort("dmac3");
                    destmac[3]=CaptFilter.getShort("dmac4");
                    destmac[4]=CaptFilter.getShort("dmac5");
                    destmac[5]=CaptFilter.getShort("dmac6");
                    rule.setDmac(destmac);
                    
                    short[] srcip=new short[4];
                    srcip[0]=CaptFilter.getShort("src_octet1");
                    srcip[1]=CaptFilter.getShort("src_octet2");
                    srcip[2]=CaptFilter.getShort("src_octet3");
                    srcip[3]=CaptFilter.getShort("src_octet4");
                    rule.setSrcHost(srcip);
                    
                    
                    short[] destip=new short[4];
                    destip[0]=CaptFilter.getShort("dst_octet1");
                    destip[1]=CaptFilter.getShort("dst_octet2");
                    destip[2]=CaptFilter.getShort("dst_octet3");
                    destip[3]=CaptFilter.getShort("dst_octet4");
                    rule.setDstHost(destip);
                    
                    short[] srcipmask=new short[4];
                    srcipmask[0]=CaptFilter.getShort("shost_mask_octet1");
                    srcipmask[1]=CaptFilter.getShort("shost_mask_octet2");
                    srcipmask[2]=CaptFilter.getShort("shost_mask_octet3");
                    srcipmask[3]=CaptFilter.getShort("shost_mask_octet4");
                    rule.setSrcHostMask(srcipmask);
                    
                    
                    short[] destipmask=new short[4];
                    destipmask[0]=CaptFilter.getShort("dhost_mask_octet1");
                    destipmask[1]=CaptFilter.getShort("dhost_mask_octet2");
                    destipmask[2]=CaptFilter.getShort("dhost_mask_octet3");
                    destipmask[3]=CaptFilter.getShort("dhost_mask_octet4");
                    rule.setDstHostMask(destipmask);
                    
                    rule.setHighDstPort(CaptFilter.getInt("dport_high"));
                    rule.setHighSrcPort(CaptFilter.getInt("sport_high"));
                    rule.setLowDstPort(CaptFilter.getInt("dport_low")); 
                    rule.setLowSrcPort(CaptFilter.getInt("sport_low"));

                    rule.setProtocol(CaptFilter.getShort("proto"));
                    captRuleList.add(rule);
                }
                
              
            } catch (SQLException ex) {
            Logger.getLogger(UserDetailsDAO.class.getName()).log(Level.SEVERE, null, ex);
            }
        try {
            conn.closeConnection();
        } catch (SQLException ex) {
            Logger.getLogger(CaptureFilterDAO.class.getName()).log(Level.SEVERE, null, ex);
        }
        return captRuleList;
    }
    
/*    public static void main (String args[])
    {
        CaptureRule cpt=new CaptureRule();
        CaptureRule retcpt=new CaptureRule();
        CaptureFilterDAO obj=new  CaptureFilterDAO();
        int vmid=109;
        short temp[]=new short[6];
        for(int i=0;i<6;i++)
        {
            
            temp[i]=(short) (10+i);
        }
        //cpt.dmac=temp;
        short temp1[]=new short[6];
        for(int i=0;i<6;i++)
        {
            temp[i]=(short) (20+i);
        }
        cpt.smac=temp;
        for(int i=0;i<4;i++)
        {
            temp[i]=(short) (192-(20*i));
        }
        cpt.dstHost=temp;
        for(int i=0;i<4;i++)
        {
            temp[i]=(short) (112-(20*i));
        }
        cpt.srcHost=temp;
        for(int i=0;i<4;i++)
        {
        temp[i]=(short)(255);
        }
        cpt.dstHostMask=temp;
        for(int i=0;i<4;i++)
        {
        temp[i]=(short)(224);
        }
        cpt.srcHostMask=temp;
        cpt.highDstPort=200;
        cpt.highSrcPort=300;
        cpt.lowDstPort=100;
        cpt.lowSrcPort=50;
        cpt.protocol=203;
        obj.insertCaptureFilter(cpt, vmid);
        
        retcpt=obj.retrieveCaptureFilter(101);
        System.out.println("protocol is"+retcpt.protocol);
        System.out.println("src port range is"+retcpt.lowSrcPort+"to"+retcpt.highSrcPort);
        System.out.println("dst port range is"+retcpt.lowDstPort+"to"+retcpt.highDstPort);
        System.out.println("source mac is");
        for(int i=0;i<6;i++)
        {
            System.out.print(retcpt.smac[i]+":");
        }
        System.out.println("destination mac is");
        for(int i=0;i<6;i++)
        {
            System.out.print(retcpt.dmac[i]+":");
        }
        System.out.println("source ip is");
        for(int i=0;i<4;i++)
        {
            System.out.println(retcpt.srcHost[i]+".");
        }
        System.out.println("destination ip is");
        for(int i=0;i<4;i++)
        {
            System.out.println(retcpt.dstHost[i]+".");
        }
        System.out.println("source ip mask is");
        for(int i=0;i<4;i++)
        {
            System.out.println(retcpt.srcHostMask[i]+".");
        }
        System.out.println("destination ip mask is");
        for(int i=0;i<4;i++)
        {
            System.out.println(retcpt.dstHostMask[i]+".");
        }
        



        
    }*/
}

