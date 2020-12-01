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

#define MAXLEN 1024
#define BLUE "\033[0;34m"
#define STANDARD_COLOR "\033[0m"

int command_execution(char *dir, char **symbols, char ***commands);
int split_command(char **args, char ****commands, char ***splitted_symbols);
int special_symbol(char *str);
int read_command(char ***args);
void free_memory(char ***args);

int main() {
    int argc = 0;
    char **args = NULL;
    char dir[MAXLEN];
    char ***commands = NULL;
    char **special_symbols = NULL;
    int flag_command = 0;
    getcwd(dir, MAXLEN);
    int command_number;

    for (;;) {
        printf("%s%s%s$ ", BLUE, dir, STANDARD_COLOR);
        argc = read_command(&args);
        if (argc > 0) {/*
	    for (int i = 0; i < argc; i++)
                printf("%d %s\n", i, args[i]);
            command_number = split_command(args, &commands, &special_symbols);
            for (int i = 0; i < command_number; i++) {
                for (int j = 0; commands[i][j] !=  NULL; j++) {
                        printf("%d: %s", i, commands[i][j]); printf("\n"); }
            }
	    for (int k = 0; special_symbols[k] != NULL; k++)
		    printf("%d: %s \n", k, special_symbols[k]);
        */
	flag_command = command_execution(dir, special_symbols, commands);
	if (flag_command == -1)
		break;
	for (int i = 0; i <= command_number; i++)
	    free_memory(&(commands[i]));
	commands = NULL;
	free_memory(&args);
	free_memory(&special_symbols);
	special_symbols = NULL;
	}
        if (argc == -1){
            printf("Memory couldn't be allocated");
            return -1;
        }
        if (argc == -2) {
            printf("\n");
            break;
        }
    }
    return 0;
}

int command_execution(char *dir, char **symbols, char ***commands) {
    int symbol_index = 0, command_index = 0;
    char *home = getenv("HOME");
    pid_t pid = 0;
    char **args = commands[0];
    int fd[2];
	int pipe_flag = 0;
    while (commands[command_index] != NULL) {
		pipe_flag = 0;
		if (strcmp(args[0], "exit") == 0) { //exit
			return -1;
		}
		if (strcmp(args[0], "cd") == 0) {   //cd
			if (args[1] == NULL)
				chdir(home);
			else if (chdir(args[1]) == -1)
				perror(args[1]);
			getcwd(dir, MAXLEN);
		}
		else {	    //other commands 
			while (symbols != NULL && symbols[symbol_index] != NULL && strcmp(symbols[symbol_index], "|") == 0) {
				pipe(fd);
				pid = fork();
				if (pid == -1) {
    	            perror("fork");
        	        exit(1);
                }
				else if (pid == 0) {
					dup2(fd[1], 1);
					if (pipe_flag == 1) 
						dup2(fd[0], 0);
					close(fd[1]);
					close(fd[0]);
					execvp(args[0], args);
	    	        perror(args[0]);
    	    	    exit(1);
				}
				wait(NULL);
				pipe_flag = 1;
				symbol_index += 1;
				command_index += 1;
				if (commands[command_index] == NULL) {
					printf("Wrong command\n");
					return -3;
				}
				args = commands[command_index];
			}
			pid = fork();
			if (pid == -1) {
				perror("fork");
				exit(1);
			}
			if (pid == 0) {
				while (symbols[symbol_index] != NULL && (strcmp(symbols[symbol_index], ">") == 0 || strcmp(symbols[symbol_index], "<") == 0 || strcmp(symbols[symbol_index], " >>") == 0)) {
					if (commands[command_index + 1] == NULL) {
							printf("Wrong command\n");
							exit(1);
						}
					if (strcmp(symbols[symbol_index], ">") == 0) {
						int fd_out = open(commands[command_index + 1][0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
						if (fd_out == -1) {
							perror(commands[command_index + 1][0]);
							exit(1);
						}
						dup2(fd_out, 1);
						close(fd_out);
					}
					else if (strcmp(symbols[symbol_index], "<") == 0) {
						int fd_in = open(commands[command_index + 1][0], O_RDONLY, 0666);
						if (fd_in == -1) {
							perror(commands[command_index + 1][0]);
							exit(1);
						}
						dup2(fd_in, 0);
						close(fd_in);
					}
					else if (strcmp(symbols[symbol_index], ">>") == 0) {
						int fd_out = open(commands[command_index + 1][0], O_WRONLY | O_CREAT, 0666);
						if (fd_out == -1) {
							perror(commands[command_index + 1][0]);
							exit(1);
						}
						dup2(fd_out, 1);
						close(fd_out);
					}
					command_index += 1;
					symbol_index += 1;
				}
				if (pipe_flag == 1) {
					dup2(fd[0], 0);
				}
				execvp(args[0], args);
				perror(args[0]);
				exit(1);
			}
			wait(NULL);
		}
		while (symbols[symbol_index] != NULL && strcmp(symbols[symbol_index], "|") != 0) {
			command_index += 1;
			symbol_index += 1;
		}
		command_index += 1;
		args = commands[command_index];
	}
    return 0;
}

int split_command(char **args, char ****commands, char ***special_symbols) {
    int command_number = 0, word_number = 0, special_symbol_number = 0;
    int i = 0;
    (*commands) = realloc(*commands, sizeof(char***));
    (*commands)[0] = NULL;
    (*special_symbols) = realloc(*special_symbols, sizeof(char **));
    (*special_symbols)[0] = NULL;

    while (args[i] != NULL) {
//      printf("split\n");
        if (special_symbol(args[i])) {
            (*special_symbols)[special_symbol_number] = realloc((*special_symbols)[special_symbol_number], strlen(args[i]) + 1);
            strcpy((*special_symbols)[special_symbol_number], args[i]);
	    special_symbol_number += 1;
	                

            (*commands)[command_number] = realloc((*commands)[command_number], (word_number + 1) * sizeof(char **));
            (*commands)[command_number][word_number] = NULL;
            command_number += 1;
            (*commands) = realloc(*commands, (command_number + 1) * sizeof(char***));
            (*commands)[command_number] = NULL;
            word_number = 0;
        }
        else {
            (*commands)[command_number] = realloc((*commands)[command_number], (word_number + 1) * sizeof(char **));
            (*commands)[command_number][word_number] = NULL;
            (*commands)[command_number][word_number] = realloc((*commands)[command_number][word_number], strlen(args[i]) + 1);
            strcpy((*commands)[command_number][word_number], args[i]);
            word_number += 1;
        }
        i++;
    }
    (*commands)[command_number] = realloc((*commands)[command_number], (word_number + 1) * sizeof(char **));
    (*commands)[command_number][word_number] = NULL;
    command_number += 1;
    if (special_symbol_number != 0) {
        (*special_symbols) = realloc(*special_symbols, (special_symbol_number + 1) * sizeof(char **));
        (*special_symbols)[special_symbol_number] = NULL;
    }
    (*commands) = realloc(*commands, (command_number + 1) * sizeof(char **));
    (*commands)[command_number] = NULL;

    return command_number;
}

int special_symbol(char *str) {
    if (strcmp(str, ">>") == 0 || strcmp(str, ">") == 0 || strcmp(str, "<") == 0 || strcmp(str, "|") == 0) {
        return 1;
    }

    return 0;
}

int read_command(char ***args) {
    int arg_size = 0, current_len = 0, arg_number = 0;
    char c[2];
    c[0] = 0;
    c[1] = '\0';
    int quote_flag = 0;
    int special_symbol_flag = 0, special_symbol_length = 0;
    while ((c[0]) != '\n') {
        if (special_symbol_flag == 0) {
            c[0] = getchar();
        }
        special_symbol_flag = 0;
        if (isspace(c[0]))
            continue;
        if (c[0] == EOF) {
            return -2;
	}

        (*args) = realloc(*args, (arg_number + 1) * sizeof(char ***));//memory allocation for new argument
        (*args)[arg_number] = NULL;
        while ((special_symbol(c) == 0 && !isspace(c[0])) || quote_flag) {  //single word reading
            if (c[0] == '"') { //quotes
                quote_flag = (quote_flag == 0 ? 1 : 0);
                c[0] = getchar();
                continue;
            }

            if (current_len == arg_size) { //memory allocation for particular argument
                arg_size = arg_size * 2 + 1;
                (*args)[arg_number] = realloc((*args)[arg_number], arg_size * sizeof(char *));
            }
            if (((*args)[arg_number]) == NULL) {
                return -1;
            }
            ((*args)[arg_number])[current_len] = c[0];
            current_len += 1;
            c[0] = getchar();
        }
	if (current_len != 0) {
            if (current_len == arg_size)
                (*args)[arg_number] = realloc((*args)[arg_number], arg_size + 1);
            ((*args)[arg_number])[current_len] = '\0';
	}

        while (special_symbol(c) == 1) {
            special_symbol_flag = 1;
            if (special_symbol_length == 0 && current_len != 0) {
		arg_number += 1;
                (*args) = realloc(*args, (arg_number + 1) * sizeof(char **));
                (*args)[arg_number] = NULL;
            }
            (*args)[arg_number] = realloc((*args)[arg_number], (2 + special_symbol_length) * sizeof(char *));
            (*args)[arg_number][special_symbol_length] = c[0];
            (*args)[arg_number][special_symbol_length + 1] = '\0';
            special_symbol_length += 1;
            c[0] = getchar();
        }
	special_symbol_length = 0;
       // if (special_symbol_flag)
         //   arg_number += 1;

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
