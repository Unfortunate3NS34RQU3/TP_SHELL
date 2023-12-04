#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#define BUF_SIZE 100 					// Taille attendue de l'entrée

void main() {
    char buf[BUF_SIZE];
    ssize_t readNb; 					// Nombre de caractères lus
    int previousStatus = 0; 				// Code de retour précédent initialisé à 0

    struct timespec startTime, endTime; 		// Structures pour stocker le temps de début et de fin

    while (1) {
        char prompt[50]; 				// Chaîne pour le prompt
        int promptLength;

        if (previousStatus == 0) {
            write(1, "enseash % ", 11); 		// Affiche le prompt initial
        } 

	else {
            if (WIFEXITED(previousStatus)) {
                int exitStatus = WEXITSTATUS(previousStatus);
                char exitStatusStr[30];
                // Formatage du prompt avec le code de retour et le temps d'exécution
                promptLength = sprintf(exitStatusStr, "[exit:%d|%ldms] %% ", exitStatus, (endTime.tv_nsec - startTime.tv_nsec) / 1000000);
                write(1, "enseash ", 8);
                write(1, exitStatusStr, promptLength); 	// Affiche le code de retour avec le temps d'exécution
            }

	else {
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
        

        int input_redirect = 0, output_redirect = 0;
        char *input_file = NULL, *output_file = NULL; 	//seront utilisés pour stocker les descripteurs de fichiers de sortie et d'entrée.
        
        //Ces lignes cherchent les caractères '>' et '<' dans la commande saisie par l'utilisateur (buf). 
        char *ptr = buf;
        while (*ptr) {
            if (*ptr == '<') {
                input_redirect = 1;
                *ptr++ = ' ';
                while (*ptr == ' ') {
                    ptr++;
                }
                input_file = ptr;
            } else if (*ptr == '>') {
                output_redirect = 1;
                *ptr++ = ' ';
                while (*ptr == ' ') {
                    ptr++;
                }
                output_file = ptr;
            } else {
                ptr++;
            }
        }
             
            pid_t pid = fork();
            if (pid == 0) {
                // Processus enfant
                
                if (input_redirect) { 				// Gestion de la redirection de l'entrée
                int input_fd = open(input_file, O_RDONLY); 	// Ouvre le fichier en lecture seule.
                if (input_fd == -1) {
                    perror("Erreur lors de l'ouverture du fichier en entrée");
                    exit(EXIT_FAILURE);
                }
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            if (output_redirect) { 				//Gestion de la redirection de la sortie
                int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0777); // Crée ou ouvre le fichier en écriture (O_WRONLY) en écrasant le 									//contenu existant (O_TRUNC) ou en créant le fichier s'il n'existe pas (O_CREAT).
                
		if (output_fd == -1) {
                    perror("Erreur lors de l'ouverture du fichier en sortie");
                    exit(EXIT_FAILURE);
                }
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            
            
            	// S'il n'y a pas de redirection dans la commande d'utilisateur
                char *token;
                char *arguments[BUF_SIZE];
                int argCount = 0;

                token = strtok(buf, " ");
                while (token != NULL) {
                    arguments[argCount++] = token;
                    token = strtok(NULL, " ");
                }
                
                arguments[argCount] = NULL;
                //execlp(argv[1],argv[1],argv[2], NULL);
                execvp(arguments[0], arguments);
                perror("Probleme execvp");
                exit(EXIT_FAILURE);
            } else {
                // Processus parent
                waitpid(pid, &previousStatus, 0);
                // Mesure du temps de fin d'exécution
                clock_gettime(CLOCK_MONOTONIC, &endTime);
            }
        
    }
}

