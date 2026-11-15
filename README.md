# TP-Git-Collaboration

Application C de securite informatique : **chiffrement / dechiffrement simple de messages**.

TP Git & collaboration realise en solo : le travail d'equipe est simule avec
plusieurs branches, des Pull Requests et un conflit volontaire.

## Structure

```
TP-Git-Collaboration/
├── README.md
├── main.c        menu et orchestration
├── module1.c     saisie et validation
├── module2.c     traitement de securite (chiffrement / integrite)
├── module3.c     affichage et rapport de securite
├── security.h    declarations partagees
├── Makefile      compilation
└── .gitignore
```

## Etat du projet

| Module | Branche | Etat |
|--------|---------|------|
| module1.c - saisie / validation | `feature/module1` | a implementer |
| module2.c - moteur de securite | `feature/module2` | a implementer |
| module3.c - affichage / rapport | `feature/module3` | a implementer |

## Compilation

```bash
gcc -std=c11 -Wall -Wextra -O2 main.c module1.c module2.c module3.c -o securemsg.exe
```

La documentation complete (utilisation, tests, workflow Git, rapport) est
ajoutee a la fin du TP.

## Auteur

SAAD - 2026
