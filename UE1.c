#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include "mytime.h"

int main(){

	struct timespec start;
	struct timespec stop;
	struct timespec diff;
	struct timespec ges;
	ges.tv_sec = 0;
	ges.tv_nsec = 0;
	
	start = get_cur_time_161314();
	write_time_161314("StartZeit: ",&start);
	sleep(5);
	stop = get_cur_time_161314();	
	write_time_161314("StopZeit: ",&stop);
	
	diff = get_diff_161314(&start,&stop);
	write_time_161314("DiffZeit: ",&diff);
	
	add_time_161314(&ges,&diff);
	write_time_161314("geszeit: ",&ges);
	
	printf("\n\n");
}