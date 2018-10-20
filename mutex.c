#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<dirent.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<openssl/md5.h>
#include<sys/stat.h>

#define MAXTHR 2
pthread_mutex_t dirsync = PTHREAD_MUTEX_INITIALIZER;

void *work( void * ptr);

int main(int argc, char **argv){

	DIR *dirp;
	//int maxthread = 2; // maxthread ist der zweite Parameter argv[2]
	pthread_t threadID[MAXTHR];
	
	//argv[1]
	dirp = opendir(argv[1]);
	
	if (!dirp){ // if = 0 dann fehler konnte nicht geöffnet werden
		fprintf(stderr,"VZ open geht net\n");
		exit(1);
	}
	
	pthread_create(threadID,NULL,work,(void *)(dirp));
	pthread_create(threadID+1,NULL,work,(void *)(dirp));
	pthread_create(threadID+2,NULL,work,(void *)(dirp));
	
	pthread_join(threadID[0],NULL); // NULL ist der Rückgabewert des Threads kann auch ein &ptr sein
	pthread_join(threadID[1],NULL);
	pthread_join(threadID[2],NULL);
	
	return 0;
	
}

void *work(void * ptr){
	static int nr = 0;
	int mynr = ++nr;
	//printf("%c",mynr);
	
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