/*
 * module1.c - Saisie et validation des entrees  (SQUELETTE)
 *
 * Cette version ne contient que les signatures : l'implementation
 * complete est realisee sur la branche feature/module1.
 */
#include "security.h"

#include <stdio.h>
#include <string.h>

static void set_reason(char *reason, size_t size, const char *message)
{
    if (reason != NULL && size > 0) {
        strncpy(reason, message, size - 1);
        reason[size - 1] = '\0';
    }
}

int input_read_line(const char *prompt, char *buffer, size_t size)
{
    (void)prompt;
    (void)buffer;
    (void)size;
    return 0;                       /* TODO(feature/module1) */
}

int input_read_choice(int min, int max)
{
    (void)min;
    (void)max;
    return -1;                      /* TODO(feature/module1) */
}

int validate_plaintext(const char *text, char *reason, size_t reason_size)
{
    (void)text;
    set_reason(reason, reason_size, "Validation non implementee (feature/module1).");
    return 0;
}

int validate_key(const char *key, char *reason, size_t reason_size)
{
    (void)key;
    set_reason(reason, reason_size, "Validation non implementee (feature/module1).");
    return 0;
}

int validate_frame(const char *frame, char *reason, size_t reason_size)
{
    (void)frame;
    set_reason(reason, reason_size, "Validation non implementee (feature/module1).");
    return 0;
}
