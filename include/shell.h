/**
 * @brief Module de gestion du shell.
 */
#ifndef _SHELL_
#define _SHELL_

/* codes et messages d'erreur */
enum {QUIT, OK, KO, EMPTY_LINE, UNKNOWN_COMMAND, MISSING_ARGS, NO_FRAMEBUFFER};
static char *err_msgs[] = {
    "Au revoir.",
    "Commande réalisée avec succès.",
    "Erreur.",
    "Ligne vide",
    "Commande inconnue.",
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
    "Quitte le simulateur et sa syntaxe",
    "Charge un programme",
    "Affiche les registres",
    "Exécuter à partir d'une adresse",
    "Affiche dans la console le code désassemblé",
    "Modifie une valeur dans un registre",
    "Affiche la mémoire",
    "Modifie la mémoire",
    "Exécution pas à pas (ligne à ligne)",
    "Exécution pas à pas (exactement)",
    "Effectue une copie d'écran",
    "Ajoute un point d'arrêt",
    "Supprime un point d'arrêt",
    "Affiche les points d'arrêt"
};

/* aide pour les fonctions */
static char *func_help_str[] = {
    "help [<command>]",
    "exit",
    "load <filename>\nfilename : nom du fichier objet à charger\nLes données lues dans le fichier filename sont placées en mémoire.\nLe fichier <filename> doit être au format ELF, exécutable ou relogeable sans symbole externe.",
    "dreg [<registerList>]\nregisterList : liste de registres\nAffiche en héxadécimale la valeur du/des registre(s). Si il est omis tous les registres seront affichés.",
    "run [<adresse>]\nCharge PC avec l'adresse fournie et lance le microprocesseur. Si l'adresse est omise, il se lance à l'adresse courante de PC.",
    "dasm [n | all]\nSans option : désassemblage de l'instruction courante.\nParamètre entier n : désassemblage de n instuctions suivantes à partir de l'instruction courante.\nParamètre all : Désassemblage de tout le programme.",
    "sreg <registerName> <value>\nregisterName : nom d'un registre\nvalue : valeur numérique valide, entrée au format hexadécimal (préfixe 0x) ou décimale\nÉcrit la valeur donnée dans le registre passé en paramètre.",
    "dmem <address> ou dmem <address1> <address2>\naddress : valeur hexadécimale représentant une adresse sur 32 bits\nAffiche le contenu de la mémoire entre address et address + 16 ou address1 et address 2",
    "smem <address> <value> [nbOctets]\naddress : valeur représentant une adresse mémoire valide\nvalue : valeur numérique, au format hexadécimale ou décimale\nnbOctets : paramètre optionnel indiquant le nombre d'octets à écrire en mémoire (1, 2 ou 4). S'il n'est pas présent, un seul octet est écrit.",
    "step\nProvoque l'exécution de l'instruction dont l'adresse est contenue dans le registre PC puis rend la main à l'utilisateur.",
    "stepi\nProvoque l'exécution de l'instruction dont l'adresse est contenue dans le registre PC puis rend la main à l'utilisateur. Si l'on rencontre un saut, seule l'instruction dans le delay slot puis le saut sont exécutés.",
    "sshot <fichier.ppm>\nfichier.ppm : nom du fichier utilisé pour sauvegarder la copie d'écran. La sauvegarde est réalisée au format ppm",
    "addbp <address>\naddress : valeur hexadécimale d'adresse\nAjoute un point d'arrêt à l'adresse fournie en paramètre. Lorsque le compteur ordinal PC sera égal à cette valeur, l'exécution sera interrompue et la main rendue à l'utilisateur.",
    "rmbp [<address>]\naddress : valeur hexadécimale d'adresse\nEnlève le point d'arrêt à l'adresse fournie en paramètre, s'il y en a un. Si le paramètre est omis, cette commande efface tous les points d'arrêt.",
    "dbp\nAffiche sur la console de visualisation l'adresse de tous les points d'arrêt positionnés dans la mémoire."
};

char **complete(const char *stem_text, int start, int end);
char *func_name_gen(const char *stem_text, int state);

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
