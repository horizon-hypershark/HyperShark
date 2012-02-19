#define MAX_FLOW_BYTE 13
#include "bitmap.h"
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
	result[res+3] = map1[byte1+3] & map2[byte2+3];
}

void set_word(unsigned char *map,int byte1,int value)
{
	map[byte1]=map[byte1+1]=map[byte1+2]=map[byte1+3]=value;
}


void set_min_words(bit_map_info *info1,bit_map_info *info2,unsigned char *result,bit_map_info *res_info,int no_zero,int diff)
{
	info1->curr_byte+=4;
	info2->curr_byte+=4;
	info2->remain=diff;
	if((res_info->curr_byte-4>=0)&&(result[res_info->curr_byte-4]==0))
		result[res_info->curr_byte-1]+=no_zero;
	else
	{
		set_word(result,res_info->curr_byte,0);
		result[res_info->curr_byte+3]=no_zero;
		res_info->curr_byte+=4; 	
	}
}

int handle_map_remain(unsigned char *map,bit_map_info *info,unsigned char *result,bit_map_info *res_info,bit_map_info *info2)
{
	if(map[info->curr_byte]&0x01)
	{
		info->remain--;		
		info->curr_byte+=4;		
	}
	else
	{
		int no_zero=map[info->curr_byte+3];
		if(no_zero<=info->remain)
		{		
			info->remain-=no_zero;
			info->curr_byte+=4;			
		}
		else
		{
			int diff=no_zero-info->remain;
			info->remain=0;
			info->curr_byte+=4;
			info2->remain=diff;			
			if((res_info->curr_byte-4>=0)&&(result[res_info->curr_byte-4]==0))
				result[res_info->curr_byte-1]+=diff;
			else
			{
				set_word(result,res_info->curr_byte,0);
				result[res_info->curr_byte+3]=diff;
				res_info->curr_byte+=4; 	
			}
			return diff;
		}
	}
	return 0;
}


unsigned char* and_bitmaps(unsigned char *map1,unsigned char *map2,int max_bits)//breaking condition not written
{
	int tot_words=max_bits/31,res_words=0,i;
	//unsigned char *result=(unsigned char *)malloc(max_bits/8+(max_bits/31)/8+2+3);//3 bytes extra
	unsigned char *result=(unsigned char *)malloc(MAX_FLOW_BYTE*sizeof(unsigned char));
	bit_map_info info1,info2,res_info;
	int extra_bytes=((max_bits%31)/8)+1;
	set_info_zero(&info1);	
	set_info_zero(&info2);
	set_info_zero(&res_info);
	while(1)
	{
		if(info1.remain==0 && info2.remain==0)
		{
			if((map1[info1.curr_byte]&0x01) && (map2[info2.curr_byte]&0x01))//check for zeroth bit
			{
				and_word(map1,map2,result,info1.curr_byte,info2.curr_byte,res_info.curr_byte);
				res_info.curr_byte+=4;
				info1.curr_byte+=4;
				info2.curr_byte+=4;
				res_words+=1;
			}
			else if(!((map1[info1.curr_byte]&0x01) || (map2[info2.curr_byte]&0x01)))
			{				
				int no_zero1=map1[info1.curr_byte+3],no_zero2=map2[info2.curr_byte+3];				
				if(no_zero1>no_zero2)
				{
					set_min_words(&info1,&info2,result,&res_info,no_zero1,no_zero1-no_zero2);
					res_words+=no_zero1;
				}
				else
				{
					set_min_words(&info2,&info1,result,&res_info,no_zero2,no_zero2-no_zero1);
					res_words+=no_zero2;								
				}
			}
			else
			{
				int no_zero=0;
				if(map2[info2.curr_byte]&0x01)
				{
					no_zero=map1[info1.curr_byte+3];
					if((res_info.curr_byte-4>=0)&&(result[res_info.curr_byte-4]==0))
						result[res_info.curr_byte-1]+=no_zero;
					else
					{
						set_word(result,res_info.curr_byte,0);
						result[res_info.curr_byte+3]=no_zero;
						res_info.curr_byte+=4; 	
					}									
					info1.curr_byte+=4;
					info2.curr_byte+=4;
					info2.remain=no_zero-1;
				}
				else
				{
					no_zero=map2[info2.curr_byte+3];	
					if((res_info.curr_byte-4>=0)&&(result[res_info.curr_byte-4]==0))
						result[res_info.curr_byte-1]+=no_zero;
					else
					{
						set_word(result,res_info.curr_byte,0);
						result[res_info.curr_byte+3]=no_zero;
						res_info.curr_byte+=4; 	
					}										
					info1.curr_byte+=4;
					info2.curr_byte+=4;
					info1.remain=no_zero-1;
				}	
				res_words+=no_zero;			
			}
		}
		else if(info1.remain)
		{
			res_words+=handle_map_remain(map1,&info1,result,&res_info,&info2);
		}
		else
		{
			res_words+=handle_map_remain(map2,&info2,result,&res_info,&info1);
		}
		if(res_words==tot_words)
			break;		
	}
//TODO last 31
	for(i=0;i<extra_bytes;i++)
	{		
		result[res_info.curr_byte+i] = map1[info1.curr_byte+i] & map2[info2.curr_byte+i];
	}
	return result;
}

