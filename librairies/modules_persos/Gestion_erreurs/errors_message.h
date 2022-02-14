/* Gestion de l'affichage de messages d'erreur générique pour l'exécution du minishell */
#ifndef __ERRORS_MESSAGE_H
#define __ERRORS_MESSAGE_H
#include <stdbool.h> /* Booléens */

/**
 * \brief Sous-programme qui gère l'affichage de message d'erreur avec l'utilisation de primitives systèmes 
 * telles que open, fork, etc...
 * \param[in] code_erreur code renvoyé par la primitive dont on analyse l'erreur ou non
 * \param[in] operation_cause nom de la primitive en cause 
 * \param[in] fichier_cause nom du fichier éventuellement en cause
 * \param[in] pid pid éventuel du processus en cause (si -1 cela signifie que le processus en cause est minishell => aucun intérêt pour l'affichage)
 * \param[in] intern si la commande est interne on n'interrompt pas minishell
 * \pre operation_cause != NULL
 * \pre pid == -1 || pid > 0
 */
int detect_Error(int code_erreur, char operation_cause[], char fichier_cause[], int pid, bool intern);

/**
 * \brief Sous-programme qui vérifier si une commande est trouvée ou non, utilisé dans le lancement d'une command externe 
 * \param[in] cmd_line commande à exécuter
 */
void print_command_not_found(char **cmd_line);

/**
 * \brief Sous-programme affichant un message d'erreur si la commande fait plus d'un mot 
 * \param[in] cmd_line ligne rentrée
 * \param[in] operation_cause nom de la commande
 * \result correct ? 
 */
bool verif_commande_unaire(char **cmd_line, char operation[]);

/**
 * \brief Sous-programme affichant un message d'erreur si la commande binaire prenant un pid comme paramètre est mal rentrée
 * \param[in] cmd_line ligne rentrée
 * \param[in] operation_cause nom de la commande
 * \result correct ? 
 */
bool verif_commande_binaire_pid(char **cmd_line, char operation[]);

#endif