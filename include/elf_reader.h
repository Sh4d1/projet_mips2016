/**
 * @file elf_reader.h
 *
 * @brief Fonctions de lecture de fichiers relogeables/executables au format ELF
 *
 * Ensimag 2016, première année.
 *
 * @section Introduction
 * Ce module fournit un ensemble de fonctions permettant d'extraire facilement
 * les données contenues dans un fichier relogeable au format ELF.
 * Il permet d'accéder:
 * -# aux sections d'instructions et de données: \em .text, \em .data
 *    et \em .bss
 * -# à la table des symboles
 * -# à la table des chaînes (noms des symboles)
 * -# aux sections de relocation \em .rel.text et \em .rel.data
 * -# au type de fichier: relogeable ou executable
 * -# au point d'entree du programme, le cas echeant
 *
 * Le principe d'utilisation est d'appeller les fonctions séquentiellement,
 * selon le shéma suivant:
 * -# read_elf: ouvre le fichier en lecture et retourne un descripteur elf
 * (de format privé) contenant toutes les informations lues
 * -# get_XXX: recupère une information, par exemple \em get_string_table
 * retourne la table de chaînes du descripteur elf
 * -# ...
 * -# close_elf: libere le descripteur elf et toute la mémoire allouée
 *
 * Il n'est pas possible d'ouvrir plusieurs fichiers ELF simultanement. Toutes
 * les fonctions get nécessitent qu'un fichier ELF ait été ouvert au préalable.
 *
 * Les données extraites suivent les formats du standard elf:
 * -# uchar8_t pour les octets des sections \em .text et \em .data
 * -# Elf32_Rel pour les entrées de relocation
 * -# Elf32_Sym pour les symboles
 * -# char * pour la table des chaînes de caractères.
 *
 * Les espaces mémoires alloués dans read_elf, dont les adresses sont récupérées
 * par les fonctions get, sont tous libérés à l'appel de close_elf.
 * A vous de les utiliser en conséquence dans votre programme.
 *
 * @note Certaines informations ELF ne sont pas accessibles via ce module
 * (entête de fichier, table des entêtes de section, ...)
 * Les fonctions fournies ici sont normalement suffisantes pour votre projet.
 *
 * @note Le code source elf_reader.c n'est PAS distribué!
 *
 * @author Matthieu.Chabanas@grenoble-inp.fr
 * @version 2.0
 * @date printemps 2016
 */

#ifndef _ELF_READER_
#define _ELF_READER_

#include <elf.h>
#include <stdbool.h>


/**
 * @struct elf_descr
 * @brief Descripteur interne d'un fichier ELF.
 *
 * Cette structure est privée, vous ne pouvez pas accéder à son contenu.
 * Elle ne peut qu'être utilisée de la manière suivante:
 * \verbatim
    struct elf_descr *elf = read_elf(nom_de_fichier);
    // tester que elf n'est pas NULL (erreur)
    // appels (get):
    ...
    get_string_table(elf, ...);
    ...
    close_elf(elf);
 \endverbatim
 */
struct elf_descr;

/**
 * @brief Lit un fichier au format ELF et stocke toute ses informations
 * dans un descripteur ELF (de format privé).
 *
 * Seuls les fichiers vérifiant les propriétés suivantes peuvent être lus:
 * - il s'agit bien d'un fichier ELF
 * - le type de machine doit etre MIPS, 32 bits, big endian
 * - le type du fichier doit etre ET_REL (objet relogeable)
 * ou ET_EXEC (executable)
 * Ses erreurs entrainent un arrêt de l'exécution du programme.
 *
 * @param filename le nom du fichier à ouvrir.
 * @return l'adresse du descripteur contenant les informations lues,
 * ou NULL en cas d'erreur.
 */
extern struct elf_descr *read_elf(const char *filename);


/**
 * @brief Ferme un descripteur ELF.
 *
 * Tous les espaces mémoire des données (sections, ...) sont libérés.
 *
 * @param elf le descripteur ELF à fermer.
 */
extern void close_elf(struct elf_descr *elf);



/**
 * @brief Lit la section d'instructions .text depuis un descripteur ELF valide.
 *
 * Le tableau *data contient le codage binaire de la section d'intructions,
 * de taille *size. Cet espace mémoire sera libéré lors de l'appel à \ref close_elf.
 *
 * L'adresse *addr est celle du premier octet de la section: 0 pour un fichier
 * relogeable, l'adresse attribué par l'éditeur de liens pour un executable.
 * L'adresse *addr est obligatoirement un multiple de la contrainte d'alignement
 * align, elle-même une puissance de 2. Pour MIPS 32 bits, l'alignement de la
 * section d'instructions est toujours de 4 octets.
 *
 * @param[in] elf le descripteur de fichier ELF
 * @param[out] data (adresse du) tableau des octets de la section, éventuellement NULL.
 * @param[out] size (adresse du) nombre d'octets de la section, éventuellement 0.
 * @param[out] address (adresse de) l'adresse d'implantation de la section, 0 si le fichier est relogeable.
 * @param[out] align (adresse de) la contrainte d'aligment de la section, 4 pour un MIPS 32 bits.
 * @return true si le descripteur de fichier ELF est valide, false sinon.
 */
extern bool get_text_section(const struct elf_descr *elf, uint8_t **data,
                             size_t *size, uint32_t *addr, uint8_t *align);


/**
 * @brief Lit la section de données .data depuis un descripteur ELF valide.
 *
 * Le tableau *data contient les données initialisées du programme,
 * de taille *size. Cet espace mémoire sera libéré lors de l'appel à \ref close_elf.
 *
 * L'adresse *addr est celle du premier octet de la section: 0 pour un fichier
 * relogeable, l'adresse attribué par l'éditeur de liens pour un executable.
 * L'adresse *addr est obligatoirement un multiple de la contrainte d'alignement
 * .align, elle-même une puissance de 2. Pour MIPS 32 bits, l'alignement de la
 * section de données est généralement 1, 2 ou 4 selon les directives utilisées.
 *
 * @param[in] elf le descripteur de fichier ELF
 * @param[out] data (adresse du) tableau des octets de la section, éventuellement NULL.
 * @param[out] size (adresse du) nombre d'octets de la section, éventuellement 0.
 * @param[out] address (adresse de) l'adresse d'implantation de la section, 0 si le fichier est relogeable.
 * @param[out] align (adresse de) la contrainte d'aligment de la section.
 * @return true si le descripteur de fichier ELF est valide, false sinon.
 */
extern bool get_data_section(const struct elf_descr *elf, uint8_t **data,
                             size_t *size, uint32_t *addr, uint8_t *align);


/**
 * @brief Lit les informations sur la section .bss depuis un descripteur ELF valide.
 *
 * Seule la taille et l'adresse de cette section sont retournées,
 * puisqu'elle n'a pas de contenu (octets tous nuls) dans le fichier ELF.
 *
 * L'adresse *addr est celle du premier octet de la section: 0 pour un fichier
 * relogeable, l'adresse attribué par l'éditeur de liens pour un executable.
 * L'adresse *addr est obligatoirement un multiple de la contrainte d'alignement
 * .align, elle-même une puissance de 2. Pour MIPS 32 bits, l'alignement de la
 * section .bss est généralement 1, 2 ou 4 selon les directives utilisées.
 *
 * @param[in] elf le descripteur de fichier ELF
 * @param[out] size (adresse du) nombre d'octets de la section .bss, éventuellement 0
 * @param[out] address (adresse de) l'adresse d'inplantation de la section, 0 si le fichier est relogeable.
 * @param[out] align (adresse de) la contrainte d'aligment de la section.
 * @return true si le descripteur de fichier ELF est valide, false sinon.
 */
extern bool get_bss_section(const struct elf_descr *elf,
                            size_t *size, uint32_t *addr, uint8_t *align);


/**
 * @brief Lit la section de relocation .rel.text depuis un descripteur ELF valide.
 *
 * En sortie, *data contient l'adresse de la table de relocation,
 * de taille *size, associée à la section d'instructions.
 * Cet espace mémoire sera libéré lors de l'appel à \ref close_elf.
 *
 * @param[in] elf le descripteur de fichier ELF
 * @param[out] data (adresse de) la table des entrées de relocation, éventuellement null.
 * @param[out] size (adresse du) nombre d'entrées de la section, éventuellement 0
 * @return true si le descripteur de fichier ELF est valide, false sinon.
 */
extern bool get_rel_text_section(const struct elf_descr *elf,
                                 Elf32_Rel **data, size_t *size);


/**
 * @brief Lit la section de relocation .rel.data depuis un descripteur ELF valide.
 *
 * En sortie, *data contient l'adresse de la table de relocation,
 * de taille *size, associée à la section des données initialisées.
 * Cet espace mémoire sera libéré lors de l'appel à \ref close_elf.
 *
 * @param[in] elf le descripteur de fichier ELF
 * @param[out] data (adresse de) la table des entrées de relocation, éventuellement null.
 * @param[out] size (adresse du) nombre d'entrées de la section, éventuellement 0
 * @return true si le descripteur de fichier ELF est valide, false sinon.
 */
extern bool get_rel_data_section(const struct elf_descr *elf,
                                 Elf32_Rel **data, size_t *size);


/**
 * @brief Lit la table des chaines depuis un descripteur ELF valide.
 *
 * La table des chaines est un tableau de caractères contenant une
 * concaténation de tous noms de sections et de symboles du fichier ELF,
 * séparés par des caractères nuls ('\\0').
 *
 * Une fois stockées dans cette table, les autres tables du fichier ELF
 * pourront faire référence aux chaines par leur indice dans la table
 * (cette astuce permet d'avoir une structure de donnée de taille fixe
 * pour décrire une entrée de relocation ou un symbole dans la table
 * des symboles, puisque l'indice dans la table des chaines est de
 * taille fixe, alors que la chaine est de taille inconnue à
 * l'avance).
 *
 * Par exemple, si un fichier ELF ne contient qu'une section .text et
 * un symbole "toto", la table des chaines sera constituée de:
 * \verbatim
 +---+---+---+---+---+---+---+---+---+---+---+---+
 |\0 | . | t | e | x | t |\0 | t | o | t | o |\0 |
 +---+---+---+---+---+---+---+---+---+---+---+---+
 \endverbatim
 * Le premier caractere est une sentinelle, toujours égale à '\0'.
 * La chaine ".text" (un nom de section est aussi un symbole!) aura
 * l'indice 1, et la chaine "toto" l'indice 7.
 * La taille totale de la table est 12 octets.
 *
 * En sortie, *str_tab contient l'adresse de la table des chaînes,
 * de taille *size.
 * Cet espace mémoire sera libéré lors de l'appel à \ref close_elf.
 *
 * @param[in] elf le descripteur de fichier ELF
 * @param[out] strtab (adresse du) tableau des chaines de caracteres, éventuellement NULL
 * @param[out] size (adresse du) nombre total d'octets de la table des chaines, éventuellement 0
 * @return true si le descripteur de fichier ELF est valide, false sinon.
 */
extern bool get_string_table(const struct elf_descr *elf,
                             char **strtab, size_t *size);


/**
 * @brief Lit la table des symboles depuis un descripteur ELF valide.
 *
 * La table des symboles décrit tous les symboles (locaux, globaux;
 * définis, indéfinis; ...) du fichier ELF. Chaque symbole est défini
 * par une structure Elf32_Sym.
 * La table contient au minimum le symbole "indéfini", puis les symboles
 * correspondant aux sections, et enfin les autres.
 *
 * En sortie, *symtab contient l'adresse de la table des symboles,
 * de taille *size.
 * Cet espace mémoire sera libéré lors de l'appel à \ref close_elf.
 *
 * @param[in] elf le descripteur de fichier ELF
 * @param[out] sym_tab (adresse du) tableau des symboles, éventuellement NULL
 * @param[out] size (adresse du) nombre de symboles dans la table, éventuellement 0
 * @return true si le descripteur de fichier ELF est valide, false sinon.
 */
extern bool get_symbol_table(const struct elf_descr *elf,
                             Elf32_Sym **symtab, size_t *size);


/**
 * @brief Lit le type de fichier ELF depuis un descripteur ELF valide.
 *
 * Les types autorisés (erreur dans \ref read_elf sinon) sont:
 * - ET_REL (valeur 1), pour un fichier objet relogeable
 * - ET_EXEC (valeur 2), pour un fichier exécutable
 * En cas d'erreur, si le descripteur n'est pas valide,
 * la valeur retounée est ET_NONE (0).
 *
 * @param[in] elf le descripteur de fichier ELF
 * @return le type de fichier du descripteur de fichier ELF,
 * ET_REL ou ET_EXEC, ou bien ET_NONE en cas d'erreur.
 */
extern uint16_t get_elf_type(const struct elf_descr *elf);


/**
 * @brief Lit le point d'entrée du fichier ELF depuis un descripteur ELF valide.
 *
 * Si le fichier est de type exécutable, le point d'entrée est l'adresse
 * (absolue) de la première instruction du programme à exécuter.
 * S'il est relogeable, le point d'entrée est 0.
 *
 * @remark La valeur en cas d'erreur est ici 0, en fait une erreur
 * sur le descripteur elf sera levée avant.
 *
 * @param[in] elf le descripteur de fichier ELF
 * @param entry (adresse du) point d'entrée du programme.
 * @return l'adresse du point d'entreé du programme, 0 en cas d'erreur.
 */
extern uint32_t get_entry_point(const struct elf_descr *elf);


#endif	 /* #define _ELF_READER_ */
