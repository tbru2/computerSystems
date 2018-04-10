#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXLINES 5000
#define BUFFSIZE 4096
#define MAXLEN 1000

char *lineptr[MAXLINES];

int _readlines(char *lineptr[], int nlines, FILE *fp); 
int _getline(char *line, int max, FILE *fp);
int getNumLinesToRead(int argc, char *arv[]);

int main(int argc, char *argv[]){

	int nLines, startLine;
	int nLinesToRead;
	FILE *fp;
	

	if(argc > 3){
		fprintf(stderr, "Too many arguments\n");
		exit(EXIT_FAILURE);
	}
	else if (argc == 3)
		fp = fopen(argv[2], "r");
	else if(argc == 2){
		fp = fopen(argv[1], "r");	
	}

	if(fp == NULL){
		printf("%s\n", "Not a valid file");
		exit(EXIT_FAILURE);
	}

	nLinesToRead  = getNumLinesToRead(argc, argv);
	nLines = _readlines(lineptr, MAXLINES, fp);

	startLine = nLines - nLinesToRead-1;
	while(startLine < nLines){
		printf("%s\n", lineptr[startLine]);
		startLine++;
	}

		
	return 0;		
}

char *_fgets(char *s, int n, FILE *fp){
	register int c;
	register char *cs;

	cs = s;
	while(--n > 0 && (c = getc(fp)) != EOF)
		if((*cs++ = c) == '\n')
			break;
	*cs = '\0';
	return (c == EOF && cs == s) ? NULL : s;
}

int _getline(char *line, int max, FILE *fp){
	if(_fgets(line, max, fp) == NULL)
		return 0;
	else 
		return strlen(line);
}

int _readlines(char *lineptr[], int maxlines, FILE *fp){
	int len, nlines;
	char *p, line[MAXLEN];

	nlines = 0;
	while ((len = _getline(line, MAXLEN, fp)) > 0)
		if (nlines >= maxlines || (p = malloc(len)) == NULL)
			return -1;
		else{
			line[len-1] = '\0';
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	return nlines;
}

int getNumLinesToRead(int argc, char *argv[]){
	int c = 10;
	
	if(argc == 3){	
		if((c = atoi(argv[1] + 1)) > 0){
			return c;
		}else{
			fprintf(stderr, "Usage: -n\n");
			exit(EXIT_FAILURE);
		}
	}
	return c;
	
}
