#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>

int main(int argc, char **argv) {
	pid_t pid;
	int fd[2];
	if (argc < 1) {
		printf("too few arguments\n");
		return 1;
	}
	else {
		pipe(fd);
		pid = fork();
		if (pid == -1) {
			perror("fork 1");
		}	
		else if (pid == 0) {
			dup2(fd[1], 1);
			close(fd[1]);
			close(fd[0]);
			execlp("sort", "sort", "-u",argv[1], NULL);
			perror("sort");
			exit(1);
		}
		else {
			wait(NULL);
			pid = fork();
			if (pid == -1) {
				perror("fork 2");
			}   
			else if (pid == 0) {
				dup2(fd[0], 0);
	            close(fd[1]);
    	        close(fd[0]);
				execlp("wc", "wc", "-l", argv[1], NULL);
				perror("sort");
				exit(1);
			}
			else {
				wait(NULL);
			}
		}
	}
	return 0;
}

