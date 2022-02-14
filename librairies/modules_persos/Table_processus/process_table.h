/* Module qui contient la table des processus de minishell */
#include "list_process.h"

list_process *table_process; /* Liste chainée avec les processus lancés par minishell */

/**
 * \brief Initialiser la table des processus par l'ajout de minishell, son id, pid et statut 
 * \param[in] argv argument de la ligne de commande
 */

void initialize_process_table(char *argv[]);

/**
 * \brief Convertit la ligne de commande en simple string, utilisé pour enregistrer dans la table des processus
 * \param[in] cmd_line commande à exécuter
 * \result string 
 */
char *to_str(char **cmd_line);