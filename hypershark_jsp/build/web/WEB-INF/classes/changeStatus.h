typedef struct start_data
{
	char option[20];
	char path[60];
	char interface[10];
	u_int32_t GFL;
	u_int16_t hash;
	u_int16_t gpkt;
}start_data;

typedef struct stop_data
{
	char option[20];
	u_int16_t hash;
}stop_data;
