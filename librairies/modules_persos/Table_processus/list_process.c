/* définition des opérations sur la liste des processus */


#include "list_process.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int NON_ECRASABLE; /* PID du minishell, impossible d'écraser cette entrée de la liste */

process_entry *create_process(int id, pid_t pid, char state, char *command)
{

    process_entry *proc = malloc(sizeof(process_entry));

    if (proc) /* Allocation réussie */
    {
        proc->id = id;
        proc->pid = pid; /* affectation du champ next à la liste vide */
        proc->state = state;
        proc->command = malloc(sizeof(command));
        strcpy(proc->command, command);
    }
    return proc;
}

pid_t get_pid(process_entry *process)
{
    return process->pid;
}

int get_id(process_entry *process)
{
    return process->id;
}

char get_state(process_entry *process)
{
    return process->state;
}

char *get_command(process_entry *process)
{
    return process->command;
}

void set_state(process_entry *process, char state)
{
    process->state = state;
}

void afficher_process(process_entry *process)
{
    printf("%d    %d     %c      %s\n", process->id, process->pid, process->state, process->command);
}

void destruct_list_process(list_process *list)
{

    /* Détruire récursivement la liste chainée des entrées */
    if (list != NULL)
    {
        /* Détruire l'entrée suivante */
        destruct_list_process(list->suivant);

        /* Libérer une entrée */
        if (list->process != NULL)
        {
            free(list->process->command);
            free(list->process);
            list->process = NULL;
        }

        free(list);
        list = NULL;
    }
}

list_process *create(process_entry *data, bool first)
{
    list_process *list = malloc(sizeof(list_process));

    if (list) /* Allocation d'une entrée réussie */
    {
        list->process = data;
        list->suivant = NULL; /* affectation du champ next à la liste vide */
    }

    /* Entrée correspondant au minishell */
    if (first)
    {
        NON_ECRASABLE = data->pid;
    }
    return list;
}

process_entry *get_process(list_process *list, pid_t numero)
{
    /* Parcours récursif de la liste chainé pour trouver la bonne entrée */
    if (list == NULL)
    {
        return NULL;
    }

    else if (list->process->pid == numero)
    {
        return list->process;
    }

    else
    {
        return get_process(list->suivant, numero);
    }
}

pid_t get_pid_from_id(list_process *list, int numero)
{

    /* Parcours récursif de la liste chainé pour trouver la bonne entrée */
    if (list == NULL)
    {
        return -1000;
    }

    else if (list->process->id == numero)
    {
        return list->process->pid;
    }

    else
    {
        return get_pid_from_id(list->suivant, numero);
    }
}

int get_position(list_process *list, pid_t numero)
{

    /* Entrée absente de la liste */
    if (list == NULL)
    {
        return -10000;
    }

    /* Entrée trouvée ! */
    else if (list->process->pid == numero)
    {
        return 0;
    }

    /* Entrée non trouvée => on continue à chercher */
    else
    {
        return 1 + get_position(list->suivant, numero);
    }
}

void modify(list_process *list, process_entry *a_rajouter)
{

    /* Liste vide => on crée la première cellule */
    if (list == NULL)
    {
        list = create(a_rajouter, false);
    }

    else
    {
        pid_t PID = a_rajouter->pid;

        /* On ne touche pas à l'entrée de minishell !! */
        if (PID == NON_ECRASABLE)
        {
        }

        else
        {
            /* Modifier une cellule en tête de liste ! */
            if (list->process->pid == PID)
            {
                list->process = a_rajouter;
            }

            /* Rajouter la cellule en tête de liste */
            else if (list->process->pid > PID)
            {
                list_process *nouveau = create(list->process, false); // copie ancienne tete de liste
                nouveau->suivant = list->suivant;
                list->process = a_rajouter;
                list->suivant = nouveau;
            }

            /* Rajouter/Modifier une cellule dans la liste */
            else
            {

                /* Chercher l'endroit de la modification */
                list_process *curseur = list;

                while (curseur->suivant != NULL && curseur->suivant->process->pid < PID)
                {
                    curseur = curseur->suivant;
                }

                /* Ajouter la cellule en fin de liste */
                if (curseur->suivant == NULL)
                {
                    list_process *nouveau = create(a_rajouter, false);
                    curseur->suivant = nouveau;
                }

                /* Ajouter la cellule au milieu de la liste */
                else
                {

                    if (curseur->suivant->process->pid == PID)
                    {
                        curseur->suivant->process = a_rajouter;
                    }

                    else
                    {
                        list_process *memory = curseur->suivant;
                        list_process *nouveau = create(a_rajouter, false);
                        curseur->suivant = nouveau;
                        nouveau->suivant = memory;
                    }
                }
            }
        }
    }
}

void supprimer(list_process *list, pid_t numero)
{

    if (list == NULL)
    {
    }

    else
    {

        /* Supprimer une cellule en tête ! */
        if (list->process->pid == numero)
        {
            /* décaler toute la liste */
            list_process *curseur = list;
            while (curseur->suivant != NULL)
            {
                curseur->process = curseur->suivant->process;

                curseur = curseur->suivant;
            }

            free(curseur->process);
            free(curseur);
        }

        /* Cellule absente => on en fait rien */
        else if (list->process->pid > numero)
        {
        }

        else
        {

            /* Chercher la position de la cellule à supprimer */
            list_process *curseur = list;

            while (curseur->suivant != NULL && curseur->suivant->process->pid < numero)
            {
                curseur = curseur->suivant;
            }

            /* Cellule absente */
            if (curseur->suivant == NULL)
            {
            }

            /* Cellule présente */
            else
            {
                /* La cellule à libérer se trouve juste après celle actuelle */
                if (curseur->suivant->process->pid == numero)
                {

                    /* Et elle n'a pas de suivant */
                    if (curseur->suivant->suivant != NULL)
                    {
                        /* Liberation des ressources */
                        list_process *a_liberer = curseur->suivant;
                        curseur->suivant = curseur->suivant->suivant;
                        free(a_liberer->process);
                        a_liberer->process = NULL;
                        free(a_liberer);
                        a_liberer = NULL;
                    }

                    /* La cellule à libérer est entre deux cellules */
                    else
                    {

                        /* Liberation des ressources */
                        free(curseur->suivant->process);
                        curseur->suivant->process = NULL;
                        free(curseur->suivant);
                        curseur->suivant = NULL;
                    }
                }
            }
        }
    }
}

void afficher_list_process(list_process *list)
{
    printf("ID    PID    STATE    COMMAND\n");
    list_process *curseur = list;
    while (curseur != NULL)
    {
        afficher_process(curseur->process);
        curseur = curseur->suivant;
    }
}