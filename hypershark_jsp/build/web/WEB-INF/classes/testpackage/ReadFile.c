#include <jni.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "testpackage_ReadFile.h"
#include "afcapture.h"


JNIEXPORT void JNICALL Java_testpackage_ReadFile_loadFile
  (JNIEnv * env,jobject jobj)
{
/*	typedef struct alltypes
	{
		u_int8_t f1;
		u_int16_t f2;
		u_int32_t f3;
		u_int64_t f4;
		char f5;
		char f6[30];
	}alltypes;
	*/	
	//alltypes a1;
        jstring str;
	hs_pkt_hdr hp;
        int i,size;
	jboolean iscopy;
		
	/*FILE *fp=fopen("pkttrace","rb");
	if (fp==NULL)
	{
      		printf("Could not open file");
		return;
    	}
	fread(&hp,sizeof(hs_pkt_hdr),1,fp);
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);*/
	jfieldID F1;
	jclass cls=(*env)->GetObjectClass(env,jobj);
	


	hs_pkt_hdr hs_pkt;
	FILE *fp=fopen("pkttrace","rb");
 
 	fread(&hs_pkt,sizeof(hs_pkt_hdr),1,fp);
    
 
 	printf("PKT ID=%d --> SRC IP=%d.%d.%d.%d",hs_pkt.pkt_id,(hs_pkt.pkf.extended_hdr.parsed_pkt.ip_src.v4) & 0xFF,
                                                                      (hs_pkt.pkf.extended_hdr.parsed_pkt.ip_src.v4 >> 8 ) & 0xFF,
                                                                      (hs_pkt.pkf.extended_hdr.parsed_pkt.ip_src.v4 >> 16) & 0xFF,
                                                                      (hs_pkt.pkf.extended_hdr.parsed_pkt.ip_src.v4 >> 24) & 0xFF);
        printf("\nIn c :- Buffer Len  %d",hs_pkt.buffer_len);
	//printf("\nIn c :- Buffer -- %s\n",hp.buffer);

        printf("\n Dst IP=%u.%u.%u.%u",(hs_pkt.pkf.extended_hdr.parsed_pkt.ip_dst.v4) & 0xFF,
									(hs_pkt.pkf.extended_hdr.parsed_pkt.ip_dst.v4 >> 8 ) & 0xFF,
									(hs_pkt.pkf.extended_hdr.parsed_pkt.ip_dst.v4 >> 16) & 0xFF,
									(hs_pkt.pkf.extended_hdr.parsed_pkt.ip_dst.v4 >> 24) & 0xFF);
	//printf("CODE src ip=%u",hp.pkf.extended_hdr.parsed_pkt.ip_src);
	//printf("CODE dest ip=%u",hp.pkf.extended_hdr.parsed_pkt.ip_dst);
	//free(hp);*/
        
        F1 = (*env)->GetFieldID(env,cls,"pkt_id","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkt_id);
 
        F1 = (*env)->GetFieldID(env,cls,"caplen","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.caplen);

        F1 = (*env)->GetFieldID(env,cls,"len","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.len);

        F1 = (*env)->GetFieldID(env,cls,"timestamp_ns","J");
	(*env)->SetLongField(env,jobj,F1,hs_pkt.pkf.extended_hdr.timestamp_ns);

        F1 = (*env)->GetFieldID(env,cls,"rx_direction","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.rx_direction);

        F1 = (*env)->GetFieldID(env,cls,"if_index","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.extended_hdr.if_index);

        F1 = (*env)->GetFieldID(env,cls,"pkt_hash","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.extended_hdr.pkt_hash);

        F1 = (*env)->GetFieldID(env,cls,"parsed_header_len","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_header_len);

//        F1 = (*env)->GetFieldID(env,cls,"dmac","[S");
//	(*env)->SetShortArrayRegion(env,dmac,0,6,hs_pkt.pkf.extended_hdr.parsed_pkt.dmac);

        //F1 = (*env)->GetFieldID(env,cls,"smac","[S");
	//(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.smac);

        F1 = (*env)->GetFieldID(env,cls,"eth_type","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.eth_type);

        F1 = (*env)->GetFieldID(env,cls,"vlan_id","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.vlan_id);

        F1 = (*env)->GetFieldID(env,cls,"ip_version","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_version);

        F1 = (*env)->GetFieldID(env,cls,"l3_proto","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.l3_proto);

        F1 = (*env)->GetFieldID(env,cls,"ip_tos","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_tos);

	F1 = (*env)->GetFieldID(env,cls,"ip_src","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_src.v4);
	
	F1 = (*env)->GetFieldID(env,cls,"ip_dst","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_dst.v4);

	F1 = (*env)->GetFieldID(env,cls,"l4_src_port","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.l4_src_port);	

	F1 = (*env)->GetFieldID(env,cls,"l4_dst_port","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.l4_dst_port);	

        // part of it but tcp structure

	F1 = (*env)->GetFieldID(env,cls,"flags","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.tcp.flags);	

	F1 = (*env)->GetFieldID(env,cls,"seq_num","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.tcp.seq_num);	

	F1 = (*env)->GetFieldID(env,cls,"ack_num","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.tcp.ack_num);

	F1 = (*env)->GetFieldID(env,cls,"last_matched_plugin_id","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.last_matched_plugin_id);	
	
	F1 = (*env)->GetFieldID(env,cls,"last_matched_rule_id","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.last_matched_rule_id);
   
        // struct pkt_offset

	F1 = (*env)->GetFieldID(env,cls,"eth_offset","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.eth_offset);	
	
	F1 = (*env)->GetFieldID(env,cls,"vlan_offset","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.vlan_offset);

	F1 = (*env)->GetFieldID(env,cls,"l3_offset","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.l3_offset);

	F1 = (*env)->GetFieldID(env,cls,"l4_offset","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.l4_offset);

	F1 = (*env)->GetFieldID(env,cls,"payload_offset","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.payload_offset);

        // struct packet_user_detail

	F1 = (*env)->GetFieldID(env,cls,"in_iface","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.in_iface);	
	
	F1 = (*env)->GetFieldID(env,cls,"out_iface","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.out_iface);

	F1 = (*env)->GetFieldID(env,cls,"samplingPopulation","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.samplingPopulation);

	F1 = (*env)->GetFieldID(env,cls,"flow_sequence","I");
	(*env)->SetIntField(env,jobj,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.flow_sequence);	
	
	   
        F1 = (*env)->GetFieldID(env,cls,"buffer","Ljava/lang/String;");
        str = (*env)->NewStringUTF(env,hs_pkt.buffer);

	(*env)->SetObjectField(env,jobj,F1,str);
	
	F1 = (*env)->GetFieldID(env,cls,"buffer_len","S");
	(*env)->SetShortField(env,jobj,F1,hs_pkt.buffer_len);
	

	fclose(fp);
}
