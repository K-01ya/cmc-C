#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int BOM_check(int fd);
int usc2utf(int BOM, int fd_in, int fd_out);

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
        perror("Can't read input file");
        return 2;
    }

    if (usc2utf(BOM_check(fd_in), fd_in, fd_out) == -1) {
    	printf("Incorrect input file\n");
	return 1;
    }

    close(fd_in);
    close(fd_out);
    return 0;
}

int BOM_check(int fd) {
    unsigned short symbol;
    int flag = read(fd, &symbol, 2);
    if (flag == -1 || symbol == 0xfffe) {
        return 0;
    }
    if (symbol == 0xfeff) {
        return 1;
    }
    lseek(fd, 0, SEEK_SET);
    return 1;
}

int usc2utf(int BOM, int fd_in, int fd_out) {
    unsigned short in_symbol;
    char out_symbol;
    int flag;
        
    for (;;) {	    
	flag = read(fd_in, &in_symbol, 2);
	if (flag == 1 || flag == -1) {
	    return -1;
	}
	if (flag <= 0) {
            return 0;
	}
       	if (BOM == 0)     
            in_symbol = (in_symbol / 256) + ((in_symbol % 256) * 256);
        
	if (in_symbol <= 127) {
            out_symbol = in_symbol;
            write(fd_out, &out_symbol, 1);
        }
        else if (in_symbol <= 2047) {
            out_symbol = 128 + 64 + in_symbol / 64;
            write(fd_out, &out_symbol, 1);
            out_symbol = 128 + in_symbol % 64;
            write(fd_out, &out_symbol, 1);
        }
        else {
            out_symbol = 128 + 64 + 32 + in_symbol / 4096;
            write(fd_out, &out_symbol, 1);
            out_symbol = 128 + (in_symbol % 4096 - in_symbol % 64) / 64;
            write(fd_out, &out_symbol, 1);
            out_symbol = 128 + in_symbol % 64;
            write(fd_out, &out_symbol, 1);
        }
    }
    return 0;
}
