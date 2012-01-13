package testpackage;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author varun
 */
import java.util.*;
import java.io.*;
import java.lang.*;

public class ReadFile
{
		// total 35 variables
                
                // hs_pkt_hdr
                int pkt_id; 
                String buffer;
                short buffer_len;

		// pfring_pkthdr
                int caplen; 
                int len; // length of the packet

	        // pfring_extended_pkthdr 
		long timestamp_ns;
                short rx_direction;
                int if_index;
                int pkt_hash;
                short parsed_header_len;

                // pkt_parsing_info
                short smac[]=new short[6];
		short dmac[]=new short[6];
		short eth_type,vlan_id,ip_version,l3_proto,ip_tos;
                int ip_src,ip_dst;  // ipv4 addresses
                short l4_src_port,l4_dst_port,flags,last_matched_plugin_id,last_matched_rule_id;
                int seq_num,ack_num;       

                // pkt_offset
                short eth_offset,vlan_offset,l3_offset,l4_offset,payload_offset;
                
                // pkt_flow_info
                int in_iface,out_iface,samplingPopulation,flow_sequence;

                                  	

	private native void loadFile();
  	static
	{
                String s=System.getProperty("java.library.path");
                System.setProperty("java.library.path",s);

   		System.loadLibrary("ReadFile");
  	}
	public static int main1()
	{
	    
    	    ReadFile mappedFile=new ReadFile();
	    mappedFile.loadFile();
	    
	/*System.out.println(mappedFile.pkt_id);
  	System.out.println(mappedFile.caplen);
	System.out.println(mappedFile.len);
	System.out.println(mappedFile.timestamp_ns);
  	System.out.println(mappedFile.rx_direction);
	System.out.println(mappedFile.if_index);
	System.out.println(mappedFile.pkt_hash);
  	System.out.println(mappedFile.parsed_header_len);
	System.out.println(mappedFile.dmac);
	System.out.println(mappedFile.smac);
  	System.out.println(mappedFile.eth_type);
	System.out.println(mappedFile.vlan_id);
	System.out.println(mappedFile.ip_version);
  	System.out.println(mappedFile.l3_proto);
	System.out.println(mappedFile.ip_tos);
	System.out.println(mappedFile.ip_src);
	System.out.println(mappedFile.ip_dst);
	System.out.println(mappedFile.l4_src_port);
	System.out.println(mappedFile.l4_dst_port);
	System.out.println(mappedFile.flags);
	System.out.println(mappedFile.seq_num);
	System.out.println(mappedFile.ack_num);
	System.out.println(mappedFile.last_matched_plugin_id);
	System.out.println(mappedFile.last_matched_rule_id);
	System.out.println(mappedFile.eth_offset);
	System.out.println(mappedFile.vlan_offset);
	System.out.println(mappedFile.l3_offset);
	System.out.println(mappedFile.l4_offset);
	System.out.println(mappedFile.payload_offset);
	System.out.println(mappedFile.in_iface);
	System.out.println(mappedFile.out_iface);
	System.out.println(mappedFile.samplingPopulation);
	System.out.println(mappedFile.flow_sequence);
	System.out.println(mappedFile.buffer);
	System.out.println(mappedFile.buffer_len);*/
            return mappedFile.buffer_len;
            //return 10;
	}  // end of void main
} // end of class

