#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLEN 1024
#define BLUE "\033[0;34m"
#define STANDARD_COLOR "\033[0m"

int read_command(char ***args);
void free_memory(char ***args);

int main() {
    char **args = NULL;
    int pid = 0;
    char dir[MAXLEN];
    char *home = getenv("HOME");
    getcwd(dir, MAXLEN);

    for (;;) {
	printf("%s%s%s$ ", BLUE, dir, STANDARD_COLOR);
	int argc = read_command(&args);
	if (argc > 0) {
	    if (strcmp(args[0], "exit") == 0) { //exit
		free_memory(&args);
		break;
	    }
	    if (strcmp(args[0], "cd") == 0) {   //cd
		if (argc == 1)
		    chdir(home);
		else if (chdir(args[1]) == -1)
		    perror(args[1]);
		getcwd(dir, MAXLEN);
	    }
	    else {                              //other commands 
	    	pid = fork();
	    	if (pid == -1) {
	            perror("fork");
	            exit(1);
	    	}
	    	if (pid == 0) {
	            execvp(args[0], args);
	       	    perror(args[0]);
	            exit(1);
	    	}
	    	wait(NULL);	
	    }
	    free_memory(&args);
	}
	if (argc == -1){
	    printf("Memory couldn't be allocated");
	    return 1;
	}
	if (argc == -2) {
	    printf("\n");
	    break;
	}
    }
    return 0;
}

int read_command(char ***args) {
    int arg_size = 0, current_len = 0, arg_number = 0;
    char c = 0;
    int quote_flag = 0;
    
    while ((c) != '\n') {
	c = getchar();
	if (isspace(c))
	    continue;
	if (c == EOF)
	    return -2;
	
	(*args) = realloc(*args, (arg_number + 1) * sizeof(char **));//memory allocation for new argument
	(*args)[arg_number] = NULL;
	while (!isspace(c) || quote_flag) {  //single word reading
	    if (c == '"') { //quotes
            	quote_flag = (quote_flag == 0 ? 1 : 0);
		c = getchar();
		continue;
	    }

            if (current_len == arg_size) { //memory allocation for particular argument
                arg_size = arg_size * 2 + 1;
   	        (*args)[arg_number] = realloc((*args)[arg_number], arg_size * sizeof(char *));
	    }
	    if (((*args)[arg_number]) == NULL) {
                return -1;
	    }
	   ((*args)[arg_number])[current_len] = c;
	    current_len += 1;
	    c = getchar();
	}
	if (current_len == arg_size) 
	    (*args)[arg_number] = realloc((*args)[arg_number], arg_size + 1);
	((*args)[arg_number])[current_len] = '\0';
	current_len = 0;
	arg_size = 0;
	arg_number += 1;
    }
    (*args) = realloc(*args, (arg_number + 1) * sizeof(char **));
    (*args)[arg_number] = NULL;
    return arg_number;
}


void free_memory(char ***args) {
    int i = 0;
    if (*args) {
        while ((*args)[i] != NULL) {
            free((*args)[i]);
	    i += 1;
        }
        free(*args);
        *args = NULL;
    }
}
