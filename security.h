/*
 * security.h - Interface commune du projet TP-Git-Collaboration
 *
 * Theme : chiffrement / dechiffrement simple de messages.
 * Auteur : SAAD (travail individuel)
 *
 * Ce fichier contient uniquement les declarations partagees par les
 * trois modules et par main.c. Aucun code executable ici.
 */
#ifndef SECURITY_H
#define SECURITY_H

#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Constantes globales                                                 */
/* ------------------------------------------------------------------ */
#define APP_TITLE   "Application de securite informatique"
#define APP_VERSION "1.0.0"

#define MAX_PLAINTEXT 512   /* taille max du message en clair          */
#define MAX_KEY       64    /* taille max de la cle                    */
#define MAX_FRAME    4096   /* taille max du message chiffre (hex + crc)*/
#define CHECKSUM_HEX  16    /* 16 caracteres hex = 64 bits             */

/* ------------------------------------------------------------------ */
/* module1.c : saisie et validation                                    */
/* ------------------------------------------------------------------ */

/* Lit une ligne sur stdin, supprime le retour chariot.
 * Retourne 1 si une ligne a ete lue, 0 en cas d'erreur / fin de flux. */
int input_read_line(const char *prompt, char *buffer, size_t size);

/* Lit un entier dans [min,max]. Retourne la valeur, ou -1 si EOF. */
int input_read_choice(int min, int max);

/* Valide un message en clair. Retourne 1 si valide, sinon 0 et
 * remplit "reason" avec un motif lisible. */
int validate_plaintext(const char *text, char *reason, size_t reason_size);

/* Valide une cle de chiffrement (1 = valide, 0 = refusee). */
int validate_key(const char *key, char *reason, size_t reason_size);

/* Valide la forme d'un message chiffre "crc16hex:donnees_hex".
 * Ne verifie pas la cryptographie, seulement la syntaxe. */
int validate_frame(const char *frame, char *reason, size_t reason_size);

/* ------------------------------------------------------------------ */
/* module2.c : traitement de securite                                  */
/* ------------------------------------------------------------------ */

/* Rapport produit par le moteur de chiffrement. */
typedef struct {
    unsigned long long checksum;    /* empreinte FNV-1a du chiffre     */
    double             entropy;     /* entropie Shannon (bits/octet)    */
    size_t             key_length;  /* longueur de la cle               */
    int                key_score;   /* robustesse de la cle sur 100     */
    size_t             input_length;
    size_t             output_length;
} crypto_report;

/* Chiffre "text" avec "key" et ecrit la frame dans "out".
 * Retourne 0 en cas de succes, un code negatif sinon et remplit "err". */
int crypto_encrypt(const char *text, const char *key,
                   char *out, size_t out_size,
                   crypto_report *report,
                   char *err, size_t err_size);

/* Dechiffre une frame produite par crypto_encrypt().
 * Verifie l'integrite (checksum) avant de rendre le texte.
 * Retourne 0 en cas de succes, un code negatif sinon. */
int crypto_decrypt(const char *frame, const char *key,
                   char *out, size_t out_size,
                   crypto_report *report,
                   char *err, size_t err_size);

/* Empreinte FNV-1a 64 bits, utilisee comme controle d'integrite. */
unsigned long long crypto_checksum(const void *data, size_t length);

/* Entropie de Shannon d'un bloc de donnees, en bits par octet. */
double crypto_entropy(const void *data, size_t length);

/* Score de robustesse d'une cle, de 0 a 100. */
int crypto_key_score(const char *key);

/* ------------------------------------------------------------------ */
/* module3.c : affichage et rapport de securite                        */
/* ------------------------------------------------------------------ */
void ui_banner(void);
void ui_menu(void);
void ui_message(const char *level, const char *message);
void report_show(const crypto_report *report, const char *title);
void report_security_advice(const crypto_report *report);

#endif /* SECURITY_H */
