#include<stdlib.h>
typedef struct bit_map_info
{
	int remain;
	int curr_byte;
}bit_map_info;

void set_info_zero(bit_map_info *info);
void and_word(unsigned char *map1,unsigned char *map2,unsigned char *result,int byte1,int byte2,int res);
void set_word(unsigned char *map,int byte1,int value);
void set_min_words(bit_map_info *info1,bit_map_info *info2,unsigned char *result,bit_map_info *res_info,int no_zero,int diff);
int handle_map_remain(unsigned char *map,bit_map_info *info,unsigned char *result,bit_map_info *res_info,bit_map_info *info2);
unsigned char* and_bitmaps(unsigned char *map1,unsigned char *map2,int max_bits);//breaking condition not written;
