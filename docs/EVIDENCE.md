# Preuves d'exécution (transcriptions de terminal)

Toutes les preuves ci-dessous sont des **copies de sorties de terminal**
(PowerShell + Git 2.56 + GitHub CLI 2.104). Les captures d'écran ne sont pas
nécessaires : les Commandes et leurs sorties exactes sont conservées ici.
Les journaux bruts des exercices avancés se trouvent dans :

- `docs/advanced-git-lab.md` — reset / revert / reflog / branches / stratégies de fusion
- `docs/advanced-git-lab-conflicts.md` — conflit multi-fichiers `--ours` / `--theirs`

Dépôt principal : <https://github.com/Saadtlidji/TP-Git-Collaboration>

---

## 1. Identité Git et dépôt distant

```text
$ git config --local --list | Select-String 'user\.'
user.name=SAAD
user.email=saaddidou738@gmail.com

$ git remote -v
origin  https://github.com/Saadtlidji/TP-Git-Collaboration.git (fetch)
origin  https://github.com/Saadtlidji/TP-Git-Collaboration.git (push)
```

## 2. Historique des commits et des fusions

```text
$ git log --oneline --graph --all -12
*   5c32001 Merge pull request #3 from Saadtlidji/feature/module3
|\
| * 7115d2e feat(module3): rapport de securite et recommandations
|/
*   474ec66 Merge pull request #2 from Saadtlidji/feature/module2
|\
| * 9064c95 feat(module2): moteur de chiffrement TP-XORMIX et controle d'integrite
|/
*   19330b2 Merge pull request #1 from Saadtlidji/feature/module1
|\
| * f7ef010 feat(module1): saisie securisee et validation des entrees
|/
* 8e6128e chore: initialisation du projet (structure, main, squelettes des modules, gitignore)
```

## 3. Pull Requests

```text
$ gh pr list --state all
5  docs: corrections de coherence (renvois, exercices avances)     fix/docs-refinements  MERGED
4  docs: documentation complete et rapport du TP                   docs/rapport-final    MERGED
3  feat(module3): affichage des resultats et rapport de securite    feature/module3       MERGED
2  feat(module2): moteur de chiffrement et integrite                feature/module2       MERGED
1  feat(module1): saisie et validation des entrees                  feature/module1       MERGED
```

URLs : `.../pull/1` à `.../pull/5`.

## 4. Synchronisation après fusion (Partie C)

```text
$ git switch main
$ git pull origin main
From https://github.com/Saadtlidji/TP-Git-Collaboration
 * branch            main       -> FETCH_HEAD
   8e6128e..19330b2  main       -> origin/main
Updating 8e6128e..19330b2
Fast-forward
 module1.c | 204 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++------
 1 file changed, 185 insertions(+), 19 deletions(-)
```

### Notifications (Watch → Custom)

```text
$ gh api -X PUT repos/Saadtlidji/TP-Git-Collaboration/subscription -F subscribed=true -F ignored=false
{"subscribed":true,"ignored":false,"reason":null,"created_at":"2026-11-15T18:09:56Z", ...}
```

Le dépôt est donc bien en mode « Custom / Participating and @mentions ».
GitHub ne génère pas de notification pour ses propres actions : en travail
solo, la réception d'une notification ne peut être déclenchée que par un tiers.

## 5. Conflit volontaire et résolution (Partie D)

Création des deux branches depuis le même commit, puis fusion de A :

```text
$ git merge --no-ff feature/conflit-A -m "merge: integration de feature/conflit-A (version A)"
Merge made by the 'ort' strategy.
 main.c | 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)
```

Fusion de B → conflit :

```text
$ git merge --no-ff feature/conflit-B -m "merge: integration de feature/conflit-B (version B)"
Auto-merging main.c
CONFLICT (content): Merge conflict in main.c
Automatic merge failed; fix conflicts and then commit the result.

$ git status --short
UU main.c
```

Contenu du fichier pendant le conflit :

```c
int main(void)
{
    int choice;

<<<<<<< HEAD
    printf("Application de securite informatique - version A\n");
=======
    printf("Application de securite informatique - version B\n");
>>>>>>> feature/conflit-B
    ui_banner();
```

Résolution manuelle (marqueurs supprimés, versions fusionnées) :

```c
    /* Conflit resolu manuellement : les deux titres sont fusionnes
     * (feature/conflit-A avait "version A", feature/conflit-B "version B"). */
    printf("Application de securite informatique - version A + B\n");
```

Vérification et commit :

```text
$ Select-String -Path main.c -Pattern '<<<<<<<|=======|>>>>>>>'
(aucune sortie)

$ gcc -std=c11 -Wall -Wextra -O2 main.c module1.c module2.c module3.c -o securemsg.exe
BUILD_EXIT=0

$ .\securemsg.exe
Application de securite informatique - version A + B

$ git add main.c
$ git commit -m "fix(conflit): resolution manuelle du conflit A/B dans main.c"
[main dd81a8d] fix(conflit): resolution manuelle du conflit A/B dans main.c
```

## 6. Authentification SSH (Partie E)

Génération de la paire de clés (la clé privée n'est jamais affichée) :

```text
$ ssh-keygen -t ed25519 -C "saaddidou738@gmail.com" -f ~/.ssh/id_ed25519 -N ""
Generating public/private ed25519 key pair.
Your identification has been saved in C:\Users\saadd\.ssh\id_ed25519
Your public key has been saved in C:\Users\saadd\.ssh\id_ed25519.pub
The key fingerprint is:
SHA256:BQxzd37Jcw7nfn5HIMlldwkBEKYCS62uZuUdb4DDKF0 saaddidou738@gmail.com
```

Clé publique déposée sur GitHub (seule la partie publique est publiée) :

```text
$ gh ssh-key add ~/.ssh/id_ed25519.pub --title "TP-Git-Collaboration Windows"
```

Test de la connexion (preuve d'authentification, sans jamais afficher la clé privée) :

```text
$ ssh -T git@github.com
Hi Saadtlidji! You've successfully authenticated, but GitHub does not provide shell access.
```

Changement du remote HTTPS → SSH :

```text
$ git remote -v                       # avant
origin  https://github.com/Saadtlidji/TP-Git-Collaboration.git (fetch)
origin  https://github.com/Saadtlidji/TP-Git-Collaboration.git (push)

$ git remote set-url origin git@github.com:Saadtlidji/TP-Git-Collaboration.git

$ git remote -v                       # apres
origin  git@github.com:Saadtlidji/TP-Git-Collaboration.git (fetch)
origin  git@github.com:Saadtlidji/TP-Git-Collaboration.git (push)
```

Preuve de push et de pull en SSH (aucune demande de mot de passe ni de token) :

```text
$ git push origin docs/rapport-final
To github.com:Saadtlidji/TP-Git-Collaboration.git
   050564c..1366949  docs/rapport-final -> docs/rapport-final

$ git pull --ff-only origin docs/rapport-final
 * branch            docs/rapport-final -> FETCH_HEAD
Already up to date.

$ git ls-remote --heads origin
136694991a2af7832230b14396e3080be0ab5d32    refs/heads/docs/rapport-final
6dd3ad13c94d86ba0d484db89cc5ef0c8c2001d4    refs/heads/feature/conflit-A
...
```

Empreinte de la clé publique utilisée (la clé privée `~/.ssh/id_ed25519` reste
sur la machine et n'a jamais été affichée ni versionnée) :

```text
SHA256:BQxzd37Jcw7nfn5HIMlldwkBEKYCS62uZuUdb4DDKF0  saaddidou738@gmail.com (ED25519)
```

## 7. .gitignore (Partie F)

```text
$ git status --short
(aucune sortie)

$ git status --ignored --short
!! .env
!! .idea/
!! .vscode/
!! app.log
!! config.local
!! notes.tmp
!! securemsg.exe

$ git check-ignore -v app.log notes.tmp .env config.local .vscode/settings.json .idea/workspace.xml securemsg.exe
.gitignore:22:*.log             app.log
.gitignore:20:*.tmp             notes.tmp
.gitignore:28:.env              .env
.gitignore:31:config.local      config.local
.gitignore:35:.vscode/          .vscode/settings.json
.gitignore:36:.idea/            .idea/workspace.xml
.gitignore:17:securemsg.exe     securemsg.exe
```

Retrait d'un fichier suivi par erreur :

```text
$ git add -f config.local
$ git commit -m "chore: ajout par erreur de config.local ..."
[main 901984a] chore: ajout par erreur de config.local (fichier local normalement ignore)
$ git rm --cached config.local
rm 'config.local'
$ git status --short
D  config.local
$ git commit -m "chore(gitignore): retrait de config.local de l'index (git rm --cached)"
[main dd5fc5b] chore(gitignore): retrait de config.local de l'index (git rm --cached)
$ Test-Path config.local
True          # le fichier est toujours sur le disque, mais plus versionné
```

## 8. Gestion des branches (Partie G)

```text
$ git branch -vv
  feature/conflit-A 6dd3ad1 [origin/feature/conflit-A] feat(conflit-A): titre version A
  feature/conflit-B afebeb7 [origin/feature/conflit-B] feat(conflit-B): titre version B
  feature/module1   f7ef010 [origin/feature/module1] feat(module1): saisie securisee et validation des entrees
  feature/module2   9064c95 [origin/feature/module2] feat(module2): moteur de chiffrement TP-XORMIX et controle d'integrite
  feature/module3   7115d2e [origin/feature/module3] feat(module3): rapport de securite et recommandations
* main              dd5fc5b [origin/main] chore(gitignore): retrait de config.local de l'index

$ git switch -c demo/temp-branch            # création
$ git push -u origin demo/temp-branch       # publication
$ git branch -m demo/temp-branch demo/renamed-branch   # renommage
$ git push origin -u demo/renamed-branch
$ git push origin --delete demo/temp-branch # suppression distante de l'ancien nom
$ git branch -D demo/renamed-branch         # suppression locale
$ git push origin --delete demo/renamed-branch
$ git fetch --prune origin
$ git branch -a
  feature/conflit-A
  feature/conflit-B
  feature/module1
  feature/module2
  feature/module3
* main
  remotes/origin/HEAD -> origin/main
  remotes/origin/feature/conflit-A
  remotes/origin/feature/conflit-B
  remotes/origin/feature/module1
  remotes/origin/feature/module2
  remotes/origin/feature/module3
  remotes/origin/main
```

## 9. Exercices avancés (dépôt secondaire)

Voir les transcriptions complètes :

- **`git reset --soft / --mixed / --hard`**, récupération par **reflog** :

```text
$ git reset --hard HEAD~1
HEAD is now at b69a965 C: ajout ligne 3

$ git reflog --format='%h %gd %gs' | Select-Object -First 6
0a96f16 HEAD@{1} commit: D ter: ajout ligne 4 (apres reset --mixed)

$ git reset --hard 0a96f16
HEAD is now at 0a96f16 D ter: ajout ligne 4 (apres reset --mixed)
$ git log --oneline
0a96f16 D ter: ajout ligne 4 (apres reset --mixed)
```

- **`git revert` d'un commit déjà poussé** :

```text
$ git push origin main
   ac360e5..8752390  main -> main
$ git log --oneline -3
8752390 Revert "E: modification defectueuse (deja poussee)"
ac360e5 E: modification defectueuse (deja poussee)
0a96f16 D ter: ajout ligne 4 (apres reset --mixed)
```

- **Stratégies de fusion** — `git log --oneline --graph --all` du dépôt de test :

```text
* f1a6afd REBASE 2: deuxieme modification
* ea9a9c5 REBASE 1: premiere modification
* 973d2a4 MAIN: commit pendant le travail de rebase-branch
* 1c857f8 SQUASH: fusion des 2 commits de squash-branch en un seul
| * e61dcb4 SQUASH 2: deuxieme modification
| * 8b37fc9 SQUASH 1: premiere modification
|/
*   439d815 merge: integration de noff-branch (--no-ff)
|\
| * 1cf2c82 NOFF: ajout noff.txt
|/
* 8c3d0c5 FF: ajout ff.txt
*   06f35c6 merge: integration de feature/renamed
|\
| * ea02834 F: travail sur branche renommee
|/
* 8752390 Revert "E: modification defectueuse (deja poussee)"
* ac360e5 E: modification defectueuse (deja poussee)
* 0a96f16 D ter: ajout ligne 4 (apres reset --mixed)
* b69a965 C: ajout ligne 3
* 982c018 B: ajout ligne 2
* b856a94 A: version initiale
```

Lecture : `8c3d0c5` (fast-forward, aucun commit de fusion), `439d815`
(`--no-ff`, commit de fusion visible), `1c857f8` (squash : un commit unique,
les commits d'origine restent orphelins), puis rebase : `REBASE 1` et
`REBASE 2` rejoués **au-dessus** de `MAIN` → historique linéaire, aucun commit
de fusion.

## 10. Fork et Pull Request depuis le fork (Partie G)

Tentative de fork du dépôt du TP (refusée par GitHub, un compte ne peut pas
posséder un dépôt et son fork) :

```text
$ gh repo fork Saadtlidji/TP-Git-Collaboration --clone=false
failed to fork: Saadtlidji/TP-Git-Collaboration cannot be forked.
A single user account cannot own both a parent and fork.
```

Fork d'un dépôt bac à sable public, avec `upstream` :

```text
$ gh repo fork octocat/Hello-World --clone=true
$ cd Hello-World
$ git remote -v
origin    https://github.com/Saadtlidji/Hello-World.git (fetch)
origin    https://github.com/Saadtlidji/Hello-World.git (push)
upstream  https://github.com/octocat/Hello-World.git (fetch)
upstream  https://github.com/octocat/Hello-World.git (push)

$ git switch -c tp/fork-demo
$ git commit -m "docs: ajout de la note d'exercice fork (TP Git)"
$ git push -u origin tp/fork-demo
branch 'tp/fork-demo' set up to track 'origin/tp/fork-demo'.
```

Pull Request ouverte depuis le fork :
<https://github.com/Saadtlidji/Hello-World/pull/1>
(la base choisie est le fork lui-même, pour ne pas solliciter le dépôt public
d'origine).

## 11. Tests fonctionnels de l'application

```text
$ .\securemsg.exe
Application de securite informatique
...
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

  RECOMMANDATIONS DE SECURITE
  [OK    ] Longueur de cle conforme aux bonnes pratiques de demonstration.
  [OK    ] Aucun probleme detecte sur ce message.
  [INFO  ] TP-XORMIX est educatif : pour un usage reel, utilisez AES-GCM ou ChaCha20-Poly1305.
```

Round-trip et cas d'erreur :

```text
=== ROUND TRIP ===
  Message en clair retrouve : Bonjour le monde, ceci est un test de chiffrement.

=== TAMPERED FRAME ===
  [ERREUR] Integrite compromise : le message chiffre a ete modifie.

=== SHORT KEY ===
  [REFUS ] Cle trop courte (4 caracteres minimum).

=== EMPTY MSG ===
  [REFUS ] Le message est vide.

=== BAD MENU ===
  Choix hors plage (0-3).
  [ERREUR] Veuillez saisir un nombre.

=== WRONG KEY ===
  [ALERTE] Cle probablement incorrecte : le resultat n'est pas du texte lisible.
```
