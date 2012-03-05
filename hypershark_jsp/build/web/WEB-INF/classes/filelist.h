typedef struct file_list
{
	char filename[50];
	struct file_list *next;
}file_list;

typedef struct capture_time
{
	int month;
	int day;
	int start_hr,start_min;
	int end_hr,end_min;
}capture_time;

file_list* get_file_node(char *filename);
int cmp_time(int hr1,int min1,int hr2,int min2);
file_list* get_filename(capture_time *time_cap,char *path);
