#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<dirent.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<openssl/md5.h>
#include<sys/stat.h>

#define MIN_REQUIRED 2

pthread_mutex_t dirsync = PTHREAD_MUTEX_INITIALIZER;

void *work_161314( void * ptr);

int main(int argc, char **argv){
	int i = 0;
	long maxthread = 0;

	//Kontrolle ob genug Argumente angegeben wurden
	if (argc <= MIN_REQUIRED){
		fprintf(stderr,"To less arguments.\nUSAGE:\n    PATH(/etc/....)\nTHRADCOUNT(1-10)\n");
		exit(1);
	}

	maxthread = atoi(argv[2]);
	//Kontrolle ob die Anzahl der Threads zwischen 1 und 10 liegen.
	if (atoi(argv[2]) > 0 && atoi(argv[2]) <= 10){
		maxthread = atoi(argv[2]);
	}
	
	printf("maxthread: %ld\n",maxthread);
	DIR *dirp;
	pthread_t threadID[maxthread];
	dirp = opendir(argv[1]);
	
	// if = 0 dann fehler konnte nicht geöffnet werden
	if (!dirp){
		fprintf(stderr,"Could not open Directory!\n");
		exit(1);
	}
	
	for(i=0;i <= maxthread;i++){
		pthread_create(threadID + i,NULL,work_161314,(void *)(dirp));
	}
	
	for(i=0;i<=maxthread;i++){
		pthread_join(threadID[i],NULL);
	}

	return 0;
	
}

void *work_161314(void * ptr){
	static int nr = 0;
	int mynr = ++nr;
	
	struct dirent * entryp;
	
	DIR *p = (DIR *)(ptr);
	while( pthread_mutex_lock(&dirsync), entryp = readdir(p)){
		pthread_mutex_unlock(&dirsync);		
		printf("Thread %d, File: %s\n",mynr,entryp->d_name);
		sleep(rand()%5);
		//Starttime
		//makeMD5(entryp->d_name)
		//StopTime		
	}
	pthread_mutex_unlock(&dirsync);
	return NULL;
}
