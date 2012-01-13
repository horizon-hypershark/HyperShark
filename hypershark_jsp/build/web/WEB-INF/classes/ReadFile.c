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
#include "testpackage_ReadFile2.h"
#include "afcapture.h"

JNIEXPORT void JNICALL Java_testpackage_ReadFile2_loadFile
  (JNIEnv * env,jobject obj)
{
        jstring str;
	int i=0,size;
	jboolean iscopy;
	jfieldID F1;
	jbyteArray jsh,jsh2;
	hs_pkt_hdr hs_pkt;
	FILE *fp=fopen("pkttrace2","rb");//file name = pkttrace
	jobjectArray oarr;
	jobject o;
	jsize length=10;


	//get class which contains structure members,allocate object of that class and set its individual fields
	jclass cls=(*env)->FindClass(env,"testpackage/data");
	
	//make an array of objects
	oarr=(*env)->NewObjectArray(env,length,cls,o);
	
	//loop for packets	
	while(i<5)
	{
		fread(&hs_pkt,sizeof(hs_pkt_hdr),1,fp);//read a single packet 
				
	//fill object	
		o=(*env)->AllocObject(env,cls);//allocate an object: setobjectarrayelement function needs this to set each element of array	
		
		F1 = (*env)->GetFieldID(env,cls,"pkt_id","I");//get fieldid of class indicated by cls "I" is the signature for integer: you can look up signatures
		// of each field of a class by using javap -s classname.Field id is returned in F1.This is required in setting fields of an object		 
		(*env)->SetIntField(env,o,F1,hs_pkt.pkt_id);//set field F1 od object o
	
		F1 = (*env)->GetFieldID(env,cls,"caplen","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.caplen);

		F1 = (*env)->GetFieldID(env,cls,"len","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.len);

		F1 = (*env)->GetFieldID(env,cls,"timestamp_ns","J");
		(*env)->SetLongField(env,o,F1,hs_pkt.pkf.extended_hdr.timestamp_ns);

		F1 = (*env)->GetFieldID(env,cls,"rx_direction","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.rx_direction);

		F1 = (*env)->GetFieldID(env,cls,"if_index","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.extended_hdr.if_index);

		F1 = (*env)->GetFieldID(env,cls,"pkt_hash","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.extended_hdr.pkt_hash);

		F1 = (*env)->GetFieldID(env,cls,"parsed_header_len","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_header_len);  
	      
		F1 = (*env)->GetFieldID(env,cls,"dmac","[B");//dmac is a byte array signature is [B, prepend signature of primitve type with [ for array of primitve 			//arrays
		jsh=(*env)->NewByteArray(env,7);//creating a byte array,7 indicates size it is a 6 byte array for mac address which is of 6 bytes
		(*env)->SetByteArrayRegion(env,jsh,0,6,hs_pkt.pkf.extended_hdr.parsed_pkt.dmac);//set values of fields in array through setbytearrayregion.It needs 			//values in terms of the whole array and not individual fields.0 is start and 6 is the size.
		(*env)->SetObjectField(env,o,F1,jsh);//similar to setintfield but as array is a reference type it is setobjectfield
		
		F1 = (*env)->GetFieldID(env,cls,"smac","[B");
		jsh2=(*env)->NewByteArray(env,7);
		(*env)->SetByteArrayRegion(env,jsh2,0,6,hs_pkt.pkf.extended_hdr.parsed_pkt.smac);
		(*env)->SetObjectField(env,o,F1,jsh2);
		
		F1 = (*env)->GetFieldID(env,cls,"eth_type","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.eth_type);

		F1 = (*env)->GetFieldID(env,cls,"vlan_id","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.vlan_id);

		F1 = (*env)->GetFieldID(env,cls,"ip_version","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_version);

		F1 = (*env)->GetFieldID(env,cls,"l3_proto","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.l3_proto);

		F1 = (*env)->GetFieldID(env,cls,"ip_tos","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_tos);

		F1 = (*env)->GetFieldID(env,cls,"ip_src","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_src.v4);
	
		F1 = (*env)->GetFieldID(env,cls,"ip_dst","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.ip_dst.v4);

		F1 = (*env)->GetFieldID(env,cls,"l4_src_port","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.l4_src_port);	

		F1 = (*env)->GetFieldID(env,cls,"l4_dst_port","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.l4_dst_port);	

		F1 = (*env)->GetFieldID(env,cls,"flags","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.tcp.flags);	

		F1 = (*env)->GetFieldID(env,cls,"seq_num","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.tcp.seq_num);	

		F1 = (*env)->GetFieldID(env,cls,"ack_num","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.tcp.ack_num);

		F1 = (*env)->GetFieldID(env,cls,"last_matched_plugin_id","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.last_matched_plugin_id);	
	
		F1 = (*env)->GetFieldID(env,cls,"last_matched_rule_id","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.last_matched_rule_id);

		F1 = (*env)->GetFieldID(env,cls,"eth_offset","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.eth_offset);	
	
		F1 = (*env)->GetFieldID(env,cls,"vlan_offset","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.vlan_offset);

		F1 = (*env)->GetFieldID(env,cls,"l3_offset","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.l3_offset);

		F1 = (*env)->GetFieldID(env,cls,"l4_offset","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.l4_offset);

		F1 = (*env)->GetFieldID(env,cls,"payload_offset","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.offset.payload_offset);

		F1 = (*env)->GetFieldID(env,cls,"in_iface","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.in_iface);	
	
		F1 = (*env)->GetFieldID(env,cls,"out_iface","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.out_iface);

		F1 = (*env)->GetFieldID(env,cls,"samplingPopulation","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.samplingPopulation);

		F1 = (*env)->GetFieldID(env,cls,"flow_sequence","I");
		(*env)->SetIntField(env,o,F1,hs_pkt.pkf.extended_hdr.parsed_pkt.pkt_detail.flow.flow_sequence);	
	
		F1 = (*env)->GetFieldID(env,cls,"buffer","Ljava/lang/String;");//string field check signature if required through javap -s classname
		str = (*env)->NewStringUTF(env,hs_pkt.buffer);//creating a string in format specified in jni
		(*env)->SetObjectField(env,o,F1,str);
	
		F1 = (*env)->GetFieldID(env,cls,"buffer_len","S");
		(*env)->SetShortField(env,o,F1,hs_pkt.buffer_len);
	
		//set object array element
		(*env)->SetObjectArrayElement(env,oarr,i,o);
		i++;
	}

	//map this object to the object of the class
	jclass cls1=(*env)->GetObjectClass(env,obj);
	F1 = (*env)->GetFieldID(env,cls1,"d","[Ltestpackage/data;");//signature for data class.also it is an array of objects so [ is required
	(*env)->SetObjectField(env,obj,F1,oarr);
	fclose(fp);
}
