#include "bmp.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(var) ((void) (var))

/**
 * Return reversed string.
 *
 * Function returns a pointer to the reversed string, which has been created
 * from the input parameter. If the input string is NULL or memory could not be
 * allocated, function returns NULL.
 *
 * @param text input string
 * @return reversed representation of input string, or NULL, if the string could
 * not be created.
 */
char *reverse(const char *text)
{
    /* TODO */
    UNUSED(text);
    return NULL;
}

/**
 * @brief Checks if key is valid.
 * @param key String that is used as a key
 * @returns true, if key is valid; false otherwise
 */
static bool check_key(const char *key)
{
    /* TODO */
    UNUSED(key);
    return false;
}

/**
 * Function for encrypting the given plaintext by applying the Vigenère cipher.
 *
 * @param key Pointer to a string representing the key which will be used
 * for encrypting the plaintext. The key is represented by a single
 * case-insensitive word consisting of only alphabetical characters.
 * @param text Pointer to a string representing the plaintext to be
 * encrypted.
 * @return the address of a copy of the ciphertext encrypted by the Vigenère
 * cipher, or NULL if the encryption was not successful.
 */
char *vigenere_encrypt(const char *key, const char *text)
{
    /* TODO */
    UNUSED(key);
    UNUSED(text);
    return NULL;
}

/**
 * Function for decrypting the given ciphertext by applying the Vigenère cipher.
 *
 * @param key Pointer to a string representing the key which has been used
 * for encrypting the ciphertext. The key is represented by a single
 * case-insensitive word consisting of only alphabetical characters.
 * @param text Pointer to a string representing the ciphertext to be
 * decrypted.
 * @return the address of a copy of the plaintext decrypted by the Vigenère
 * cipher, or NULL if the decryption was not successful.
 */
char *vigenere_decrypt(const char *key, const char *text)
{
    /* TODO */
    UNUSED(key);
    UNUSED(text);
    return NULL;
}

/**
 * Function for bitwise encryption according to the following process:
 * The character about to be encrypted is divided in half (4 bits + 4 bits).
 * Subsequently, the bits in the left half are divided into two pairs and the
 * values inside each pair are swapped. The four bits created by this way are
 * then used in a bitwise XOR operation with the remaining 4 bits.
 *
 * @param text String representing the plaintext to be encrypted.
 * @return a pointer to a newly created string containing the ciphertext
 * produced by encrypting the plaintext, or NULL if the encryption was not
 * successful.
 */
unsigned char *bit_encrypt(const char *text)
{
    /* TODO */
    UNUSED(text);
    return NULL;
}

/**
 * Function for bitwise decryption - it is the inverse of the bitwise encryption
 * function.
 *
 * @param text String representing the ciphertext to be decrypted.
 * @return a pointer to a newly created string containing the plaintext produced
 * by decrypting the ciphertext, or NULL if the decryption was not successful.
 */
char *bit_decrypt(const unsigned char *text)
{
    /* TODO */
    UNUSED(text);
    return NULL;
}

/**
 * Function for encrypting the given plaintext by applying the BMP cipher.
 * The process of encrypting by BMP:
 * <ol>
 *      <li> the provided input string is encrypted by function reverse()
 *      <li> the acquired string is encrypted by function vigenere_encrypt()
 *      <li> function bit_encrypt() is applied on the resulting string
 * </ol>
 *
 * @param key Pointer to a string representing the key which will be used
 * for encrypting the plaintext. The key is represented by a single
 * case-insensitive word consisting of only alphabetical characters.
 * @param text String representing the plaintext to be encrypted.
 * @return the address of a copy of the ciphertext encrypted by the BMP cipher,
 * or NULL if the encryption was not successful.
 */
unsigned char *bmp_encrypt(const char *key, const char *text)
{
    /* TODO */
    UNUSED(key);
    UNUSED(text);
    return NULL;
}

/**
 * Function for decrypting the given ciphertext by applying the BMP cipher.
 * The process of decrypting by BMP is the opposite of BMP encryption.
 *
 * @param key Pointer to a string representing the key which has been used
 * for encrypting the ciphertext. The key is represented by a single
 * case-insensitive word consisting of only alphabetical characters.
 * @param text String representing the ciphertext to be decrypted.
 * @return the address of a copy of the plaintext decrypted by the BMP cipher,
 * or NULL if the decryption was not successful.
 */
char *bmp_decrypt(const char *key, const unsigned char *text)
{
    /* TODO */
    UNUSED(key);
    UNUSED(text);
    return NULL;
}
