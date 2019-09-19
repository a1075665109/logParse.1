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
//used for boolean values
#include <stdbool.h>
//used for timer functions
#include <time.h>

// global file pointer
FILE *fp;
FILE *wfp;
// global variable to hold child process id
pid_t child_pid = -1;

// global to hold processes status, used for parent process to wait on remaining child process
int status=0;
pid_t wpid;

// sub set array pointer and subset counter;
int *sub;
int setCounter =0;

// initialize the max duration to be 10 seconds
int t = 10;

 // setting up default input and output file
char *inputFile = "input.dat";
char *outputFile = "output.dat";


// alarm handler that outputs to the proper output file with proper message
// close all the files and end the master process
void alarmHandler(int sig){
	fclose(fp);
        wfp = fopen(outputFile,"a");
        printf("Main program terminated because maximum allowed time has passed\n");
	fprintf(wfp,"%d : Master process terminated after %d seconds\n\n",getpid(),t);
	fclose(wfp);
	if(child_pid!=-1){
		kill(child_pid,SIGTERM);
	}
        kill(getpid(),SIGTERM);
}

// function that finds the subset then output the subset to the output file
// takes in the original time of when the process is created
// recursively looks for the difference to make sure it does not go over 1 second
bool subset(int set[],int n, int sum,time_t checkTime){
	time_t newTime;
	time(&newTime);
	if(newTime-checkTime >=1){
		wfp = fopen(outputFile,"a");
        	printf("Child Program terminated because 1 second has passed\n\n");
        	fprintf(wfp,"%d : Child process terminated without a result after 1 seconds\n",getpid());
        	fclose(wfp);
		kill(getpid(),SIGTERM);	
	}
	if(sum == 0){
		return true;
	}
	if (n == 0 && sum!= 0){
		return false;
	}
	if(set[n-1]>sum){
		return subset(set,n-1,sum,checkTime);
	}	
	if(subset(set,n-1,sum-set[n-1],checkTime)){
			sub[setCounter] = set[n-1];
                        setCounter +=1;
			return true;
	}
	if(subset(set,n-1,sum,checkTime)){
	  	sub[setCounter] = set[n-1];
                        return true;
	}
	return false;
}


// main function
int main(int argc, char* argv[]){
	signal(SIGALRM,alarmHandler);

	// integer to check for command line options and maximum running time
	int opt;
	int alarmSet = 0;
	sub = malloc(sizeof(int)*15);	
	// integer variable to hold the number of child/subtasks read from the input file.
	int numOfChild;
	
	// flags indicator for command line options
	int iflag,oflag,tflag;

	// modifying perror with executable
        char errorMsg[80];
        strcpy(errorMsg,argv[0]);
        strcat(errorMsg,": Error:");
	

	printf("\n=============================================================\n");
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
			 	printf("Input file: %s\n",optarg);
                                break;

			// setting up output file on -o
			case 'o':
				oflag = 1;
				outputFile = optarg;
				printf("Putput file: %s\n",outputFile);
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

	// formatting the output to the screen
	if(tflag!=1){
	printf("Max time duration: %d seconds\n", t);
	}
	if(iflag!=1){
	printf("Input file: %s\n",inputFile);
	}
	if(oflag!=1){
	printf("Output file: %s\n",outputFile);
	}
	if(iflag!=1&&oflag!=1&&tflag!=1){
	printf("=============================================================\n\n");
	}
	if(iflag==1||oflag==1||tflag==1){
	printf("=============================================================\n\n");
	}

	// set the alarm for the main process
	alarm(t);	

	// file pointer to open the target inputfile, then read the first number from that file;
	fp = fopen (inputFile,"r");
	if(fp == NULL){
		perror(errorMsg);
		return 0;
	}
	fscanf(fp,"%d",&numOfChild);
	printf("total number of Child: %d\n\n",numOfChild);
	
	// child counter for printing all child process
	int childCounter = numOfChild;
	// child array to hold all the child process id
	int childArray[numOfChild];	


	// loop through the number of child given by the input file
	while(numOfChild>0){
		int parentPID = getpid();
		//wait for the child to finish process
		//fork a child process then store the child process id into the child process id array;
		child_pid = fork();
		childArray[numOfChild-1] = child_pid;
			
		// parent task;
		if(child_pid>0){
			// wait for the child
			while ((wpid = wait(&status)) > 0);
			int temp;
                        fscanf(fp,"%d",&temp);		
			char temp1;
			
			// reduce child counter to determine the continuation of the while loop
			numOfChild -=1;

			temp1 = fgetc(fp);			
			// have the file pointer point to the next line
			while(temp1!='\n'&& temp1!=EOF){
				temp1=fgetc(fp);
			}
		// child task;
		}else{
		
			// uncomment to print child process id
			//printf("child process id: %d\n",getpid());
		
			// set up the starting time of the child process	
			time_t oriTime;
			time(&oriTime);
			
			// scanning the first number and set it to be the sum for the subsets
			int sum;
			fscanf(fp,"%d",&sum);
			printf("sum: %d\n",sum);
			// initialize an array that could store up to 10 integers
			int size = 10;
			int *temp;
			int i =0;
			temp = malloc(sizeof(int)*size);			
			
			// checking to see if next character is the next line
			int num;
			char temp1;
			temp1 = fgetc(fp);
			
			// keep scanning and add numbers to integer array until everything is done
			while(temp1 != '\n'){
				fscanf(fp,"%d",&num);
				temp[i]=num;
				i+=1;
				temp1 = fgetc(fp);
			}
		
			// uncomment to see all the numbers of the main set on console	
			int a = 0;
			if(a<i){
				printf("The main set: ");
			}
			while (a < i){
				printf("%d ",temp[a]);
				a+=1;
			}
			printf("\n");
			// open a writing file, if the subset is found, output to output file
			wfp = fopen(outputFile,"a");
			if(subset(temp,i,sum,oriTime)){
				wfp = fopen(outputFile,"a");
		
				int b =0, c=0;
				fprintf(wfp,"%d : ",getpid());
				while(b< setCounter){
					if(c != 0){
					 fprintf(wfp,"+ ");
					}
					fprintf(wfp,"%d ",sub[b]);
					b+=1;
					c += 1;			
				}
				fprintf(wfp,"= %d\n",sum);
				fclose(wfp);
				fprintf(wfp,"\n");

			// else output to the output file with a failed message
			}else{
				fprintf(wfp,"%d : No subset of numbers summed to %d\n",getpid(),sum);
				fclose(wfp);
			}

			//terminating the child process after process is finished
			kill(getpid(),SIGTERM);
		}
	}
	// wait for the chile to finish process;
	while ((wpid = wait(&status)) > 0);

	// format the display then
	// print all the process id to the screen and to the file	
	printf("\nAll the child process id: \n");
        wfp = fopen(outputFile,"a");
	fprintf(wfp,"\nMain process id: %d\n\n",getpid());
	fprintf(wfp,"All the child process id: ");
	while(childCounter > 0){
		printf("%d\n",childArray[childCounter-1]);
		fprintf(wfp,"%d  ",childArray[childCounter-1]);
		childCounter-=1;
	}
	printf("\n");
        fprintf(wfp,"\n\n");
	
	//close all files;
	fclose(wfp);
	fclose(fp);
	return 0;
}
