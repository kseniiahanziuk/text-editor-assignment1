#include <iostream>

extern "C" {
    char* Encrypt(char *startText, int key) {
        int len = strlen(startText);
        char* encryptedText = (char*)malloc(len + 1);

        key = key % 26;
        if (key < 0) {
            key += 26;
        }

        for (int i = 0; i < len; i++) {
            char character = startText[i];

            if (isalpha(character)) {
                if (isupper(character)) {
                    encryptedText[i] = (character - 'A' + key) % 26 + 'A';
                } else {
                    encryptedText[i] = (character - 'a' + key) % 26 + 'a';
                }
            } else {
                encryptedText[i] = character;
            }
        }
        encryptedText[len] = '\0';
        return encryptedText;
    }

    char* Decrypt(char *encryptedText, int key) {
        int len = strlen(encryptedText);
        char* decryptedText = (char*)malloc(len + 1);

        key = key % 26;
        if (key < 0) {
            key += 26;
        }

        for (int i = 0; i < len; i++) {
            char character = encryptedText[i];

            if (isalpha(character)) {
                if (isupper(character)) {
                    decryptedText[i] = (character - 'A' - key + 26) % 26 + 'A';
                } else {
                    decryptedText[i] = (character - 'a' - key + 26) % 26 + 'a';
                }
            } else {
                decryptedText[i] = character;
            }
        }
        decryptedText[len] = '\0';
        return decryptedText;
    }
}