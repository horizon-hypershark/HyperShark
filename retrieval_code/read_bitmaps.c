#include<stdio.h>

typedef struct bit_map_info
{
	int remain;
	int curr_byte;
}bit_map_info;

void set_info_zero(bit_map_info *info)
{
	info->remain=0;
	info->curr_byte=0;
}

void and_word(unsigned char *map1,unsigned char *map2,unsigned char *result,int byte1,int byte2,int res)
{
	result[res] = map1[byte1] & map2[byte2];
	result[res+1] = map1[byte1+1] & map2[byte2+1];
	result[res+2] = map1[byte1+2] & map2[byte2+2];
	result[res+3] = map1[byte1+3] & map2[info2.curr_byte+3];
}
unsigned char* and_bitmaps(unsigned char *map1,unsigned char *map2,int max_bits)
{
	int tot_words=max_bits/31;
	unsigned char *result=(unsigned char *)malloc(max_bits/8+(max_bits/31)/8+2+3);//3 bytes extra
	bit_map_info info1,info2,res_info;
	set_info_zero(&info1);	
	set_info_zero(&info2);
	set_info_zero(&res_info);
	while(1)
	{
		if(info1.map_remain==0 && info2.map_remain==0)
		{
			if((map1[info1.curr_byte]&0x01) && (map2[info2.curr_byte]&0x01))//check for zeroth bit
			{
				result[res_info.curr_byte] = map1[info1.curr_byte] & map2[info2.curr_byte];
				result[res_info.curr_byte+1] = map1[info1.curr_byte+1] & map2[info2.curr_byte+1];
				result[res_info.curr_byte+2] = map1[info1.curr_byte+2] & map2[info2.curr_byte+2];
				result[res_info.curr_byte+3] = map1[info1.curr_byte+3] & map2[info2.curr_byte+3];
				
				res_info.curr_byte+=4;
				info1.curr_byte+=4;
				info2.curr_byte+=4
				if(res_info.curr_byte==(tot_words*4))
					break;
			}
			else if(!(map1[info1.curr_byte]&0x01) || (map2[info2.curr_byte]&0x01))
			{
				int no_zero1=map1[info1.curr_byte+3],no_zero2=map2[info2.curr_byte+3];
				int diff;
				if(no_zero1>no_zero2)
				{
					diff=no_zero1-no_zero2;
					info1.curr_byte+=(no_zero1*4);	
				}
				else
				{
					diff=no_zero2-no_zero1;
					info2.curr_byte+=(no_zero2*4);
					
				}		
			}
		}		
	}
	return result;
}
