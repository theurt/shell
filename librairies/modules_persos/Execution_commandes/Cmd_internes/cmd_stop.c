#include "../../Gestion_erreurs/errors_message.h"
#include "./cmd_stop.h"
#include <unistd.h>                  /* Primitives systèmes comme fork ! */
#include <signal.h>                  /* traitement des signaux */
#include <stdlib.h>                  /* exit */
#include <string.h>                  /* chaine en C */
#include <errno.h>                   /* Pour les messages d'erreur système */
#include "../../Gestion_erreurs/errors_message.h"
#include "../../Table_processus/process_table.h"


extern list_process *table_process; /*table des processus défini dans le programme principal*/
extern int errno; /* Code d'erreur produit par des primitives telles que open, fork, etc... */

void cmd_stop(char **cmd_line){
    
    
    int id_minishell = atoi(cmd_line[1]);

    int pid = get_pid_from_id(table_process, id_minishell);
    errno = ESRCH;
    detect_Error(pid, "conversion id to pid ", NULL, -1, true);

    int retour = kill(pid, SIGSTOP);
    detect_Error(retour,"kill_sigstop",NULL,-1,true);

}
