/*  @file shell.c

    @brief Implémentation de shell.h
    @author Patrik Cyvoct
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "../include/shell.h"
#include "../include/mem.h"
#include "../include/gpr.h"
#include "../include/instructions.h"

// int main(int argc, char **argv)
// {
//
//     shell_loop(); // call the shell loop
//
//     return EXIT_SUCCESS; // everything went well
// }

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

int shell_load(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "Pas assez d'arguments pour load\n");
        return 1;
    } else {
        file_to_memory(args[1]);
    }
    return 1;
}

int shell_dreg(char **args)
{
    if (args[1] == NULL) {
        print_gpr();
    } else {
        uint32_t i = 1;
        while (args[i] != NULL) {
            print_a_gpr(args[i]);
            i++;
        }
    }
    return 1;
}

int shell_run(char **args)
{
    if (args[1] != NULL) {
        run(get_address_from_string(args[1]));
    } else {
        run(get_PC_value());
    }

    return 1;
}

int shell_dasm(char **args)
{
    if (args[1] == NULL) {
        dasm_line(1);
    } else if (strcmp("all", args[1]) == 0) {
        dasm();
    } else if (isNumeric(args[1])) {
        dasm_line(strtol(args[1], NULL, 10));
    }


    return 1;
}

int shell_sreg(char **args)
{
    uint32_t value;
    if (strncmp("0x", args[2], 2) == 0) {
        value = strtoul(args[2], NULL, 16);
    } else {
        value = strtol(args[2], NULL, 10);
    }
    if (isNumeric(args[1]) == 1) {
        set_register_value(atoi(args[1]), value);
    } else {
        set_register_value_by_name(args[1], value);
    }
    return 1;
}


int shell_dmem(char **args)
{
    if (args[1] != NULL && args[2] == NULL) {
        display_memory(get_address_from_string(args[1]));
    } else if (args[1] != NULL && args[2] != NULL) {
        diplay_memory_between(get_address_from_string(args[1]), get_address_from_string(args[2]));
    } else {
        printf("Il manque un argument.\n");
    }
    return 1;
}

int shell_smem(char **args)
{
    if (args[2] != NULL) {
        uint32_t value;
        if (strncmp("0x", args[2], 2) == 0) {
            args[2]+=2;
            value = strtoul(args[2], NULL, 16);
            printf("%x\n", value);
        } else {
            value = strtol(args[2], NULL, 10);
        }
        uint32_t nbOctets;
        if (args[3] == NULL) {
            nbOctets = 1;
        }
        else {
            nbOctets = strtol(args[3], NULL, 10);
        }
        switch (nbOctets) {
            case 1:
                if (is_byte(value)) {
                    set_byte(get_address_from_string(args[1]), value);
                } else {
                    fprintf(stderr, "La valeur est trop grande\
pour rentrer sur un octet\n");
                }
                break;
            case 2:
                if (is_half_word(value)) {
                    set_half_word(get_address_from_string(args[1]), value);
                } else {
                    fprintf(stderr, "La valeur est trop grande\
pour rentrer sur un demi-mot\n");
                }
                break;
            case 4:
                set_word(get_address_from_string(args[1]), value);
                break;
            default:
                fprintf(stderr, "Le nombre d'octets à écrire est soit 1, 2 ou 4\n");
        }
    } else {
        fprintf(stderr, "Il manque des arguments\n");
    }
    return 1;
}

int shell_step(char **args)
{
    run_line();
    return 1;
}

int shell_stepi(char **args)
{
    run_line();
    return 1;
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
