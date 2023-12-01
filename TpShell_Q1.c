#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Welcome message
void displayWelcomeMessage() {
    char *message1 = "Welcome to the ENSEA Shell\n";
    char *message2 = "To exit, type 'exit'\n";
    char *message3 = "enseash % \n";

    write(1, message1, strlen(message1));
    write(1, message2, strlen(message2));
    write(1, message3, strlen(message3));
}

void main() {
    displayWelcomeMessage();
}

