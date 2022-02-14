#include <unistd.h>   /* Primitives systèmes comme fork ! */
#include <signal.h>   /* traitement des signaux */
#include <sys/wait.h> /* wait */
#include <stdlib.h>   /* exit */
#include <string.h>   /* chaine en C */
#include <stdbool.h>
#include <errno.h> /* Pour les messages d'erreur système */
#include <fcntl.h> /* opérations sur les fichiers */
#include "../Gestion_erreurs/errors_message.h"

int rediriger(char *fichier, bool intern, bool sortie)
{

    /* Redirection de la sortie standard */
    int desc;
    int nv_desc;

    if (sortie)
    {

        /* Ouverture fichier */
        desc = open(fichier, O_WRONLY | O_CREAT | O_TRUNC, 0640);
        detect_Error(desc, "open_sortie", fichier, getpid(), intern);

        /* La sortie standard devient le fichier ouvert */
        nv_desc = dup2(desc, STDOUT_FILENO);
        detect_Error(nv_desc, "dup2_copie_sortie", NULL, getpid(), intern);
    }

    else
    {

        /* Ouverture fichier */
        desc = open(fichier, O_RDONLY, 0640);
        detect_Error(desc, "open_entree", fichier, getpid(), intern);

        /* L'entrée standard devient le fichier ouvert */
        nv_desc = dup2(desc, STDIN_FILENO);
        detect_Error(nv_desc, "dup2_copie_entree", NULL, getpid(), intern);
    }

    return nv_desc;
}

void raz_STD(int desc, bool intern, bool sortie)
{

    int nv_desc;

    if (sortie)
    {
        nv_desc = dup2(desc, STDOUT_FILENO);
        detect_Error(nv_desc, "dup2_raz_sortie", NULL, getpid(), intern);
    }
    else
    {
        nv_desc = dup2(desc, STDIN_FILENO);
        detect_Error(nv_desc, "dup2_raz_entree", NULL, getpid(), intern);
    }
}