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

int main(int argc, char *argv[]){
	struct stat sb;
	bool lOption = false;
	int c;	
	DIR *dp;
	struct dirent *d;	
	int argCount = argc;
	struct passwd *pw;
	struct group *gr;
	struct tm *timeMod;
	char buf[120];
	char path[257];
	if(argc > 3){
		fprintf(stderr, "TOO MANY ARGUMENTS\n");
		exit(EXIT_FAILURE);
	}	
	

	while(argc > 1 && --argc > 0 && (*++argv)[0] == '-'){
		while((c = *++argv[0])){  
			if(c == 'l'){
				lOption = true;
			}
		
			else{
				fprintf(stderr, "Usage: -l\n");
				exit(EXIT_FAILURE);
			}
		}
	}	
	printf("%s\n", *argv);	
	if(argCount == 1 || (argCount == 2 && lOption == true)){
		dp = opendir(".");
		assert(dp != NULL);
	}

	else if(argCount == 2 && lOption == false){
		if(stat(*argv, &sb) == -1){
			perror("stat");
		exit(EXIT_FAILURE);			
		}
		printf("%s\n",*argv);
		dp = opendir(*argv);
	}

	else if(argCount  == 3 && lOption == true)
	{
		if(stat(*argv, &sb) == -1){
			perror("stat");
			exit(EXIT_FAILURE);
		}

		dp = opendir(*argv);
	}
	
	while((d = readdir(dp)) != NULL){
		sprintf(path, "%s/%s", *argv, d->d_name);
		if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
		       continue;
		if(stat(path, &sb) == -1)
		{
			printf("reading\n");			
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
	closedir(dp);
	return 0;
}



