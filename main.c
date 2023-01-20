#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void addExtension(char* filename, char* extension) {
    strcat(filename, extension);
}

bool hasExtension(char* filename){
	bool extensionFound = false;
	int i = 0;
	
	for(i; i < strlen(filename) && !extensionFound; i++){
		if(filename[i] == '.'){
			extensionFound = true;
		}
	}
	
	return extensionFound;
}

bool promptFileName(char* fileName) {
	bool flag = true;
	size_t bufsize = 100;

	/*scan for filename*/
	getline(&fileName,&bufsize, stdin);
	/*if filename is null*/
	if (fileName[0] == '\0'){
		flag = false;
	}

	printf("%c", fileName[0]);
	return flag;
}

bool openInputFile(char* inputFileName, FILE* inputFile) {
	bool flag = true;
	/* If no extension add .IN */
	if(!hasExtension(inputFileName)) {
		addExtension(inputFileName, ".IN");
	}
	
	/*if does not exist*/
	inputFile = fopen(inputFileName, "r");
	while(inputFile == NULL){
		printf("File does not exist. /nEnter an existing input file name: ");
		/*if filename string is not null try to open*/
		if(promptFileName(inputFileName)){
			inputFile = fopen(inputFileName, "r");
		} else {
			flag = false;
		}
	}
	
	
	return flag;
}

void openOutputFile(char* outputFileName, FILE* outputFile) {
	/* If no extension add .OUT */
	int userInput;
	if(!hasExtension(outputFileName)) {
		addExtension(outputFileName, ".OUT");
	}
		
	outputFile = fopen(outputFileName, "r");
	if(outputFile == NULL){
		outputFile = fopen(outputFileName, "w");
	}else{
		printf("File exists /nEnter 1 to overwrite/nEnter 2 to enter a new output file name/nEnter 3 to quit");
		scanf("%i", userInput);
		if(userInput == 1){
			outputFile = fopen(outputFileName, "w");
		}else if(userInput == 2){
			if(promptFileName(outputFileName)){
				outputFile = fopen(outputFileName, "r");
			}
		}else if(userInput == 3){

		}
	}
	/* if exists choose to overwrite, enter new output file name, or terminate */
}

void closeFile(FILE* file){
	if(file != NULL){
		fclose(file);
	}
}

int main(int argc, char *argv[]) {
	
	char inputFileName[100];
	char outputFileName[100];
	FILE * inputFile;
	FILE * outputFile;
	bool quit = false;
	
	/*Command Line Parameters*/
	if(argc == 3) {
		/* Copy string from command line args */
		strcpy(inputFileName, argv[1]);
		/*if input file opens then try to open output file*/
		if(openInputFile(inputFileName, inputFile)) {
			/*Copy string from command line args */
			strcpy(outputFileName, argv[2]);
			openOutputFile(outputFileName, outputFile);
		}
	}else if(argc == 2) {
		/* Copy string from command line args */
		strcpy(inputFileName, argv[1]);
		
		/*if input file opens then try to open output file*/
		if(openInputFile(inputFileName, inputFile)) {
			/*Copy string from command line args */
			printf("Enter the output file name: ");
			promptFileName(outputFileName);
			openOutputFile(outputFileName, outputFile);
		}
		
	}else {
		/*prompt for input file*/
		printf("Enter the input file name: ");
		/*if input filename is entered*/
		if(promptFileName(inputFileName)){
			if(openInputFile(inputFileName, inputFile)) {
				printf("Enter the output file name: ");
				promptFileName(outputFileName);
				openOutputFile(outputFileName, outputFile);
			}
		}
	}
	closeFile(inputFile);
	closeFile(outputFile);
	return 0;
}