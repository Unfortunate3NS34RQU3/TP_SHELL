# TP_SHELL
 Réalisé par CHEDI et LOSIO

Vous trouverez ci-après notre compte-rendu du premier TP de synthèse de majeure informatique.

#1) TpShell_Q1
    -Fonction displayWelcomeMessage() :
        *)Cette fonction affiche un message de bienvenue et des instructions simples pour utiliser le shell.
        *)Elle utilise la fonction write() pour imprimer les messages dans la sortie standard.

    -Fonction main() :
        *)Le programme principal appelle displayWelcomeMessage().
        *)Il n'y a pas de boucle d'exécution continue pour l'interactivité typique d'un shell. Il s'arrête après l'affichage du message.

#2) TpShell_Q2
    -Fonctionnalité du Shell :
   	*)Le programme affiche un prompt enseash % dans une boucle infinie.
    	*)L'utilisateur peut saisir une commande et appuyer sur Enter.
    	*)Le code lit la commande saisie via read() et stocke cette commande dans le buffer buf.
    	*)Si une erreur survient pendant la lecture, elle est affichée et le programme se termine.

    -Exécution de la commande saisie :
  	*)Le code crée un nouveau processus avec fork().
    	*)Dans le processus enfant, il utilise execlp() pour exécuter la commande saisie stockée dans buf:
        	-->execlp() prend la commande, ses arguments (dans ce cas, aucun argument), et un pointeur NULL.
        	-->En cas d'échec de l'exécution (execlp() renvoie -1), il affiche une erreur avec perror() et termine le processus enfant avec exit().
    	*)Dans le processus parent, il attend la fin du processus enfant avec waitpid().

#3) TpShell_Q3
    -Prompt et saisie de commandes :
        *)Le programme fonctionne dans une boucle infinie.
        *)À chaque itération de la boucle, il affiche un prompt enseash % pour indiquer que l'utilisateur peut saisir une commande.
        *)Il lit ensuite la commande saisie par l'utilisateur à l'aide de read().

    -Fonctionnalités du shell :
        *)Si l'utilisateur saisit "exit" ou utilise Ctrl+d (EOF), le programme affiche "Bye bye..." et se termine, sortant de la boucle.
        *)Si une commande est saisie autre que "exit":
            -->Le programme crée un nouveau processus avec fork().
            -->Dans le processus enfant, il exécute la commande saisie à l'aide de execlp(buf, buf, NULL).
                -->execlp() prend la commande saisie, ses arguments (dans ce cas, aucun argument), et un pointeur NULL.
                -->Si l'exécution de la commande échoue, il affiche une erreur avec perror() et termine le processus enfant avec exit().
            -->Dans le processus parent, il attend la fin du processus enfant avec waitpid().

    -Exemple d'utilisation :
        *)L'utilisateur saisit la commande exit.
        *)Le programme affiche "Bye bye..." et se termine.
        *)Après cela, le shell est arrêté et le contrôle est rendu à l'invite de commande du système ($).

#4) TpShell_Q4
    -Prompt et saisie de commandes :
    	*)Le programme fonctionne dans une boucle infinie.
        *)Il affiche un prompt enseash % pour indiquer que l'utilisateur peut saisir une commande.
    	*)Il lit la commande saisie par l'utilisateur à l'aide de read().

    -Exécution de commandes :
    	*)Si l'utilisateur saisit "exit" ou utilise Ctrl+d (EOF), le programme affiche "Bye bye..." et se termine.
    	*)Si une commande est saisie autre que "exit":
            -->Le programme crée un nouveau processus avec fork().
            -->Dans le processus enfant, il exécute la commande saisie à l'aide de execlp(buf, buf, NULL):
            	-->execlp() prend la commande saisie, ses arguments (dans ce cas, aucun argument), et un pointeur NULL.
            	-->En cas d'échec de l'exécution de la commande, il affiche une erreur avec perror() et termine le processus enfant avec exit().
            -->Dans le processus parent, il attend la fin du processus enfant avec waitpid() et récupère le statut de sortie de ce processus.

    -Affichage du code de retour ou du signal :
    	*)Avant de lire une nouvelle commande, le shell vérifie le statut du processus précédent (previousStatus).
    	*)Si le processus précédent s'est terminé normalement, il affiche le propmt normal .
    	*)Si le processus précédent s'est terminé avec une erreur, il affiche le code de retour entre crochets [exit:code_retour]  .
    	*)Si le processus précédent a été interrompu par un signal, il affiche le numéro de signal entre crochets [sign:numéro_signal]  .

#5) TpShell_Q5
    -Prompt et saisie de commandes :
        *)Le shell utilise une boucle infinie pour attendre les commandes de l'utilisateur.
        *)Il affiche un prompt initial enseash % ou un prompt incluant le code de retour / signal précédent.

    -Gestion du code de retour / signal :
        *)Avant de lire une nouvelle commande, il vérifie le statut du processus précédent (previousStatus).
        *)Si le processus précédent s'est terminé normalement, il affiche le code de retour entre crochets [exit:code_retour|temps_execution] %.
        *)Si le processus précédent a été interrompu par un signal, il affiche le numéro de signal entre crochets [sign:numéro_signal|temps_execution] %.

    -Saisie et exécution des commandes :
        *)L'utilisateur saisit une commande, et le shell la lit à l'aide de read().
        *)Si l'utilisateur saisit "exit" ou utilise Ctrl+d (EOF), le programme affiche "Bye bye..." et se termine.
        *)Sinon, le shell crée un nouveau processus avec fork() pour exécuter la commande saisie.
        *)Dans le processus enfant, il exécute la commande saisie avec execlp(buf, buf, NULL):
            -->S'il y a une erreur lors de l'exécution de la commande, il affiche un message d'erreur avec perror() et termine le processus enfant avec exit().
        *)Dans le processus parent, il attend la fin du processus enfant avec waitpid() et récupère le statut de sortie de ce processus.
        *)Il mesure le temps d'exécution de la commande à l'aide de clock_gettime() avant et après l'exécution.

#6) TpShell_Q6
    -Prompt et saisie de commandes :
        *)L'utilisateur voit un prompt initial enseash % ou un prompt incluant le code de retour / signal précédent.
        *)Les commandes peuvent être entrées, y compris celles avec des arguments.

    -Gestion du code de retour / signal :
        *)Avant de lire une nouvelle commande, il vérifie le statut du processus précédent (previousStatus).
        *)Si le processus précédent s'est terminé normalement, il affiche le code de retour entre crochets [exit:code_retour|temps_execution] %.
        *)Si le processus précédent a été interrompu par un signal, il affiche le numéro de signal entre crochets [sign:numéro_signal|temps_execution] %.

    -Saisie et exécution des commandes :
        *)L'utilisateur saisit une commande, potentiellement avec des arguments.
        *)Le shell sépare la commande et ses arguments en utilisant strtok() pour obtenir chaque partie de la commande.
        *)Il exécute la commande avec execvp() dans un processus enfant créé par fork().
        *)S'il y a une erreur lors de l'exécution de la commande, il affiche un message d'erreur avec perror() et termine le processus enfant avec exit().

    -Mesure du temps d'exécution :
        *)Le script utilise clock_gettime() pour mesurer le temps de début et de fin de l'exécution de chaque commande.

#7) TpShell_Q7
    -Prompt et saisie de commandes :
        *)Affiche un prompt initial enseash % ou un prompt incluant le code de retour / signal précédent.
        *)Attend l'entrée de commandes de l'utilisateur.

    -Gestion du code de retour / signal :
        *)Avant de lire une nouvelle commande, vérifie le statut du processus précédent (previousStatus).
        *)Affiche le code de retour entre crochets [exit:code_retour|temps_execution] % si le processus précédent s'est terminé normalement.
        *)Si le processus précédent a été interrompu par un signal, affiche le numéro de signal entre crochets [sign:numéro_signal|temps_execution] %.

    -Saisie et exécution des commandes avec redirections :
        *)L'utilisateur saisit une commande.
        *)Si des redirections sont détectées (< pour l'entrée, > pour la sortie) :
            -->Pour la redirection de la sortie (>), redirige la sortie standard vers un fichier spécifié.
            -->Pour la redirection de l'entrée (<), redirige l'entrée standard depuis un fichier spécifié.
            -->Exécute ensuite la commande saisie avec les redirections éventuelles.

    -Mesure du temps d'exécution :
        *)Utilise clock_gettime() pour mesurer le temps de début et de fin de chaque commande.

#8) TpShell_Q8
    -Prompt et saisie de commandes :
        *)Affiche un prompt initial enseash % ou un prompt incluant le code de retour / signal précédent.
        *)Attend l'entrée de commandes de l'utilisateur.

    -Gestion du code de retour / signal :
        *)Avant de lire une nouvelle commande, vérifie le statut du processus précédent (previousStatus).
        *)Affiche le code de retour entre crochets [exit:code_retour|temps_execution] % si le processus précédent s'est terminé normalement.
        *)Si le processus précédent a été interrompu par un signal, affiche le numéro de signal entre crochets [sign:numéro_signal|temps_execution] %.

    -Saisie et exécution des commandes avec redirections :
        *)L'utilisateur saisit une commande.
        *)Si des redirections sont détectées (< pour l'entrée, > pour la sortie), elles sont traitées comme dans les versions précédentes du shell.
        *)Pour les pipes (|), le shell divise la commande en deux parties, séparées par le pipe.
        *)Il crée un tube (pipe()) pour la communication entre les deux parties.

    -Exécution des deux parties de la commande :
        *)Le processus parent attend que les deux parties de la commande se terminent (waitpid()).
        *)La sortie du premier processus est redirigée vers l'entrée du second processus via le tube (dup2()).
        *)Chaque partie de la commande est exécutée à l'aide de execvp().

