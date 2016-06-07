/*  @file shell.h

    @brief Fonctions du shell

    @author Patrik Cyvoct
*/

#ifndef _SHELL_
#define _SHELL_

/* codes et messages d'erreur */
enum {QUIT, OK, KO, EMPTY_LINE, UNKNOWN_FUNCTION, MISSING_ARGS, NO_FRAMEBUFFER};
static char *err_msgs[] = {
    "Au revoir.",
    "Commande réalisée avec succès.",
    "Erreur.",
    "Ligne vide",
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
    "sshot",
    "addbp",
    "rmbp",
    "dbp"
};

/* descriptions des fonctions */
static char *func_desc_str[] = {
    "Affiche l'aide",
    "Quitte le simulateur",
    "charge un fichier (load filename)",
    "affiche un ou tout les registres",
    "exécute le programme",
    "affiche le code désassemblé",
    "modifie un registre",
    "affiche la mémoire",
    "modifie la mémoire",
    "step",
    "stepi",
    "sshot",
    "addbp",
    "rmbp",
    "dbp"
};

/* aide pour les fonctions */
static char *func_help_str[] = {
    "Syntaxe : help [<command>]\nAffiche l'aide d'une commande et sa syntaxe.",
    "Syntaxe : exit\nQuitte le simulateur.",
    "Syntaxe : load <filename>\nLes données lues dans le fichier filename sont placées en mémoire.",
    "Syntaxe : dreg [<registerName>]\nAffiche en héxadécimale la valeur du registre. Si il est omis tout les registres seront affichés.",
    "Syntaxe : run [<adresse>]\nCharge PC avec l'adresse fournie et lance le microprocesseur. Si l'adresse est omise, il se lance à l'adresse courante de PC.",
    "Syntaxe : dasm [n | all]\nSans option : désassemblage de l'instruction courante.\nParamètre entier n : désassemblage de n instuctions suivantes à partir de l'instruction courante.\nParamètre all : Désassemblage de tout le programme.",
    "modifie un registre",
    "affiche la mémoire",
    "modifie la mémoire",
    "step",
    "stepi",
    "sshot",
    "addbp",
    "rmbp",
    "dbp"
};

/* declarations des fonctions du shell */
int shell_help(char **args);
int shell_exit();
int shell_load(char **args);
int shell_dreg(char **args);
int shell_run(char **args);
int shell_dasm(char **args);
int shell_sreg(char **args);
int shell_dmem(char **args);
int shell_smem(char **args);
int shell_step();
int shell_stepi();
int shell_sshot();
int shell_addbp(char **args);
int shell_rmbp(char **args);
int shell_dbp();

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
    shell_sshot,
    shell_addbp,
    shell_rmbp,
    shell_dbp
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

/* retourne la valeur d'une string ecrite au format decimal ou hexa */
uint32_t get_value_from_string(char *string);

#endif
