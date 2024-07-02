/**
* MDU - The code is constructed to calculate how much disc usage a specified
* file or directory takes on the disk.
*
* Course: 5DV088 Systemnära programmering.
*
* Author: Gustaf Söderlund (et14gsd@cs.umu.se)
* Version information: 2021/11/18 V2.0
*
*/

/*****************************************
			Includes
*****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>
#include <math.h>
#include "queue.h"

/*****************************************
				struct
*****************************************/
typedef struct threadinfo{
	queue *que;
	int threadNum;
	int i;
	char** targarray;
	int size;
	int sleepth;
}threadinfo;

/*****************************************
			Function Declaration
*****************************************/
void *thread_func(void *);
void openrecur(char *,queue*);
void checktarget(char*,queue*);
int sizeOfFile(char*);
int dirsizecalculator(char*);
void threadInit(queue *, int ,pthread_t* ,char* );
void threadchecktarget(char*, queue*);
void threadopenrecur(char *,queue*);
int threaddirsizecalculator(char* );
void init_struct(threadinfo *ti, int numThreads, int argc, int optind, char**argv);

/*****************************************
			global variables
*****************************************/
pthread_mutex_t lock;
pthread_cond_t cond;
bool errorhandle = true; 

//to keep track on the size of the specified directory / file.
int sum = 0;

/*****************************************
				Main
*****************************************/
int main(int argc, char **argv) 
{
	// set 1 thread in beginning
	int numThreads = 0; 		
	int opt;		

	//target file or dirr.	
	char* target;		 

	//Check arguments
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <filename> <directory>\n", argv[0]);
		return 1;
	}

	//check how many threads
	while ((opt = getopt(argc, argv, "j:")) != -1)
	{
		switch (opt)
		{
			case 'j':
				if((numThreads = atoi(optarg)) < 1)
				{
					fprintf(stderr,"Can't have negative amount of flags.\n");
					exit(EXIT_FAILURE);
				}
				break;

			default:
				fprintf(stderr,"Wrong usage, (mdu -j {Amount of threads} {file} [files ...])\n");
				exit(EXIT_FAILURE);
				break;
		}
	}

	/*****************************************
				multi threading
	*****************************************/
	if(numThreads>0)
	{
		//Create struct to send into threadfunc. 
		threadinfo *ti = malloc(sizeof(threadinfo));
		if (ti == NULL)
		{
			fprintf(stderr, "malloc failed.\n");
			exit(EXIT_FAILURE);
		}
		//init the struct. 
		init_struct(ti,numThreads,argc,optind,argv);

		//create an empty queue
		queue q;
		queue_empty(&q);
		//add queue to struct. 
		ti->que = &q;

		// initializing the mutex lock and
		pthread_mutex_init( &lock, NULL);
		pthread_cond_init( &cond, NULL);

		//set amount of threads
		pthread_t thread[numThreads];

		//create threads
		for (int i = 0; i < numThreads; i++)
		{
			if(pthread_create(&thread[i], NULL, thread_func, (void*)ti)<0)
			{
				perror("pthread_create error");
				exit(EXIT_FAILURE);
			}
		}
		
		//Main Thread waits for threads here. 
		for (int i = 0; i < numThreads; i++)
		{
			if((pthread_join(thread[i],NULL))<0)
			{
				perror("pthread_join error");
				exit(EXIT_FAILURE);
			}
		}
		
		//print the size of file. 
		printf("%d\t%s\n", ti->size+24, ti->targarray[0]);

		//Destroy the mutex. 
		pthread_mutex_destroy(&lock);
		pthread_cond_destroy(&cond);

		//free the struct. 
		for (int i = 0; i < argc - optind; i++)
		{	
			free(ti->targarray[i]);
		}
		free(ti->targarray);
		free(ti);
	}

	/*****************************************
				not multi threading
	*****************************************/
	else
	{
		//create an empty queue
		queue q;
		queue_empty(&q);
		//loop the amount of aguments given. 
		for (int i = 0; i < argc - optind; i++)
		{
			sum = 0;
			target = argv[optind + i];

			//check if the target is valid file or directory.
			checktarget(target,&q);
		}
	}
	if(!errorhandle)
	{
		return -1;
	} 
	return 0;
}

/**
* init_struct() - This function is used to initialize the struct with variables and 
* datatypes to use inside the threadfunc. 
* @param ti the struct
* @param numThreads number of threads. 
* @param argc amount of arguments. 
* @param optind index
* @param argv aguments
*/
void init_struct(threadinfo *ti, int numThreads, int argc, int optind, char**argv)
{
	//Allocate targarray - this array will hold the main paths set by the user. 
	ti->targarray = malloc(sizeof(char*)*(argc - optind + 1));
	if (ti->targarray == NULL)
	{
		fprintf(stderr, "malloc failed.\n");
		exit(EXIT_FAILURE);
	}

	//loop the amount of aguments given. 
	int ind = 0; 
	for (int i = optind; i < argc; i++)
	{	
		ti->targarray[ind] = malloc(strlen(argv[i]) + 1);
		if (ti->targarray[ind] == NULL)
		{
			fprintf(stderr, "malloc failed.\n");
			exit(EXIT_FAILURE);
		}
		//copy the argument to the array once we allocated space. 
		if(strcpy(ti->targarray[ind], argv[i])==NULL)
		{
			fprintf(stderr, "Strcpy failed.\n");
			exit(EXIT_FAILURE);
		}
		ind++;
	}
	// init the other variables in the struct. 
	ti->threadNum = numThreads;
	ti->i = 0;
	ti->sleepth = 0;
	ti->size = 0;
	ti->targarray[ind] = NULL;
}

/**
* thread_func () - In this function the threads will enter with a struct.
* The struct contains information about what directory.
*
* @arg the struct with threadinfo of the queue.
*/
void *thread_func(void *arg)
{
	threadinfo *ti = (struct threadinfo*) arg;
	int size = 0;
	
	// This loops until all threads are sleeping. 
	while(true)
	{	
		//lock mutex. 
		pthread_mutex_lock(&lock);
		//check the argument. 
		if(ti->targarray[ti->i] != NULL)
		{
			int ind = 0;
			ind =  ti->i++;
			//add the main path to the queue. 
			queue_add(ti->que, ti->targarray[ind]);
			//Signal the conditional variable. 
			pthread_cond_signal(&cond);
			//Check if the given argument is a valid directory. 
			threadchecktarget(ti->targarray[ind],ti->que);
			
		}else{
			if(!queue_is_empty(ti->que))
			{
				//get the filepath. 
				char path[strlen(queue_first(ti->que)) + 1];
				strcpy(path, queue_first(ti->que));
				queue_remove(ti->que);
				pthread_mutex_unlock(&lock);
				// calculate the size when mutex unlocked.
				size = dirsizecalculator(path);
				
				//Lock the mutex and add the size of the files to the struct. 
				pthread_mutex_lock(&lock);
				ti->size+=size;
				pthread_mutex_unlock(&lock);
				
			}else
			{	
				//if sleeping threads is equal to the amount of threads. 
				if(ti->sleepth == ti->threadNum)
				{
					break;
				}
				ti->sleepth++;
				//if all threads are sleeping when we added +1 on the sleeping threads 
				// Use broadcast and break. 
				if(ti->sleepth == ti->threadNum)
				{
					pthread_cond_broadcast(&cond);
					break;
				//else sleep	
				}else
				{
					//keeps moving upon when signal is used on condition variable. 
					pthread_cond_wait(&cond,&lock);
					if(ti->sleepth != ti->threadNum)
					{
						ti->sleepth--;
					}
					pthread_mutex_unlock(&lock);
				}
			}
		}	
	}
	//unlock the mutex. 
	pthread_mutex_unlock(&lock);
	return NULL;
}

/**
* threadchecktarget() - The purpose of this function is to make sure
* that the target is directory in this function Mutex is used.
*
* @param target is the directory we want to open.
* @param q the queue.
*/
void threadchecktarget(char* target, queue *q)
{	
	pthread_mutex_unlock(&lock);
	// The macro S_ISREG() returns true if the given st_mode is a regular file
	struct stat file_info;

	//Check if <target> really is a regular file
	if (stat(target, &file_info) < 0)
	{
		fprintf(stderr, "Error: Can't stat file '%s' on line %d\n",
				target, __LINE__);			
	}
	//check if directory
	if (S_ISDIR(file_info.st_mode))
	{	 
		pthread_mutex_lock(&lock);
		threadopenrecur(target, q);	
	}
	return;
}

/**
* threadopenrecur() - open the directory recursive and add the directory path
* to the queue. creates a queue where the value stores the
* directory path.
*
* @param target is the directory we want to open.
* @param q the queue.
*/
void threadopenrecur(char *target,queue *q)
{
	pthread_mutex_unlock(&lock);
	DIR *dir;
	struct dirent *dr;
	char path[1024];
	// open the specified directory.
	if((dir = opendir(target)) == NULL)
	{
		fprintf(stderr, "du: cannot read directory '%s': Permission denied\n",target);
		errorhandle = false;
	}
	else
	{
		while(( dr = readdir(dir)) != NULL )
		{
			if(strcmp(dr->d_name,".")== 0 || strcmp(dr->d_name,"..") == 0)
			{
				continue;
			}
			// create the path
			snprintf(path,sizeof(path), "%s/%s",target, dr->d_name);

			// if the file type is a directory.
			if(dr->d_type == DT_DIR)
			{	
				// lock mutex before adding path to queue to avoid Race condition. 
				pthread_mutex_lock(&lock);
				queue_add(q,path);	
				//signal that a path is available in the queue. 
				pthread_cond_signal(&cond);
				//call the function again to see if there is any sub folders
				threadopenrecur(path,q);
			}
		}
	}
	closedir(dir);
}

/**
* checktarget() - The purpose of this function is to make sure
* that the target is a file or a directory.
*
* @param target is the directory we want to open.
* @param q the queue.
*/
void checktarget(char* target, queue *q)
{
	// The macro S_ISREG() returns true if the given st_mode is a regular file
	struct stat file_info;

	//Check if <target> really is a regular file
	if (stat(target, &file_info) < 0)
	{
		fprintf(stderr, "Error: Can't stat file '%s' on line %d\n",
				target, __LINE__);			
	}
	//check if file. 
	if (S_ISREG(file_info.st_mode))
	{
		//calculate the file size. 
		sum += file_info.st_blocks;
		printf("%d	%s\n",sum, target);
	}
	//check if directory
	if (S_ISDIR(file_info.st_mode))
	{	
		openrecur(target, q);
		queue_add(q,target);
		//While the Queue is not empty calculate the size of the directiory. 
		while(!(queue_is_empty(q)))
		{
			sum += dirsizecalculator(q->firstnode->value);	
			queue_remove(q);	
		}
		printf("%d	%s\n",sum+16, target);	
	}
	return;
}

/**
* openrecur() - open the directory recursive and add the directory path
* to the queue. So function creates a queue where the node value stores the
* directory path.
*
* @param target is the directory we want to open.
* @param q the queue.
*/
void openrecur(char *target,queue *q)
{
	DIR *dir;
	struct dirent *dr;
	char path[1024];
	// open the specified directory.
	if((dir = opendir(target))==NULL)
	{
		fprintf(stderr, "du: cannot read directory '%s': Permission denied\n",target);
		errorhandle = false;
	}
	else
	{
		while(( dr = readdir(dir)) != NULL )
		{
			if(strcmp(dr->d_name,".")== 0 || strcmp(dr->d_name,"..") == 0)
			{
				continue;
			}
			// create the path
			snprintf(path,sizeof(path), "%s/%s",target, dr->d_name);

			// if the file type is a directory.
			if(dr->d_type == DT_DIR)
			{
				queue_add(q,path);
				openrecur(path,q);
			}
		}
	}
	closedir(dir);
}

/**
* sizeOfFile() - Calculate the size of the file.
*
* @param filename the name of the file.
*/
int sizeOfFile(char* filename)
{
	struct stat file;
	if(lstat(filename,&file)<0)
	{
		perror(filename);
		exit(EXIT_FAILURE);
	}
	return file.st_blocks;
}

/**
* dirsizecalculator() - this function calculate the size of the directory.
*
* @param target the path to the directory.
*/
int dirsizecalculator(char* target){
	//struct stat file;
	DIR *dir;
	struct dirent *dr;
	char path[1024];
	int size = 0;

	if((dir = opendir(target))!=NULL)
	{
		while(( dr = readdir(dir))!=NULL)
		{
			if(strcmp(dr->d_name,".")== 0 || strcmp(dr->d_name,"..")== 0)
			{
				continue;
			}
			snprintf(path,sizeof(path), "%s/%s",target, dr->d_name);
			//calculate filesize. 
			size += sizeOfFile(path);
		}
	}
	//Close dir and return the size. 
	closedir(dir);
	return size;
}
