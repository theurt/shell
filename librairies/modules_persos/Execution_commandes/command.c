/* Modules built-in */
#include <stdio.h>    /* E/S comme printf */
#include <unistd.h>   /* Primitives systèmes comme fork ! */
#include <stdbool.h>  /* Booléens */
#include <stdlib.h>   /* exit */
#include <sys/wait.h> /* wait */
#include <string.h>   /* chaine en C */
#include <errno.h>    /* Pour les messages d'erreur système */
#include <signal.h>   /* traitement des signaux */
#include <fcntl.h>    /* opérations sur les fichiers */

/* Modules maisons */
#include "../Table_processus/list_process.h"   /* Gérer la table des processus */
#include "../Gestion_erreurs/errors_message.h" /* Gérer l'affichage des erreurs */
#include "../Table_processus/process_table.h"  /* Gérer la table des processus */
#include "command.h"
#include "redirection.h"

/* Commandes internes */
#include "./Cmd_internes/cmd_exit.h"
#include "./Cmd_internes/cmd_cd.h"
#include "./Cmd_internes/cmd_jobs.h"
#include "./Cmd_internes/cmd_stop.h"
#include "./Cmd_internes/cmd_fg.h"
#include "./Cmd_internes/cmd_bg.h"

extern list_process *table_process; /*table des processus défini dans le programme principal*/

static int id_minishell = 0; /* Id dans minishell, variable globale */

void suivi_fils(int sig)
{

    int etat_fils, pid_fils;

    /* Traiter CHAQUE SIGCHLD !!,cf moodle */
    do
    {

        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGINT);
        sigaddset(&mask, SIGTSTP);
        sigprocmask(SIG_BLOCK, &mask, NULL);

        /* Récupération du fils concerné */
        pid_fils = (int)waitpid(-1, &etat_fils, WNOHANG | WUNTRACED | WCONTINUED);

        /* Erreur de waitpid */
        if ((pid_fils == -1) && (errno != ECHILD))
        {

            /* Erreur potentiellement grave => on interrompt */
            detect_Error(pid_fils, "waitpid", NULL, getpid(), false);
            exit(EXIT_FAILURE);
        }

        /* Détection d'un processus fils a changé d'état */
        else if (pid_fils > 0)
        {

            /* Entrée de la table à mettre à jour */
            process_entry *proc;

            /* Processus suspendu */
            if (WIFSTOPPED(etat_fils))
            {

                /* MAJ de l'état du processus */
                proc = get_process(table_process, pid_fils);
                set_state(proc, 'S');
            }

            /* Processus relancé */
            else if (WIFCONTINUED(etat_fils))
            {

                /* Mettre à jour la table*/
                proc = get_process(table_process, pid_fils);
                set_state(proc, 'R');
            }

            /* Processus terminé */
            else if (WIFEXITED(etat_fils))
            {

                /* Supprimer l'entrée de la table des processus*/
                supprimer(table_process, pid_fils);
            }

            /* Processus interrompu par un signal */
            else if (WIFSIGNALED(etat_fils))
            {

                /* Supprimer l'entrée de la table des processus*/
                supprimer(table_process, pid_fils);
            }
        }
    } while (pid_fils > 0);
}

void execute(char ***ligne_commande, bool background, int nb_commande, bool *leave, int *pid_actif, char *in, char *out)
{

    bool commande_interne = false; /* Détection d'une commande interne */
    *pid_actif = 0;

    int tube[2];
    int indice_courant = 0; /* Numéro de la commande traitée */

    /* traiter les commandes en pipe */
    while (ligne_commande[indice_courant] != NULL)
    {

        /* Un tube relie 2 processus */
        int retour = pipe(tube);
        detect_Error(retour, "pipe", NULL, -1, true);

        /* Détection/Exécution d'une commande interne */
        commande_interne = intern_cmd(ligne_commande[indice_courant], leave, pid_actif, in, out);

        /* Execution (éventuelle) d'une commande externe avec MAJ de la table des processus*/
        if (!commande_interne)
        {

            extern_cmd(ligne_commande[indice_courant], background, pid_actif, in, out, tube, indice_courant, nb_commande);
        }

        indice_courant++;
    }
}

void extern_cmd(char **cmd_line, bool background, int *pid_relance, char *in, char *out, int tube[2], int indice_courant, int nb_commande)

{

    int retour;                /* Réussite ou non de la création d'un processus fils  */
    int fils_termine, wstatus; /* Vérification sur le statut d'exécution de la commande */

    /* Sigaction nécessaire pour gérer les processus fils depuis minishell*/
    struct sigaction maj_fils;
    maj_fils.sa_flags = 0;
    sigemptyset(&maj_fils.sa_mask);
    maj_fils.sa_handler = suivi_fils;

    /* Processus fils dans lequel on lance la commande */
    retour = fork();

    /* Si erreur du fork, on considère que c'est grave et on interrompt mini shell */
    detect_Error(retour, "fork", NULL, -1, false);
    id_minishell++;

    /* Exécuter la commande dans le fils */
    if (retour == 0)
    {

        /* Plusieurs commandes */
        if (nb_commande > 1)
        {

            /* Premier fils */
            if (indice_courant == 0)
            {

                /* Redirection éventuelle */
                if (in != NULL)
                {
                    int desc = rediriger(in, false, false);
                    detect_Error(desc, "dup_1", NULL, -1, false);
                }

                /* On ferme la sortie du tube */
                int retour = close(tube[0]);
                detect_Error(retour, "close_1", NULL, -1, false);

                /* On redirige la sortie du fils vers l'entrée du tube */
                retour = dup2(tube[1], STDOUT_FILENO);
                close(tube[1]);
                detect_Error(retour, "dup_1", NULL, -1, false);
            }

            /* Deuxieme fils */
            else
            {
                /* On ferme l'entrée du tube */
                int retour = close(tube[1]);
                detect_Error(retour, "close_6", NULL, -1, false);

                /* On redirige la sortie du tube vers l'entrée du fils */
                retour = dup2(tube[0], STDIN_FILENO);
                detect_Error(retour, "dup_6", NULL, -1, false);

                /* Redirection éventuelle */
                if (out != NULL)
                {
                    int desc = rediriger(out, false, true);
                    detect_Error(desc, "dup_1", NULL, -1, false);
                }
            }
        }

        /* Une seule commande */
        else
        {

            if (in != NULL)
            {
                rediriger(in, false, false);
            }

            if (out != NULL)
            {
                rediriger(out, false, true);
            }
        }

        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGINT);
        sigaddset(&mask, SIGTSTP);
        sigprocmask(SIG_BLOCK, &mask, NULL);
        execvp(cmd_line[0], cmd_line);

        /* Si l'exécution échoue */
        print_command_not_found(cmd_line);
        exit(errno);
    }

    /* MAJ de la table des processus dans le père et éventuel attente du processus en foreground */
    else
    {

        /* Fermer le tube dans le père pour éviter des erreurs */
        close(tube[0]);
        close(tube[1]);

        /* MAJ de la table quand le processus fils change d'état */
        sigaction(SIGCHLD, &maj_fils, NULL);

        /* Ajout du processus en foreground à la table */
        modify(table_process, create_process(id_minishell, retour, 'R', to_str(cmd_line)));

        if (!background)
        {

            *pid_relance = retour;
            fils_termine = waitpid(retour, &wstatus, 0);

            supprimer(table_process, fils_termine);

            /* On ne peut pas controler ce wait car il peut être interrompu par un SIGSTOP */
            //detect_Error(fils_termine, "wait", NULL, getpid(), false);
        }
    }
}

bool intern_cmd(char **cmd_line, bool *leave, int *pid_relance, char *in, char *out)
{

    bool intern = true; /* Commande interne detectée ? */
    *leave = false;     /* Exit détecté */

    /* Commande exit */
    if (strcmp(cmd_line[0], "exit") == 0)
    {
        *leave = cmd_exit(cmd_line);
    }

    /* Commande interne cd */
    else if (strcmp(cmd_line[0], "cd") == 0)
    {
        cmd_cd(cmd_line, in);
    }

    /* Commande interne jobs */
    else if (strcmp(cmd_line[0], "jobs") == 0)
    {
        cmd_jobs(cmd_line);
    }

    /* Commande interne stop*/
    else if (strcmp(cmd_line[0], "stop") == 0)
    {
        cmd_stop(cmd_line);
    }

    /* Commande interne bg*/
    else if (strcmp(cmd_line[0], "bg") == 0)
    {
        cmd_bg(cmd_line, in);
    }

    /* Commande interne fg */
    else if (strcmp(cmd_line[0], "fg") == 0)
    {
        cmd_fg(cmd_line, pid_relance);
    }

    else
    {
        intern = false;
    }

    return intern;
}