#include <iostream>

void commandPrompt() {
    printf("\nChoose what option do you want to use:\n"
           "0 - Help.\n"
           "1 - Append text to the end.\n"
           "2 - Start a new line.\n"
           "3 - Save text to a file.\n"
           "4 - Load text from a file.\n"
           "5 - Print text to console.\n"
           "6 - Insert text by line and index.\n"
           "7 - Search the text.\n"
           "8 - Insert a substring.\n"
           "9 - Exit the program\n");

}

void commandValidation(int *command) {
    printf("\nEnter the command you want to use: ");
    while (true) {
        if (scanf("%d", command) == 1 && 0 <= *command <= 9) {
            break;
        }
        printf("Invalid input! Enter a command from 0 to 9: ");
        while (getchar() != '\n');
    }
}

void getCommand(int command) {
    switch (command) {
        case 0:
            commandPrompt();
            break;
        case 1:
            printf("Command has not been implemented yet.\n");
            break;
        case 2:
            printf("Command has not been implemented yet.\n");
            break;
        case 3:
            printf("Command has not been implemented yet.\n");
            break;
        case 4:
            printf("Command has not been implemented yet.\n");
            break;
        case 5:
            printf("Command has not been implemented yet.\n");
            break;
        case 6:
            printf("Command has not been implemented yet.\n");
            break;
        case 7:
            printf("Command has not been implemented yet.\n");
            break;
        case 8:
            printf("Command has not been implemented yet.\n");
            break;
        case 9:
            printf("Exiting the program...\n");
            break;
        default:
            printf("Oops! Write a correct command from 0 to 9.\n");
    }
}

int main() {
    int command;
    commandPrompt();
    do {
        commandValidation(&command);
        getCommand(command);
    } while (command != 9);
    return 0;
}
