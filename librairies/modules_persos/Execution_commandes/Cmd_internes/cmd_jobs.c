#include "../../Gestion_erreurs/errors_message.h"
#include "../../Table_processus/process_table.h"
#include "cmd_jobs.h"

extern list_process *table_process; /*table des processus défini dans le programme principal*/

void cmd_jobs(char **cmd_line){
    
    if (verif_commande_unaire(cmd_line,"jobs")){
            /* On choisit de ne pas inclure jobs dans la table puisque le pid est identique à minishell ce qui s'éloigne du comportement du bash */
            afficher_list_process(table_process);
    }

}
