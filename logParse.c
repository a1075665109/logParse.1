#include <stdio.h>
#include <stdlib.h>
//used to get optarg from command line
#include <unistd.h>
//used to string copy and concatenaation
#include <string.h>
//used for forking child process
#include <sys/types.h> 
//used for signaling actions on a process 
#include <signal.h>

// global variable to hold child process id
pid_t child_pid = -1;

// function to kill the child process on alarm;

int main(int argc, char* argv[]){
	// integer to check for command line options and maximum running time
	int opt;
	int t;
	
	// integer variable to hold the number of child/subtasks read from the input file.
	int numOfChild;
	
	// flags indicator for command line options
	int iflag,oflag,tflag;

	// modifying perror with executable
        char errorMsg[80];
        strcpy(errorMsg,argv[0]);
        strcat(errorMsg,": Error:");
	
	// setting up default input and output file
	char *inputFile = "input.dat";
	char *outputFile = "output.dat";

	printf("=============================================================\n");
	// loop through the command line option
	while((opt = getopt(argc,argv,"hi:o:t:"))!=-1){
	        switch(opt){	
			// display informations on -h
                        case 'h':
                                printf("\nLegal command line options: -h -i -o -t\n");
                                printf("command option -i and -o changes the inputfile and outputfile\n");
				printf("the correct format should be -i or -o followed by the preferred file name (default file name input.dat,output.dat)\n");
				printf("use -t followed by a number to specify the max amount of time the program should run in seconds (default 10 seconds)\n\n");
				break;
	
			// setting up input file on -i
                        case 'i':
				iflag = 1;
				inputFile = optarg;
			 	printf("input file changed to: %s\n",optarg);
                                break;

			// setting up output file on -o
			case 'o':
				oflag = 1;
				outputFile = optarg;
				printf("output file changed to: %s\n",outputFile);
				break;

			// setting up the time on -t
			case 't':
				tflag = 1;
				t = atoi (optarg);
				printf("Max time duration: %d seconds\n", t);
				break;
			
			// default case to print out incorrect command options
			default :
                              // perror(errorMsg);
				break;
		}
	}
	if(iflag==1||oflag==1||tflag==1){
	printf("=============================================================\n\n");
	}
	
	// file pointer to open the target inputfile, then read the first number from that file;
	FILE *fp;
	fp = fopen (inputFile,"r");
	if(fp == NULL){
		perror(errorMsg);
		return 0;
	}
	fscanf(fp,"%d",&numOfChild);
	printf("number of Child: %d\n",numOfChild);
	
	// child counter for printing all child process
	int childCounter = numOfChild;
	// child array to hold all the child process id
	int childArray[numOfChild];	

	// loop through the number of child given by the input file
	while(numOfChild >0){
		
		//fork a child process then store the child process id into the child process id array;
		child_pid = fork();
		childArray[numOfChild-1] = child_pid;

		// parent task; 
		if(child_pid >0){
			printf("main process id: %d\n",getpid());
			printf("child process id: %d\n",child_pid);
			numOfChild -=1;
		
		// child task;
		}else{

			// terminating the child process after process is finished
			kill(getpid(),SIGKILL);
		}
	}

	// printing all the child process id;	
	printf("\nAll the child process id: \n");
	while(childCounter > 0){
		printf("%d\n",childArray[childCounter-1]);
		childCounter-=1;
	}

	// close the input file
	fclose(fp);
		
	return 0;
}
