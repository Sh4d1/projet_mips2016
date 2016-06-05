/*  @file shell.c

    @brief Implémentation de shell.h
    @author Patrik Cyvoct
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include "../include/shell.h"
#include "../include/mem.h"
#include "../include/gpr.h"
#include "../include/instructions.h"
#include "../include/framebuffer.h"

#define UNUSED(x) (void)(x)

void shell_loop(void)
{
    char *line;
    char **args;

    uint8_t status = 0;

    do {
        printf("simips@test > ");
        line = shell_read_line();
        args = shell_split_line(line);
        status = shell_exec(args);
        printf("%s\n", err_msgs[status]);
    } while (status != QUIT);
    free(line);
    free(args);
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
            // on ne rentre jamais dans ce if ?
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
    UNUSED(args);

    printf("Commandes Simips\n");
    for (uint8_t i = 0; i < shell_num_func(); i++) {
        printf(" %s\n", func_str[i]);
    }
    return OK;
}

int shell_exit(char **args)
{
    UNUSED(args);

    return QUIT;
}

int shell_load(char **args)
{
    if (!args[1]) return MISSING_ARGS;

    file_to_memory(args[1]);
    return OK;
}

int shell_dreg(char **args)
{
    if (!args[1]) {
        print_gpr();
    } else {
        uint32_t i = 0;
        while (args[++i]) {
            print_a_gpr(args[i]);
        }
    }
    return OK;
}

int shell_run(char **args)
{
    if (args[1]) {
        run(get_value_from_string(args[1]));
    } else {
        run(get_PC_value());
    }
    return OK;
}

int shell_dasm(char **args)
{
    if (!args[1]) {
        dasm_line(1);
    } else if (!strcmp("all", args[1])) {
        dasm();
    } else {
        dasm_line(get_value_from_string(args[1]));
    }
    return OK;
}

int shell_sreg(char **args)
{
    if (!args[2]) return MISSING_ARGS;

    printf("set_register_value_by_name(%s, get_value_from_string(%s));\n", args[1], args[2]);
    set_register_value_by_name(args[1], get_value_from_string(args[2]));
    return OK;
}

int shell_dmem(char **args)
{
    if (!args[1]) return MISSING_ARGS;

    if (args[2]) {
        diplay_memory_between(get_value_from_string(args[1]), get_value_from_string(args[2]));
    } else {
        display_memory(get_value_from_string(args[1]));
    }
    return OK;
}

int shell_smem(char **args)
{
    if (!args[2]) return MISSING_ARGS;

    uint32_t value = get_value_from_string(args[2]);
    uint32_t nbOctets = (args[3]) ? get_value_from_string(args[3]) : 1;
    switch (nbOctets) {
    case 1:
        if (is_byte(value)) {
            set_byte(get_value_from_string(args[1]), value);
        } else {
            fprintf(stderr, "La valeur est trop grande\
pour rentrer sur un octet\n");
        }
        break;
    case 2:
        if (is_half_word(value)) {
            set_half_word(get_value_from_string(args[1]), value);
        } else {
            fprintf(stderr, "La valeur est trop grande\
pour rentrer sur un demi-mot\n");
        }
        break;
    case 4:
        set_word(get_value_from_string(args[1]), value);
        break;
    default:
        fprintf(stderr, "Le nombre d'octets à écrire est soit 1, 2 ou 4\n");
    }
    return OK;
}

int shell_step(char **args)
{
    UNUSED(args);

    run_line();
    return OK;
}

int shell_stepi(char **args)
{
    UNUSED(args);

    run_line();
    return OK;
}

int shell_sshot()
{
    if (/* condition */) return NO_FRAMEBUFFER;

    // creation du nom de fichier en fonction de la date
    char filename[35];
    time_t t = time(NULL);
    struct tm *tmp = localtime(&t);
    strftime(filename, 35, "screenshots/screenshot_%Y-%m-%d_%H:%M:%S.ppm", tmp);

    // ouverture du fichier et ecriture de l'entete
    FILE *file = fopen(filename, "wb");
    fprintf(file, "P6\n%d %d\n255\n", FRAMEBUFFER_W, FRAMEBUFFER_H);

    // ecriture de chaque pixel du framebuffer
    unsigned char color[3];
    for (uint32_t i = 0; i < FRAMEBUFFER_W * FRAMEBUFFER_H; i++) {
        color[0] = get_byte(0xFFFF0600 + i);
        color[1] = get_byte(0xFFFF0600 + i);
        color[2] = get_byte(0xFFFF0600 + i);
        fwrite(color, 0, 2, file);
    }

    // fermeture du fichier
    fclose(file);
    return OK;
}

int shell_exec(char ** args)
{
    // si ligne vide : ne rien faire
    if (!args[0]) return UNKNOWN_FUNCTION;

    for (uint8_t i = 0; i < shell_num_func(); i++) {
        if (!strcmp(args[0], func_str[i])) return (*func_ptr[i])(args);
    }
    return UNKNOWN_FUNCTION;
}
