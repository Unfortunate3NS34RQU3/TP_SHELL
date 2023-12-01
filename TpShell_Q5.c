#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define BUF_SIZE 100 // Taille attendue de l'entrée

void main() {
    char buf[BUF_SIZE];
    ssize_t readNb; // Nombre de caractères lus
    int previousStatus = 0; // Code de retour précédent initialisé à 0

    struct timespec startTime, endTime; // Structures pour stocker le temps de début et de fin

    while (1) {
        char prompt[50]; // Chaîne pour le prompt
        int promptLength;

        // Affichage du prompt initial ou avec le code de retour / signal précédent
        if (previousStatus == 0) {
            write(1, "enseash % ", 11); // Affiche le prompt initial
        } else {
            if (WIFEXITED(previousStatus)) {
                int exitStatus = WEXITSTATUS(previousStatus);
                char exitStatusStr[30];
                // Formatage du prompt avec le code de retour et le temps d'exécution
                promptLength = sprintf(exitStatusStr, "[exit:%d|%ldms] %% ", exitStatus, (endTime.tv_nsec - startTime.tv_nsec) / 1000000);
                write(1, "enseash ", 8);
                write(1, exitStatusStr, promptLength); 	// Affiche le code de retour avec le temps d'exécution
            } else {
                int signalNum = WTERMSIG(previousStatus);
                char signalNumStr[30];
                // Formatage du prompt avec le signal et le temps d'exécution
                promptLength = sprintf(signalNumStr, "[sign:%d|%ldms] %% ", signalNum, (endTime.tv_nsec - startTime.tv_nsec) / 1000000);
                write(1, "enseash ", 8);
                write(1, signalNumStr, promptLength); 	// Affiche le signal avec le temps d'exécution
            }
        }

        readNb = read(0, buf, BUF_SIZE);
        if (readNb == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        } else if (readNb == 0) { 			// Si l'utilisateur tape Ctrl+d (EOF)
            write(1, "\nBye bye...\n", 12);
            break;
        }

        buf[readNb - 1] = '\0'; 			// Remplace le saut de ligne par le caractère de fin de chaîne

        if (strcmp(buf, "exit") == 0) { 		// Si l'utilisateur saisit "exit"
            write(1, "\nBye bye...\n", 12);
            break;
        }

       
        clock_gettime(CLOCK_MONOTONIC, &startTime); 	// Mesure du temps de début d'exécution
        pid_t pid = fork(); 				// Création d'un nouveau processus
        if (pid == 0) {
            execlp(buf, buf, (char *)NULL);		// Exécute la commande saisie
            perror("execlp"); 				// Gère l'erreur si l'exécution échoue
            exit(EXIT_FAILURE); 			// Termine le processus enfant en cas d'échec
        } else {
            waitpid(pid, &previousStatus, 0); 		// Attend la fin du processus enfant et récupère le statut
            clock_gettime(CLOCK_MONOTONIC, &endTime); 	// Mesure du temps de fin d'exécution
        }
    }
}

