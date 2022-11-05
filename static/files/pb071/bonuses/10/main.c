#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hangman.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s path-to-wordlist\n", argv[0]);
        return 1;
    }

    char secret[50];

    srand(time(NULL));
    if (get_word(argv[1], secret) != 0) {
        return 1;
    }
    hangman(secret);

    return 0;
}
