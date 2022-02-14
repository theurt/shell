/* Commande interne cd */
#include "../../Gestion_erreurs/errors_message.h"
#include "cmd_cd.h"
#include <unistd.h>
#include <stdbool.h>
#include "../redirection.h"

void cmd_cd(char **cmd_line, char *in)
{

    if (in == NULL)
    {

        int retour = chdir(cmd_line[1]);
        detect_Error(retour, "cd", NULL, -1, true);
    }

    else
    {

        int desc = rediriger(in, true, false);
        fchdir(desc);
        raz_STD(desc, true, false);
    }
}
