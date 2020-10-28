#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int utf2ucs(int fd_in, int fd_out);
int check_10(char symbol);
	
int main (int argc, char *argv[]) {
    if (argc < 3) {
	printf("Enter 2 arguments: input file, output file\n");
        return 1;
    }
    
    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1) {
        perror("Can't read input file");
        return 2;
    }
    
    int fd_out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT);
    if (fd_out == -1) {
        perror("Can't read output file");
        return 2;
    }

    if (utf2ucs(fd_in, fd_out) == -1) {
    	printf("Incorrect input file\n");
	return 1;
    }

    close(fd_in);
    close(fd_out);
    return 0;	
}

int utf2ucs(int fd_in, int fd_out) {
    char in_symbol;
    unsigned short out_symbol;
    int flag, counter = 0;

    for (;;) {
	if ((flag = read(fd_in, &in_symbol, 1)) != 1) {
	    return flag;
	}
	counter++;
	out_symbol = 0;
	if ((in_symbol & 0xF0) == 0xE0) {
	    out_symbol = in_symbol & 0xF;
	    for (int i = 0; i < 2; i++) { 
	        if ((read(fd_in, &in_symbol, 1) != 1) || check_10(in_symbol)){
		    fprintf(stderr, "In %d symbol %d byte should should start with 10\n", counter, i + 2);
		    continue;
		}
		else
		    out_symbol = (out_symbol << 6) + (in_symbol & 0x3F);
	    }
    }
	else if ((in_symbol & 0xE0) == 0xC0) {
	    out_symbol = (in_symbol & 0x1F);
	    if ((read(fd_in, &in_symbol, 1) != 1) || check_10(in_symbol)){
	        fprintf(stderr, "In %d symbol 2 byte should should start with 10\n", counter);
	        continue;
	    }
            else
                out_symbol = (out_symbol << 6) + (in_symbol & 0x3F);
	}
	else if ((in_symbol & 0x80) == 0)
	    out_symbol = in_symbol;
	else {
	    fprintf(stderr, "Incorrect %d symbol\n", counter);
       	    continue;
	}
	write(fd_out, &out_symbol, 2);
    }
}

int check_10(char symbol) {
    if ((symbol & 0xC0) == 0x80)
        return 0;
    return -1;
}
