/*
 * module3.c - Affichage et rapport de securite  (SQUELETTE)
 *
 * Cette version affiche une interface minimale : la mise en forme
 * complete et les recommandations sont realisees sur feature/module3.
 */
#include "security.h"

#include <stdio.h>

void ui_banner(void)
{
    printf("\n=== %s ===\n", APP_TITLE);
}

void ui_menu(void)
{
    puts("");
    puts("  1. Chiffrer un message");
    puts("  2. Dechiffrer un message");
    puts("  3. Afficher les informations de securite");
    puts("  0. Quitter");
}

void ui_message(const char *level, const char *message)
{
    printf("  [%s] %s\n", level == NULL ? "INFO" : level,
           message == NULL ? "" : message);
}

void report_show(const crypto_report *report, const char *title)
{
    (void)report;
    printf("\n  %s : rapport non implemente (feature/module3).\n",
           title == NULL ? "RAPPORT" : title);   /* TODO(feature/module3) */
}

void report_security_advice(const crypto_report *report)
{
    (void)report;
    puts("  Recommandations non implementees (feature/module3).");
}
