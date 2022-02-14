/* ETAPE 9 : Tubes simples */
#include <stdio.h>                                                 /* E/S comme printf */
#include <stdbool.h>                                               /* Booléens */
#include <stdlib.h>                                                /* exit */
#include <errno.h>                                                 /* Pour les messages d'erreur système */
#include <signal.h>                                                /* traitement des signaux */
#include "../librairies/fournitures/readcmd.h"                         /* import de l'analyseur syntaxique */
#include "../librairies/modules_persos/Table_processus/list_process.h" /* Module maison pour gérer la table des processus */
#include "../librairies/modules_persos/Execution_commandes/command.h"
#include "../librairies/modules_persos/Table_processus/process_table.h"

extern list_process *table_process; /* Liste chainée avec les processus lancés par minishell définie dans process_table.h */

extern int errno; /* Code d'erreur produit par des primitives telles que open, fork, etc... */

int *pid_actif;

/**
 * \brief Handler qui transforme un sigint en sigkill
 * \param[in] sig numéro du nignal émis 
 */
void handler_sigint(int sig)
{

    if (*pid_actif != 0)
    {
        kill(*pid_actif, SIGKILL);
        *pid_actif = 0;
        printf("\n");
    }
}

/**
 * \brief Handler qui transforme un sigstp en sigstop
 * \param[in] sig numéro du nignal émis 
 */
void handler_sigstp(int sig)
{

    if (*pid_actif != 0)
    {
        kill(*pid_actif, SIGSTOP);
        *pid_actif = 0;
        printf("\n");
    }
}

/**
 * \brief Sous-programme qui empêche le lancement si trop d'arguments sur la ligne de commande principale
 * \param[in] argc nombre d'arguments
 */
void check_launch(int argc)
{
    if (argc > 1)
    {
        printf("Trop d'argument pour lancer un shell ! Réesayez sans option \n");
        exit(EXIT_FAILURE);
    }
}

/**
 * \brief Compter le nombre de de commandes sur une ligne
 * \param[in] ligne_commande 
 */
int get_nb_commande(char ***ligne_commande)
{

    /* Compter le nombre de commandes */
    int j = 0;
    while (ligne_commande[j] != NULL)
    {
        j++;
    }

    return j;
}

/**
 * \brief Sous-programme qui analyse la ligne de commande pour en tirer les infos principales
 * \param[in out] ligne_commande Tableau avec la décomposition de la ligne de commande 
 * \param[out]  background  commande en tache de fond ? 
 * \param[out]  nb_commande nombre de commande à exécuter (en pipe)
 */
bool analyser_ligne(char ****ligne_commande, bool *background, int *nb_commande, char **in, char **out)
{

    struct cmdline *analyse_ligne = NULL; /* Analyse de la ligne de commande par le sous-programme cmdline */
    bool ligne_vide = true;               /* Détection d'une ligne vide ou d'une commande interne = > pas de fils*/

    /* Saisie utilisateur*/
    printf(">>>");
    fflush(stdout);

    /* Analyse de la ligne rentrée */
    do
    {
        analyse_ligne = readcmd();
    } while (analyse_ligne == NULL);
    *ligne_commande = analyse_ligne->seq;

    /* Ligne vide => ne rien faire  */
    if (ligne_commande[0] == NULL)
    {

        ligne_vide = true;
    }

    else
    {
        /* Commande en tache de fond ? */
        *background = (analyse_ligne->backgrounded != NULL);

        /* Redirections ? */
        *in = analyse_ligne->in;
        *out = analyse_ligne->out;

        *nb_commande = get_nb_commande(*ligne_commande);

        ligne_vide = false;
    }

    return ligne_vide;
}

/* Emulateur de shell appelé "minishell" */
int main(int argc, char *argv[])
{

    check_launch(argc);

    /* déclaration des variables */
    char ***ligne_commande = NULL;      /*Tableau avec la décomposition de la ligne de commande */
    bool ligne_vide = true;             /* Détection d'une ligne vide ou d'une commande interne = > pas de fils*/
    bool *leave = malloc(sizeof(bool)); /* quitter ? */
    bool background;                    /* Commande ne tache de fond */
    int nb_commande;                    /* Nombre de commande */
    char *in = malloc(sizeof(char));    /* Redirection entrée */
    char *out = malloc(sizeof(char));   /* Redirection sortie */
    pid_actif = malloc(sizeof(int));

    /* Le minishell ne sera pas interrompu par ctrl C ou ctrl z*/
    struct sigaction sig_int;
    sig_int.sa_flags = 0;
    sigemptyset(&sig_int.sa_mask);
    sig_int.sa_handler = handler_sigint;
    sigaction(SIGINT, &sig_int, NULL);

    struct sigaction sig_stp;
    sig_stp.sa_flags = 0;
    sigemptyset(&sig_stp.sa_mask);
    sig_stp.sa_handler = handler_sigstp;
    sigaction(SIGTSTP, &sig_stp, NULL);

    /* Initialisation de la variable permettant de quitter */
    if (leave)
    {
        *leave = false;
    }

    /* Ajout de minishell à la table des processus */
    initialize_process_table(argv);

    /* Faire la boucle de base */
    do
    {

        /* Analyser la ligne de commande rentrée */
        ligne_vide = analyser_ligne(&ligne_commande, &background, &nb_commande, &in, &out);

        /* ne rien faire si ligne vide */
        if (ligne_vide)
        {
        }

        /* Exécuter les commandes sinon */
        else
        {
            ligne_vide = false;
            execute(ligne_commande, background, nb_commande, leave, pid_actif, in, out);
        }

    } while (ligne_vide || !*leave);

    /* Libérer les processus fils */
    list_process *curseur = table_process->suivant;
    int pid;
    while (curseur != NULL)
    {
        pid = curseur->process->pid;
        kill(pid, SIGTERM);
        curseur = curseur->suivant;
    }

    /* Libérer les ressources */
    destruct_list_process(table_process);

    exit(EXIT_SUCCESS);
}
