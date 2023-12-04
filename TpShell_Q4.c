#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 100 					// Taille attendue de l'entrée

void main() {
    char buf[BUF_SIZE];
    ssize_t readNb; 					// Nombre de caractères lus
    int previousStatus = 0; 				// Code de retour précédent initialisé à 0

    while (1) {
        char prompt[30];
        int promptLength;

        if (previousStatus == 0) {
            write(1, "enseash % ", 11); 		// Affiche le prompt initial + a chaque fois la commande précedante est correcte
        }

	else {
        
            if (WIFEXITED(previousStatus)) { 		// Ici si par exemple le fichier n'existe pas 
                int exitStatus = WEXITSTATUS(previousStatus);
                char exitStatusStr[20];
                promptLength = sprintf(exitStatusStr, "[exit:%d] %%", exitStatus);
                write(1, "enseash ", 8);
                write(1, exitStatusStr, promptLength); 	// Affiche le code de retour
                
            }

	else {  					// Ici si un signal fais l'interruption on envoie son numero
                int signalNum = WTERMSIG(previousStatus);
                char signalNumStr[20];
                promptLength = sprintf(signalNumStr, "[sign:%d] %% ", signalNum);
                write(1, "enseash ", 8);
                write(1, signalNumStr, promptLength); 	// Affiche le signal
            }
        }

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

        pid_t pid = fork(); 				// Crée un nouveau processus
        if (pid == 0) {

            // Processus enfant
            execlp(buf, buf, (char *)NULL); 		// Exécute la commande saisie
            perror("execlp"); 				// Gère l'erreur si l'exécution échoue
            exit(EXIT_FAILURE); 			// Termine le processus enfant en cas d'échec
            //kill(0,SIGINT);
        }

	else {
            // Processus parent
            waitpid(pid, &previousStatus, 0); 		// Attend la fin du processus enfant et récupère le statut
        }
    }
}

