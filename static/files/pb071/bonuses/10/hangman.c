#include "hangman.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_LENGTH 50
#define ALPHABET_LENGTH 27

int get_word(const char *wordlist, char secret[])
{
    FILE *fp = fopen(wordlist, "rb");
    if (fp == NULL) {
        fprintf(stderr, "No such file or directory: %s\n", wordlist);
        return 1;
    }

    struct stat st;
    stat(wordlist, &st);
    long int size = st.st_size;

    /* FIXME: Can fall into infinite loop... */
    do {
        long int random = (rand() % size) + 1;

        fseek(fp, random, SEEK_SET);

        int result = fscanf(fp, "%*s %20s", secret);
        if (result != EOF)
            break;
    } while (1);

    fclose(fp);

    return 0;
}

static void print_word(const char *word)
{
    for (size_t i = 0; word[i]; i++) {
        printf(" %c", word[i]);
    }
    printf("\n");
}

bool word_guessed(const char secret[], const char letters_guessed[])
{
    size_t l = strlen(secret), k=0;

    for (size_t j = 0; letters_guessed[j]; j++) {
        for (size_t i = 0; secret[i]; i++) {
            if (secret[i] == letters_guessed[j]) {
                k++;
            }
        }
    }

    return l <= k;
}

void censor_word(const char secret[], const char letters_guessed[], char guessed_word[])
{
    size_t i;
    for (i = 0; secret[i]; i++, guessed_word++) {
        *guessed_word = secret[i];

        bool guessed = false;
        for (int j = 0; letters_guessed[j]; j++) {
            if (secret[i] == letters_guessed[j]) {
                guessed = true;
            }
        }

        if (!guessed) {
            *guessed_word = '_';
        }
    }

    *guessed_word = '\0';
}

void get_available_letters(const char letters_guessed[],
        char available_letters[])
{
    size_t k = 0;

    for (char letter = 'a'; letter <= 'z'; letter++) {
        bool to_be_put = true;

        for (size_t j = 0; letters_guessed[j]; j++) {
            if (letters_guessed[j] == letter) {
                to_be_put = false;
                break;
            }
        }

        if (to_be_put) {
            available_letters[k++] = letter;
        }
    }
    available_letters[k] = '\0';
}

static bool been_already_guessed(const char *guessed_letters,
        const char letter)
{
    for (size_t i = 0; guessed_letters[i]; i++) {
        if (guessed_letters[i] == letter) {
            return true;
        }
    }
    return false;
}

static bool is_good_guess(const char secret[], char guessed_word[], const char letter)
{
    bool result = false;
    for (size_t i = 0; secret[i]; i++) {
        if (secret[i] == letter) {
            result = true;
            break;
        }
    }

    if (result) {
        printf("Good guess:");
    } else {
        printf("Oops! That letter is not in my word:");
    }

    print_word(guessed_word);
    return result;
}

static bool guessed_whole(const char *secret, const char *guess)
{
    for (size_t i = 0; secret[i]; i++)
        if (tolower(guess[i]) != secret[i]) {
            return false;
        }

    return true;
}

static int get_guess(char *guess)
{
    int result = scanf("%s", guess);
    if (result == 1) {
        *guess = tolower(*guess);
        return result;
    }

    int ch = 0;
    fprintf(stderr, "Input was not successful.\n");
    while (ch != EOF && (ch = getchar()) != '\n')
        ;
    return result;
}

void hangman(const char secret[])
{
    printf("Welcome to the game, Hangman!\n");

    size_t length_of_word = strlen(secret);
    printf("I am thinking of a word that is %lu letters long.\n",
            length_of_word);

    int guesses = 8, no_of_guessed_letters = 0;
    char available_letters[ALPHABET_LENGTH],
            guessed_letters[ALPHABET_LENGTH] = { 0 };
    char guess[MAX_LENGTH];
    char guessed_word[MAX_LENGTH] = { 0 };
    get_available_letters(guessed_letters, available_letters);

    while (guesses > 0) {
        printf("-------------\n");
        if (word_guessed(secret, guessed_letters)) {
            printf("Congratulations, you won!\n");
            return;
        }

        printf("You have %d guesses left.\n", guesses);
        printf("Available letters: %s\n", available_letters);

        printf("Please guess a letter: ");

        int read_status = get_guess(guess);
        if (read_status == EOF) {
            fprintf(stderr, "No more input to read.\n");
            return;
        } else if (read_status == 0) {
            continue;
        }

        if (guess[1]) {
            if (guessed_whole(secret, guess)) {
                printf("Congratulations, you won!\n");
            } else {
                printf("Sorry, bad guess. The word was %s.\n", secret);
            }
        }

        if (*guess < 'a' || *guess > 'z') {
            printf("Oops! '%c' is not a valid letter:", *guess);
            censor_word(secret, guessed_letters, guessed_word);
            print_word(guessed_word);
            continue;
        }

        if (been_already_guessed(guessed_letters, *guess)) {
            printf("Oops! You've already guessed that letter:");
            censor_word(secret, guessed_letters, guessed_word);
            print_word(guessed_word);
            continue;
        }

        guessed_letters[no_of_guessed_letters++] = *guess;
        get_available_letters(guessed_letters, available_letters);
        censor_word(secret, guessed_letters, guessed_word);

        if (!is_good_guess(secret, guessed_word, *guess)) {
            guesses--;
        }
    }

    if (guesses == 0) {
        printf("-------------\n");
        printf("Sorry, you ran out of guesses. The word was %s.\n", secret);
    }
}
