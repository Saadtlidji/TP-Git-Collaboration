/*
 * module3.c - Affichage des resultats et rapport de securite
 *
 * Ce module ne realise aucun calcul de securite : il presente les
 * donnees produites par module2.c et emet des recommandations.
 */
#include "security.h"

#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Elements d'interface                                                */
/* ------------------------------------------------------------------ */

static void separator(void)
{
    puts("+----------------------------------------------------------------+");
}

static void line(const char *text)
{
    printf("| %-62s |\n", text);
}

static void key_meter(int score)
{
    const int width = 30;
    int filled = (score * width) / 100;
    int i;

    printf("  Cle  [");
    for (i = 0; i < width; i++) {
        putchar(i < filled ? '#' : '.');
    }
    printf("] %3d/100  ", score);
    if (score < 40) {
        puts("FAIBLE");
    } else if (score < 70) {
        puts("MOYENNE");
    } else if (score < 90) {
        puts("BONNE");
    } else {
        puts("EXCELLENTE");
    }
}

void ui_banner(void)
{
    puts("");
    separator();
    line(APP_TITLE);
    line("Chiffrement de messages - demonstration pedagogique");
    line("Version " APP_VERSION "  |  Travail individuel : SAAD");
    separator();
}

void ui_menu(void)
{
    puts("");
    puts("  1. Chiffrer un message");
    puts("  2. Dechiffrer un message");
    puts("  3. Afficher les informations de securite");
    puts("  0. Quitter");
    puts("");
}

void ui_message(const char *level, const char *message)
{
    printf("  [%-6s] %s\n", level == NULL ? "INFO" : level,
           message == NULL ? "" : message);
}

/* ------------------------------------------------------------------ */
/* Rapport de securite                                                 */
/* ------------------------------------------------------------------ */

void report_show(const crypto_report *report, const char *title)
{
    if (report == NULL) {
        return;
    }
    puts("");
    separator();
    line(title == NULL ? "RAPPORT" : title);
    separator();
    printf("  Algorithme          : TP-XORMIX v1 (flux xorshift64*)\n");
    printf("  Taille entree       : %lu octet(s)\n", (unsigned long)report->input_length);
    printf("  Taille sortie       : %lu octet(s)\n", (unsigned long)report->output_length);
    printf("  Longueur de la cle  : %lu caractere(s)\n", (unsigned long)report->key_length);
    printf("  Entropie du message : %.2f bits/octet (%.1f bits au total)\n",
           report->entropy,
           report->entropy * (double)report->input_length);
    printf("  Checksum FNV-1a     : %016llx\n", report->checksum);
    key_meter(report->key_score);
    separator();
}

void report_security_advice(const crypto_report *report)
{
    int warnings = 0;

    if (report == NULL) {
        return;
    }
    puts("");
    puts("  RECOMMANDATIONS DE SECURITE");

    if (report->key_length < 8) {
        ui_message("ALERTE", "Cle courte : utilisez au moins 12 caracteres.");
        warnings++;
    } else if (report->key_length < 12) {
        ui_message("ATTENTION", "Cle de longueur moyenne : 12 caracteres ou plus sont conseilles.");
        warnings++;
    } else {
        ui_message("OK", "Longueur de cle conforme aux bonnes pratiques de demonstration.");
    }

    if (report->key_score < 50) {
        ui_message("ALERTE", "Cle faible : melangez minuscules, majuscules, chiffres et symboles.");
        warnings++;
    }

    if (report->entropy < 2.0) {
        ui_message("ATTENTION",
                   "Message tres repetitif : un attaquant pourrait deviner sa structure.");
        warnings++;
    }

    if (warnings == 0) {
        ui_message("OK", "Aucun probleme detecte sur ce message.");
    }

    ui_message("INFO", "TP-XORMIX est educatif : pour un usage reel, utilisez AES-GCM ou ChaCha20-Poly1305.");
    puts("");
}
