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

// global file pointer
FILE *fp;
// global variable to hold child process id
pid_t child_pid = -1;

// global to hold processes status, used for parent process to wait on remaining child process
int status=0;
pid_t wpid;


// main function
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
	while(numOfChild>0){
		//wait for the child to finish process
		//fork a child process then store the child process id into the child process id array;
		child_pid = fork();
		while ((wpid = wait(&status)) > 0);
		childArray[numOfChild-1] = child_pid;
		

		// parent task;
		if(child_pid>0){	
			char temp1;
			printf("main process id: %d\n",getpid());
			printf("child process id: %d\n",child_pid);
			numOfChild -=1;
			temp1 = fgetc(fp);
			
			// have the file pointer point to the next line
			while(temp1!='\n'&& temp1!=EOF){
				temp1=fgetc(fp);
			}
		// child task;
		}else{
			int sum;
			fscanf(fp,"%d",&sum);
			printf("sum: %d\n",sum);
			// initialize an array that could store up to 10 integers
			int size = 10;
			int *temp;
			int i =0;
			temp = malloc(sizeof(int)*size);
			
			fscanf(fp,"%d[^\n]",&sum);
			while(!feof(fp)){
				temp[i] = sum;
				printf("%d\n",temp[i]);
				i+=1;
				fscanf(fp,"%d[^\n]",&sum);
			}
			// terminating the child process after process is finished
			kill(getpid(),SIGTERM);
		}
	}
	// wait for the chile to finish process;
	while ((wpid = wait(&status)) > 0);
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
