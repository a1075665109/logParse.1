#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	int opt;
	int hflag,iflag,oflag,tflag;
	char *inputFile = "input.dat";
	char *outputFile = "output.dat";
	while((opt = getopt(argc,argv,":hi:o:t"))!=-1){
                switch(opt){
                        case 'h':
                                printf("Legal command line options: -h -i -o -t\n\n");
                                printf("command option -i and -o changes the inputfile and outputfile\n\n");
				printf("the correct format should be -i or -o followed by the preferred file name (default file name input.dat,output.dat)\n\n");
				printf("use -t followed by a number to specify the max amount of time the program should run in seconds (default 10 seconds) \n\n");
				return 0;
                                break;
                        case 'i':
                                break;
			case 'o':
				break;
			case 't':
				break;
			default :
				printf("%s\t%s\n",inputFile,outputFile);
		}
	}
	return 0;
}
