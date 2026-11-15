/*
 * module1.c - Saisie et validation des entrees utilisateur
 *
 * Responsabilites :
 *   - lire une ligne de texte de facon sure (pas de scanf nu)
 *   - lire et valider un choix de menu
 *   - valider le message en clair, la cle et la syntaxe d'une frame
 *
 * Aucune logique cryptographique ici : seulement de la validation.
 */
#include "security.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Outils internes                                                     */
/* ------------------------------------------------------------------ */

/* Vide le reste d'une ligne trop longue pour ne pas polluer la
 * saisie suivante. */
static void input_flush(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
        /* on jette les caracteres en trop */
    }
}

/* Ecrit un motif de refus sans depasser la taille du tampon. */
static void set_reason(char *reason, size_t size, const char *message)
{
    if (reason != NULL && size > 0) {
        strncpy(reason, message, size - 1);
        reason[size - 1] = '\0';
    }
}

/* ------------------------------------------------------------------ */
/* Lecture                                                             */
/* ------------------------------------------------------------------ */

int input_read_line(const char *prompt, char *buffer, size_t size)
{
    size_t length;

    if (buffer == NULL || size < 2) {
        return 0;
    }
    if (prompt != NULL) {
        printf("%s", prompt);
        fflush(stdout);
    }
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;                       /* EOF ou erreur de flux */
    }

    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
        length--;
    } else if (length == size - 1) {
        input_flush();                  /* ligne plus longue que le tampon */
    }
    if (length > 0 && buffer[length - 1] == '\r') {
        buffer[length - 1] = '\0';      /* fins de ligne Windows */
    }
    return 1;
}

int input_read_choice(int min, int max)
{
    char line[32];
    char *end = NULL;
    long value;

    for (;;) {
        if (!input_read_line("Votre choix > ", line, sizeof line)) {
            return -1;                  /* fin de flux : on sort proprement */
        }
        value = strtol(line, &end, 10);
        if (end == line) {
            ui_message("ERREUR", "Veuillez saisir un nombre.");
            continue;
        }
        while (end != NULL && isspace((unsigned char)*end)) {
            end++;
        }
        if (end != NULL && *end != '\0') {
            ui_message("ERREUR", "Caracteres invalides apres le nombre.");
            continue;
        }
        if (value < min || value > max) {
            printf("  Choix hors plage (%d-%d).\n", min, max);
            continue;
        }
        return (int)value;
    }
}

/* ------------------------------------------------------------------ */
/* Validation metier                                                   */
/* ------------------------------------------------------------------ */

int validate_plaintext(const char *text, char *reason, size_t reason_size)
{
    size_t i;
    size_t length;

    if (text == NULL) {
        set_reason(reason, reason_size, "Message absent.");
        return 0;
    }
    length = strlen(text);
    if (length == 0) {
        set_reason(reason, reason_size, "Le message est vide.");
        return 0;
    }
    if (length > MAX_PLAINTEXT - 1) {
        set_reason(reason, reason_size, "Message trop long (512 caracteres max).");
        return 0;
    }
    for (i = 0; i < length; i++) {
        unsigned char c = (unsigned char)text[i];

        if (c < 32 || c > 126) {
            set_reason(reason, reason_size,
                       "Le message doit rester en ASCII imprimable (pas d'accents).");
            return 0;
        }
    }
    for (i = 0; i < length; i++) {
        if (!isspace((unsigned char)text[i])) {
            break;                      /* au moins un caractere visible */
        }
    }
    if (i == length) {
        set_reason(reason, reason_size, "Le message ne contient que des espaces.");
        return 0;
    }
    return 1;
}

int validate_key(const char *key, char *reason, size_t reason_size)
{
    size_t i;
    size_t length;

    if (key == NULL) {
        set_reason(reason, reason_size, "Cle absente.");
        return 0;
    }
    length = strlen(key);
    if (length < 4) {
        set_reason(reason, reason_size, "Cle trop courte (4 caracteres minimum).");
        return 0;
    }
    if (length > MAX_KEY - 1) {
        set_reason(reason, reason_size, "Cle trop longue (63 caracteres maximum).");
        return 0;
    }
    for (i = 0; i < length; i++) {
        unsigned char c = (unsigned char)key[i];

        if (c < 33 || c > 126) {
            set_reason(reason, reason_size,
                       "La cle ne doit contenir ni espace ni caractere special non affichable.");
            return 0;
        }
    }
    return 1;
}

int validate_frame(const char *frame, char *reason, size_t reason_size)
{
    size_t i;
    size_t length;
    size_t data_length;

    if (frame == NULL) {
        set_reason(reason, reason_size, "Message chiffre absent.");
        return 0;
    }
    length = strlen(frame);
    if (length < CHECKSUM_HEX + 3) {
        set_reason(reason, reason_size, "Message chiffre trop court.");
        return 0;
    }
    if (length > MAX_FRAME - 1) {
        set_reason(reason, reason_size, "Message chiffre trop long.");
        return 0;
    }
    for (i = 0; i < CHECKSUM_HEX; i++) {
        if (!isxdigit((unsigned char)frame[i])) {
            set_reason(reason, reason_size,
                       "Les 16 premiers caracteres doivent etre un checksum hex.");
            return 0;
        }
    }
    if (frame[CHECKSUM_HEX] != ':') {
        set_reason(reason, reason_size, "Separateur ':' manquant apres le checksum.");
        return 0;
    }
    data_length = length - CHECKSUM_HEX - 1;
    if (data_length == 0 || (data_length % 2) != 0) {
        set_reason(reason, reason_size, "Donnees hexadecimale de longueur invalide.");
        return 0;
    }
    for (i = CHECKSUM_HEX + 1; i < length; i++) {
        if (!isxdigit((unsigned char)frame[i])) {
            set_reason(reason, reason_size,
                       "Le corps du message doit etre hexadecimal (0-9, a-f).");
            return 0;
        }
    }
    return 1;
}
