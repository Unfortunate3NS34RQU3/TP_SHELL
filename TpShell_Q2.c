#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 100 					// Taille attendue de l'entrée

void main() {
    char buf[BUF_SIZE];
    ssize_t readNb;					//nombre de caractères lus

    while (1) {
        write(1, "enseash % ", 11); 			// Affiche le prompt, 1 : sortie standard, et 11 : nombre de caractères à écrire à savoir 10 pour la 							// chaîne "enseash % " + le caractère de fin de chaine "\0"  
        
        readNb = read(0, buf, BUF_SIZE); 		// Lit la commande de l'utilisateur
        if (readNb == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        buf[readNb - 1] = '\0'; 			// Remplace le saut de ligne par le caractère de fin de chaîne

        pid_t pid = fork(); 				// Crée un nouveau processus
        if  (pid == 0) {

            // Processus enfant
            execlp(buf, buf, (char *)NULL); 		// Exécute la commande saisie
            perror("execlp"); 				// Gère l'erreur si l'execution échoue
            exit(EXIT_FAILURE); 			// Termine le processus enfant en cas d'échec
        } 
	
	else {
            // Processus parent
            waitpid(pid, NULL, 0);			// Attend la fin du processus enfant 
        }
    }
}

