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
	int c[2];
	c[1] =  0;
	int previous;
	read(fd, c, 1);
	while (!eof(fd)) {
		
	}
}
