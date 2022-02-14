#include "./cmd_fg.h"
#include <unistd.h>                  /* Primitives systèmes comme fork ! */
#include <signal.h>                  /* traitement des signaux */
#include <sys/wait.h>                /* wait */
#include <stdlib.h>                  /* exit */
#include <string.h>                  /* chaine en C */
#include <errno.h>                   /* Pour les messages d'erreur système */
#include "../../Gestion_erreurs/errors_message.h"
#include "../../Table_processus/process_table.h"

extern list_process *table_process; /*table des processus défini dans le programme principal*/
extern int errno; /* Code d'erreur produit par des primitives telles que open, fork, etc... */

void cmd_fg(char **cmd_line, int* pid_relance){
    
    int etat_fils; 

    if (verif_commande_binaire_pid(cmd_line,"fg")){

            /*Obtenir le pid correspondant à l'id de minishell */
            int id_minishell = atoi(cmd_line[1]);
            *pid_relance = get_pid_from_id(table_process, id_minishell);
            errno = ESRCH;
            detect_Error(*pid_relance, "conversion id to pid ", NULL, -1, true);

            /* Relancer le processus correspondant */
            int retour = kill(*pid_relance, SIGCONT);
            detect_Error(retour,"kill_sigcont",NULL,-1,true);

            /* Le processus est au premier pla : on attend */
            int pid_fils = (int)waitpid(*pid_relance, &etat_fils, WUNTRACED);
            

            /* Un signal sigstp peut etre encore en mémoire et provoqué l'échec du waitpid => on l'évacue*/
            if (pid_fils == -1){
                pid_fils = (int)waitpid(*pid_relance, &etat_fils, WUNTRACED);
            }

            //detect_Error(pid_fils, "waitpid ", NULL, -1, true);

    }


}
