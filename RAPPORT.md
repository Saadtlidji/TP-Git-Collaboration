# RAPPORT — TP Git & collaboration

**Titre :** Application C de sécurité informatique — chiffrement / déchiffrement de messages
**Dépôt :** <https://github.com/Saadtlidji/TP-Git-Collaboration>
**Auteur :** SAAD — **travail strictement individuel**
**Date :** novembre 2026

---

## 1. Déclaration de travail individuel

Je déclare avoir réalisé ce TP **seul**. Aucun autre étudiant n'a participé au
projet : je n'ai donc pas eu de binôme. Pour pratiquer malgré tout l'ensemble
des compétences visées (branches, Pull Requests, revue, conflits, fusion), j'ai
**simulé une collaboration** :

- une branche dédiée par module (`feature/module1`, `feature/module2`,
  `feature/module3`) ;
- une Pull Request par branche, avec description et fusion dans `main` ;
- deux branches concurrentes (`feature/conflit-A`, `feature/conflit-B`)
  modifiant la **même ligne** de `main.c` pour provoquer un conflit volontaire,
  puis résolution manuelle.

**Outils utilisés :** terminal uniquement (PowerShell + Git for Windows 2.56,
GitHub CLI `gh` 2.104, GCC/MinGW-w64 16.2 pour la compilation). Aucune
interface graphique Git (pas de GitHub Desktop, pas de TortoiseGit, pas de
VS Code Git).

---

## 2. Présentation du projet

### 2.1 Thème choisi

**Chiffrement / déchiffrement simple de messages** (algorithme pédagogique
« TP-XORMIX v1 »).

### 2.2 Fonctionnement

1. L'utilisateur saisit un message et une clé.
2. La clé est transformée en graine 64 bits par l'empreinte **FNV-1a**.
3. Un générateur **xorshift64\*** produit un flux pseudo-aléatoire ; chaque
   octet est combiné par **XOR** avec ce flux et avec un décalage dépendant de
   sa position.
4. Le résultat est encodé en hexadécimal et précédé d'un **checksum FNV-1a
   64 bits** : `checksum(16 hex):données(hex)`.
5. Au déchiffrement, le checksum est **recalculé avant** de déchiffrer : toute
   altération d'un seul caractère est détectée.
6. Un **rapport de sécurité** affiche l'entropie de Shannon, la longueur et la
   robustesse de la clé, puis des recommandations.

### 2.3 Découpage en modules

| Fichier | Rôle | Branche |
|---------|------|---------|
| `main.c` | menu, boucle principale, orchestration | `main` |
| `module1.c` | saisie sûre (`fgets` + purge), validation message / clé / frame | `feature/module1` |
| `module2.c` | chiffrement, déchiffrement, checksum, entropie, score de clé | `feature/module2` |
| `module3.c` | bannière, menu, rapport de sécurité, recommandations | `feature/module3` |
| `security.h` | déclarations partagées | `main` |
| `Makefile` | compilation | `main` |

---

## 3. Réalisation des parties A à G

### Partie A — Installation et premier commit

```bash
git config --global user.name  "SAAD"
git config --global user.email "saaddidou738@gmail.com"
git init -b main
git remote add origin https://github.com/Saadtlidji/TP-Git-Collaboration.git
git add .gitignore README.md main.c module1.c module2.c module3.c security.h Makefile
git commit -m "chore: initialisation du projet ..."
git push -u origin main
```

Dépôt créé avec GitHub CLI (`gh repo create TP-Git-Collaboration --public`),
puis initialisé et poussé. Le premier commit contient la structure complète
avec les modules à l'état de squelette.

### Partie B — Branches de fonctionnalité et Pull Requests

| Branche | Commit | Pull Request | État |
|---------|--------|--------------|------|
| `feature/module1` | `feat(module1): saisie securisee et validation des entrees` | [#1](https://github.com/Saadtlidji/TP-Git-Collaboration/pull/1) | fusionnée |
| `feature/module2` | `feat(module2): moteur de chiffrement TP-XORMIX et controle d'integrite` | [#2](https://github.com/Saadtlidji/TP-Git-Collaboration/pull/2) | fusionnée |
| `feature/module3` | `feat(module3): rapport de securite et recommandations` | [#3](https://github.com/Saadtlidji/TP-Git-Collaboration/pull/3) | fusionnée |
| `docs/rapport-final` | documentation, rapport, preuves | #4 | fusionnée |
| `fix/docs-refinements` | corrections de cohérence et renvois | #5 | fusionnée |
| `docs/pr-list` | liste complète des Pull Requests | #6 | fusionnée |

Chaque branche a été compilée et testée **avant** le commit, puis poussée et
fusionnée dans `main` via `gh pr merge --merge` (merge commit, pas de
fast-forward) afin de conserver la trace de la Pull Request.

### Partie C — Synchronisation et notifications

Après chaque fusion : `git switch main && git pull origin main` (plusieurs
fast-forwards constatés, voir `docs/EVIDENCE.md`).

Notifications : le dépôt est en **Watch → Custom** (vérifié par l'API :
`{"subscribed":true,"ignored":false}`). Limite du travail solo : GitHub ne
notifie pas un utilisateur de ses propres actions ; aucune notification n'a donc
pu être déclenchée par un tiers. La configuration est en place et
`https://github.com/notifications` est actif pour le dépôt.

### Partie D — Conflit volontaire

1. `main.c` contient la ligne commune
   `printf("Application de securite informatique\n");`
2. `feature/conflit-A` la remplace par `... - version A` ; `feature/conflit-B`
   par `... - version B` (les deux branches partent du même commit).
3. `feature/conflit-A` est fusionnée dans `main` puis poussée.
4. La fusion de `feature/conflit-B` échoue :

```
Auto-merging main.c
CONFLICT (content): Merge conflict in main.c
Automatic merge failed; fix conflicts and then commit the result.
```

```
<<<<<<< HEAD
    printf("Application de securite informatique - version A\n");
=======
    printf("Application de securite informatique - version B\n");
>>>>>>> feature/conflit-B
```

5. Résolution **manuelle** (édition du fichier, suppression des marqueurs) :

```c
printf("Application de securite informatique - version A + B\n");
```

6. `git add main.c`, puis commit de fusion `fix(conflit): resolution manuelle
   du conflit A/B dans main.c`, rendu exécutable après recompilation, et push.

### Partie E — Authentification SSH

```bash
ssh-keygen -t ed25519 -C "saaddidou738@gmail.com" -f ~/.ssh/id_ed25519
gh ssh-key add ~/.ssh/id_ed25519.pub --title "TP-Git-Collaboration Windows"
ssh -T git@github.com
git remote set-url origin git@github.com:Saadtlidji/TP-Git-Collaboration.git
```

Clé **Ed25519** (empreinte `SHA256:BQxzd37Jcw7nfn5HIMlldwkBEKYCS62uZuUdb4DDKF0`),
clé publique seule publiée sur GitHub, clé privée jamais affichée ni versionnée.
Preuves : `ssh -T` et un `git push` / `git pull` effectués avec l'URL SSH
(section 6 de `docs/EVIDENCE.md`).

### Partie F — .gitignore

Fichiers ignorés : binaires (`*.exe`, `*.o`), temporaires (`*.tmp`, `*.log`),
secrets (`*.local`, `.env`, `config.local`), dossiers d'éditeur (`.vscode/`,
`.idea/`), fichiers système et dossiers de build.

Vérifications :

```
$ git status --short
(aucune sortie : rien à committer)

$ git check-ignore -v app.log notes.tmp .env config.local .vscode/settings.json
.gitignore:22:*.log                    app.log
.gitignore:20:*.tmp                    notes.tmp
.gitignore:28:.env                     .env
.gitignore:31:config.local             config.local
.gitignore:35:.vscode/                 .vscode/settings.json
```

Exercice `git rm --cached` : ajout forcé de `config.local` (`git add -f`),
constat de l'erreur, puis `git rm --cached config.local` : le fichier reste sur
le disque mais sort du versionnement et réapparaît comme ignoré.

### Partie G — Git avancé (dépôt secondaire `advanced-git-lab`)

Réalisé dans un dépôt de test séparé pour ne pas polluer l'historique du projet
(transcriptions complètes : `docs/advanced-git-lab.md` et
`docs/advanced-git-lab-conflicts.md`).

- **Reset** : `--soft` (commit annulé, modifications indexées), `--mixed`
  (modifications désindexées), `--hard` (tout supprimé).
- **Reflog** : commit perdu après `reset --hard` retrouvé (`git reflog` →
  `0a96f16` → `git reset --hard 0a96f16`).
- **Revert** : commit défectueux `E` déjà poussé annulé par
  `git revert --no-edit HEAD`, puis poussé (l'historique reste complet).
- **Fork** : fork de `octocat/Hello-World`, remote `upstream` ajouté, branche
  `tp/fork-demo` poussée, Pull Request
  [#1](https://github.com/Saadtlidji/Hello-World/pull/1) ouverte depuis le fork.
  (GitHub interdit le fork de son propre dépôt : « A single user account cannot
  own both a parent and fork », d'où l'usage d'un dépôt bac à sable public.)
- **Branches** : création, renommage (`git branch -m`), suppression locale
  (`-d`/`-D`) et distante (`git push origin --delete`), `git branch -vv`,
  `git fetch --prune` (branche distante supprimée → `gone`).
- **Conflit multi-fichiers** : `alpha.txt` et `beta.txt` modifiés des deux
  côtés, résolution par `git checkout --ours alpha.txt` et
  `git checkout --theirs beta.txt`.
- **Stratégies de fusion** : fast-forward, `--no-ff`, `--squash`, puis rebase
  d'une branche sur `main` suivie d'un `--ff-only`.

---

## 4. Réponses aux questions

**Q1. Quelle est la différence entre Git et GitHub ?**
Git est un logiciel de gestion de versions décentralisé qui fonctionne
localement (commits, branches, historique). GitHub est un service d'hébergement
de dépôts Git qui ajoute la collaboration (Pull Requests, revues, issues,
notifications, droits d'accès). Git peut exister sans GitHub ; l'inverse est
faux.

**Q2. À quoi sert une branche ? Pourquoi une branche par fonctionnalité ?**
Une branche est un pointeur mobile vers un commit : elle isole un travail en
cours sans impacter `main`. Une branche par fonctionnalité permet de travailler
et de tester indépendamment, de livrer des changements de petite taille,
facilement relisibles, et de revenir en arrière sans casser la version stable.

**Q3. Qu'est-ce qu'une Pull Request et quel est son intérêt ?**
C'est une demande de fusion d'une branche vers une autre, accompagnée d'un
diff, de commentaires et d'un historique de discussion. Elle permet la revue de
code, les tests automatisés et la traçabilité (ici PR #1 à #3). Même en solo,
elle force à documenter la modification et à vérifier la compilation avant
fusion.

**Q4. Comment un conflit apparaît-il et comment l'avez-vous résolu ?**
Un conflit apparaît quand deux branches modifient la même zone d'un fichier
depuis leur ancêtre commun : Git ne peut pas choisir. J'ai reproduit le cas sur
la ligne du titre dans `main.c` (`A` vs `B`). J'ai édité le fichier pour
supprimer `<<<<<<<`, `=======`, `>>>>>>>`, choisi une formulation fusionnant les
deux versions, recompilé pour vérifier, puis `git add` + commit de fusion.
Dans le dépôt de test, j'ai aussi résolu un conflit multi-fichiers avec
`--ours` / `--theirs`.

**Q5. Différence entre `fetch`, `pull`, `merge`, `rebase` ?**
`fetch` télécharge les nouveautés sans toucher à la copie de travail ; `pull` =
`fetch` + `merge` (ou rebase) ; `merge` crée un commit de fusion qui conserve
les deux histoires ; `rebase` rejoue les commits locaux sur une nouvelle base et
produit un historique linéaire, mais réécrit les identifiants de commits (à
éviter sur des branches déjà partagées).

**Q6. Différence entre `reset --soft`, `--mixed`, `--hard` et `revert` ? Reflog ?**
`reset` déplace la branche en arrière : `--soft` garde les modifications
indexées, `--mixed` les désindexe, `--hard` supprime aussi les modifications de
la copie de travail. `reset` réécrit l'historique : à proscrire sur des commits
déjà poussés. `revert` crée au contraire un nouveau commit qui annule un commit
existant : c'est la méthode sûre pour un commit public. `reflog` est le journal
local des déplacements de HEAD : il permet de retrouver un commit « perdu »
après un `reset --hard`.

**Q7. À quoi sert `.gitignore` ? Pourquoi `git rm --cached` ?**
`.gitignore` empêche Git de proposer les fichiers non suivis correspondant aux
motifs (binaires, journaux, secrets, dossiers d'IDE). Il ne s'applique pas aux
fichiers déjà suivis : pour arrêter de versionner un fichier tout en le gardant
sur le disque, il faut `git rm --cached <fichier>` puis committer. C'est ce que
j'ai fait pour `config.local`, qui contenait une configuration locale.

**Q8. HTTPS vs SSH ? Pourquoi Ed25519 ?**
HTTPS demande un identifiant/token à chaque échange (ou un gestionnaire
d'identifiants) ; SSH utilise une paire de clés : la clé publique est déposée
sur GitHub, la clé privée reste sur la machine. SSH évite de manipuler des
tokens et permet `ssh -T git@github.com` comme test. Ed25519 est un algorithme
moderne, rapide, à clé courte (256 bits) et très largement recommandé.

**Q9. Différence entre `clone` et `fork` ? À quoi sert `upstream` ?**
`clone` copie un dépôt sur la machine ; `fork` crée une copie du dépôt dans
**son propre compte GitHub**, ce qui permet de proposer des modifications sans
droit d'écriture sur l'original. `upstream` est le nom conventionnel du remote
pointant vers le dépôt d'origine, pour récupérer ses mises à jour :
`git fetch upstream`.

**Q10. Stratégies de fusion pratiquées ?**
Fast-forward (aucun commit de fusion, historique linéaire), `--no-ff` (commit de
fusion explicite, la branche reste visible), `--squash` (tous les commits de la
branche regroupés en un seul, sans lien de parenté), rebase (commits rejoués,
historique linéaire sans commit de fusion). Le choix dépend de la politique de
l'équipe ; `--no-ff` est pratique pour garder la trace des Pull Requests.

**Q11. Quel intérêt de cette organisation en équipe ?**
Elle réduit les risques : chacun travaille isolé sur sa branche, les fusions
sont relues, les conflits sont détectés tôt et les tests peuvent être automatisés
sur chaque Pull Request. L'historique devient lisible et chaque modification est
justifiée par un message de commit explicite.

**Q12. Qu'avez-vous appris ?**
La différence entre l'état de la copie de travail, l'index et les commits ;
la mécanique exacte d'un conflit et la manière de le résoudre proprement ;
l'utilité du reflog comme filet de sécurité ; la différence entre réécrire
l'historique (`reset`, `rebase`) et l'enrichir (`revert`, merge) ; et la mise en
place d'une authentification SSH.

---

## 5. Tests fonctionnels de l'application

| Test | Entrée | Résultat observé |
|------|--------|------------------|
| Aller-retour | `Bonjour le monde, ceci est un test de chiffrement.` / `S3cret!Key2026` | texte identique après déchiffrement |
| Intégrité | un caractère hexadécimal modifié | `[ERREUR] Integrite compromise : le message chiffre a ete modifie.` |
| Clé trop courte | `abc` | `[REFUS ] Cle trop courte (4 caracteres minimum).` |
| Message vide | ligne vide | `[REFUS ] Le message est vide.` |
| Message de espaces | `     ` | `[REFUS ] Le message ne contient que des espaces.` |
| Caractère non ASCII | octet `0x82` | `[REFUS ] Le message doit rester en ASCII imprimable` |
| Mauvaise clé | `MauvaiseCle99` | `[ALERTE] Cle probablement incorrecte : le resultat n'est pas du texte lisible.` |
| Choix invalide | `9`, `abc` | `Choix hors plage (0-3)` / `Veuillez saisir un nombre.` |

Compilation : `gcc -std=c11 -Wall -Wextra -O2 main.c module1.c module2.c module3.c -o securemsg.exe`
→ aucune erreur, aucun avertissement.

---

## 6. Difficultés rencontrées

1. **Fork impossible sur son propre dépôt** : GitHub refuse (« A single user
   account cannot own both a parent and fork »). Solution : fork d'un dépôt
   public de démonstration (`octocat/Hello-World`), avec `upstream` configuré et
   une Pull Request ouverte depuis le fork.
2. **Notifications en solo** : GitHub ne notifie pas ses propres actions.
   Solution : configuration Watch → Custom vérifiée par API, notification
   documentée comme dépendante de l'interaction d'un tiers.
3. **Conflit volontaire** : la première fusion ne doit pas être un squash, sinon
   l'ancêtre commun change et le conflit n'apparaît pas comme prévu. J'ai donc
   fusionné `feature/conflit-A` avec un merge commit.
4. **Environnement Windows** : encodage de la console (accents) et fins de ligne
   CRLF/LF. L'application refuse volontairement les caractères non ASCII pour
   rester portable ; `.gitattributes` n'a pas été nécessaire.

---

## 7. Conclusion

Le TP couvre l'ensemble de la chaîne : initialisation, branches, Pull Requests,
synchronisation, conflit volontaire et résolution, authentification SSH,
`.gitignore` et exercices avancés (reset/revert/reflog, fork, gestion de
branches, conflits multi-fichiers, stratégies de fusion). L'application C
compile sans avertissement, est testée sur huit cas d'erreur et de succès, et
illustre concrètement un traitement de sécurité : chiffrement à clé partagée,
contrôle d'intégrité et rapport de sécurité. Ce travail a été réalisé
**individuellement**, avec le terminal comme seul outil Git.
