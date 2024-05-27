#include <iostream>
#include <cstdlib>

enum commands {
    HELP = 0,
    APPEND = 1,
    START_NEW_LINE = 2,
    SAVE = 3,
    LOAD = 4,
    PRINT = 5,
    INSERT_BY_INDEX = 6,
    SEARCH = 7,
    CLEAR = 8,
    EXIT = 9
};

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
           "8 - Clear console.\n"
           "9 - Exit the program.\n");

}

char *allInputs = NULL;
size_t bufferInput = 128;

void commandValidation(int *command) {
    printf("\nEnter the command you want to use: ");
    while (true) {
        if (scanf("%d", command) == 1 && HELP <= *command && *command <= EXIT) {
            printf("Your command is: %d\n", *command);
            break;
        }
        printf("Invalid input! Enter a number from 0 to 9: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
}

void appendText() {
    printf("Enter the text you want to append: ");
    size_t capacity = bufferInput;
    char *userInput = (char *)malloc(capacity * sizeof(char));
    if (!userInput) {
        printf("Memory allocation failed.\n");
        return;
    }

    if (fgets(userInput, capacity, stdin) != NULL) {
        size_t len = strlen(userInput);
        if (userInput[len - 1] == '\n') {
            userInput[len - 1] = '\0';
            len--;
        }

        if (allInputs == NULL) {
            allInputs = (char *)malloc((len + 1) * sizeof(char));
            if (!allInputs) {
                printf("Memory allocation failed.\n");
                free(userInput);
                return;
            }
            strcpy(allInputs, userInput);
        } else {
            size_t newSize = strlen(allInputs) + len + 1;
            char *temp = (char *)realloc(allInputs, newSize * sizeof(char));
            if (!temp) {
                printf("Memory reallocation failed.\n");
                free(userInput);
                return;
            }
            allInputs = temp;
            strcat(allInputs, userInput);
        }
        printf("The text was successfully appended.\n");
    } else {
        printf("Error while reading input.\n");
    }
    free(userInput);
}

void newLine(){
    bufferInput *= 2;

    allInputs = (char *) realloc(allInputs, bufferInput * sizeof(char));
    if (!allInputs) {
        printf("Memory reallocation failed.\n");
        return;
    }
    strcat(allInputs, "\n");
    printf("The new line has been started.\n");
}

char filename[50];

void saveToFile() {
    FILE* file;
    printf("Enter the name of the file for saving: ");
    scanf("%s", filename);
    file = fopen(filename, "w");
    if (!file) {
        perror("Error while opening the file.\n");
        return;
    }

    fprintf(file, "%s", allInputs);
    fclose(file);

    printf("The text was successfully saved to %s \n", filename);
}

void loadFromFile() {
    printf("Enter the name of the file for loading: ");
    FILE* fileLoad;
    scanf("%s", filename);
    fileLoad = fopen(filename, "r");
    if (!fileLoad) {
        perror("Error while opening the file.\n");
        return;
    }
    char buffer[1000];
    if (fgets(buffer, sizeof(buffer), fileLoad) != NULL) {
        printf("Text successfully loaded from %s:\n %s\n", filename, buffer);
    } else {
        printf("Error reading file content\n");
    }

    fclose(fileLoad);
    printf("The load was successfully done.\n");
}

void printText() {
    if (allInputs) {
        printf("Your text is: %s\n", allInputs);
    } else {
        printf("There is no text.\n");
    }
}

void insertByIndex() {

}

void searchText() {

}

void clearConsole() {

}

void getCommand(int command) {
    switch (command) {
        case HELP:
            commandPrompt();
            break;
        case APPEND:
            appendText();
            break;
        case START_NEW_LINE:
            newLine();
            break;
        case SAVE:
            saveToFile();
            break;
        case LOAD:
            loadFromFile();
            break;
        case PRINT:
            printText();
            break;
        case INSERT_BY_INDEX:
            printf("Command has not been implemented yet.\n");
            break;
        case SEARCH:
            printf("Command has not been implemented yet.\n");
            break;
        case CLEAR:
            printf("Command has not been implemented yet.\n");
            break;
        case EXIT:
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
    } while (command != EXIT);
    return 0;
}
