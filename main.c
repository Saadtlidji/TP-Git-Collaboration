/*
 * main.c - Point d'entree de l'application de securite
 *
 * Theme : chiffrement / dechiffrement simple de messages.
 *
 * Organisation du projet :
 *   module1.c  -> saisie et validation des entrees
 *   module2.c  -> moteur de chiffrement, integrite, entropie
 *   module3.c  -> affichage des resultats et rapport de securite
 *
 * Compilation (MinGW-w64) :
 *   gcc -std=c11 -Wall -Wextra -O2 main.c module1.c module2.c module3.c -o securemsg.exe
 */
#include "security.h"

#include <stdio.h>
#include <string.h>

static void do_encrypt(void)
{
    char text[MAX_PLAINTEXT];
    char key[MAX_KEY];
    char frame[MAX_FRAME];
    char reason[128];
    char error[128];
    crypto_report report;

    printf("\n  --- CHIFFREMENT ---\n");
    if (!input_read_line("  Message en clair : ", text, sizeof text)) {
        ui_message("ERREUR", "Lecture du message impossible.");
        return;
    }
    if (!validate_plaintext(text, reason, sizeof reason)) {
        ui_message("REFUS", reason);
        return;
    }
    if (!input_read_line("  Cle de chiffrement : ", key, sizeof key)) {
        ui_message("ERREUR", "Lecture de la cle impossible.");
        return;
    }
    if (!validate_key(key, reason, sizeof reason)) {
        ui_message("REFUS", reason);
        return;
    }

    if (crypto_encrypt(text, key, frame, sizeof frame, &report, error, sizeof error) != 0) {
        ui_message("ERREUR", error);
        return;
    }

    puts("\n  Message chiffre (a copier tel quel pour dechiffrer) :");
    printf("  %s\n", frame);
    report_show(&report, "RAPPORT DE CHIFFREMENT");
    report_security_advice(&report);
}

static void do_decrypt(void)
{
    char frame[MAX_FRAME];
    char key[MAX_KEY];
    char text[MAX_PLAINTEXT];
    char reason[128];
    char error[128];
    crypto_report report;

    printf("\n  --- DECHIFFREMENT ---\n");
    if (!input_read_line("  Message chiffre : ", frame, sizeof frame)) {
        ui_message("ERREUR", "Lecture du message chiffre impossible.");
        return;
    }
    if (!validate_frame(frame, reason, sizeof reason)) {
        ui_message("REFUS", reason);
        return;
    }
    if (!input_read_line("  Cle de dechiffrement : ", key, sizeof key)) {
        ui_message("ERREUR", "Lecture de la cle impossible.");
        return;
    }
    if (!validate_key(key, reason, sizeof reason)) {
        ui_message("REFUS", reason);
        return;
    }

    if (crypto_decrypt(frame, key, text, sizeof text, &report, error, sizeof error) != 0) {
        ui_message("ERREUR", error);
        return;
    }
    /* Une cle erronee ne fait pas echouer le checksum (calcule sur le
     * chiffre) : on verifie donc que le resultat est bien du texte. */
    if (!validate_plaintext(text, reason, sizeof reason)) {
        ui_message("ALERTE", "Cle probablement incorrecte : le resultat n'est pas du texte lisible.");
        return;
    }

    printf("\n  Message en clair retrouve : %s\n", text);
    report_show(&report, "RAPPORT DE DECHIFFREMENT");
    report_security_advice(&report);
}

static void do_about(void)
{
    puts("");
    puts("  A propos de cette application");
    puts("  -----------------------------");
    puts("  Projet      : TP Git & collaboration (travail individuel)");
    puts("  But         : illustrer branches, pull requests et conflits Git");
    puts("  Modele      : flux xorshift64* melange par XOR + checksum FNV-1a");
    puts("  Limite      : algorithme pedagogique, non adapte a la production");
    puts("  Remplacement: AES-256-GCM, ChaCha20-Poly1305, libsodium");
    puts("");
}

int main(void)
{
    int choice;

    printf("Application de securite informatique\n");
    ui_banner();

    for (;;) {
        ui_menu();
        choice = input_read_choice(0, 3);
        if (choice < 0) {
            ui_message("INFO", "Fin de flux detectee, fermeture.");
            break;
        }
        switch (choice) {
        case 1:
            do_encrypt();
            break;
        case 2:
            do_decrypt();
            break;
        case 3:
            do_about();
            break;
        case 0:
            ui_message("INFO", "Merci d'avoir utilise l'application. A bientot.");
            return 0;
        default:
            ui_message("ERREUR", "Choix inconnu.");
            break;
        }
    }
    return 0;
}
