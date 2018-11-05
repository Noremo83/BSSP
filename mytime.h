#ifndef MYTIME_H_
#define MYTIME_H

#include<stdio.h>
#include<time.h>

// gibt die aktuelle Zeit als Wert zurück
struct timespec get_cur_time_161314();

// gibt den String und die übergebene Zeit aus
void write_time_161314(const char *msg, const struct timespec *t);

// liefert die Differenz der beiden Zeiten wieder als Zeitobjekt (sec/nanosec)
struct timespec get_diff_161314(const struct timespec *start, const struct timespec *stop);

// addiert zur Zeit a die Zeit b
void add_time_161314(struct timespec *summe, const struct timespec *addtime);

struct timespec get_cur_time_161314(){	
	struct timespec spec;	
	clock_gettime(CLOCK_REALTIME, &spec);
	
	return spec;
}

struct timespec get_diff_161314(const struct timespec *start, const struct timespec *stop){
	struct timespec diff;
	if ((stop->tv_nsec - start->tv_nsec) < 0) {
        diff.tv_sec = stop->tv_sec - start->tv_sec - 1;
        diff.tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        diff.tv_sec = stop->tv_sec - start->tv_sec;
        diff.tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
    return diff;
}

void write_time_161314(const char *msg, const struct timespec *t){
	printf("%s",msg);
	printf("%ld s/ %ld ns\n",t->tv_sec,t->tv_nsec);
}

void add_time_161314(struct timespec *summe, const struct timespec *addtime){	
	summe->tv_sec += addtime->tv_sec;
	summe->tv_nsec += addtime->tv_nsec;
}



#endif // MYTIME_H_
