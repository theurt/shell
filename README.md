Auteur : HEURTEBISE Tom 
Version : 2.0

# Qu'est ce que le projet bash ? 

Il s'agit d'utiliser les appels systèmes en C pour recréer un bash 100% fonctionnel. Notez que sur ce projet il reste à améliorer la création et la cohérence des tubes de données. 

# Comment lancer le shell ?

-Pour lancer le shell veuillez exécuter la commande make suivi de ./Q9 ou ./Q10
-Pour nettoyer les fichiers veuillez exécuter make clean 


# Organisation du repository

-src : dossier contenant le fichier principal shell.c

-librairies : & modules_persos  regroupe le coeur du projet et se subdivise lui même en plusieurs dossiers : 
	
	-> sous-section  Execution_commandes : c'est le COEUR du projet ou figure notamment les commandes internes dans des fichiers séparés, 
		le module permettant de faire des redirections mais aussi et surtout les fichiers command* qui comporte les méthodes qui lancent
		les processus fils ou exécutent les commandes internes 

	-> sous-section Gestion_erreurs :fichiers qui permettent de réaliser un affichage systématique des erreurs de minishell 

	-> sous-section Table_process : Gestion de la table avec les processus lancés (suppression, insertion,...)


	& fournitures : parser pour le shell
