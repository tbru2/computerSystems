#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <stdbool.h>
#include <dirent.h>
#include <assert.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

bool isLOption(int argc, char *argv[]);
char *  getDir(int argCount, bool lOption, char *argv[]);
void printDirStats(DIR *dp, bool lOption, char *dir);

int main(int argc, char *argv[]){
	bool lOption = false;
	DIR *dp;
	char *dir;

	if(argc > 3){
		fprintf(stderr, "TOO MANY ARGUMENTS\n");
		exit(EXIT_FAILURE);
	}	
	
	lOption = isLOption(argc, argv);
	dir = getDir(argc, lOption, argv);	
	dp = opendir(dir);	
	printDirStats(dp, lOption, dir);	
	closedir(dp);
	return 0;
}

void printDirStats(DIR *dp, bool lOption, char *dir){
	struct stat sb;
	char path[280];
	struct passwd *pw;
	struct group *gr;
	struct tm *timeMod;
	struct dirent *d;
	char buf[120];

	while((d = readdir(dp)) != NULL){
		sprintf(path, "%s/%s", dir, d->d_name);
		if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
		       continue;
		if(stat(path, &sb) == -1)
		{
			perror("stat");
			exit(EXIT_FAILURE);
		}	
		if(lOption == true){
			putchar( (S_ISDIR(sb.st_mode)) ? 'd' : '-');
			putchar((sb.st_mode & S_IRUSR) ? 'r' : '-');
			putchar((sb.st_mode & S_IWUSR) ? 'w' : '-');
			putchar((sb.st_mode & S_IXUSR) ? 'x' : '-');
			putchar((sb.st_mode & S_IRGRP) ? 'r' : '-');
			putchar((sb.st_mode & S_IWGRP) ? 'w' : '-');
			putchar((sb.st_mode & S_IXGRP) ? 'x' : '-');
			putchar((sb.st_mode & S_IROTH) ? 'r' : '-');
			putchar((sb.st_mode & S_IWOTH) ? 'w' : '-');
			putchar((sb.st_mode & S_IXOTH) ? 'x' : '-');
			
			pw = getpwuid(sb.st_uid);
			gr = getgrgid(sb.st_gid);

			printf(" %i %s %s %5lu",(int)sb.st_nlink, pw->pw_name, 
				gr->gr_name,(unsigned long) sb.st_size);
			timeMod = localtime(&sb.st_mtime);
			strftime(buf, sizeof(buf), "%b %e %H:%M", timeMod);
			printf(" %s", buf);
			printf(" %s\n", d->d_name);

		}else{
			printf("%s\n", d->d_name);
		}
	}		
}

char * getDir( int argCount, bool lOption, char *argv[]){
	char *dir;	
	struct stat sb;	
	if(argCount == 1 || (argCount == 2 && lOption == true)){
		dir = ".";	
	}

	else if(argCount == 2 && lOption == false){
		dir = argv[1];
		if(stat(argv[1], &sb) == -1){
			perror("stat");
			exit(EXIT_FAILURE);			
		}
	}

	else if(argCount  == 3 && lOption == true)
	{
		dir = argv[2];
		if(stat(argv[2], &sb) == -1){
			perror("stat");
			exit(EXIT_FAILURE);
		}

		
	}
	return dir;
}


bool isLOption(int argc, char *argv[]){
	
	int c;
	while(argc > 1 && --argc > 0 && (*++argv)[0] == '-'){
		while((c = *++argv[0])){  
			if(c == 'l'){
				return true;
			}
		
			else{
				fprintf(stderr, "Usage: -l\n");
				exit(EXIT_FAILURE);
			}
		}
	}	
	return false;
}


