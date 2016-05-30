/*  @file shell.h

    @brief Fonctions du shell

    @author Patrik Cyvoct
*/

#ifndef _SHELL_
#define _SHELL_

/* declarations des fonctions du shell */
int shell_help(char **args);
int shell_exit(char **args);
int shell_load(char **args);
int shell_dreg(char **args);

/* shell_num_func : retourne le nombre de fonctions */
int shell_num_func();

/* liste des fonctions du shell, suivies de leur commandes */
static char *func_str[] = {
    "help",
    "exit",
    "load",
    "dreg"
};

static int (*func[]) (char **) = {
    &shell_help,
    &shell_exit,
    &shell_load,
    &shell_dreg
};

/* shell_loop : launch the shell lopp */
void shell_loop(void);

/* shell_read_line : read a line of the shell */
char *shell_read_line(void);

/* shell_split_line : split the line */
char **shell_split_line(char *line);

/* shell_exec : execute a command */
int shell_exec(char ** args);


#endif
