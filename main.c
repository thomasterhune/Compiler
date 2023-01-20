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

void addExtension(char* filename, char* extension) {
    strcat(filename, extension);
}

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

<<<<<<< HEAD
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
=======
bool promptFileName(char* Input_File_Name) {
	bool NotEntered = false;
	printf("Enter the input file name: ");
	scanf("%s", Input_File_Name);
	
	return NotEntered;
}

bool openInputFile(char* Input_File_Name, FILE* Input_File) {
	bool terminate = false;
>>>>>>> 44cba13f8db4f420d97345197f8b33f6ba162b2e
	/* If no extension add .IN */
	if(!hasExtension(Input_File_Name)) {
		addExtension(Input_File_Name, ".IN");
	}
	
	/*if does not exist*/
<<<<<<< HEAD
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
=======
	Input_File = fopen(Input_File_Name, "r");
	if(Input_File == NULL){
		//promptFileName();
	}
>>>>>>> 44cba13f8db4f420d97345197f8b33f6ba162b2e
}

void openOutputFile(char* Output_File_Name, FILE* Output_File) {
	/* If no extension add .OUT */
<<<<<<< HEAD
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
=======
	if(!hasExtension(Output_File_Name)) {
		addExtension(Output_File_Name, ".OUT");
	}
		
	Output_File = fopen(Output_File_Name, "w");
>>>>>>> 44cba13f8db4f420d97345197f8b33f6ba162b2e
	/* if exists choose to overwrite, enter new output file name, or terminate */
}

void closeFile(FILE* file){
	if(file != NULL){
		fclose(file);
	}
}

int main(int argc, char *argv[]) {
	
<<<<<<< HEAD
	char inputFileName[100];
	char outputFileName[100];
	FILE * inputFile;
	FILE * outputFile;
	bool quit = false;
=======
	char Input_File_Name[100];
	char Output_File_Name[100];
	FILE * Input_File, Output_File;
>>>>>>> 44cba13f8db4f420d97345197f8b33f6ba162b2e
	
	/*Command Line Parameters*/
	if(argc == 3) {
		/* Copy string from command line args */
<<<<<<< HEAD
		strcpy(inputFileName, argv[1]);
		/*if input file opens then try to open output file*/
		if(openInputFile(inputFileName, inputFile)) {
			/*Copy string from command line args */
			strcpy(outputFileName, argv[2]);
			openOutputFile(outputFileName, outputFile);
=======
		strcpy(Input_File_Name, argv[1]);
		
		if(openInputFile(Input_File_Name, Input_File)){
			//Copy string from command line args
			strcpy(Input_File_Name, argv[1]);
>>>>>>> 44cba13f8db4f420d97345197f8b33f6ba162b2e
		}
		
		
		
	}else if(argc == 2) {
		/* Copy string from command line args */
<<<<<<< HEAD
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
=======
		strcpy(Input_File_Name, argv[1]);
	}else {
		
	}
		/* closing files */
	fclose(Input_File);
  	fclose(Output_File);
	
>>>>>>> 44cba13f8db4f420d97345197f8b33f6ba162b2e
	return 0;
}
