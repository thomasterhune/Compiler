#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void addExtension(char* filename) {
    strcat(filename, ".x");
}

bool isExtension(char* filename){
	bool Extension_Found = false;
	int i = 0;
	
	for(i; i < strlen(filename) && Extension_Found == false; i++){
		if(filename[i] == '.'){
			Extension_Found = true;
		}
	}
	
	return Extension_Found;
}

int main(int argc, char *argv[]) {
	
	/*Command Line Parameters*/
	switch(argc){
		case 1:
			break;
			
		case 2:
			printf("The input file name is %s\n", argv[1]);
			break;
			
		case 3:
			printf("The input file name is %s\n", argv[1]);
			printf("The input file name is %s\n", argv[2]);
			break;
	}
	
	
	
	
	
	
	
	
	char filename[100] = "Hello.txt";
	
	int size = strlen(filename);
	
	printf("%s", filename);
	
	printf("%i", isExtension(filename));
	
	
	addExtension(filename);
	printf("%s", filename);
}