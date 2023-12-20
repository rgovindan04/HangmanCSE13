#include "hangman_helpers.h"

int main(int argc, char *argv[]) {
    char secret[MAX_LENGTH]; // To store the secret phrase
    char guessed_characters[26] = ""; // To store guessed characters
    int mistakes = 0; // Count of incorrect guesses

    // Ensure that a secret phrase is provided as a command-line argument
    if (argc != 2) {
        fprintf(stdout, "Usage: ./hangman <secret word or phrase>\n");
        fprintf(stdout, "If the secret is multiple words, you must enclose it in quotes.\n");
        return 1;
    }

    // Get the secret phrase from the command-line argument
    strncpy(secret, argv[1], sizeof(secret));
    secret[sizeof(secret) - 1] = '\0'; // Ensure null-termination

    // Validate the secret phrase
    if (!validate_secret(secret)) {
        fprintf(stdout, "the secret phrase must contain only lowercase letters, spaces, hyphens, "
                        "and apostrophes\n");
        return 1;
    }

    // Initialize Hangman game
    size_t secret_length = strlen(secret);
    char revealed[secret_length]; // To store revealed characters

    for (size_t i = 0; i < secret_length; i++) {
        if (string_contains_character(punctuation, secret[i])) {
            revealed[i] = secret[i];
        } else {
            revealed[i] = '_';
        }
    }

    // Main game loop
    while (mistakes < LOSING_MISTAKE) {

        //Clear the screen
        printf(CLEAR_SCREEN);

        //Display Hangman
        printf("%s\n", arts[mistakes]);

        // Display the Hangman phrase
        printf("\n    Phrase: ");
        for (size_t i = 0; i < secret_length; i++) {
            printf("%c", revealed[i]);
        }
        printf("\n");

        // Display eliminated letters in alphabetical order
        printf("Eliminated: ");
        char sortedEliminated
            [26]; // Create a temporary array to store eliminated characters in sorted order
        int sortedIndex = 0;

        for (int i = 0; i < 26; i++) {
            if (guessed_characters[i] != '\0') {
                sortedEliminated[sortedIndex++] = guessed_characters[i];
            }
        }

        // Sort the characters in alphabetical order using bubble sort
        for (int i = 0; i < sortedIndex - 1; i++) {
            for (int j = 0; j < sortedIndex - i - 1; j++) {
                if (sortedEliminated[j] > sortedEliminated[j + 1]) {
                    char temp = sortedEliminated[j];
                    sortedEliminated[j] = sortedEliminated[j + 1];
                    sortedEliminated[j + 1] = temp;
                }
            }
        }

        // Print the sorted eliminated letters
        for (int i = 0; i < sortedIndex; i++) {
            printf("%c", sortedEliminated[i]);
        }
        printf("\n\n");

        // Get a guess from the user
        char guess = read_letter();
        while (1) {
            if (!is_lowercase_letter(guess)) {
                guess = read_letter();
                continue;
            }
            // Check if an incorrect guess is already made
            if (string_contains_character(guessed_characters, guess)) {
                guess = read_letter();
                continue;
            }
            // Check if a correct gues has been made
            if (string_contains_character(revealed, guess)) {
                guess = read_letter();
                continue;
            } else {
                break;
            }
        }

        // Check if the guess is in the secret phrase
        bool found = false;
        for (size_t i = 0; i < secret_length; i++) {
            if (secret[i] == guess) {
                found = true;
                revealed[i] = guess;
            }
        }
        // Check if the guess is not already in the guessed_characters array
        if (!found) {
            bool alreadyGuessed = false;
            for (int i = 0; i < 26; i++) {
                if (guessed_characters[i] == guess) {
                    alreadyGuessed = true;
                    break;
                }
            }

            if (!alreadyGuessed) {
                guessed_characters[mistakes] = guess;
                mistakes++;
            }
        }

        // Check if the player has won
        bool won = true;
        for (size_t i = 0; i < secret_length; i++) {
            if (string_contains_character("abcdefghijklmnopqrstuvwxyz", secret[i])
                && revealed[i] == '_') {
                won = false;
                break;
            }
        }

        if (won) {
            printf(CLEAR_SCREEN);
            printf("%s\n", arts[mistakes]);
            printf("\n    Phrase: %s\n", secret);
            printf("Eliminated: ");
            for (int i = 0; i < sortedIndex; i++) {
                printf("%c", sortedEliminated[i]);
            }

            printf("\n\nYou win! The secret phrase was: %s\n", secret);
            break;
        }
        if (mistakes == LOSING_MISTAKE) {
            printf(CLEAR_SCREEN);
            printf("%s\n", arts[LOSING_MISTAKE]);
            printf("\n    Phrase: ");
            for (size_t i = 0; i < secret_length; i++) {
                printf("%c", revealed[i]);
            }
            printf("\nEliminated: ");
            for (int i = 0; i < 26; i++) {
                if (guessed_characters[i] != '\0') {
                    sortedEliminated[sortedIndex++] = guessed_characters[i];
                }
            }
            for (int i = 0; i < sortedIndex - 1; i++) {
                for (int j = 0; j < sortedIndex - i - 1; j++) {
                    if (sortedEliminated[j] > sortedEliminated[j + 1]) {
                        char temp = sortedEliminated[j];
                        sortedEliminated[j] = sortedEliminated[j + 1];
                        sortedEliminated[j + 1] = temp;
                    }
                }
            }
            // Print the sorted eliminated letters
            //            for (int i = 0; i < sortedIndex; i++) {
            //                printf("%c", sortedEliminated[i]);
            //            }

            char uniqueLetters[26]; // To store unique letters
            int uniqueCount = 0; // Counter for unique letters

            for (int i = 0; i < sortedIndex; i++) {
                char letter = sortedEliminated[i];

                // Check if the letter is unique
                bool isUnique = true;
                for (int j = 0; j < uniqueCount; j++) {
                    if (uniqueLetters[j] == letter) {
                        isUnique = false;
                        break;
                    }
                }

                // If it's a unique letter, print and store it
                if (isUnique) {
                    printf("%c", letter);
                    uniqueLetters[uniqueCount++] = letter;
                }
            }
            printf("\n\nYou lose! The secret phrase was: %s\n", secret);
        }
    }
    return 0;
}
