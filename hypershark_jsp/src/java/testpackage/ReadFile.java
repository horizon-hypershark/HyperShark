package testpackage;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author varun
 */
	
public class ReadFile
{
	public int pkt_id; 
	public String buffer;
	public short buffer_len;
	// pfring_pkthdr
	public int caplen; 
	public int len;
	// pfring_extended_pkthdr 
	public long timestamp_ns;
	public short rx_direction;
	public int if_index;
	public int pkt_hash;
	public short parsed_header_len;
	// pkt_parsing_info
	public short eth_type,vlan_id,ip_version,l3_proto,ip_tos;
	public int ip_src,ip_dst;  // ipv4 addresses
	public short l4_src_port,l4_dst_port,flags,last_matched_plugin_id,last_matched_rule_id;
	public int seq_num,ack_num;       
	// pkt_offset
	public short eth_offset,vlan_offset,l3_offset,l4_offset,payload_offset;
	// pkt_flow_info
	public int in_iface,out_iface,samplingPopulation,flow_sequence;
	public byte dmac[]=new byte[6];
	public byte smac[]=new byte[6];
	public native void loadFile();
	static
	{
		System.loadLibrary("ReadFile");
	}
	public static void main(String args[])
	{
		//int j=0;
		//ReadFile mappedFile=new ReadFile();
		//mappedFile.loadFile();
		/*while(j<6)
		{	
			System.out.println("in java"+mappedFile.d[j].pkt_id);
			System.out.print("\nSMAC = ");
			for (int i=0;i<mappedFile.d[j].smac.length; i++)
			{ 
				System.out.print(Integer.toString((mappedFile.d[j].smac[i] & 0xff) + 0x100, 16).substring(1));
				if(i!=mappedFile.d[j].smac.length-1)
				System.out.print("-");
			}
			System.out.print("\nDMAC = ");
			for (int i=0;i<mappedFile.d[j].dmac.length; i++)
			{ 
		      		System.out.print(Integer.toString((mappedFile.d[j].dmac[i] & 0xff) + 0x100, 16).substring(1));
		      		if(i!=mappedFile.d[j].dmac.length-1)
				System.out.print("-");
			}	
			System.out.print("\nSRC IP= ");
			System.out.println(((mappedFile.d[j].ip_src) & 0xFF) + "." + ((mappedFile.d[j].ip_src >> 8) & 0xFF) + "." + ((mappedFile.d[j].ip_src >> 16) & 				0xFF)+"."+((mappedFile.d[j].ip_src >> 24) & 0xFF));
			System.out.print("DST IP= ");
			System.out.println(((mappedFile.d[j].ip_dst) & 0xFF) + "." + ((mappedFile.d[j].ip_dst >> 8) & 0xFF) + "." + ((mappedFile.d[j].ip_dst >> 16) & 				0xFF)+"."+ ((mappedFile.d[j].ip_dst >> 24) & 0xFF));
			//rest of the fields 			
			System.out.println(mappedFile.d[j].caplen);
			System.out.println(mappedFile.d[j].len);
			System.out.println(mappedFile.d[j].timestamp_ns);
  			System.out.println(mappedFile.d[j].rx_direction);
			System.out.println(mappedFile.d[j].if_index);
			System.out.println(mappedFile.d[j].pkt_hash);
		  	System.out.println(mappedFile.d[j].parsed_header_len);
			System.out.println(mappedFile.d[j].eth_type);
			System.out.println(mappedFile.d[j].vlan_id);
			System.out.println(mappedFile.d[j].ip_version);
		  	System.out.println(mappedFile.d[j].l3_proto);
			System.out.println(mappedFile.d[j].ip_tos);
			System.out.println(mappedFile.d[j].l4_src_port);
			System.out.println(mappedFile.d[j].l4_dst_port);
			System.out.println(mappedFile.d[j].flags);
			System.out.println(mappedFile.d[j].seq_num);
			System.out.println(mappedFile.d[j].ack_num);
			System.out.println(mappedFile.d[j].last_matched_plugin_id);
			System.out.println(mappedFile.d[j].last_matched_rule_id);
			System.out.println(mappedFile.d[j].eth_offset);
			System.out.println(mappedFile.d[j].vlan_offset);
			System.out.println(mappedFile.d[j].l3_offset);
			System.out.println(mappedFile.d[j].l4_offset);
			System.out.println(mappedFile.d[j].payload_offset);
			System.out.println(mappedFile.d[j].in_iface);
			System.out.println(mappedFile.d[j].out_iface);
			System.out.println(mappedFile.d[j].samplingPopulation);
			System.out.println(mappedFile.d[j].flow_sequence);
			System.out.println(mappedFile.d[j].buffer);
			System.out.println(mappedFile.d[j].buffer_len);
			j++;
			System.out.println("New Packet");	
		}*/
	}  // end of void main
} // end of class



	
        
        /*public void main1(String args[])
	{
	    
    	    rf.loadFile();
		// final code to convert byte array to hex!!!!!!
		 System.out.print("\nSMAC = ");
		for (int i = 0; i < mappedFile.smac.length; i++)
		{ 
		      System.out.print(Integer.toString((mappedFile.smac[i] & 0xff) + 0x100, 16).substring(1));
		      if(i!=mappedFile.smac.length-1)
			System.out.print("-");
		}

		System.out.print("\nDMAC = ");
		for (int i = 0; i < mappedFile.dmac.length; i++)
		{ 
		      System.out.print(Integer.toString((mappedFile.dmac[i] & 0xff) + 0x100, 16).substring(1));
		      if(i!=mappedFile.dmac.length-1)
			System.out.print("-");
		}	
	
	
		    	System.out.print("\nSRC IP= ");
		System.out.println(((mappedFile.ip_src) & 0xFF) + "." + ((mappedFile.ip_src >> 8) & 0xFF) + "." + ((mappedFile.ip_src >> 16) & 0xFF) +"."+ ((mappedFile.ip_src >> 24) & 0xFF));
			    
		    	System.out.print("DST IP= ");
		System.out.println(((mappedFile.ip_dst) & 0xFF) + "." + ((mappedFile.ip_dst >> 8) & 0xFF) + "." + ((mappedFile.ip_dst >> 16) & 0xFF) +"."+ ((mappedFile.ip_dst >> 24) & 0xFF));




	System.out.println(mappedFile.pkt_id);
  	System.out.println(mappedFile.caplen);
	System.out.println(mappedFile.len);
	System.out.println(mappedFile.timestamp_ns);
  	System.out.println(mappedFile.rx_direction);
	System.out.println(mappedFile.if_index);
	System.out.println(mappedFile.pkt_hash);
  	System.out.println(mappedFile.parsed_header_len);
	System.out.println(mappedFile.eth_type);
	System.out.println(mappedFile.vlan_id);
	System.out.println(mappedFile.ip_version);
  	System.out.println(mappedFile.l3_proto);
	System.out.println(mappedFile.ip_tos);
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
	  // end of void main
 // end of class

