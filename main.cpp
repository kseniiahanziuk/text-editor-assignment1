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
    DELETE = 8,
    UNDO = 9,
    REDO = 10,
    CUT = 11,
    COPY = 12,
    PASTE = 13,
    INSERT_WITH_REPLACEMENT = 14,
    EXIT = 15
};

char *allInputs = NULL;
size_t bufferInput = 128;
char filename[50];
char *undoBuffer = NULL;
char *redoBuffer = NULL;
char *clipboardBuffer = NULL;

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
           "8 - Delete text by line, index and number of symbols.\n"
           "9 - Undo a command.\n"
           "10 - Redo a command.\n"
           "11 - Cut the text.\n"
           "12 - Copy the text.\n"
           "13 - Paste the text.\n"
           "14 - Insert with replacement.\n"
           "15 - Exit the program.\n");
}

void commandValidation(int *command) {
    printf("\nEnter the command you want to use: ");
    while (true) {
        if (scanf("%d", command) == 1 && HELP <= *command && *command <= EXIT) {
            printf("Your command is: %d\n", *command);
            break;
        }
        printf("Invalid input! Enter a number from 0 to 15: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
}

void saveState() {
    if (allInputs) {
        if (undoBuffer) {
            free(undoBuffer);
        }
        undoBuffer = (char *)malloc((strlen(allInputs) + 1) * sizeof(char));
        if (undoBuffer) {
            strcpy(undoBuffer, allInputs);
        } else {
            printf("Memory allocation failed for undo buffer.\n");
        }
    } else {
        if (undoBuffer) {
            free(undoBuffer);
        }
        undoBuffer = NULL;
    }
}

void appendText() {
    saveState();
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
    saveState();
    bufferInput *= 2;

    allInputs = (char *) realloc(allInputs, bufferInput * sizeof(char));
    if (!allInputs) {
        printf("Memory reallocation failed.\n");
        return;
    }
    strcat(allInputs, "\n");
    printf("The new line has been started.\n");
}

void saveToFile() {
    saveState();
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

    printf("Text successfully loaded from %s:\n", filename);
    while (fgets(buffer, sizeof(buffer), fileLoad) != NULL) {
        printf("%s\n", buffer);
    }

    fclose(fileLoad);
}

void printText() {
    if (allInputs) {
        printf("Your text is: %s\n", allInputs);
    } else {
        printf("There is no text.\n");
    }
}

void insertByIndex(const char *filename, int line, int index, char *textToInsert) {
    saveState();
    printf("Enter filename: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize + 1);
    if (!buffer) {
        printf("Memory allocation failed.");
        fclose(file);
        return;
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file);

    printf("Enter line number: ");
    scanf("%d", &line);

    int currentLine = 1;
    int charCount = 0;
    while (currentLine < line && buffer[charCount] != '\0') {
        if (buffer[charCount] == '\n') {
            currentLine++;
        }
        charCount++;
    }

    if (currentLine < line) {
        printf("Error! This line number exceeds the number of lines in the file.\n");
        free(buffer);
        return;
    }

    int lineStart = charCount;
    while (buffer[charCount] != '\n' && buffer[charCount] != '\0') {
        charCount++;
    }
    int lineLength = charCount - lineStart;

    printf("Enter index: ");
    scanf("%d", &index);

    if (index > lineLength) {
        printf("Error! This index exceeds the length of the line.\n");
        free(buffer);
        return;
    }

    printf("Enter text to insert: ");
    getchar();
    fgets(textToInsert, sizeof(textToInsert), stdin);
    textToInsert[strcspn(textToInsert, "\n")] = '\0';

    size_t newSize = fileSize + strlen(textToInsert);
    char *newBuffer = (char *)malloc(newSize + 1);
    if (!newBuffer) {
        printf("Memory allocation failed.");
        free(buffer);
        return;
    }

    strncpy(newBuffer, buffer, lineStart + index);
    newBuffer[lineStart + index] = '\0';
    strcat(newBuffer, textToInsert);
    strcat(newBuffer, buffer + lineStart + index);

    file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.");
        free(buffer);
        free(newBuffer);
        return;
    }
    fwrite(newBuffer, 1, newSize, file);
    fclose(file);

    free(buffer);
    allInputs = (char *)realloc(allInputs, newSize + 1);
    if (!allInputs) {
        printf("Memory allocation failed.\n");
        free(newBuffer);
        return;
    }
    strcpy(allInputs, newBuffer);
    free(newBuffer);

    printf("Text successfully inserted into %s at line %d, index %d\n", filename, line, index);
}

void searchText() {
    char filename[256];
    char searchText[256];

    printf("Enter filename: ");
    scanf("%s", filename);

    printf("Enter text to search: ");
    getchar();
    fgets(searchText, sizeof(searchText), stdin);
    searchText[strcspn(searchText, "\n")] = '\0';

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize + 1);
    if (!buffer) {
        printf("Memory allocation failed.");
        fclose(file);
        return;
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file);

    int searchTextLen = strlen(searchText);
    int found = 0;
    int line = 1;
    int column = 1;

    for (char *pos = buffer; *pos != '\0'; ++pos) {
        if (strncmp(pos, searchText, searchTextLen) == 0) {
            found = 1;
            printf("Text '%s' found at line %d, index %d.\n", searchText, line, column);
        }

        if (*pos == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }

    if (!found) {
        printf("Text '%s' not found in the file.\n", searchText);
    }

    free(buffer);
}

void deleteText() {
    saveState();
    int line, index, length;
    printf("Choose the line: ");
    scanf("%d", &line);

    printf("Choose the index: ");
    scanf("%d", &index);

    printf("Choose the number of symbols to delete: ");
    scanf("%d", &length);

    if (!allInputs) {
        printf("There is no text to delete from.\n");
        return;
    }

    int currentLine = 1;
    int charCount = 0;
    while (currentLine < line && allInputs[charCount] != '\0') {
        if (allInputs[charCount] == '\n') {
            currentLine++;
        }
        charCount++;
    }

    if (currentLine < line) {
        printf("Error! This line number exceeds the number of lines in the text.\n");
        return;
    }

    int lineStart = charCount;
    while (allInputs[charCount] != '\n' && allInputs[charCount] != '\0') {
        charCount++;
    }
    int lineLength = charCount - lineStart;

    if (index > lineLength) {
        printf("Error! This index exceeds the length of the line.\n");
        return;
    }

    if (index + length > lineLength) {
        length = lineLength - index;
    }

    int i = lineStart + index;
    while (allInputs[i + length] != '\0') {
        allInputs[i] = allInputs[i + length];
        i++;
    }
    allInputs[i] = '\0';

    printf("Text successfully deleted from line %d, index %d, length %d\n", line, index, length);
}

void undoCommand() {
    if (undoBuffer) {
        if (redoBuffer) {
            free(redoBuffer);
        }
        redoBuffer = (char *)malloc((allInputs ? strlen(allInputs) : 0) + 1);
        if (redoBuffer) {
            if (allInputs) {
                strcpy(redoBuffer, allInputs);
            } else {
                redoBuffer[0] = '\0';
            }
        } else {
            printf("Memory allocation failed for redo buffer.\n");
            return;
        }

        if (allInputs) {
            free(allInputs);
        }
        allInputs = (char *)malloc((strlen(undoBuffer) + 1) * sizeof(char));
        if (allInputs) {
            strcpy(allInputs, undoBuffer);
            printf("Undo has been successful.\n");
        } else {
            printf("Memory allocation failed for undo operation.\n");
        }
        free(undoBuffer);
        undoBuffer = NULL;
    } else {
        printf("No commands to undo.\n");
    }
}

void redoCommand() {
    if (redoBuffer) {
        saveState();
        if (allInputs) {
            free(allInputs);
        }
        allInputs = (char *)malloc((strlen(redoBuffer) + 1) * sizeof(char));
        if (allInputs) {
            strcpy(allInputs, redoBuffer);
            printf("Redo has been successful.\n");
        } else {
            printf("Memory allocation failed for redo operation.\n");
        }
        free(redoBuffer);
        redoBuffer = NULL;
    } else {
        printf("No commands to redo.\n");
    }
}

void cutText() {
    saveState();
    int line, index, length;
    printf("Choose the line: ");
    scanf("%d", &line);

    printf("Choose the index: ");
    scanf("%d", &index);

    printf("Choose the number of symbols to cut: ");
    scanf("%d", &length);

    if (!allInputs) {
        printf("There is no text to cut from.\n");
        return;
    }

    int currentLine = 1;
    int charCount = 0;
    while (currentLine < line && allInputs[charCount] != '\0') {
        if (allInputs[charCount] == '\n') {
            currentLine++;
        }
        charCount++;
    }

    if (currentLine < line) {
        printf("Error! This line number exceeds the number of lines in the text.\n");
        return;
    }

    int lineStart = charCount;
    while (allInputs[charCount] != '\n' && allInputs[charCount] != '\0') {
        charCount++;
    }
    int lineLength = charCount - lineStart;

    if (index > lineLength) {
        printf("Error! This index exceeds the length of the line.\n");
        return;
    }

    if (index + length > lineLength) {
        length = lineLength - index;
    }

    clipboardBuffer = (char *)malloc((length + 1) * sizeof(char));
    if (!clipboardBuffer) {
        printf("Memory allocation failed for clipboard.\n");
        return;
    }
    strncpy(clipboardBuffer, &allInputs[lineStart + index], length);
    clipboardBuffer[length] = '\0';

    int i = lineStart + index;
    while (allInputs[i + length] != '\0') {
        allInputs[i] = allInputs[i + length];
        i++;
    }
    allInputs[i] = '\0';

    printf("Text successfully cut from line %d, index %d, length %d\n", line, index, length);
}

void copyText() {
    int line, index, length;
    printf("Choose the line: ");
    scanf("%d", &line);

    printf("Choose the index: ");
    scanf("%d", &index);

    printf("Choose the number of symbols to copy: ");
    scanf("%d", &length);

    if (!allInputs) {
        printf("There is no text to copy from.\n");
        return;
    }

    int currentLine = 1;
    int charCount = 0;
    while (currentLine < line && allInputs[charCount] != '\0') {
        if (allInputs[charCount] == '\n') {
            currentLine++;
        }
        charCount++;
    }

    if (currentLine < line) {
        printf("Error! This line number exceeds the number of lines in the text.\n");
        return;
    }

    int lineStart = charCount;
    while (allInputs[charCount] != '\n' && allInputs[charCount] != '\0') {
        charCount++;
    }
    int lineLength = charCount - lineStart;

    if (index > lineLength) {
        printf("Error! This index exceeds the length of the line.\n");
        return;
    }

    if (index + length > lineLength) {
        length = lineLength - index;
    }

    clipboardBuffer = (char *)malloc((length + 1) * sizeof(char));
    if (!clipboardBuffer) {
        printf("Memory allocation failed for clipboard.\n");
        return;
    }
    strncpy(clipboardBuffer, &allInputs[lineStart + index], length);
    clipboardBuffer[length] = '\0';

    printf("Text successfully copied from line %d, index %d, length %d\n", line, index, length);
}

void pasteText() {
    saveState();
    int line, index;
    printf("Choose the line: ");
    scanf("%d", &line);

    printf("Choose the index: ");
    scanf("%d", &index);

    if (!clipboardBuffer) {
        printf("Clipboard is empty.\n");
        return;
    }

    if (!allInputs) {
        printf("There is no text to paste into.\n");
        return;
    }

    int currentLine = 1;
    int charCount = 0;
    while (currentLine < line && allInputs[charCount] != '\0') {
        if (allInputs[charCount] == '\n') {
            currentLine++;
        }
        charCount++;
    }

    if (currentLine < line) {
        printf("Error! This line number exceeds the number of lines in the text.\n");
        return;
    }

    int lineStart = charCount;
    while (allInputs[charCount] != '\n' && allInputs[charCount] != '\0') {
        charCount++;
    }
    int lineLength = charCount - lineStart;

    if (index > lineLength) {
        printf("Error! This index exceeds the length of the line.\n");
        return;
    }

    size_t clipboardLength = strlen(clipboardBuffer);
    size_t newSize = strlen(allInputs) + clipboardLength + 1;
    char *newBuffer = (char *)malloc(newSize * sizeof(char));
    if (!newBuffer) {
        printf("Memory allocation failed.\n");
        return;
    }

    strncpy(newBuffer, allInputs, lineStart + index);
    newBuffer[lineStart + index] = '\0';
    strcat(newBuffer, clipboardBuffer);
    strcat(newBuffer, &allInputs[lineStart + index]);

    free(allInputs);
    allInputs = newBuffer;

    printf("Text has been successfully pasted at line %d, index %d\n", line, index);
}

void insertWithReplacement() {

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
            char filename[256];
            int line, index;
            char textToInsert[256];

            insertByIndex(filename, line, index, textToInsert);
            break;
        case SEARCH:
            searchText();
            break;
        case DELETE:
            deleteText();
            break;
        case UNDO:
            undoCommand();
            break;
        case REDO:
            redoCommand();
            break;
        case CUT:
            cutText();
            break;
        case COPY:
            copyText();
            break;
        case PASTE:
            pasteText();
            break;
        case INSERT_WITH_REPLACEMENT:
            printf("The command has not been implemented yet.");
            break;
        case EXIT:
            printf("Exiting the program...\n");
            break;
        default:
            printf("Oops! Write a correct command from 0 to 15.\n");
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
