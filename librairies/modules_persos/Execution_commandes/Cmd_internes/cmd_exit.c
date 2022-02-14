#include "../../Gestion_erreurs/errors_message.h"
#include "cmd_exit.h"
#include <stdbool.h>                 /* Booléens */

bool cmd_exit(char **cmd_line){
    
    bool leave = false;
    /* Commande exit bien rentrée ! */
    if (verif_commande_unaire(cmd_line,"exit")){

            leave = true;
    }

    return leave;
}
