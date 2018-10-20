#include<stdio.h>
#include<unistd.h>
#include "mytime.h"

int main(){

	struct timespec start;
	struct timespec stop;
	struct timespec diff;
	struct timespec ges;
	ges.tv_sec = 0;
	ges.tv_nsec = 0;
	int i = 0;
	
	for(i=0;i<5;i++){
	start = get_cur_time_161314();
	sleep(2);
	stop = get_cur_time_161314();
	diff = get_diff_161314(&start,&stop);
	write_time_161314("Laufzeit: ",&diff);	
	add_time_161314(&ges,&diff);
	write_time_161314("Gesamt Zeit: ",&ges);	
	printf("\n");
	}
}