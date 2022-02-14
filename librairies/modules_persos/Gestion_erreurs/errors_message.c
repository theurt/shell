#include <stdio.h>   /* E/S comme printf */
#include <unistd.h>  /* Primitives systèmes comme fork ! */
#include <stdbool.h> /* Booléens */
#include <stdlib.h>  /* exit */
#include <string.h>  /* chaine en C */
#include <errno.h>   /* Pour les messages d'erreur système */
#include <signal.h>  /* traitement des signaux */
#include "../Gestion_erreurs/errors_message.h"

#define TAILLE_MESSAGE 400 /* Taille des messages d'erreur */

extern int errno; /* Code d'erreur produit par des primitives telles que open, fork, etc... */

/**
 * \brief Sous-programme qui décide d'ajouter le pid d'un processus dans un message d'erreur
 * \param[in] pid processus en cause
 * \param[in] message chaine à concaténer ou non 
 */
void print_not_minishell(int pid, char message[TAILLE_MESSAGE])
{

    /* Si un processus autre que minishell est impliqué on inclut son pid dans le message */
    if (pid != -1)
    {
        strcat(message, "during execution of process ");
        char append[sizeof(long)]; // On fait l'hypothèse qu'un pid ne dépassera pas la taille d'un long
        sprintf(append, "%d", pid);
        strcat(message, append);
        strcat(message, " PID ");
    }
}

int detect_Error(int code_erreur, char operation_cause[], char fichier_cause[], int pid, bool intern)
{
    /* Il y a une erreur ! */
    if (code_erreur < 0)
    {
        char message[TAILLE_MESSAGE] = "";

        strcat(message, "An Error occured with the operation ");

        strcat(message, operation_cause);

        /* Si un fichier est en cause on l'affiche */
        if (fichier_cause != NULL)
        {
            strcat(message, " on the file ");
            strcat(message, fichier_cause);
        }

        /* Si un processus autre que minishell est impliqué dans l'erreur on affiche son pid */
        print_not_minishell(pid, message);

        /* Affichage du message */
        perror(message);

        /*Permet de ne pas interrompre minishell en cas d'erreur d'une commande interne*/
        if (!intern)
        {
            exit(errno);
        }
        return 0;
    }

    return 0;
}

/**
 * \brief Sous-programme qui vérifier si une commande est trouvée ou non, utilisé dans le lancement d'une command externe 
 * \param[in] cmd_line commande à exécuter
 */
void print_command_not_found(char **cmd_line)
{
    /*Si l'erreur est "fichier ou dossier" non trouvé c'est vraisemblablement que la commande n'a pas été trouvée */
    if (errno == ENOENT)
    {
        printf("%s : Command not found\n", cmd_line[0]);
    }

    /* Sinon on affiche l'erreur par défaut gérée par les primitives */
    else
    {
        perror(NULL);
    }
}

bool verif_commande_unaire(char **cmd_line, char operation[])
{

    /* Un argument en trop ! */
    if (cmd_line[1] != NULL)
    {
        errno = EINVAL;
        detect_Error(-1, operation, NULL, -1, true);
        return false;
    }

    else
    {
        return true;
    }
}

bool verif_commande_binaire_pid(char **cmd_line, char operation[])
{

    /* Il y a au moins deux arguments */
    if (cmd_line[1] != NULL)
    {

        strtol(cmd_line[1], NULL, 10);

        /* Deuxieme argument non entier */
        if (errno == EINVAL)
        {
            detect_Error(-1, operation, NULL, -1, true);
            return false;
        }

        /* Verifier qu'il n'y a pas 3 arguments ou plus */
        else
        {

            if (cmd_line[2] == NULL)
            {
                return true;
            }
            errno = E2BIG;
            detect_Error(-1, operation, NULL, -1, true);
            return false;
        }
    }

    /* Un seul argument */
    else
    {
        errno = EINVAL;
        detect_Error(-1, operation, NULL, -1, true);
        return false;
    }
}
