/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l4Proto;

import Utils.Conversions;

/**
 *
 * @author root
 */
public class Icmphdr extends L4proto
{
    public static final short ICMP = 1;
    private byte type;
    private byte code;
    private short checksum;
    private String typeStr;
    private String codeStr;
    private int idOrUnused;
    private int sequenceOrMtu;

    public int getIdOrUnused() {
        return idOrUnused;
    }

    public void setIdOrUnused(int idOrUnused) {
        this.idOrUnused = idOrUnused;
    }

    public int getSequenceOrMtu() {
        return sequenceOrMtu;
    }

    public void setSequenceOrMtu(int sequenceOrMtu) {
        this.sequenceOrMtu = sequenceOrMtu;
    }
    
    public short getChecksum() {
        return checksum;
    }

    public void setChecksum(short checksum) {
        this.checksum = checksum;
    }

    public byte getCode() {
        return code;
    }

    public void setCode(byte code) {
        this.code = code;
    }

    public byte getType() {
        return type;
    }

    public void setType(byte type) {
        this.type = type;
    }

    public String getCodeStr() {
        return codeStr;
    }

    public void setCodeStr(String codeStr) {
        this.codeStr = codeStr;
    }

    public String getTypeStr() {
        return typeStr;
    }

    public void setTypeStr(String typeStr) {
        this.typeStr = typeStr;
    }
    
  /*union {
	struct {
		__be16	id;
		__be16	sequence;
	} echo;
	__be32	gateway;
	struct {
		__be16	__unused;
		__be16	mtu;
	} frag;
  } un;*/

    @Override
    public String getPacket()
    {
        String icmp_pkt_str=new String();
        //System.out.println("in getpacket fr icmp type is"+type+"code is"+Conversions.byteToUnsigned(code)+"checksum is"+Conversions.shortToUnsigned(checksum));
        icmp_pkt_str="<b>ICMP HEADER</b>"+"<br>"+"<br>"+"<b>TYPE&nbsp;&nbsp;::&nbsp;&nbsp;"+type+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>CODE&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.byteToUnsigned(code)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+"<b>CHECKSUM&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+Conversions.shortToUnsigned(checksum)+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
                          
        
        if(typeStr!=null)
            icmp_pkt_str+="<b>TYPE_DESCRIPTION&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+typeStr+"<br/>";
        if(codeStr!=null)
            icmp_pkt_str+="<b>CODE_DESCRIPTION&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+codeStr;
        icmp_pkt_str+="<b>ID OR UNUSED&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+idOrUnused+"<b>SEQORMTU&nbsp;&nbsp;::&nbsp;&nbsp;</b>"+sequenceOrMtu;
        
        return(icmp_pkt_str);
    }
}
