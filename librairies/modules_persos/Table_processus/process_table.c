#ifndef __PROCESS_TABLE_H
#define __PROCESS_TABLE_H

#include <stdio.h>        /* E/S comme printf */
#include <stdlib.h>       /* E/S comme printf */
#include <stdbool.h>      /* Booléens */
#include <string.h>       /* chaine en C */
#include "list_process.h" /* Module maison pour gérer la table des processus */
#include "process_table.h"

#define TAILLE_CHAINE 1000   /* Taille maximale d'une ligne de commande */
list_process *table_process; /* Liste chainée avec les processus lancés par minishell */

void initialize_process_table(char *argv[])
{

    /* Conversion de la ligne de commande qui a lancée minishell en chaine simple */
    int indice = 0;
    char chaine[200] = "";
    while (argv[indice] != NULL)
    {
        strcat(chaine, argv[indice]);
        strcat(chaine, " ");
        indice++;
    }

    /* Enregistrement de minishell qui est la première entrée du tableau */
    table_process = create(create_process(0, getpid(), 'R', chaine), true);
}

char *to_str(char **cmd_line)
{
    /* Préparation d'une chaine de 1000 caractères (valeur modifiable) */
    int indice = 0;
    char *chaine = malloc(TAILLE_CHAINE * sizeof(char));

    /* Concaténation du tableau en séparant chaque mot de la ligne de commande par un espace */
    while (cmd_line[indice] != NULL)
    {
        strcat(chaine, cmd_line[indice]);
        strcat(chaine, " ");
        indice++;
    }
    return chaine;
}

#endif