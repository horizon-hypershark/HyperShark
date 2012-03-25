/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l3Proto;

import Utils.Conversions;

/**
 *
 * @author root
 */
public class ArpHdr extends L3proto 
{
    private short ar_hdr;  //format of h/w address
    private short ar_pro;  //format of protocol address
    private short ar_hln;   //length of h/w address
    private short ar_pln;   //length of protocol address
    private short ar_op;   //ARP opcode
    private byte h_dest[]= new byte[6];//ETH_ALEN value = 6 to be changed
    private byte h_source[]= new byte[6];//ETH_ALEN value = 6 to be changed
    private int saddr;
    private int daddr;

    public int getDaddr() {
        return daddr;
    }

    public void setDaddr(int daddr) {
        this.daddr = daddr;
    }

   public int getSaddr() {
        return saddr;
    }

    public void setSaddr(int saddr) {
        this.saddr = saddr;
    }

    public short getAr_hdr() {
        return ar_hdr;
    }

    public void setAr_hdr(short ar_hdr) {
        this.ar_hdr = ar_hdr;
    }

    public short getAr_hln() {
        return ar_hln;
    }

    public void setAr_hln(short ar_hln) {
        this.ar_hln = ar_hln;
    }

    public byte[] getH_dest() {
        return h_dest;
    }

    public void setH_dest(byte[] h_dest) {
        this.h_dest = h_dest;
    }

    public byte[] getH_source() {
        return h_source;
    }

    public void setH_source(byte[] h_source) {
        this.h_source = h_source;
    }

    public short getAr_op() {
        return ar_op;
    }

    public void setAr_op(short ar_op) {
        this.ar_op = ar_op;
    }

    public short getAr_pln() {
        return ar_pln;
    }

    public void setAr_pln(short ar_pln) {
        this.ar_pln = ar_pln;
    }

    public short getAr_pro() {
        return ar_pro;
    }

    public void setAr_pro(short ar_pro) {
        this.ar_pro = ar_pro;
    }
    @Override
   public String getPacket()
     {
         //String str = new Integer(saddr).toString();
         //String name= new String();
         //InetAddress ia = null;
            //name=ia.getHostName();
                
           String arp_pkt_str=new String();
           //System.out.println("\n in java arphdr h/w add is"+ar_hdr);
           arp_pkt_str="<b>ARP HEADER</b>"+"<br>"+"<b>HARDWARE ADDRESS&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(ar_hdr)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>PROTOCOL ADDRESS&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(ar_pro)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>H/W ADD LENGTH&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(ar_hln)+
                   "<br/>"+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>PROTO ADD LENGTH&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(ar_pln)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>OPCODE&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(ar_op)+"<br/>";
           return(arp_pkt_str);
     }

    @Override
    public byte getProtocol() {
        throw new UnsupportedOperationException("Not supported yet.");
    }
    
}
