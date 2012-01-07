//Header Files 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 256//Max Limit for Array Size of Hash Table


typedef struct flow_record{
  u_int32_t flow_id;
  u_int16_t src_port, dst_port;
  u_int32_t
   ip_src, ip_dst;
  u_int8_t  protocol;
  u_int64_t timestamp_ns;
  u_int32_t nop;
  u_int32_t start_pkt_no;
  //u_int32_t *offset_table;
  u_int32_t nxtfr;
}flow_record;

typedef struct offset_node{
  loff_t offset;
  struct offset_node *next;
}offset_node;

typedef struct offset_table{
  struct offset_node *start;
  struct offset_node *end;
}offset_table;

typedef struct vm_id
{
  u_int32_t  v_id[4];
}vm_id;
//Customer Records Mapping Structure

typedef struct ip_bits
{
	char octet_1[13];//for 100 bits
	char octet_2[13];
	char octet_3[13];
	char octet_4[13];
}ip_bits;

typedef struct port_bits
{
	char octet_1[13];
}port_bits;

typedef struct protcol_bits
{
	char octet_1[13];
}protocol_bits;

typedef struct bitmap
{
	ip_bits *src_ip,*dst_ip;//255 instances 
	port_bits *src_port,*dst_port;//65536 instances
	protocol_bits *protocol;//255 instances
}bitmap;

typedef struct maprecord{
  u_int32_t  cust_id;
  vm_id vmid;
  char path[15];
  u_int16_t  link;
  u_int16_t GFL;//global flow location
  u_int32_t GPC;//global packet count	
  loff_t pkt_offset;
  offset_table *off_table;
  flow_record *flow_start;
  bitmap *bit_map;
}maprecord;


int main(){
  //Allocate memory for hash table
  maprecord *map=(maprecord*)calloc(SIZE, sizeof(maprecord));
  maprecord *def=(maprecord*)malloc(sizeof(maprecord));
  unsigned int cust_id;
  vm_id vmid;
  int hash,i,j,k;
  char *buf=(char*)malloc(30*sizeof(char));
  vmid.v_id[0]=1234;vmid.v_id[1]=2345;vmid.v_id[2]=3456;vmid.v_id[3]=4567;
  memset(map,0,SIZE*sizeof(maprecord));
  for(j=0;j<2;j++)
  {
      fflush(stdin);
      printf("Enter Cust Id:");      //Entering Cust and VM id which will be passed to the program
      scanf("%d",&cust_id);
      /*Hash Function computed using Folding Method*/
       hash=cust_id;
       for(i=0;i<4;i++)
		hash+=vmid.v_id[i];
       hash=hash%256;
       
	printf("Hash is :%d",hash);
      /*If hashed location is empty
	1.] Fill in the appropraite structure fields
	2.] Create a directory based on the Cust and VM id
	3.] Storing the Directory Path in the .config File*/
        if(map[hash].cust_id==0)
        {
		  map[hash].cust_id=cust_id;
		  for(i=0;i<4;i++)
		        map[hash].vmid.v_id[i]=vmid.v_id[i];
		  strcpy(map[hash].path,"hs");
		  snprintf(buf,10,"%d",cust_id);
		  strcat(map[hash].path,buf);
		  strcat(map[hash].path,"/");
		  sprintf(buf,"mkdir %s",map[hash].path);
		  printf("%s",buf);
		  system(buf);
		  strcat(buf,"Packets");
		  printf("%s",buf);
		  system(buf);
		  sprintf(buf,"mkdir %s",map[hash].path);
		  strcat(buf,"Flowrecords");
		  printf("%s",buf);
		  system(buf);
		
	}
      /*If Collision occurs then link to the already existing strcuture 
	and use linear probing to search for nect free location*/
      else
	{
	  for(i=hash+1;i<hash;i=(i+1)%256)
	    {
	      //Circular Search from hash+1 location until free entry found
	      if(map[i].cust_id==0) 
		{	  
		  	map[i].cust_id=cust_id;
	        	for(k=0;k<4;k++)
		         	 map[i].vmid.v_id[k]=vmid.v_id[k];
			strcpy(map[i].path,"hs");
	 	        snprintf(buf,10,"%d",cust_id);
		  	strcat(map[i].path,buf);
			sprintf(buf,"mkdir %s",map[i].path);
		        system(buf);
			map[hash].link=i;
		  	strcat(buf,"Packets");
		  	printf("%s",buf);
		  	system(buf);
		  	sprintf(buf,"mkdir %s",map[hash].path);
		  	strcat(buf,"Flowrecords");
		  	printf("%s",buf);
		  	system(buf);
			break;
		}
	    }
    	}
    }

  //Writing Hash Table to config File
  FILE *fp;
  fp=fopen("Mapping.config","wb");
	  for(i=0;i<256;i++)
		      fwrite(&map[i],sizeof(maprecord),1,fp);
  fclose(fp);
  return 0;	
}
