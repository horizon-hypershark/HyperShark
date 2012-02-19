/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core.Protocols.l4Proto;

/**
 *
 * @author root
 */
public class Icmphdr extends L4proto
{
    public static final short ICMP = 1;
    /*byte type;
    byte code;
    short checksum;
  union {
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
    public String getPacket() {
        //throw new UnsupportedOperationException("Not supported yet.");
        return null;
    }
}
