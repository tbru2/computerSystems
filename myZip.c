#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000

int main(int argc, char *argv[]){ 
	FILE *fp;
	char buffer[MAXLINE];
	int counter = 0;
	char prevChar;
	int charCount = 0;

	if(argc > 2){
		fprintf(stderr, "my-zip: file1 [file2 ...]\n");
		exit(1);		
	}

	if((fp = fopen(argv[1], "r")) == NULL){
		fprintf(stderr,	"Cannot open file\n");
		exit(EXIT_FAILURE);	
	} 

	while(fgets(buffer, MAXLINE, fp) != 0){
		prevChar = buffer[0];
		while(counter < strlen(buffer) || buffer[counter] == EOF){
			if(buffer[counter] == '\n'){
				printf("%c", buffer[counter]);
				counter++;
				charCount = 0;
				continue;
			}
			if(buffer[counter] == prevChar){
				charCount++;
			}
			else{
				printf("%i", charCount+1);
				printf("%c", prevChar);
				charCount = 0;
				prevChar = buffer[counter];
			}
		counter++;
		}
		counter = 0;
	}

	fclose(fp);

	return 0;	
}



