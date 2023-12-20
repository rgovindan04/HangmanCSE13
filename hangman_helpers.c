#include "hangman_helpers.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool is_lowercase_letter(char c) {
    // Check if the character is in the range of lowercase letters (a to z)
    if (c >= 'a' && c <= 'z') {
        return true;
    } else {
        return false;
    }
}

bool validate_secret(const char *secret) {

    size_t length = strlen(secret);

    if (length > 256) {
        printf("The secret phrase is over 256 characters.\n");
        return false;
    }

    for (size_t i = 0; i < length; i++) {
        char c = secret[i];

        if (!((c >= 'a' && c <= 'z') || c == ' ' || c == '\'' || c == '-')) {
            printf("invalid character: '%c'\n", c);
            return false;
        }
    }

    return true;
}

bool string_contains_character(const char *s, char c) {
    while (*s != '\0') {
        if (*s == c) {
            return true;
        }
        s++; // Move to the next character in the string
    }
    return false;
}

char read_letter(void) {
    char input;
    printf("Guess a letter: ");
    int c;

    while (1) {
        c = getchar();
        if (c != '\n' && c != EOF) {
            input = (char) c;
            break;
        }
    }
    return input;
}
