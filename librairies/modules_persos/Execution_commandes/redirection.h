#include <unistd.h>                  /* Primitives systèmes comme fork ! */
#include <signal.h>                  /* traitement des signaux */
#include <sys/wait.h>                /* wait */
#include <stdlib.h>                  /* exit */
#include <string.h>                  /* chaine en C */
#include <stdbool.h>
#include <errno.h>                   /* Pour les messages d'erreur système */
#include <fcntl.h>                  /* opérations sur les fichiers */

/**
 * \brief Redirige une entrée/sortie standard vers fichier
 * \param[in] fichier
 * \param[in] commande interne ? 
 * \param[in] redirection sortie ? 
 * \result pid du processus en avant -plan
 */
int rediriger(char *fichier,bool intern,bool sortie);

/** 
 * \brief Remet l'entrée/sortie standard par défaut
 * \param[in] desc descripteur du fichier qui a remplacé l'entrée/sortie standard
 * \param[in] commande interne ? 
 * \param[in] redirection sortie ? 
 * \result pid du processus en avant -plan
 */
void raz_STD(int desc, bool intern,bool sortie );
