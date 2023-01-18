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
	
	for(i; i < strlen(filename) && Extension_Found == false; i++){
		if(filename[i] == '.'){
			Extension_Found = true;
		}
	}
	
	return Extension_Found;
}

void promptFileName(char* Input_File_Name) {
	printf("Enter the input file name: ");
	scanf("%s", Input_File_Name);
	if (str[0] == '\0') {
	}
}

bool openInputFile() {
	bool terminate = false;
	//If no extension add .IN
	if(!hasExtension(Input_File_Name)) {
		addExtension(Input_File_Name, ".IN");
	}
	
	/*if does not exist*/
	if(Input_File = fopen(Input_File_Name, "r") == NULL){
		//promptFileName();
	}
}

void openOutputFile() {
	//Print for testing purposeses
	printf("The output file name is %s\n", argv[2]);
	//Copy string from command line args
	strcpy(Output_File_Name, argv[2]);
	//If no extension add .OUT
	if(!hasExtension(Output_File_Name)) {
		addExtension(Output_File_Name, ".OUT");
	}
		
	Output_File = fopen(Output_File_Name, r);
}

int main(int argc, char *argv[]) {
	
	char Input_File_Name[100];
	char Output_File_Name[100];
	FILE * Input_File, Output_File;
	
	/*Command Line Parameters*/
	if(argc == 3) {
		//Copy string from command line args
		strcpy(Input_File_Name, argv[1]);
		
		if(openInputFile()){
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
		
		
		
	
	printf("%s", filename);
	
	printf("%i", isExtension(filename));
	
	
	addExtension(filename);
	printf("%s", filename);
}