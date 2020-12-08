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

int bad(int fd) {
	int c = 0, previous;
	int flag = 0;
	read(fd, &c, 1);
	if (c == -1 || c == '\n') {
		return 0;
	}
	for(;;) {
 	       previous = c;
               read(fd, &c, 1);
	       if (c == -1 || c == '\n')
		       return 0;
               if (flag * (c - previous) < 0)
		       return 1;
	       flag = c - previous;
       }

}

int main(int argc, char **argv) {
	if (argc < 2) 
		return 0;
	int fd = open(argv[1], O_WRONLY, 0666);
	fd = 0;
	int n[1024], i = 0, n_str = 0;
	int c = 0;
	while (feof(STDIN)) {
		if (!bad(fd)) {
			n[i] = n_str;
			i++;
		}
		else {
			read(fd, &c, 1);
			while (!feof(STDIN) && c != '\n')
				read(fd, &c, 1);
		}
		n_str++;
	}
	printf("%d\n", i); 
	
}
