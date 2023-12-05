#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#define BUF_SIZE 100 					// Taille attendue de l'entrée

int main() {
    char buf[BUF_SIZE];
    ssize_t readNb; 					// Nombre de caractères lus
    int previousStatus = 0; 				// Code de retour précédent initialisé à 0

    struct timespec startTime, endTime; 		// Structures pour stocker le temps de début et de fin

    while (1) {
        char prompt[50]; 				// Chaîne pour le prompt
        int promptLength;

        // Affichage du prompt initial ou des résultats précédents
        if (previousStatus == 0) {
            write(1, "enseash % ", 11); 		// Affiche le prompt initial
        }

	else {
            // Si le processus précédent s'est terminé normalement
            if (WIFEXITED(previousStatus)) {
                int exitStatus = WEXITSTATUS(previousStatus);
                char exitStatusStr[30];
                // Formatage du prompt avec le code de retour et le temps d'exécution
                promptLength = sprintf(exitStatusStr, "[exit:%d|%ldms] %% ", exitStatus, (endTime.tv_nsec - startTime.tv_nsec) / 1000000);
                write(1, "enseash ", 8);
                write(1, exitStatusStr, promptLength); 	// Affiche le code de retour avec le temps d'exécution
            }
		
	    else {
                // Si le processus précédent s'est terminé par un signal
                int signalNum = WTERMSIG(previousStatus);
                char signalNumStr[30];
                // Formatage du prompt avec le signal et le temps d'exécution
                promptLength = sprintf(signalNumStr, "[sign:%d|%ldms] %% ", signalNum, (endTime.tv_nsec - startTime.tv_nsec) / 1000000);
                write(1, "enseash ", 8);
                write(1, signalNumStr, promptLength); 	// Affiche le signal avec le temps d'exécution
            }
        }

        // Lecture de la commande de l'utilisateur
        readNb = read(0, buf, BUF_SIZE);
        if (readNb == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

	else if (readNb == 0) { 			// Si l'utilisateur tape Ctrl+d (EOF)
            write(1, "\nBye bye...\n", 12);
            break;
        }

        buf[readNb - 1] = '\0'; 			// Remplace le saut de ligne par le caractère de fin de chaîne

        if (strcmp(buf, "exit") == 0) { 		// Si l'utilisateur saisit "exit"
            write(1, "\nBye bye...\n", 12);
            break;
        }

        // Mesure du temps de début d'exécution
        clock_gettime(CLOCK_MONOTONIC, &startTime);

        pid_t pid = fork(); 				// Crée un nouveau processus
        if (pid == 0) {
            // Processus enfant

            // Gestion des redirections (entrée/sortie)
            char *outputRedirect; 
            char *inputRedirect;  
            char *argv[BUF_SIZE / 2] = { NULL };
            int i = 0;
            int fd;

            // Traitement de la redirection de la sortie
            outputRedirect = strtok(buf, ">");
            inputRedirect = strtok(NULL, ">");
            if (inputRedirect != NULL) {
                fd = open(inputRedirect, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
                if (fd == -1) {
                    perror("Erreur lors de l'ouverture du fichier en sortie");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO); 		// Redirige la sortie standard vers le fichier
                close(fd);
                strcpy(buf, outputRedirect); 		// Modifie le buffer pour exclure la partie de redirection
            }

            // Traitement de la redirection de l'entrée
            outputRedirect = strtok(buf, "<");
	    inputRedirect = strtok(NULL, "<");
	    if (inputRedirect != NULL) {
    		inputRedirect = strtok(inputRedirect, " \t\n"); 	// Supprimer les espaces en début/fin
   		fd = open(inputRedirect, O_RDONLY);
    		if (fd == -1) {
        	perror("Erreur lors de l'ouverture du fichier en entrée");
        	exit(EXIT_FAILURE);
    	     	}
    	     	dup2(fd, STDIN_FILENO); 		// Redirige l'entrée standard depuis le fichier
    		close(fd);
    		strcpy(buf, outputRedirect); 		// Modifie le buffer pour exclure la partie de redirection
	    }

            // Séparation des arguments pour l'exécution
            argv[0] = strtok(buf, " ");
            while (argv[i] != NULL) {
                i++;
                argv[i] = strtok(NULL, " ");
            }

            // Exécution de la commande
            execvp(argv[0], argv);
            // En cas d'échec de l'exécution, affiche un message d'erreur
            write(STDOUT_FILENO, "Erreur lors de l'exécution de la commande.\n", strlen("Erreur lors de l'exécution de la commande.\n"));
            exit(EXIT_FAILURE);
        }

	else {
            // Processus parent
            waitpid(pid, &previousStatus, 0); 		// Attend la fin du processus enfant
            // Mesure du temps de fin d'exécution
            clock_gettime(CLOCK_MONOTONIC, &endTime);
        }
    }

    return 0;
}

