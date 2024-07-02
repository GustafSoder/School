#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
/*
* This is a program that can execute a pipeline. The input is read from
* a given file or directly from standard input.
*
* Authors: Gustaf Söderlund (et14gsd@cs.umu.se)
*
* Version information:
*   2022-09-29: v1.0
*
*
*/

/** 
 * Struct declaration
 */

struct buf{
	char **args;
};

/** 
 * Function declaration
 */
void stringSep(int ,char** , struct buf*);
FILE* checkInput(int,char**);
void pipefork(int, struct buf *, int [][2]);
void readfile( FILE*,int *, char**,int);
void closepipe(int, int [][2]);
void freearray(int, struct buf *);
void pipecreate(int ,int [][2]);

/**
 * Main 
 */
int main(int argc, char *argv[])
{
	int rows = 0;
	int status;
	struct buf *commands; 
	// check the input and create a FILE
	FILE *inputfile = checkInput(argc,argv);

	//set size for file
	int size = 1024;
		
	char **stringholder = malloc(sizeof(char*)*size); 
	if(stringholder == NULL)
	{
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}

	//read the given file
	readfile(inputfile, &rows, stringholder,size);

	//Alocate memory for commands. 
	commands = malloc(sizeof(*commands)*rows);
	if(commands == NULL)
	{
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}
	
	//seperate the arguments and store them in @commands. 
	stringSep(rows,stringholder, commands);
	
	//create file descriptors and create pipes. 
	int numpipe = rows - 1;
	int fd[numpipe][2];
	pipecreate(numpipe, fd);

	//closing the inputfile before fork() to avoid the childprocesses to iherit the file. 
	fclose(inputfile);
	
	pipefork(rows, commands, fd);
	
	//the purpose of this loop is to wait for the child process, and make sure
	// there is no errors in the child processes.
	for (int i = 0; i < rows; i++)
	{
		wait(&status);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) != 0)
			{
				freearray(rows, commands);
				exit(EXIT_FAILURE);
			}
		}
	}
	freearray(rows, commands);
	exit(0);
}

/*
* stringSep()- Seperates the strings into arguments. And store the argument så @commands.
*
* @rows - integer to keep track on the amount of lines in file. 
* @stringholder - temporary hold the file. 
* @commands - where we want to store the arguments. 
*/
void stringSep(int rows,char** stringholder, struct buf* commands)
{
	int k = 0;
	
	for(int i = 0; i < rows ; i++)
	{
		char* piece = strtok(stringholder[i]," ");
		commands[i].args = (char**)malloc(sizeof(char*)*sizeof(stringholder));
		while(piece != NULL)
		{	
			
			commands[i].args[k] = (char*)malloc(strlen(piece)+1*sizeof(char));
			strcpy(commands[i].args[k], piece);
			piece = strtok(NULL, " \n");
		 	k++;
		}
		commands[i].args[k] = NULL;
		k = 0;
	}
	
	for(int i = 0; i < rows; i++)
	{
		 free(stringholder[i]);
	}
	free(stringholder);
	return;
}


/**
* readfile()- Read the file get amount of lines and store the file temporary in stringholder.
*
* @param rows - integer to keep track on the amount of lines in file. 
* @param stringholder - temporary hold the file. 
* @param inputfile- the input file.
* @param size - the size of the file. 
*/

void readfile(FILE *inputfile, int *rows, char** stringholder, int size)
{
	int r=0;
	char line[size];

	while(fgets(line, size, inputfile) != NULL)
	{
		line[strcspn(line, "\n")] = 0;
		stringholder[r] = strdup(line);
		r++;
	}
	*rows = r;
	return;
}

/**
* pipecreate - Creates pipes from the given amount of commands and arguments.
*
* @param numpipe: integer variable that keeps track on the amount of pipes to create. 
*
* @param fd: integer array that's used to create the pipes.
*/
 void pipecreate(int numpipe,int fd[numpipe][2])
{
	// Creating pipes.
	for (int i = 0; i < numpipe; i++)
	{
		if (pipe(fd[i]) != 0)
		{
			perror("Failed to create pipe");
			exit(EXIT_FAILURE);
		}
	}	
	return; 
}

/**
* pipefork - This function creates the child processes and depending on the processid 
*			 each command that's given will be executed. 
*				
* @param rows: 	The int variabel that keeps track on the amount of
*	 			rows in the inputfile. Rows represent the amount of commands in the
*				input file.
*
* @param commands:	An two dimensional struct array of strings that stores the
*					commands and arguments.
*
* Returns: Nothing.
*/
void pipefork(int rows, struct buf *commands,int fd[][2])
{
	int processid;
	int numpipe = rows -1;
	
	//create childprocesses, create as many children as rows in the inputfile.
	for (int child = 0; child < rows; child++)
	{
		if ((processid = fork()) == -1)
		{
			perror("No child process was created.");
			exit(EXIT_FAILURE);
		}
		
		if (processid == 0)
		{
			// if its only one command.
			
			if (rows == 1)
			{
				if (execvp(commands[0].args[0], commands[0].args) == -1)
				{
					perror("ERROR");
					exit(EXIT_FAILURE);
				}
			}
			//first child process
			if (child == 0)
			{		
				// rearrange the file descriptor.
				if (dup2(fd[child][1], STDOUT_FILENO) == -1)
				{
					perror("dup2 failed");
					
					exit(EXIT_FAILURE);
				}

				//close the pipes after dup.
				closepipe(numpipe, fd);

				// execute the command
				if (execvp(commands[child].args[0], commands[child].args) == -1)
				{
					perror("Failed to execute the command");
					freearray(rows, commands);
					exit(EXIT_FAILURE);
				}
			}

			// if the child is on the last iteration.
			if (child == numpipe)
			{	
				if (dup2(fd[child - 1][0], STDIN_FILENO) == -1)
				{
					perror("dup2 failed");
					exit(EXIT_FAILURE);
				}
				closepipe(numpipe, fd);

				if (execvp(commands[child].args[0], commands[child].args) == -1)
				{
					perror("failed to execute the last command");
					freearray(rows, commands);
					exit(EXIT_FAILURE);
				}
			}
			// not first or last child process.
			if ((child > 0) && (child < numpipe))
			{
				// rearrange the file descriptor.
				if (dup2(fd[child - 1][0], STDIN_FILENO) == -1)
				{
					perror("dup2 failed");
					exit(EXIT_FAILURE);
				}
				// rearrange the file descriptor.
				if (dup2(fd[child][1], STDOUT_FILENO) == -1)
				{
					perror("dup2 failed");
					exit(EXIT_FAILURE);
				}

				closepipe(numpipe, fd);

				if (execvp(commands[child].args[0], commands[child].args) == -1)
				{
					perror("Failed to execute the command");
					freearray(rows, commands);
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	closepipe(numpipe, fd);
	return;
}

/**
* closepipe - This function closes the pipes to the file descriptors
*	that we are not intressted in.
*
* @param fd: An two dimensional array that stores the pipes.
* Returns: Nothing.
*/
void closepipe(int numpipe, int fd[][2])
{
	for (int i = 0; i < numpipe; i++)
	{
		close(fd[i][0]);
		close(fd[i][1]);
	}
	return;
}

/**
* freearray - This function deallocates the memory that was previously allocated.
*
* @param rows amount of rows in file. 
* @commands: An two dimensional struct array of strings that stores the
*	commands and arguments.
* Returns: Nothing.
*/
void freearray(int rows, struct buf *commands)
{
	for (int i = 0; i < rows; i++)
	{
		int j = 0;
		while (commands[i].args[j] != NULL)
		{
			free(commands[i].args[j]);
			j++;
		}
		free(commands[i].args);
	}
	free(commands);
}

/**
 * checkInput() - This function checks if the input is valid and creates a FILEpointer. 
 * @param argc number of arguments.
 * @param argv the arguments.
 * @return a filepointer. 
 * */
FILE* checkInput(int argc, char* argv[])
{
	FILE *inputfile;
	if (argc == 1)
	{
		inputfile = stdin;
	}
	else if (argc == 2)
	{
		if ((inputfile = fopen(argv[1], "r")) == NULL)
		{
			perror(argv[1]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		fprintf(stderr, "Wrong number of arguments.\n");
		fprintf(stderr, "usage: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	return inputfile;
}