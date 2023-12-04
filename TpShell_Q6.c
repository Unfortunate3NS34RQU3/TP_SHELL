#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define BUF_SIZE 100 						// Taille attendue de l'entrée

void main() {
    char buf[BUF_SIZE];
    ssize_t readNb; 						// Nombre de caractères lus
    int previousStatus = 0; 					// Code de retour précédent initialisé à 0
    struct timespec startTime, endTime; 			// Structures pour stocker le temps de début et de fin

    while (1) {
        char prompt[50]; 					// Chaîne pour le prompt
        int promptLength;

        if (previousStatus == 0) {
            write(1, "enseash % ", 11); 			// Affiche le prompt initial
        }

	else {
            if (WIFEXITED(previousStatus)) {
                int exitStatus = WEXITSTATUS(previousStatus);
                char exitStatusStr[30];
                // Formatage du prompt avec le code de retour et le temps d'exécution
                promptLength = sprintf(exitStatusStr, "[exit:%d|%ldms] %% ", exitStatus, (endTime.tv_nsec - startTime.tv_nsec) / 1000000);
                write(1, "enseash ", 8);
                write(1, exitStatusStr, promptLength); 		// Affiche le code de retour avec le temps d'exécution
            } 

	    else {
                int signalNum = WTERMSIG(previousStatus);
                char signalNumStr[30];
                // Formatage du prompt avec le signal et le temps d'exécution
                promptLength = sprintf(signalNumStr, "[sign:%d|%ldms] %% ", signalNum, (endTime.tv_nsec - startTime.tv_nsec) / 1000000);
                write(1, "enseash ", 8);
                write(1, signalNumStr, promptLength); 		// Affiche le signal avec le temps d'exécution
            }
        }

        // Lecture de la commande de l'utilisateur
        readNb = read(0, buf, BUF_SIZE);
        if (readNb == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        } 

	else if (readNb == 0) { 				// Si l'utilisateur tape Ctrl+d (EOF)
            write(1, "\nBye bye...\n", 12);
            break;
        }

        buf[readNb - 1] = '\0'; 				// Remplace le saut de ligne par le caractère de fin de chaîne

        if (strcmp(buf, "exit") == 0) { 			// Si l'utilisateur saisit "exit"
            write(1, "\nBye bye...\n", 12);
            break;
        }

        // Mesure du temps de début d'exécution
        clock_gettime(CLOCK_MONOTONIC, &startTime);

        // Création d'un nouveau processus
        pid_t pid = fork();
        if (pid == 0) {
            // Processus enfant
            char *token; 					// Un pointeur "token" pour stocker chaque partie de la commande séparée par des espaces
            char *arguments[BUF_SIZE]; 				// Un tableau "arguments" pour stocker les différents morceaux de la commande
            int argCount = 0; 					// Un compteur "argCount" pour suivre le nombre d'arguments.

            // Séparation de la commande et de ses arguments
            token = strtok(buf, " "); 				// Ici la chaîne saisie par l'utilisateur "buf" est divisée en morceaux (tokens) en 									// utilisant l'espace comme délimiteur

            while (token != NULL) {
                arguments[argCount++] = token;
                token = strtok(NULL, " ");
            }
            
            arguments[argCount] = NULL; 			// Pour s'assurer que execvp puisse reconnaître la fin des arguments, nous plaçons un 									// marqueur NULL à la fin du tableau.
            
            execvp(arguments[0], arguments); 			// Exécute la commande avec ses arguments
            perror("execvp"); 					// Gère l'erreur si l'exécution échoue
            exit(EXIT_FAILURE); 				// Termine le processus enfant en cas d'échec
        } 
	
	else {
            // Processus parent
            waitpid(pid, &previousStatus, 0); 			// Attend la fin du processus enfant et récupère le statut
            // Mesure du temps de fin d'exécution
            clock_gettime(CLOCK_MONOTONIC, &endTime);
        }
    }
}

