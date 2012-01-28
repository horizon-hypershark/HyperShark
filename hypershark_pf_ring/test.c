#include<stdio.h>
#include<sys/types.h>

int shift_1_bit_right(unsigned char *bit_map_col,u_int32_t start_bit_no,u_int32_t bit_len)//changes required
{
	int i;
	int valid_bits_in_last_byte=bit_len%8;//handle condition when this comes zero
	int start_index=(bit_len-valid_bits_in_last_byte)/8;	
	int last_index=start_bit_no/8;//last_index<start_index	
	
	//u_int32_t no_bytes_to_traverse=((bit_len-valid_bits_in_last_byte)-start_bit_no)/8;//from end
	//no_bytes_to_traverse++;//specific to our case where start_bit_no always indicates 1st bit of any byte i.e. start_bit_no%8=1
	if(valid_bits_in_last_byte==0)
	{
		unsigned char tmp=bit_map_col[start_index-1]&0x01;		
		bit_map_col[start_index]=tmp<<7;				
	}
	bit_map_col[start_index]<<=1;//check this for worst case
	start_index--;	
	for(i=start_index;i>=last_index;i--)
	{			
		bit_map_col[i+1]|=(bit_map_col[i]&0x80)>>7;//shift last bit
		bit_map_col[i]<<=1;		
		bit_map_col[i]=bit_map_col[i]&0xFE;				
			
	}
	printf("\n12==%d",bit_map_col[12]);	
	return 0;//Do not know why
}
/********************************************************************************************************************************/
int shift_4_bytes(unsigned char *bit_map_col,u_int32_t start_index,u_int32_t bit_len)
{
	u_int32_t end_index=bit_len/8,i;
	for(i=start_index;i<end_index-1-4;i++)
	{
		bit_map_col[i]=bit_map_col[i+4];
	}
	if((bit_len%8)!=0)
		bit_map_col[end_index-1]=bit_map_col[end_index];
	return 0;
}
/********************************************************************************************************************************/
int compress_bit_map(unsigned char * bit_map_col,u_int32_t bit_len)//make everything unsigned char
{
	u_int32_t no_loop=bit_len/31;
	//u_int32_t remain_bits=bit_len%31;
	u_int32_t j=0,was_last_zero=0,i;
	
	int z;
	/*printf("\nBefore compression:");
	for(z=0;z<13;z++)
		printf("%u   ",bit_map_col[z]);	*/					
	for(i=0;i<no_loop;i++)
	{		
		printf("\nj=%d",j);
		u_int8_t byte1=bit_map_col[j]&0xFF;
		u_int8_t byte2=bit_map_col[j+1]&0xFF;
		u_int8_t byte3=bit_map_col[j+2]&0xFF;
		u_int8_t byte4=bit_map_col[j+3]&0x7F;
		
		shift_1_bit_right(bit_map_col,j*8,bit_len);
		/*for(z=0;z<13;z++)
			printf("%u   ",bit_map_col[z]);	*/
		bit_len++;
		if(byte1 || byte2 || byte3 ||byte4)
		{
			printf("\nIn first");
			bit_map_col[j]|=0x01;	
			was_last_zero=0;
			j+=4;			
		}
		else if(!was_last_zero)
		{
			printf("\nIn second");
			was_last_zero++;
			bit_map_col[j+3]|=0x01;
			j+=4;
		}
		else
		{
			printf("\nIn third");
			if(!(++bit_map_col[j-1]))
			{
				++bit_map_col[j-2];//this restricts no of flow records to approx 256*256
			}			
			shift_4_bytes(bit_map_col,j,bit_len);
			bit_len-=32;
			was_last_zero++;
		}		
		//for(z=0;z<13;z++)
			//printf("%u   ",bit_map_col[z]);	
	}																							
	return bit_len;
}


int main()
{
	unsigned char coll[13];
	int i;
	coll[0]=0;
	coll[1]=0;
	coll[2]=128;
	coll[3]=56;
	coll[4]=30;
coll[5]=28;
coll[6]=0;
coll[7]=0;
coll[8]=1;
coll[9]=192;
coll[10]=128;
coll[11]=25;
coll[12]=12;
	//shift_1_bit_right(coll,0,100);
	compress_bit_map(coll,100);
	for(i=0;i<13;i++)
		printf("%d ->",coll[i]);

}
