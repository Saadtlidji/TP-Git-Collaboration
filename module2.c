/*
 * module2.c - Moteur de securite  (SQUELETTE)
 *
 * Cette version ne contient que les signatures : le chiffrement, le
 * checksum et l'entropie sont implementes sur feature/module2.
 */
#include "security.h"

#include <stdio.h>
#include <string.h>

static void set_error(char *err, size_t size, const char *message)
{
    if (err != NULL && size > 0) {
        strncpy(err, message, size - 1);
        err[size - 1] = '\0';
    }
}

unsigned long long crypto_checksum(const void *data, size_t length)
{
    (void)data;
    (void)length;
    return 0ULL;                    /* TODO(feature/module2) */
}

double crypto_entropy(const void *data, size_t length)
{
    (void)data;
    (void)length;
    return 0.0;                     /* TODO(feature/module2) */
}

int crypto_key_score(const char *key)
{
    (void)key;
    return 0;                       /* TODO(feature/module2) */
}

int crypto_encrypt(const char *text, const char *key,
                   char *out, size_t out_size,
                   crypto_report *report,
                   char *err, size_t err_size)
{
    (void)text;
    (void)key;
    (void)out;
    (void)out_size;
    (void)report;
    set_error(err, err_size, "Moteur non implemente (feature/module2).");
    return -1;
}

int crypto_decrypt(const char *frame, const char *key,
                   char *out, size_t out_size,
                   crypto_report *report,
                   char *err, size_t err_size)
{
    (void)frame;
    (void)key;
    (void)out;
    (void)out_size;
    (void)report;
    set_error(err, err_size, "Moteur non implemente (feature/module2).");
    return -1;
}
