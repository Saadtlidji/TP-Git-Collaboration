# TP-Git-Collaboration

Petite application C de **securite informatique** : chiffrement et dechiffrement
de messages avec controle d'integrite, rapport de securite et conseils.

> TP Git & collaboration realise **en solo** (voir `RAPPORT.md`) : le travail
> d'equipe est simule avec plusieurs branches, des Pull Requests et un conflit
> volontaire.

## Theme choisi

**Chiffrement / dechiffrement simple de messages** (TP-XORMIX v1).

## Structure du projet

```
TP-Git-Collaboration/
├── README.md        Documentation du projet
├── RAPPORT.md       Rapport ecrit (compte rendu du TP)
├── main.c           Menu principal et orchestration
├── module1.c        Saisie et validation des entrees
├── module2.c        Moteur de securite (chiffrement, integrite, entropie)
├── module3.c        Affichage des resultats et rapport de securite
├── security.h       Declarations partagees
├── Makefile         Compilation (gcc / mingw32-make)
├── .gitignore       Fichiers exclus du versionnement
└── docs/            Preuves d'execution et captures texte des exercices Git
```

| Fichier | Role |
|---------|------|
| `main.c` | Affiche le menu, appelle les trois modules, boucle jusqu'a la sortie |
| `module1.c` | `fgets` securise, lecture du choix, validation du texte / cle / frame |
| `module2.c` | Chiffrement XOR a flux xorshift64*, checksum FNV-1a, entropie, score de cle |
| `module3.c` | Banniere, menu, messages, rapport de securite et recommandations |

## Compilation

Avec MinGW-w64 / GCC :

```bash
gcc -std=c11 -Wall -Wextra -O2 main.c module1.c module2.c module3.c -o securemsg.exe
```

ou avec le Makefile :

```bash
make          # produit securemsg.exe
make run      # compile puis execute
make clean    # supprime les binaires
```

Le code compile sans avertissement avec `-Wall -Wextra`.

## Utilisation

```
Application de securite informatique

+----------------------------------------------------------------+
| Application de securite informatique                           |
| Chiffrement de messages - demonstration pedagogique            |
| Version 1.0.0  |  Travail individuel : SAAD                    |
+----------------------------------------------------------------+

  1. Chiffrer un message
  2. Dechiffrer un message
  3. Afficher les informations de securite
  0. Quitter
```

Exemple de session :

```
Votre choix > 1
  Message en clair : Bonjour le monde, ceci est un test de chiffrement.
  Cle de chiffrement : S3cret!Key2026

  Message chiffre (a copier tel quel pour dechiffrer) :
  9f775a586219fcaf:e7b26fd9c5e30137bdc3e5966ca9406e47abf29d6df7cd5a06a35486977a7ccf8a275cd5bfc701d7bcaa342b91bc83c1e82e

+----------------------------------------------------------------+
| RAPPORT DE CHIFFREMENT                                         |
+----------------------------------------------------------------+
  Algorithme          : TP-XORMIX v1 (flux xorshift64*)
  Taille entree       : 50 octet(s)
  Taille sortie       : 117 octet(s)
  Longueur de la cle  : 14 caractere(s)
  Entropie du message : 3.89 bits/octet (194.7 bits au total)
  Checksum FNV-1a     : 9f775a586219fcaf
  Cle  [##############################] 100/100  EXCELLENTE
+----------------------------------------------------------------+
```

Le message chiffre a la forme `checksum(16 hex):donnees(hex)` :
le checksum FNV-1a 64 bits sert de controle d'integrite et permet de
detecter toute alteration du message chiffre avant dechiffrement.

## Fonctionnalites

- Chiffrement / dechiffrement symetrique a cle partagee
- Controle d'integrite par checksum FNV-1a 64 bits
- Validation stricte des entrees (message, cle, format du message chiffre)
- Rapport de securite : taille, entropie de Shannon, score de robustesse de la cle
- Recommandations automatiques (cle courte, cle faible, message repetitif)
- Detection d'une cle erronee (le resultat dechiffre n'est pas du texte lisible)
- Aucune dependance externe (C11 + libm)

## Tests realises

| Test | Entree | Resultat attendu | Verifie |
|------|--------|------------------|---------|
| Aller-retour | texte + cle valide | texte identique | oui |
| Message altere | 1 caractere hex modifie | `Integrite compromise` | oui |
| Cle trop courte | `abc` | `Cle trop courte` | oui |
| Message vide | ligne vide | `Le message est vide` | oui |
| Caractere accentue | `e` (0x82) | `ASCII imprimable` | oui |
| Cle erronee | mauvaise cle | `Cle probablement incorrecte` | oui |
| Choix de menu invalide | `9`, `abc` | message d'erreur, nouvelle saisie | oui |

Les traces completes sont dans `docs/EVIDENCE.md`.

## Avertissement de securite

TP-XORMIX est un algorithme **pedagogique** : il illustre le principe d'un
chiffrement a flux, pas la cryptographie moderne. Il n'est ni authentifie ni
audite et ne doit jamais proteger de donnees reelles. Pour un usage reel :
**AES-256-GCM**, **ChaCha20-Poly1305** ou une bibliotheque comme libsodium.

## Workflow Git utilise

1. `main` protegee par convention, une branche par module.
2. `feature/module1`, `feature/module2`, `feature/module3` : une Pull Request
   par branche, revue puis fusion dans `main`.
3. `feature/conflit-A` et `feature/conflit-B` : conflit volontaire sur la
   premiere ligne de `main.c`, resolu manuellement.
4. Authentification SSH (cle Ed25519) pour les echanges avec GitHub.
5. `.gitignore` verifie avec `git check-ignore -v`.

Details et preuves : `docs/EVIDENCE.md` et `RAPPORT.md`.

## Auteur

SAAD - travail individuel - 2026.
