/**
  * 
  * Author: Tom Terhune
  * Author: Karl Miller
  * Author: Anthony Stepich
  * 
  * program 1 for CSC 460.
  * 
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "file_util.h"



bool hasExtension(char* filename){
	bool Extension_Found = false;
	int i = 0;
	
	for(i; i < strlen(filename) && !Extension_Found; i++){
		if(filename[i] == '.'){
			Extension_Found = true;
		}
	}
	
	return Extension_Found;
}

bool promptFileName(char* fileName) {
	bool flag = true;
	size_t bufsize = 100;

	/*scan for filename*/
	

	/*
		Note, I have commented out getline for now, but it needs to be removed.

		getline was added around 2010 and, if getline is used, the code will not compile with the -ansi flag (which limits us to 1990 C).

		-K
	
	*/
	/*getline(&fileName,&bufsize, stdin);*/
	/*if filename is null*/
	if (fileName[0] == '\0'){
		flag = false;
	}

	printf("%c", fileName[0]);
	return flag;
}

bool openInputFile(char* inputFilename, FILE* inputFile) {
	bool flag = true;

	if(!hasExtension(inputFilename)) {
		addExtension(inputFilename, ".IN");
	}
	
	if(fileExists(inputFilename)) {
		inputFile = fopen(inputFilename, "r");
	} else {
		printf("File does not exist.");
		printf("Enter an existing input file name: ");
		if(promptFileName(inputFilename)){
			openInputFile(inputFilename, inputFile);
		} else {
			flag = false;
		}
	}
	
	
	return flag;
}


bool openOutputFile(char* outputFilename, FILE* outputFile) {
	/* If no extension add .OUT */
	int userInput = -1;
	bool flag = true;
	if(!hasExtension(outputFilename)) {
		addExtension(outputFilename, ".OUT");
	}

	if(fileExists(outputFilename)) {
		while(userInput == USER_OUTPUT_TERMINATE_INVALID_ENTRY){
			userInput = promptUserOverwriteSelection();
		}
		switch(userInput) {
			case USER_OUTPUT_OVERWRITE_REENTER_FILENAME_SELECTED:
				if(promptFileName(outputFilename)){
					openOutputFile(outputFilename, outputFile);
				} else {
					/*generate the output file from the source file name with the .OUT*/

				}
				break;
			case USER_OUTPUT_OVERWRITE_OVERWRITE_EXISTING_FILE:
				outputFile = fopen(outputFilename, "w");
				break;
			case USER_OUTPUT_OVERWRITE_DEFAULT_FILENAME:

				break;
			case USER_OUTPUT_TERMINATE_PROGRAM:
				flag = false;
				break;
		}
	} else {
		outputFile = fopen(outputFilename, "w");
	}

	return flag;
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