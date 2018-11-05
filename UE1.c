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
#include<string.h>

#define MIN_REQUIRED 2

pthread_mutex_t dirsync = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t get_time = PTHREAD_MUTEX_INITIALIZER;

struct timespec time_opendir = {0,0};
struct timespec time_openfile = {0,0};
struct timespec time_hash = {0,0};
struct timespec time_all = {0,0};

char *path;

void *work_161314( void * ptr);
void mkhash(const char *filename);
void calc_time(const struct timespec *start, struct timespec *update);
int is_regular_file(const char *path);
int is_directory(const char *path);


int main(int argc, char **argv){
	int i = 0;
	long maxthread = 0;
	struct timespec	start = {0,0};

	//Kontrolle ob genug Argumente angegeben wurden
	if (argc <= MIN_REQUIRED){
		fprintf(stderr,"Zu wenig Argumente.\nUSAGE:\n    PATH(/etc/....)\n    THRADCOUNT(1-10)\n");
		exit(1);
	}

	//Kontrolle ob die Anzahl der Threads zwischen 1 und 10 liegen.
	if (atoi(argv[2]) > 0 && atoi(argv[2]) <= 10){
		maxthread = atoi(argv[2]) -1;
	}
	
	//Anlegen Verzeichnis pointer und festlegen der MaxThread
	DIR *dirp;
	pthread_t threadID[maxthread];
	
	path = argv[1];
	// Öffnen Directory und Zeit messen
	start = get_cur_time_161314();
	//char *toopen = "/bin";
	dirp = opendir(path);
	printf("Verzeichnis das überpruft wird: %s\n",path);
	
	// Kontrolle ob Directory geöffnet werden konnte
	if (!dirp){
		fprintf(stderr,"Konnte Ordner nicht öffnen!\n");
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
	
	closedir(dirp);
	calc_time(&start, &time_opendir);
	//Ausgabe gesamt Zeit
	fprintf(stderr,"\nGesamt Zeit öffnen Ordners: %lds,%ldns\n",time_opendir.tv_sec,time_opendir.tv_nsec);
	fprintf(stderr,"Gesamt Zeit öffnen Files: %lds,%ldns\n",time_openfile.tv_sec,time_openfile.tv_nsec);
	fprintf(stderr,"Gesamt Zeit berechnen HASH: %lds,%ldns\n\n",time_hash.tv_sec,time_hash.tv_nsec);
	
	add_time_161314(&time_all,&time_opendir);
	add_time_161314(&time_all,&time_openfile);
	add_time_161314(&time_all,&time_hash);
	fprintf(stderr,"Gesamt Zeit: %lds,%ldns\n\n",time_all.tv_sec,time_all.tv_nsec);
	
	return 0;
	
}

void *work_161314(void * ptr){
	struct dirent * entryp;	
	
	DIR *p = (DIR *)(ptr);
	pthread_mutex_lock(&dirsync);
		
	while((entryp = readdir(p)) != NULL){
		pthread_mutex_unlock(&dirsync);
		if(is_regular_file(entryp->d_name)){
		// (entryp->d_type == DT_DIR){
			//intf("Is dir\n");
			sleep(rand()%5);
			mkhash(entryp->d_name);
		}
		
		/*
		//rekursives abarbeiten 
		//"Problem ich brauch den Pfad von argv[1] um einen neuerliches opendir machen zu können"
		//Kontrolle ob entryp typ ein Directory ist
		else if(entryp->d_type ==DT_DIR)
		{
		struct stat info;
			//Ausschließen von . und ..
			if (strcmp(entryp->d_name, ".") != 0 && strcmp(entryp->d_name, "..") != 0){	
				char *tmppath = "";
				strcpy(tmppath, path);
				strcat(tmppath, "/");
				strcat(tmppath, entryp->d_name);
				printf("Subdirectory: %s\n",tmppath);
				if (stat(tmppath, &info) != 0)
					fprintf(stderr, "stat() error on %s: %s\n", path,strerror(errno));
				else if (S_ISDIR(info.st_mode)){				
					DIR *subdir;
					subdir = opendir(tmppath);
					work_161314((void *)(subdir));
				}
			}			
		}*/
		pthread_mutex_lock(&dirsync);
	}
	pthread_mutex_unlock(&dirsync);
	return NULL;
}

//Kontroll Funktion ob es ein Datei ist
int is_regular_file(const char *path){
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}

//Berechnet die vergangene Zeit und speichert sie in struct update
void calc_time(const struct timespec *start, struct timespec *update){
	struct timespec tmp_time = {0,0};
	struct timespec stop = {0,0};
	
	pthread_mutex_lock(&get_time);
	
	stop = get_cur_time_161314();
	tmp_time = get_diff_161314(start,&stop);
	add_time_161314(update,&tmp_time);
	
	pthread_mutex_unlock(&get_time);
}
//macht den HASH
void mkhash(const char *filename){
	unsigned char *c = malloc(MD5_DIGEST_LENGTH);	
	int bytes;
	unsigned char data[1024];
	
	int i;
	//Initialisieren MD5
	MD5_CTX mdContext;	
	MD5_Init(&mdContext);
	
	//Öffne Datei und speichern der Start Zeit
	struct timespec start_openfile = get_cur_time_161314();
	int file = open(filename, O_RDONLY);
	
	//Start HASH und speichern der Start Zeit
	struct timespec start_hash = get_cur_time_161314();
	while ((bytes = read (file, data, 1024)) != 0)
		MD5_Update(&mdContext, data, bytes);
	
	//Zeit berechnen und HASH abschließen
	calc_time(&start_openfile, &time_openfile);
	MD5_Final(c,&mdContext);
	calc_time(&start_hash,&time_hash);
	//Ausgeben des HASH Wertes
	for(i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i]);
	printf (" %s\n", filename);
	
	//return 0;
}



