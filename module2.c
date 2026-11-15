/*
 * module2.c - Moteur de securite : chiffrement, dechiffrement, integrite
 *
 * Algorithme "TP-XORMIX v1" (pedagogique, PAS de la cryptographie forte) :
 *   1. la cle est transformee en graine 64 bits via FNV-1a ;
 *   2. un generateur xorshift64* produit un flux pseudo-aleatoire ;
 *   3. chaque octet du message est combine (XOR) avec le flux et avec
 *      un decalage dependant de sa position ;
 *   4. le resultat est encode en hexadecimal et precede d'un checksum
 *      FNV-1a 64 bits : "0123456789abcdef:48656c6c6f".
 *
 * Le checksum sert de controle d'integrite : toute modification d'un
 * seul caractere du message chiffre est detectee au dechiffrement.
 */
#include "security.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Outils internes                                                     */
/* ------------------------------------------------------------------ */

static void set_error(char *err, size_t size, const char *message)
{
    if (err != NULL && size > 0) {
        strncpy(err, message, size - 1);
        err[size - 1] = '\0';
    }
}

/* Empreinte FNV-1a 64 bits avec graine explicite (usage interne). */
static unsigned long long fnv1a(const unsigned char *data, size_t length,
                                unsigned long long hash)
{
    size_t i;

    for (i = 0; i < length; i++) {
        hash ^= (unsigned long long)data[i];
        hash *= 1099511628211ULL;
    }
    return hash;
}

/* Generateur xorshift64* : rapide, deterministe, suffisant pour un TP. */
static unsigned long long xorshift_next(unsigned long long *state)
{
    unsigned long long x = *state;

    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    *state = x;
    return x * 2685821657736338717ULL;
}

/* Transforme la cle (et sa longueur) en graine non nulle. */
static unsigned long long seed_from_key(const char *key)
{
    unsigned long long seed;

    seed = fnv1a((const unsigned char *)key, strlen(key), 1469598103934665603ULL);
    seed ^= (unsigned long long)strlen(key) * 0x9E3779B97F4A7C15ULL;
    if (seed == 0ULL) {
        seed = 0xDEADBEEFCAFEBABEULL;
    }
    return seed;
}

static void bytes_to_hex(const unsigned char *input, size_t length, char *output)
{
    static const char digits[] = "0123456789abcdef";
    size_t i;

    for (i = 0; i < length; i++) {
        output[2 * i]     = digits[(input[i] >> 4) & 0x0F];
        output[2 * i + 1] = digits[input[i] & 0x0F];
    }
    output[2 * length] = '\0';
}

static int hex_to_bytes(const char *input, size_t length, unsigned char *output)
{
    size_t i;
    int high;
    int low;

    for (i = 0; i < length; i += 2) {
        high = (input[i] >= '0' && input[i] <= '9') ? input[i] - '0'
             : (input[i] >= 'a' && input[i] <= 'f') ? input[i] - 'a' + 10
             : (input[i] >= 'A' && input[i] <= 'F') ? input[i] - 'A' + 10
             : -1;
        low  = (input[i + 1] >= '0' && input[i + 1] <= '9') ? input[i + 1] - '0'
             : (input[i + 1] >= 'a' && input[i + 1] <= 'f') ? input[i + 1] - 'a' + 10
             : (input[i + 1] >= 'A' && input[i + 1] <= 'F') ? input[i + 1] - 'A' + 10
             : -1;
        if (high < 0 || low < 0) {
            return 0;
        }
        output[i / 2] = (unsigned char)((high << 4) | low);
    }
    return 1;
}

/* Applique (ou annule) le masque de chiffrement : XOR est symetrique. */
static void apply_stream(unsigned char *data, size_t length,
                         unsigned long long state)
{
    size_t i;

    for (i = 0; i < length; i++) {
        unsigned char stream = (unsigned char)(xorshift_next(&state) & 0xFF);
        unsigned char position = (unsigned char)((i * 31u + 7u) & 0xFF);

        data[i] = (unsigned char)(data[i] ^ stream ^ position);
    }
}

/* ------------------------------------------------------------------ */
/* API publique du module                                             */
/* ------------------------------------------------------------------ */

unsigned long long crypto_checksum(const void *data, size_t length)
{
    return fnv1a((const unsigned char *)data, length, 1469598103934665603ULL);
}

double crypto_entropy(const void *data, size_t length)
{
    const unsigned char *bytes = (const unsigned char *)data;
    unsigned long histogram[256];
    size_t i;
    double entropy = 0.0;

    if (bytes == NULL || length == 0) {
        return 0.0;
    }
    memset(histogram, 0, sizeof histogram);
    for (i = 0; i < length; i++) {
        histogram[bytes[i]]++;
    }
    for (i = 0; i < 256; i++) {
        if (histogram[i] > 0) {
            double p = (double)histogram[i] / (double)length;
            entropy -= p * (log(p) / log(2.0));
        }
    }
    return entropy;
}

int crypto_key_score(const char *key)
{
    int score = 0;
    int has_lower = 0;
    int has_upper = 0;
    int has_digit = 0;
    int has_symbol = 0;
    size_t i;
    size_t length;

    if (key == NULL) {
        return 0;
    }
    length = strlen(key);
    for (i = 0; i < length; i++) {
        unsigned char c = (unsigned char)key[i];

        if (c >= 'a' && c <= 'z')      has_lower  = 1;
        else if (c >= 'A' && c <= 'Z') has_upper  = 1;
        else if (c >= '0' && c <= '9') has_digit  = 1;
        else                            has_symbol = 1;
    }

    score += (int)(length * 3);                 /* longueur : 3 pts/car.  */
    if (score > 40) score = 40;                 /* plafond longueur       */
    score += has_lower  ? 12 : 0;
    score += has_upper  ? 12 : 0;
    score += has_digit  ? 12 : 0;
    score += has_symbol ? 12 : 0;
    if (length >= 12) score += 12;              /* bonus longue cle       */
    if (score > 100) score = 100;
    return score;
}

int crypto_encrypt(const char *text, const char *key,
                   char *out, size_t out_size,
                   crypto_report *report,
                   char *err, size_t err_size)
{
    unsigned char cipher[MAX_PLAINTEXT];
    char hex[(MAX_PLAINTEXT - 1) * 2 + 1];
    unsigned long long checksum;
    size_t length;
    size_t needed;
    int written;

    if (text == NULL || key == NULL || out == NULL) {
        set_error(err, err_size, "Parametres invalides.");
        return -1;
    }
    length = strlen(text);
    if (length == 0) {
        set_error(err, err_size, "Texte vide : rien a chiffrer.");
        return -1;
    }
    if (length >= MAX_PLAINTEXT) {
        set_error(err, err_size, "Texte trop long pour le moteur.");
        return -1;
    }

    memcpy(cipher, text, length);
    apply_stream(cipher, length, seed_from_key(key));
    checksum = crypto_checksum(cipher, length);
    bytes_to_hex(cipher, length, hex);

    needed = (size_t)CHECKSUM_HEX + 1 + strlen(hex) + 1;
    if (out_size < needed) {
        set_error(err, err_size, "Tampon de sortie trop petit.");
        return -2;
    }
    written = snprintf(out, out_size, "%016llx:%s", checksum, hex);
    if (written < 0 || (size_t)written >= out_size) {
        set_error(err, err_size, "Ecriture du message chiffre impossible.");
        return -2;
    }

    if (report != NULL) {
        memset(report, 0, sizeof *report);
        report->checksum      = checksum;
        report->entropy       = crypto_entropy(text, length);
        report->key_length    = strlen(key);
        report->key_score     = crypto_key_score(key);
        report->input_length  = length;
        report->output_length = (size_t)written;
    }
    return 0;
}

int crypto_decrypt(const char *frame, const char *key,
                   char *out, size_t out_size,
                   crypto_report *report,
                   char *err, size_t err_size)
{
    unsigned char cipher[MAX_PLAINTEXT];
    unsigned long long expected;
    unsigned long long actual;
    unsigned long long parsed;
    char *end = NULL;
    size_t frame_length;
    size_t data_length;
    size_t byte_length;

    if (frame == NULL || key == NULL || out == NULL) {
        set_error(err, err_size, "Parametres invalides.");
        return -1;
    }
    frame_length = strlen(frame);
    if (frame_length <= CHECKSUM_HEX + 1 || frame[CHECKSUM_HEX] != ':') {
        set_error(err, err_size, "Format attendu : checksum(16 hex) ':' donnees hex.");
        return -3;
    }
    data_length = frame_length - CHECKSUM_HEX - 1;
    if ((data_length % 2) != 0) {
        set_error(err, err_size, "Donnees hexadecimales de longueur impaire.");
        return -3;
    }
    byte_length = data_length / 2;
    if (byte_length >= MAX_PLAINTEXT) {
        set_error(err, err_size, "Message chiffre trop long.");
        return -3;
    }

    /* 1. le checksum annonce */
    parsed = strtoull(frame, &end, 16);
    if (end == frame || end != frame + CHECKSUM_HEX) {
        set_error(err, err_size, "Checksum hexadecimal invalide.");
        return -3;
    }
    expected = parsed;

    /* 2. le corps hexadecimal */
    if (!hex_to_bytes(frame + CHECKSUM_HEX + 1, data_length, cipher)) {
        set_error(err, err_size, "Caractere non hexadecimal dans le message.");
        return -3;
    }

    /* 3. controle d'integrite AVANT de dechiffrer */
    actual = crypto_checksum(cipher, byte_length);
    if (actual != expected) {
        set_error(err, err_size,
                  "Integrite compromise : le message chiffre a ete modifie.");
        return -4;
    }

    /* 4. dechiffrement */
    if (out_size < byte_length + 1) {
        set_error(err, err_size, "Tampon de sortie trop petit.");
        return -2;
    }
    apply_stream(cipher, byte_length, seed_from_key(key));
    memcpy(out, cipher, byte_length);
    out[byte_length] = '\0';

    if (report != NULL) {
        memset(report, 0, sizeof *report);
        report->checksum      = expected;
        report->entropy       = crypto_entropy(out, byte_length);
        report->key_length    = strlen(key);
        report->key_score     = crypto_key_score(key);
        report->input_length  = frame_length;
        report->output_length = byte_length;
    }
    return 0;
}
