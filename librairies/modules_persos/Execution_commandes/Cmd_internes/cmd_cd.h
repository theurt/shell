/* Commande interne cd */

#ifndef __CMD_CD_H
#define __CMD_CD_H

/**
 * \brief Commande pour changer de répertoire
 * \param[in] cmd_line ligne de commande
 * \param[in] in redirection éventuelle 
 */
void cmd_cd(char **cmd_line, char *in);

#endif