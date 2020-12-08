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

int good(int fd) {
	int c = 0;
	int flag = 0;
	read(fd, &c, 1);
	if (c == -1 || c != '\n') {
		return 1;
	}
	int previous = c;
	read(fd, &c, 1);
	if (c == -1 || c != '\n')
		return 0;
	flag = c - previous;
	read(fd, &c, 1);
	for(;;) {
 	       previous = c;
               read(fd, &c, 1);
	       if (c == -1 || c != '\n')
		       return 0;
               if (flag * (c - previous) < 0)
		       return 1;
	       flag = c - previous;
       }

}

int main(int argc, char **argv) {
/*	int fd = open(argv[1], O_WRONLY, 0666);
	int c;
	int n[1024];
	int i = 0;
	int previous;
	int flag = 0;
	int flag1 = 0;
	int flag2 = 0; */
//	read(fd, c, 1);
	printf("%d", good(0));

	/*
	while (!eof(fd)) {
		read(fd, &c, 1);

		while (!eof(fd) && c != '\n') {
			previous = c;
			read(fd, &c, 1);
			if (previous > c) {
				printf
			}
		}
	}
	*/
}
