#include "bmp.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define CUT_MAIN
#include "cut.h"

void test_reverse(const char *input, const char *output)
{
    char *reversed = reverse(input);

    // check if not NULL
    ASSERT(reversed != NULL);

    int result = strcmp(reversed, output);
    if (result != 0) {
        DEBUG_MSG("Expected: %s, but got: %s\n", output, reversed);
    }

    ASSERT(strcmp(reversed, output) == 0);
    free(reversed);
}

TEST(REVERSE)
{
    SUBTEST(ABCD)
    {
        test_reverse("ABCD", "DCBA");
    }
    SUBTEST(BADACAD)
    {
        test_reverse("BADACAD", "DACADAB");
    }
    SUBTEST(qqsfyCCDAADQWQLLLlccaasq)
    {
        test_reverse("qqsfyCCDAADQWQLLLlccaasq", "QSAACCLLLLQWQDAADCCYFSQQ");
    }
    SUBTEST(NULL)
    {
        ASSERT(reverse(NULL) == NULL);
    }
    SUBTEST(EMPTY)
    {
        char *result = reverse("");
        ASSERT(result[0] == '\0');
        free(result);
    }
    SUBTEST(RANDOM)
    {
        test_reverse("hello", "OLLEH");
        test_reverse("malloc", "COLLAM");
        test_reverse("calloc", "COLLAC");
        test_reverse("realloc", "COLLAER");
        test_reverse("everything", "GNIHTYREVE");
        test_reverse("failing", "GNILIAF");
        test_reverse("LOL", "LOL");
        test_reverse("1273912739&^%$$*((", "((*$$%^&9372193721");
    }
}

void test_bit(const char *input, const unsigned char *output, size_t length)
{
    unsigned char *encrypted = bit_encrypt(input);
    char *decrypted = bit_decrypt(encrypted);

    for (size_t i = 0; i < length; i++) {
        ASSERT(encrypted[i] == output[i]);
        ASSERT(decrypted[i] == input[i]);
    }

    free(decrypted);
    free(encrypted);
}

TEST(BIT)
{
    SUBTEST(HELLO_WORLD)
    {
        const unsigned char output[] = { 0x80, 0x9c, 0x95, 0x95, 0x96, 0x11, 0xbc, 0x96, 0xb9, 0x95, 0x9d, 0x10 };
        test_bit("Hello world!", output, 12);
    }
    SUBTEST(MALLOC)
    {
        const unsigned char output[] = { 0x94, 0x98, 0x95, 0x95, 0x96, 0x9a };
        test_bit("malloc", output, 6);
    }
    SUBTEST(CALLOC)
    {
        const unsigned char output[] = { 0x9a, 0x98, 0x95, 0x95, 0x96, 0x9a };
        test_bit("calloc", output, 6);
    }
    SUBTEST(REALLOC)
    {
        const unsigned char output[] = { 0xb9, 0x9c, 0x98, 0x95, 0x95, 0x96, 0x9a };
        test_bit("realloc", output, 7);
    }
    SUBTEST(EVERYTHING)
    {
        const unsigned char output[] = { 0x9c, 0xbd, 0x9c, 0xb9, 0xb2, 0xbf, 0x91, 0x90, 0x97, 0x9e };
        test_bit("everything", output, 10);
    }
    SUBTEST(FAILING)
    {
        const unsigned char output[] = { 0x9f, 0x98, 0x90, 0x95, 0x90, 0x97, 0x9e };
        test_bit("failing", output, 7);
    }
    SUBTEST(LOL)
    {
        const unsigned char output[] = { 0x84, 0x87, 0x84 };
        test_bit("LOL", output, 3);
    }
    SUBTEST(GARBAGE)
    {
        const unsigned char output[] = { 0x32, 0x31, 0x34, 0x30, 0x3a, 0x32, 0x31, 0x34, 0x30, 0x3a, 0x17, 0xa4, 0x14, 0x15, 0x15, 0x1b, 0x19, 0x19 };
        test_bit("1273912739&^%$$*((", output, 18);
    }
    SUBTEST(HELLO_FI)
    {
        const unsigned char output[] = { 0x91, 0x9c, 0x95, 0x95, 0x96 };
        test_bit("hello", output, 5);
    }
    SUBTEST(BYE_FI)
    {
        const unsigned char output[] = { 0x9b, 0xb2, 0x9c };
        test_bit("bye", output, 3);
    }
}

void test_vigenere(const char *key, const char *input, const char *output, size_t length)
{
    char *encrypted = vigenere_encrypt(key, input);
    char *decrypted = vigenere_decrypt(key, encrypted);

    ASSERT(encrypted != NULL);
    ASSERT(decrypted != NULL);

    for (size_t i = 0; i < length; i++) {
        ASSERT(encrypted[i] == output[i]);
        ASSERT(decrypted[i] == toupper(input[i]));
    }

    free(encrypted);
    free(decrypted);
}

TEST(VIGENERE)
{
    SUBTEST(HELLO_WORLD)
    {
        test_vigenere("CoMPuTeR", "Hello world!", "JSXAI PSINR!", 12);
    }
    SUBTEST(MALLOC)
    {
        test_vigenere("fails", "malloc", "RATWGH", 6);
    }
    SUBTEST(CALLOC)
    {
        test_vigenere("fails", "calloc", "HATWGH", 6);
    }
    SUBTEST(REALLOC)
    {
        test_vigenere("fails", "realloc", "WEIWDTC", 7);
    }
    SUBTEST(EVERYTHING)
    {
        test_vigenere("FAILS", "everything", "JVMCQYHQYY", 10);
    }
    SUBTEST(FAILING)
    {
        test_vigenere("fails", "failing", "KAQWASG", 7);
    }
    SUBTEST(LOL)
    {
        test_vigenere("oopsie", "LOL", "ZCA", 3);
    }
    SUBTEST(GARBAGE)
    {
        test_vigenere("fi", "1273912739&^%$$*((", "1273912739&^%$$*((", 18);
    }
    SUBTEST(HELLO_FI)
    {
        test_vigenere("fi", "hello", "MMQTT", 5);
    }
    SUBTEST(BYE_FI)
    {
        test_vigenere("fi", "bye", "GGJ", 3);
    }
}

void test_bmp(const char *key, const char *input, const unsigned char *encrypted_expected, const char *decrypted_expected, size_t length)
{
    unsigned char *encrypted = bmp_encrypt(key, input);
    char *decrypted = bmp_decrypt(key, encrypted);

    ASSERT(encrypted != NULL);
    ASSERT(decrypted != NULL);

    for (size_t i = 0; i < length; i++) {
        ASSERT(encrypted[i] == encrypted_expected[i]);
        ASSERT(decrypted[i] == decrypted_expected[i]);
    }

    free(encrypted);
    free(decrypted);
}

TEST(BMP)
{
    SUBTEST(HELLO)
    {
        const unsigned char encrypted_expected[] = { 0xae, 0xae, 0xab, 0x85, 0x85 };
        test_bmp("fi", "hello", encrypted_expected, "HELLO", 5);
    }
    SUBTEST(FI)
    {
        const unsigned char encrypted_expected[] = { 0xaa, 0x82 };
        test_bmp("hello", "fi", encrypted_expected, "FI", 2);
    }
    SUBTEST(RANDOM)
    {
        const unsigned char encrypted_expected[] = { 0xa9, 0x87, 0xaf, 0x87, 0x89, 0xa2 };
        test_bmp("garbage", "random", encrypted_expected, "RANDOM", 6);
    }
    SUBTEST(LETS)
    {
        const unsigned char encrypted_expected[] = { 0x83, 0xa2, 0x81, 0x8c };
        test_bmp("see", "lets", encrypted_expected, "LETS", 4);
    }
    SUBTEST(how)
    {
        const unsigned char encrypted_expected[] = { 0x8d, 0x80, 0xaa };
        test_bmp("it", "how", encrypted_expected, "HOW", 3);
    }
    SUBTEST(works)
    {
        const unsigned char encrypted_expected[] = { 0xa9, 0x8b, 0x89, 0xa8, 0x80 };
        test_bmp("asjdljasdja", "works", encrypted_expected, "WORKS", 5);
    }
    SUBTEST(NOSPACES)
    {
        const unsigned char encrypted_expected[] = { 0x8d, 0x81, 0x82, 0x85, 0xae, 0xa0, 0x89, 0xa8, 0xa0, 0x85, 0x84, 0x89, 0x85, 0x84, 0x89, 0x8e, 0x8a, 0x84, 0x8e, 0xac, 0x84, 0xa9, 0xa8, 0xac, 0xa2 };
        test_bmp("meh", "longerTextThatHasNoSpaces", encrypted_expected, "LONGERTEXTTHATHASNOSPACES", 25);
    }
    SUBTEST(ayaya)
    {
        const unsigned char encrypted_expected[] = { 0x80, 0xa9, 0x80, 0x8e, 0xaf, 0x8e, 0x80, 0xa9, 0x80, 0x8e, 0xaf, 0x8e, 0x80, 0xa9, 0x80, 0x8e, 0xaf, 0x8e, 0x80, 0xa9, 0x80, 0x8e, 0xaf, 0x8e, 0x80, 0xa9, 0x80, 0x8e, 0xaf, 0x8e, 0x80, 0xa9, 0x80 };
        test_bmp("HUH", "ayayayayayayayayayayayayayayayaya", encrypted_expected, "AYAYAYAYAYAYAYAYAYAYAYAYAYAYAYAYA", 33);
    }
}
