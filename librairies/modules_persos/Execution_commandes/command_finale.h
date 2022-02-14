/* Exécution des commandes rentrées et gestion de la table des processus */

#ifndef __COMMAND_H
#define __COMMAND_H
#include <stdio.h>                           /* E/S comme printf */
#include <unistd.h>                          /* Primitives systèmes comme fork ! */
#include <stdbool.h>                         /* Booléens */
#include <stdlib.h>                          /* exit */
#include <sys/wait.h>                        /* wait */
#include <string.h>                          /* chaine en C */
#include <errno.h>                           /* Pour les messages d'erreur système */
#include <signal.h>                          /* traitement des signaux */
#include "../Table_processus/list_process.h" /* Module maison pour gérer la table des processus */
#include "../Gestion_erreurs/errors_message.h"

typedef struct
{
    int cote[2];
} t_pipe;

/**
 * \brief Handler qui permet la MAJ de la table des processus 
 * \param[in] sig signal émis 
 * \result pid du processus en avant -plan
 */
void suivi_fils(int sig);

/**
 * \brief Porgramme qui exécute une commande
 * \param[in] ligne_commande 
 * \param[in] leave     quitter minishell ? 
 * \result pid du processus en avant -plan
 */
void execute(char ***ligne_commande, bool background, int nb_commande, bool *leave, int *pid_actif, char *in, char *out);

/**
 * \brief Sous-programme qui gère l'exécution de commandes externes à minishell (e.g ls)
 * \param[in] cmd_line commande à exécuter
 * \param[in] background commande à exécuter en arrière plan ? 
 * \result pid du processus au premier plan
 */
void extern_cmd(char **cmd_line, bool background, t_pipe pipes[], int *pid_relance, char *in, char *out, int indice_courant, int nb_commande);

/**
 * \brief Sous-programme qui gère l'exécution de commandes internes à minishell (e.g cd)
 * \param[in] cmd_line commande à exécuter
 * \param[out] leave commande exit rentrée ? 
 * \param[out] pid_relance pid du processus à relancer en avant plan 
 * \result commande interne exécutée ? 
 */
bool intern_cmd(char **cmd_line, bool *leave, int *pid_relance, char *in, char *out);

#endif