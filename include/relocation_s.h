/**
 * @brief Module de relocation des données extraites d'un fichier ELF.
 *
 * @section Introduction
 * Les données contenues dans un fichier ELF peuvent être accompagnées
 * d'informations permettant de reloger les symboles et les sections
 * d'instructions (.text) et de données (.data, .bss) à des adresses mémoires
 * spécifiques.
 *
 * Ce module fournit un ensemble de fonctions permettant:
 * -# de récuperer les symboles dans un format "clair" (nom, adresse)
 * -# d'extraire les symboles (absolus) d'un fichier ELF executable
 * -# de reloger les symboles et les sections d'un fichier ELF relogeable
 *
 * Les données en entrée de ces fonctions sont les informations brutes issues
 * du fichier ELF, lues à l'aide du module \em elf_reader.
 */


#ifndef _RELOCATION_
#define _RELOCATION_ 1
#include <string.h>

#include "elf_reader.h"


/**
 * @brief Structure représentant un symbole dans un format "clair".
 */
struct symbole {
    char* nom;          /**< p'tit nom */
    uint32_t adresse;   /**< adresse memoire */
};


/**
 * @brief Traduit une table des symboles au format ELF en format "clair"
 *
 * Construit une table de \ref Symbole à partir de la table de symboles
 * et de la table des chaînes d'un fichier ELF.
 * Les noms des symboles sont lus dans la table des chaînes, leurs adresses
 * sont simplement celles définies dans \em symtab.st_value.
 * Si symtab est issue d'un fichier exécutable ces adresses sont absolues,
 * sinon elles sont relatives (avant relocation donc).
 *
 * Le nombre et l'ordre des symboles sont identiques dans les deux formats de
 * table.
 *
 * @param[in] symtab table des symboles au format ELF
 * @param[in] symtab_size nombre de symboles de la table ELF
 * @param[in] strtab table des chaînes au format ELF
 * @param[out] table_symboles (adresse de la) table des symboles format "clair"
 * @param[out] nb_symboles (adresse du) nombre de symboles, identique à symtab_size
 */
extern void traduit_table_symboles(const Elf32_Sym *symtab, size_t symtab_size,const char *strtab,struct symbole **table_symboles,size_t *nb_symboles);


/**
 * @brief Reloge les symboles d'une table ELF.
 *
 * Les adresses des symboles sont relogées à partir des adresses de section
 * définies en paramètres. Puis, une table de symboles identique à celle
 * générée par \ref traduit_table_symboles est construite.
 *
 * La validité des adresses (cohérence, non recouvrement, respect des
 * contraintes d'alignement, etc.) n'est \em pas vérifiée. Il revient à
 * l'utilisateur de le faire, à partir des contraintes du fichier ELF
 * et de son simulateur.
 *
 * @param[in] symtab table des symboles au format ELF
 * @param[in] symtabSize nombre de symboles de la table ELF
 * @param[in] strtab table des chaînes au format ELF
 * @param[in] addr_text adresse absolue de la section .text
 * @param[in] addr_data adresse absolue de la section .data
 * @param[in] addr_bss adresse absolue de la section .bss
 * @param[out] table_symboles (adresse de la) table des symboles format "clair"
 * @param[out] nb_symboles (adresse du) nombre de symboles, identique à symtab_size
 */
extern void reloge_symboles(const Elf32_Sym *symtab, size_t symtab_size,
                            const char *strtab,
                            uint32_t addr_text, uint32_t addr_data, uint32_t addr_bss,
                            struct symbole **table_symboles, size_t *nb_symboles);


/**
 * @brief Reloge une section d'instruction ou de données.
 *
 * Reloge une section à partir de ses données initiales, de sa table
 * de relocation associée, et d'une table de symboles déjà relogée.
 *
 * L'adresse addr doit naturellement correspondre à celle ayant
 * été utilisé pour reloger les symboles avec la fonction \ref reloge_symboles.
 *
 * @param[in] addr l'adresse où reloger la section
 * @param[in,out] data les octets de la section, avant puis après relogement
 * @param[in] rel_table la table de relocation associée à la section
 * @param[in] rel_size le nombre d'entrées dans la table de relocation
 * @param[in] table_symboles la table des symboles relogés
 */
extern void reloge_section(uint32_t addr, uint8_t *data,
                           const Elf32_Rel *rel_table, size_t rel_size,
                           const struct symbole *table_symboles);

#endif
