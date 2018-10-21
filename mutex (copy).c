#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<dirent.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<openssl/md5.h>
#include<sys/stat.h>
#include "mytime.h"
#include<time.h>

#define MIN_REQUIRED 2

pthread_mutex_t dirsync = PTHREAD_MUTEX_INITIALIZER;

struct timespec time_opendir = {0,0};
struct timespec time_openfile = {0,0};
struct timespec time_hash = {0,0};

void *work_161314( void * ptr);
int mkhash(const char *filename);
void calc_time(const struct timespec *start, struct timespec *update);

int main(int argc, char **argv){
	int i = 0;
	long maxthread = 0;
	struct timespec	start = {0,0};

	//Kontrolle ob genug Argumente angegeben wurden
	if (argc <= MIN_REQUIRED){
		fprintf(stderr,"To less arguments.\nUSAGE:\n    PATH(/etc/....)\nTHRADCOUNT(1-10)\n");
		exit(1);
	}

	//Kontrolle ob die Anzahl der Threads zwischen 1 und 10 liegen.
	if (atoi(argv[2]) > 0 && atoi(argv[2]) <= 10){
		maxthread = atoi(argv[2]) -1;
	}
	
	//printf("maxthread: %ld\n",maxthread);
	DIR *dirp;
	pthread_t threadID[maxthread];
	
	// Öffnen Directory und Zeit messen
	start = get_cur_time_161314();
	dirp = opendir(argv[1]);
	calc_time(&start, &time_opendir);
	
	// Kontrolle ob Directory geöffnet werden konnte
	if (!dirp){
		fprintf(stderr,"Could not open Directory!\n");
		exit(1);
	}
	
	//Erzeugen der Threads
	for(i=0;i <= maxthread;i++){
		pthread_create(threadID + i,NULL,work_161314,(void *)(dirp));
	}
	//Syncronisieren
	for(i=0;i<=maxthread;i++){
		pthread_join(threadID[i],NULL);
	}

	//Ausgabe gesamt Zeit
	fprintf(stderr,"Gesamt Zeit öffnen Ordners: %lds/%ldns\n",time_opendir.tv_sec,time_opendir.tv_nsec);
	fprintf(stderr,"Gesamt Zeit öffnen Files: %lds/%ldns\n",time_openfile.tv_sec,time_openfile.tv_nsec);
	fprintf(stderr,"Gesamt Zeit berechnen HASH: %lds/%ldns\n",time_hash.tv_sec,time_hash.tv_nsec);
	return 0;
	
}

void *work_161314(void * ptr){
	//static int nr = 0;
	//int mynr = ++nr;
	struct dirent * entryp;
	
	DIR *p = (DIR *)(ptr);
	while( pthread_mutex_lock(&dirsync), entryp = readdir(p)){
		//struct stat is_file;
		//stat(entryp->d_name, &is_file);
		//if(S_ISREG(is_file.st_mode)){	
			pthread_mutex_unlock(&dirsync);		
			//printf("\nThread %d, File: %s\n",mynr,entryp->d_name);
			sleep(rand()%5);
			mkhash(entryp->d_name);
		//}
	}
	pthread_mutex_unlock(&dirsync);
	return NULL;
}

void calc_time(const struct timespec *start, struct timespec *update){
	struct timespec tmp_time = {0,0};
	struct timespec stop = {0,0};

	stop = get_cur_time_161314();
	tmp_time = get_diff_161314(start,&stop);
	add_time_161314(update,&tmp_time);
	//write_time_161314("zeit: ",update);
}

int mkhash(const char *filename){
	unsigned char *c = malloc(MD5_DIGEST_LENGTH);
	int i;

	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];

	struct timespec start_openfile = get_cur_time_161314();
	int file = open (filename, O_RDONLY);
	
	struct timespec start_hash = get_cur_time_161314();
	MD5_Init (&mdContext);

	while ((bytes = read (file, data, 1024)) != 0)
		MD5_Update (&mdContext, data, bytes);
	
	calc_time(&start_openfile, &time_openfile);
	MD5_Final (c,&mdContext);
	calc_time(&start_hash,&time_hash);

	for(i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i]);
	
	printf (" %s\n", filename);
	//fclose (inFile);
	return 0;
}



