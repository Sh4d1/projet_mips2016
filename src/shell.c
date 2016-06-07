/*  @file shell.c

    @brief Implémentation de shell.h
    @author Patrik Cyvoct
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../include/shell.h"
#include "../include/mem.h"
#include "../include/gpr.h"
#include "../include/instructions.h"
#include "../include/framebuffer.h"

char **command_completion(const char *stem_text, int start, int end)
{
    char **matches = NULL;
    (void)(end);
    (void)(start);
    matches = rl_completion_matches(stem_text, func_name_gen);

    return matches;
}

char *func_name_gen(const char *stem_text, int state)
{
    static int count;
    if (state == 0) {
        count = -1;
    }

    int text_len = strlen(stem_text);

    while (count < shell_num_func()-1) {
        count++;
        if (strncmp(func_str[count], stem_text, text_len) == 0) {
            char *d = malloc (strlen(func_str[count]) + 1);
            if (d == NULL) return NULL;
            strcpy (d,func_str[count]);
            return d;
        }
    }
    return NULL;
}


void shell_loop(void)
{
#ifdef READLINE

    char* input;// shell_prompt[100];
    char **args;
    // Configure readline to auto-complete paths when the tab key is hit.
    rl_bind_key('\t', rl_complete);
    rl_attempted_completion_function = command_completion;

    printf("Bienvenue dans le simulateur MIPS32. N'hésitez pas à utiliser la commande help.\n");
    uint8_t status = 0;
    do {

        //snprintf(shell_prompt, sizeof(shell_prompt), "%s:%s $ ", getenv("USER"), getcwd(NULL, 1024));
        input = readline("simips > ");
        if (!input)
            break;
        if (strcmp(input, "") != 0) {
            add_history(input);
        }
        args = shell_split_line(input);
        status = shell_exec(args);
        if (status != OK && status != EMPTY_LINE) {
            printf("%s\n", err_msgs[status]);
        }


        free(input);
        free(args);
    } while (status != QUIT);
#else
    char *line;
    char **args;

    printf("Bienvenue dans le simulateur MIPS32. N'hésitez pas à utiliser la commande help.\n");
    uint8_t status = 0;
    do {
        printf("simips > ");
        line = shell_read_line();
        args = shell_split_line(line);
        status = shell_exec(args);
        if (status != OK) {
            printf("%s\n", err_msgs[status]);
        }
        free(line);
        free(args);
    } while (status != QUIT);
#endif


}

#define SHELL_RL_BUFSIZE 1024
char *shell_read_line(void)
{
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
                fprintf(stderr, "Shell : allocation error\n");
                exit(EXIT_FAILURE);
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
    if (!args[1]) {
        printf("Commandes Simips\nPour plus d'aide taper help <nom de la fonction>\n");
        for (uint8_t i = 0; i < shell_num_func(); i++) {
            printf(" %5s : %s\n", func_str[i], func_desc_str[i]);
        }
    } else {
        for (uint8_t i = 0; i < shell_num_func(); i++) {
            if (strcmp(args[1], func_str[i]) == 0) {
                printf("%s\n", func_help_str[i]);
            }
        }
    }

    return OK;
}

int shell_exit()
{
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
        dasm_text();
        dasm_data();
    } else {
        dasm_line(get_value_from_string(args[1]));
    }
    return OK;
}

int shell_sreg(char **args)
{
    if (!args[2]) return MISSING_ARGS;

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

int shell_step()
{
    run_line();
    return OK;
}

int shell_stepi()
{
    runi_line();
    return OK;
}

int shell_sshot(char **args)
{
    if (!get_framebuffer()) return NO_FRAMEBUFFER;

    char filename[50];
    if (!args[1]) {
        // creation du nom de fichier en fonction de la date
        time_t t = time(NULL);
        struct tm *tmp = localtime(&t);
        strftime(filename, 50, "screen_%Y-%m-%d_%H:%M:%S.ppm", tmp);
    } else {
        strcpy(filename, args[1]);
    }

    // ouverture du fichier et ecriture de l'entete
    FILE *file = fopen(filename, "wb");
    fprintf(file, "P5\n%u %u\n255\n", FRAMEBUFFER_W, FRAMEBUFFER_H);

    // ecriture de chaque pixel du framebuffer
    unsigned char pixel;
    for (uint32_t i = 0; i < FRAMEBUFFER_W * FRAMEBUFFER_H; i++) {
        pixel = get_byte(0xFFFF0600 + i);
        fwrite(&pixel, 1, 1, file);
    }

    // fermeture du fichier
    fclose(file);
    return OK;
}

int shell_addbp(char **args)
{
    add_bp(strtoul(args[1], NULL, 16));
    return OK;
}

int shell_rmbp(char **args)
{
    if (args[1]) {
        rm_bp(strtoul(args[1], NULL, 16));
    } else {
        free_bp();
    }

    return OK;

}

int shell_dbp()
{
    display_bp();
    return OK;

}

int shell_exec(char **args)
{
    // si ligne vide : ne rien faire
    if (!args[0]) return EMPTY_LINE;

    for (uint8_t i = 0; i < shell_num_func(); i++) {
        if (!strcmp(args[0], func_str[i])) return (*func_ptr[i])(args);
    }
    return UNKNOWN_FUNCTION;
}

uint32_t get_value_from_string(char *string)
{
    return strtoul(string, NULL, 0);
}
