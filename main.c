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

bool promptFileName(char* Input_File_Name) {
	bool NotEntered = false;
	printf("Enter the input file name: ");
	scanf("%s", Input_File_Name);
	
	return NotEntered;
}

bool openInputFile(char* Input_File_Name, FILE* Input_File) {
	bool terminate = false;
	//If no extension add .IN
	if(!hasExtension(Input_File_Name)) {
		addExtension(Input_File_Name, ".IN");
	}
	
	/*if does not exist*/
	Input_File = fopen(Input_File_Name, "r");
	if(Input_File == NULL){
		//promptFileName();
	}
}

void openOutputFile(char* Output_File_Name, FILE* Output_File) {
	//If no extension add .OUT
	if(!hasExtension(Output_File_Name)) {
		addExtension(Output_File_Name, ".OUT");
	}
		
	Output_File = fopen(Output_File_Name, "w");
	//if exists choose to overwrite, enter new output file name, or terminate
}

int main(int argc, char *argv[]) {
	
	char Input_File_Name[100];
	char Output_File_Name[100];
	FILE * Input_File, Output_File;
	
	/*Command Line Parameters*/
	if(argc == 3) {
		//Copy string from command line args
		strcpy(Input_File_Name, argv[1]);
		
		if(openInputFile(Input_File_Name, Input_File)){
			//Copy string from command line args
			strcpy(Input_File_Name, argv[1]);
		}
		
		
		
	}else if(argc == 2) {
		//Print for testing purposeses
		printf("The input file name is %s\n", argv[1]);
		//Copy string from command line args
		strcpy(Input_File_Name, argv[1]);
	}else {
		
	}
		
	return 0;
}