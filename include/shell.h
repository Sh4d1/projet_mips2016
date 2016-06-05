/*  @file shell.h

    @brief Fonctions du shell

    @author Patrik Cyvoct
*/

#ifndef _SHELL_
#define _SHELL_

/* codes et messages d'erreur */
enum {QUIT, OK, KO, UNKNOWN_FUNCTION, MISSING_ARGS, NO_FRAMEBUFFER};
char *err_msgs[] = {
    "Au revoir.",
    "Commande réalisée avec succès.",
    "Erreur.",
    "Fonction inconnue.",
    "Argument(s) manquant(s).",
    "Screenshot impossible, le framebuffer n'est pas activé."
};

/* liste des fonctions du shell */
static char *func_str[] = {
    "help",
    "exit",
    "load",
    "dreg",
    "run",
    "dasm",
    "sreg",
    "dmem",
    "smem",
    "step",
    "stepi",
    "sshot"
};

/* declarations des fonctions du shell */
int shell_help(char **args);
int shell_exit(char **args);
int shell_load(char **args);
int shell_dreg(char **args);
int shell_run(char **args);
int shell_dasm(char **args);
int shell_sreg(char **args);
int shell_dmem(char **args);
int shell_smem(char **args);
int shell_step(char **args);
int shell_stepi(char **args);
int shell_sshot();

/* tableau de pointeurs sur les differentes fonctions */
static int (*func_ptr[]) (char **) = {
    shell_help,
    shell_exit,
    shell_load,
    shell_dreg,
    shell_run,
    shell_dasm,
    shell_sreg,
    shell_dmem,
    shell_smem,
    shell_step,
    shell_stepi,
    shell_sshot
};

/* shell_num_func : retourne le nombre de fonctions */
int shell_num_func();

/* shell_loop : launch the shell lopp */
void shell_loop(void);

/* shell_read_line : read a line of the shell */
char *shell_read_line(void);

/* shell_split_line : split the line */
char **shell_split_line(char *line);

/* shell_exec : execute a command */
int shell_exec(char ** args);

#endif
