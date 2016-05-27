/*  @file shell.c

    @brief Implémentation de shell.h
    @author Patrik Cyvoct
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "shell.h"

int main(int argc, char **argv)
{

    shell_loop(); // call the shell loop

    return EXIT_SUCCESS; // everything went well
}

void shell_loop(void)
{
    char *line;
    char **args;

    int8_t status;

    do {
        printf("simips@test > ");
        line = shell_read_line();
        args = shell_split_line(line);
        status = shell_exec(args);

        free(line);
        free(args);
    } while (status);

}

#define SHELL_RL_BUFSIZE 1024
char *shell_read_line(void)
{

    /* may work --
    char *line = NULL;
    ssize_t bufsize = 0; // have getline allocate a buffer for us
    getline(&line, &bufsize, stdin);
    return line;
    */

    int32_t bufsize = SHELL_RL_BUFSIZE;
    int32_t position = 0;
    char *buffer = malloc(sizeof(char)*bufsize);
    int32_t c;

    if (!buffer) {
        fprintf(stderr, "Shell : allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Lecture d'un caractère
        c = getchar();

        // si EOF, on remplace par le symbole de fin de chaine (ou de ligne)
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        if (position >= bufsize) {
            bufsize += SHELL_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "Shell : allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

#define SHELL_TOK_BUFSIZE 64
#define SHELL_TOK_DELIM " \t\r\n\a"
char **shell_split_line(char *line)
{
    int32_t bufsize = SHELL_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "Shell : allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, SHELL_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += SHELL_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "Shell : allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, SHELL_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int shell_num_func()
{
    return sizeof(func_str) / sizeof(char *);
}


int shell_help(char **args)
{
    uint8_t i;
    printf("Simips\n");
    for (i = 0; i < shell_num_func(); i++) {
        printf(" %s\n", func_str[i]);
    }

    return 1;
}

int shell_exit(char **args)
{
    return 0;
}

int shell_exec(char ** args)
{
    uint8_t i;

    if (args[0] == NULL) {
        // empty command
        return 1;
    }

    for (i=0; i < shell_num_func(); i++) {
        if (strcmp(args[0], func_str[i]) == 0) {
            return (*func[i])(args);
        }
    }
    return 1;
}
