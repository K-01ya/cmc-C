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
	int fd = open(argv[1], O_WRONLY, 0666);
	int c;
	int n[1024];
	int i = 0;
	int previous;
	int flag = 0;
	int flag1 = 0;
	int flag2 = 0;
	read(fd, c, 1);
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
	
}
