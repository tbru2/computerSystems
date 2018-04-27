#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>

//A program that prints out the name of each file and directory in the file 
//system tree, starting at a given point in the tree.

void printDirRec(DIR *dp, char *path, int steps);  

int main(int argc, char *argv[]){
	DIR *dp;
	char *path;
	struct dirent *d;

	if(argc > 2){
		fprintf(stderr, "TOO MANY ARGUMENTS\n");
		exit(EXIT_FAILURE);
	}
	else if(argc == 2){
		path = argv[1];
	}
	else{
		path = ".";
	}

	printDirRec(dp, path, 0);

	return 0;
}

void printDirRec(DIR *dp, char *path, int steps){
	struct stat stbuf;
	struct dirent *d;
	char recurPath[1024];	

	if((dp = opendir(path)) == NULL){
		return;		
	}
	
	while((d = readdir(dp)) != NULL)
	{
		sprintf(recurPath, "%s/%s", path, d->d_name);

		if(strcmp(d->d_name, ".") == 0 || 
			strcmp(d->d_name, "..") == 0){
				continue;
		}
		if ((stat(path, &stbuf)) == -1){

			perror("stat");
			exit(EXIT_FAILURE);
		}	 		
		if((stbuf.st_mode & S_IFMT) == S_IFDIR){
			steps++;
			printDirRec(dp, recurPath, steps);
		}
		printf("%*s %s\n", steps, "", d->d_name);
	}
	closedir(dp);
}
