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
	
	for(i; i < strlen(filename) && !Extension_Found; i++){
		if(filename[i] == '.'){
			extensionFound = true;
		}
	}
	
	return extensionFound;
}

bool promptFileName(char* inputFileName) {
	bool notEntered = false;
	printf("Enter the input file name: ");
	scanf("%s", inputFileName);
	if (inputFileName[0] == '\0'){
		notEntered = true;
	}
	
	
	return notEntered;
}

bool openInputFile(char* inputFileName, FILE* inputFile) {
	bool terminate = false;
	//If no extension add .IN
	if(!hasExtension(inputFileName)) {
		addExtension(inputFileName, ".IN");
	}
	
	/*if does not exist*/
	inputFile = fopen(inputFileName, "r");
	if(inputFile == NULL){
		//promptFileName();
	}
	
	
	return terminate;
}

void openOutputFile(char* outputFileName, FILE* outputFile) {
	//If no extension add .OUT
	if(!hasExtension(outputFileName)) {
		addExtension(outputFileName, ".OUT");
	}
		
	outputFile = fopen(outputFileName, "w");
	//if exists choose to overwrite, enter new output file name, or terminate
}

int main(int argc, char *argv[]) {
	
	char inputFileName[100];
	char outputFileName[100];
	FILE * inputFile, outputFile;
	
	/*Command Line Parameters*/
	if(argc == 3) {
		//Copy string from command line args
		strcpy(inputFileName, argv[1]);
		
		if(openInputFile(inputFileName, inputFile)) {
			//Copy string from command line args
			strcpy(outputFileName, argv[2]);
			openOutputFile(outputFileName, outputFile);
		}
	}else if(argc == 2) {
		//Print for testing purposeses
		printf("The input file name is %s\n", argv[1]);
		//Copy string from command line args
		strcpy(inputFileName, argv[1]);
		
		openInputFile(inputFileName, inputFile)
		/*Prompt for output file*/
		printf("Enter the output file name: ");
		
	}else {
		printf("Enter the input file name: ");
		
		printf("Enter the output file name: ");
	}
		
	return 0;
}