#include "hangman.h"

#include <stdbool.h>
#include <string.h>

#define CUT_MAIN
#include "cut.h"

TEST(word_guessed)
{
    SUBTEST(example_not_guessed)
    {
        CHECK(!word_guessed("secret", "aeiou"));
    }

    SUBTEST(example_guessed)
    {
        CHECK(word_guessed("hi", "aeihoul"));
    }

    SUBTEST(multiple_letters)
    {
        CHECK(word_guessed("baba", "ba"));
    }
}

TEST(censor_word)
{
    SUBTEST(example)
    {
        char result[30];
        censor_word("container", "arpstxgoieyu", result);
        CHECK(strcmp(result, "_o_tai_er") == 0);
    }

    SUBTEST(bigger_example)
    {
        char result[30];
        censor_word("underserved", "arpstxgoieyu", result);
        CHECK(strcmp(result, "u__erser_e_") == 0);
    }
}

TEST(get_available_letters)
{
    SUBTEST(example)
    {
        char result[30];
        get_available_letters("arpstxgoieyu", result);
        CHECK(strcmp(result, "bcdfhjklmnqvwz") == 0);
    }

    SUBTEST(all) {
        char result[30];
        get_available_letters("", result);
        CHECK(strcmp(result, "abcdefghijklmnopqrstuvwxyz") == 0);
    }

    SUBTEST(none) {
        char result[30];
        get_available_letters("abcdefghijklmnopqrstuvwxyz", result);
        CHECK(strcmp(result, "") == 0);
    }
}
