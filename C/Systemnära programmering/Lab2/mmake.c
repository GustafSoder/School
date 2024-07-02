#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include "parser.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>

/**
* The code is constructed to make a small version of the unix-tool Make in C.
*
* Course: 5DV088 Systemnära programmering.
*
* Author: Gustaf Söderlund (et14gsd@cs.umu.se)
* Version information: 2021/09/27 V1.0
*
* Functions like makeparse() and print_vec() are inspired from the example code recieved from lecutre two.
*
*/

void execute(rule *, bool);
int arraysize(const char **);
void recur_rule(makefile *, const char *, bool, bool, int);
void print_vec(const char **strings);
makefile *makeparse(char *);
void build(makefile *, const char *, rule *, const char **, struct stat, struct stat, bool, bool, int, int,int);
void comptime(makefile *, const char *, rule *, const char **, struct stat, struct stat, bool, bool, int, int);


int main(int argc, char **argv)
{

	int opt;
	char *newfile = {"mmakefile"};
	bool forceExecute = false;
	bool noOutput = false;
	const char *target;
	int count = 0;

	while ((opt = getopt(argc, argv, "f:Bs")) != -1)
	{
		switch (opt)
		{
		case 'f':
			newfile = optarg;
			break;
		case 'B':
			forceExecute = true;
			break;
		case 's':
			noOutput = true;
			break;
		default:
			break;
		}
	}

	//parse the file.
	makefile *m = makeparse(newfile);

	// input to the recur_rule depends on the users input.
	int numtarget = argc - optind;
	// if several targets. 
	if (numtarget > 0)
	{
		for (int i = 0; i < numtarget; i++)
		{
			target = argv[optind + i];
			recur_rule(m, target, forceExecute, noOutput, count);
		}
	}
	// only one target. 
	else
	{

		target = makefile_default_target(m);
		recur_rule(m, target, forceExecute, noOutput, count);
	}

	//Delete the makefile @m. 
	makefile_del(m);

	return 0;
}

/*
* arraysize() - This function calculate the size of an array.
*
* @ruleArr: the array contains prerequisits.
*
* @return: an integer with the size of the array.
*/
int arraysize(const char **ruleArr)
{
	int size = 0;
	while (ruleArr[size] != NULL)
	{
		size++;
	}
	return size;
}

/**
 *recur_rule - this function creates a rule from the specified target.
 * It's also used recursive to traverse through the makefile.
 *
 * @m: The makefile.
 * @target: Name of a target.
 * @forceExecute: A Bool that is true if the B flag is used.
 * @noOutput: A bool that is true if the S flag is used.
 * @count: counts the amount of times the recursive is done. 
 */

void recur_rule(makefile *m, const char *target, bool forceExecute, bool noOutput, int count)
{
	// struct will help tell time 
	struct stat ruleStat, targetStat;
	const char **ruleArr;

	//Create a rule from target.
	rule *rule_r = makefile_rule(m, target);

	if (rule_r == NULL)
	{
		return;
	}

	// create an array with the prerequisits from the rule. 
	ruleArr = rule_prereq(rule_r);

	int tracker = 0;
	// Calculate the size of the array. 
	int size = arraysize(ruleArr);

	//build the none existing prereqs.
	build(m, target, rule_r, ruleArr, ruleStat, targetStat, forceExecute, noOutput, count,tracker, size);
	
	if(tracker != size)
	{
		comptime(m, target, rule_r, ruleArr, ruleStat, targetStat, forceExecute, noOutput,count,size);
	}
}


/*
*build() - Builds the target if the file does not exist.
* The function will go back to recur_rule() if the prereq file does not exist but it has
* a rule. 
* If a prereq file does not exist and it has no rule, error will be printed to stderr and program will exit. 
* If a prereq file does exist but no target. @Tracker will get + 1 added. 
* Once @tracker has the same value as @Size the rule will be built and the for-loop breakes. 
* 
* @m: The makefile.
* @target: Name of a target.
* @rule_r: The rule.
* @ruleArr: An array that contains prerequisits.
* @ruleStat: Buffert for the prerequsite file stat.
* @targetStat: Buffert for the target file stat.
* @forceExecute: a Bool that is true if the B flag is used.
* @noOutput: a bool that is true if the S flag is used.
* @tracker keeps track on how many prereqfiles that exist but has no target. 
* @count counts the amount of times the recursive is done. 
* @size: the size of the array with 
*/
void build(makefile * m, const char * target, rule * rule_r, const char **ruleArr,
 struct stat ruleStat, struct stat targetStat, bool forceExecute, bool noOutput, int count, int tracker, int size)
{
	for (int i = 0; ruleArr[i] != NULL; i++)
	{

		//if the prereq file does not exist but it has a rule traverse
		if ((stat(ruleArr[i], &ruleStat) == -1) && (makefile_rule(m, ruleArr[i]) != NULL))
		{
			recur_rule(m, ruleArr[i], forceExecute, noOutput, count);
		}

		//if the prereq file does not exist and it has no rule.
		if ((stat(ruleArr[i], &ruleStat) == -1) && (makefile_rule(m, ruleArr[i]) == NULL))
		{
			fprintf(stderr, "mmake: No rule to make target '%s'\n", target);
			exit(EXIT_FAILURE);
		}

		// if prereq file exist but no target add +1 to tracker. 
		if ((stat(ruleArr[i], &ruleStat) == 0 && (stat(target, &targetStat) == -1)))
		{
			tracker++;
		}

		//When tracker has reached the same value as the size of the amount of prerequsites that @size hold 
		// we Execute and break loop.
		if ((tracker == size))
		{
			execute(rule_r, noOutput);
			break;
		}
	}
}


/*
* comptime() - Comepare the prerequisist last modified time.
* Check for errors in the given rule and prerequisits.
*
*
* @m: The makefile.
* @target: Name of a target.
* @rule_r: The rule.
* @ruleArr: An array that contains prerequisits.
* @ruleStat: Buffert for the prerequsite file stat.
* @targetStat: Buffert for the target file stat.
* @forceExecute: a Bool that is true if the B flag is used.
* @noOutput: a bool that is true if the S flag is used.
* @count: counts the amount of times the recursive is done. 
* @size: the size of the array with 
*/
void comptime(makefile * m, const char * target, rule * rule_r, const char ** ruleArr,
 struct stat ruleStat, struct stat targetStat,
  bool forceExecute, bool noOutput, int count, int size)
{
	for (int i = 0; i < size; i++)
	{
		//if there's no rule print error and exit. 
		if ((stat(ruleArr[i], &ruleStat) < 0))
		{
			perror(ruleArr[i]);
			exit(EXIT_FAILURE);
		}
		// if no target exist print error and exit. 
		if ((stat(target, &targetStat) < 0))
		{
			perror(target);
			exit(EXIT_FAILURE);
		}
		
		// Get the rule for building a specific target in a makefile and check if it's not null, and
		// check if the "rule-time" was modified later or the at the same time as the targets modified time. 
		if(makefile_rule(m, ruleArr[i]) != NULL && (targetStat.st_mtime >= ruleStat.st_mtime ))
		{
			recur_rule(m, ruleArr[i], forceExecute, noOutput, count);
		}

		// if the prerequisites was modified later then the target. or if B-flag is used build anyway. 
		if ((targetStat.st_mtime < ruleStat.st_mtime) || forceExecute == true)
		{
			execute(rule_r, noOutput);
			break;
		}

		// Get the rule for building a specific target in a makefile and check if it's null, and
		// check if the "rule-time" was modified later or the at the same time as the targets modified time.
		if(makefile_rule(m, ruleArr[i]) == NULL && (targetStat.st_mtime >= ruleStat.st_mtime) )
		{
			continue;
		}

		//if @target is equal to @m's default target and we are on the second to last iteration of the loop and @count is not equal to @size. 
		// we add 1 to @count and recursive go back to recur_rule() with the @target as input. 
		if ((target == makefile_default_target(m)) && (i == size-1) && (size != count))
		{
			count++;
			recur_rule(m, target, forceExecute, noOutput, count);
		}
	}
}

/*
*execute() - This function executes the given rule.
*
*@rule_r: the rule
*@noOutput: a bool that is true if the S flag is used.
*
*/
void execute(rule *rule_r, bool noOutput)
{
	int processid;
	int status;
	char **commands = rule_cmd(rule_r);
	// Fork
	if ((processid = fork()) == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	// if the list has no values aka there's no commands to execute.
	// print error and exit program.
	if(commands[0] == NULL)
	{
		perror("No command's to execute.");
		exit(EXIT_FAILURE);
	}

	if (processid == 0)
	{	
		//silent execute. if the s- flag is used by the user.  
		if (noOutput == false)
		{
			print_vec((const char **)rule_cmd(rule_r));
			fflush(stdout);
		}
		
		//Execute the rule. 
		if (execvp(commands[0], rule_cmd(rule_r)) == -1)
		{
			perror("Failed to execute the command.");
			exit(EXIT_FAILURE);
		}
	}
	// wait. 
	wait(&status);
	if (WIFEXITED(status)) {

		if (WEXITSTATUS(status) != 0) {

			perror("Waitpid error");
			exit(EXIT_FAILURE);
		}
	}
}

/*
* makeparse() - This function opens a file and creates a makefile from
* the file.
*
* @newmakefile - The file that need to be open and parsed.
*
* return: parsed makefile.
*/

makefile *makeparse(char *newmakefile)
{

	// Open makefile file
	FILE *fp = fopen(newmakefile, "r");

	if (fp == NULL)
	{
		perror(newmakefile);
		exit(EXIT_FAILURE);
	}

	// Parse makefile
	makefile *m = parse_makefile(fp);

	if (m == NULL)
	{ // Handle syntax error / failed parsing

		fprintf(stderr, "%s: Could not parse makefile\n", newmakefile);
		exit(EXIT_FAILURE);
	}

	// Close file
	fclose(fp);
	return m;
}


/*
* print_vec(Print array of strings terminated with NULL)
*
* @strings: The array that will be printed
*
*/
void print_vec(const char **strings)
{

	for (int i = 0; strings[i] != NULL; i++)
	{
		if (strings[i + 1] == NULL)
		{
			printf("%s", strings[i]);
		}
		else
		{
			printf("%s ", strings[i]);
		}
	}
	printf("\n");
}