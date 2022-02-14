/* Module qui implante les opérations nécessaires à la table des processus de minishell */
#ifndef __LIST_PROCESS_H
#define __LIST_PROCESS_H
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

/* Une entrée dans la table des processus ( AU SENS de la commande ps !! ) de minishell*/
typedef struct process_entry process_entry;
struct process_entry
{
    int id;        /* Identifiant du processus dans minishell (minishell = 0 ) */
    pid_t pid;     /* PID dans le shell classique */
    char state;    /* Etat du processus : actif/suspendu*/
    char *command; /* Ligne de commande lancée */
};

/* Liste des processus de minishell */
typedef struct list_process list_process;
struct list_process
{
    list_process *suivant;  /* pointeur sur le reste de la liste de processus*/
    process_entry *process; /* pointeur un processus*/
};

/**
 * \brief Construit une entrée de la table des processus
 * \param[in] id id dans minishell
 * \param[in] pid pid dans le shell normal
 * \param[in] state état (suspendu, cours) du processus
 * \param[in] command ligne de commande ayant lancé le processus
 * \result entrée de la table de processus
 */
process_entry *create_process(int id, pid_t pid, char state, char *command);

/* Get sur les entrées processus */

/**
 * \brief Renvoie le pid d'une entrée processus
 * \param [in] process entrée de la table considérée
 * \result pid dans le shell
 */
pid_t get_pid(process_entry *process);

/**
 * \brief Renvoie l'id d'une entrée processus
 * \param [in] process entrée de la table considérée
 * \result id dans le minishell
 */
int get_id(process_entry *process);

/**
 * \brief Renvoie l'état d'une entrée processus
 * \param [in] process entrée de la table considérée
 * \result état dans le minishell
 */
char get_state(process_entry *process);

/**
 * \brief Renvoie la ligne de commande liée à un processus
 * \param [in] process entrée de la table considérée
 * \result ligne de commande
 */
char *get_command(process_entry *process);

/* Set sur les entrées processus */

/**
 * \brief Change l'état de l'entrée d'un processus
 * \param [in out] process entrée de la table considérée
 * \param [in] ligne de commande
 */
void set_state(process_entry *process, char state);

/**
 * \brief Libérer la mémoire 
 * \param [in out] list à détruire
 */
void destruct_list_process(list_process *list);
/**
 * \brief Afficher l'entrée liée à un processus dans la table
 * \param [in] process entrée de la table considérée
 */
void afficher_process(process_entry *process);

/**
 * \brief Construit la table des processus
 * \result table des processus créées
 */
list_process *create(process_entry *process, bool first);

/**
 * \brief Obtenir l'entrée liée à un pid
 * \param[in] list_process table à considérer
 * \param[in] numero  pid à considérer
 * \result entrée liée au processus de pid numero
 */
process_entry *get_process(list_process *list, pid_t numero);

/**
 * \brief Pour du debug seulement
 * \param[in] list_process table à considérer
 * \param[in] numero  id à considérer
 * \result pid du processus
 */
pid_t get_pid_from_id(list_process *list, int numero);

/**
 * \brief Pour du debug seulement
 * \param[in] list_process table à considérer
 * \param[in] numero  pid à considérer
 * \result position d'une entrée dans la table
 */
int get_position(list_process *list, pid_t numero);

/**
 * \brief Ajouter ou modifier une entrée de la table
 * \param[in out] list_process table à considérer
 * \param[in] a_rajouter  entrée à rajouter
 */
void modify(list_process *list, process_entry *a_rajouter);

/**
 * \brief Supprimer une entrée de la table (ne fait rien si aucune entrée ne correspond)
 * \param[in] list_process table à considérer
 * \param[in] numero  numéro du pid de l'entrée à supprimer
 */
void supprimer(list_process *list, pid_t numero);

/**
 * \brief Afficher la table des processus
 * \param[in] list_process table à considérer
 */
void afficher_list_process(list_process *list);

#endif