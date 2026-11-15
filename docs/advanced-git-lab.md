
PS> git init -b main
Initialized empty Git repository in C:/Users/saadd/advanced-git-lab/.git/

PS> git config user.name 'SAAD'


PS> git config user.email 'saaddidou738@gmail.com'


PS> git add app.txt


PS> git commit -m 'A: version initiale'
[main (root-commit) b856a94] A: version initiale
 1 file changed, 1 insertion(+)
 create mode 100644 app.txt

PS> git commit -am 'B: ajout ligne 2'
[main 982c018] B: ajout ligne 2
 1 file changed, 1 insertion(+)

PS> git commit -am 'C: ajout ligne 3'
[main b69a965] C: ajout ligne 3
 1 file changed, 1 insertion(+)

PS> git commit -am 'D: ajout ligne 4'
[main dadf43b] D: ajout ligne 4
 1 file changed, 1 insertion(+)

PS> git log --oneline
dadf43b D: ajout ligne 4
b69a965 C: ajout ligne 3
982c018 B: ajout ligne 2
b856a94 A: version initiale

PS> git reset --soft HEAD~1


PS> git status --short
M  app.txt

PS> git log --oneline
b69a965 C: ajout ligne 3
982c018 B: ajout ligne 2
b856a94 A: version initiale

PS> git commit -m 'D bis: ajout ligne 4 (apres reset --soft)'
[main c2574b9] D bis: ajout ligne 4 (apres reset --soft)
 1 file changed, 1 insertion(+)

PS> git reset --mixed HEAD~1
Unstaged changes after reset:
M	app.txt

PS> git status --short
 M app.txt

PS> git commit -am 'D ter: ajout ligne 4 (apres reset --mixed)'
[main 0a96f16] D ter: ajout ligne 4 (apres reset --mixed)
 1 file changed, 1 insertion(+)

PS> git reset --hard HEAD~1
HEAD is now at b69a965 C: ajout ligne 3

PS> git log --oneline
b69a965 C: ajout ligne 3
982c018 B: ajout ligne 2
b856a94 A: version initiale

PS> git reflog  (le commit perdu est reperable ici)

PS> git reflog --format='%h %gd %gs' | Select-Object -First 6
b69a965 HEAD@{0} reset: moving to HEAD~1
0a96f16 HEAD@{1} commit: D ter: ajout ligne 4 (apres reset --mixed)
b69a965 HEAD@{2} reset: moving to HEAD~1
c2574b9 HEAD@{3} commit: D bis: ajout ligne 4 (apres reset --soft)
b69a965 HEAD@{4} reset: moving to HEAD~1
dadf43b HEAD@{5} commit: D: ajout ligne 4

Commit perdu retrouve : 0a96f16

PS> git reset --hard 0a96f16
HEAD is now at 0a96f16 D ter: ajout ligne 4 (apres reset --mixed)

PS> git log --oneline
0a96f16 D ter: ajout ligne 4 (apres reset --mixed)
b69a965 C: ajout ligne 3
982c018 B: ajout ligne 2
b856a94 A: version initiale

PS> git remote add origin 'C:\Users\saadd\advanced-git-lab-remote.git'


PS> git push -u origin main
To C:\Users\saadd\advanced-git-lab-remote.git
 
 * [new branch]      main -> main
branch 'main' set up to track 'origin/main'.

PS> git commit -am 'E: modification defectueuse (deja poussee)'
[main ac360e5] E: modification defectueuse (deja poussee)
 1 file changed, 1 insertion(+)

PS> git push origin main
To C:\Users\saadd\advanced-git-lab-remote.git
 
   0a96f16..ac360e5  main -> main

PS> git revert --no-edit HEAD
[main 8752390] Revert "E: modification defectueuse (deja poussee)"
 Date: Sun Nov 15 19:07:04 2026 +0100
 1 file changed, 1 deletion(-)

PS> git push origin main
To C:\Users\saadd\advanced-git-lab-remote.git
 
   ac360e5..8752390  main -> main

PS> git log --oneline -3
8752390 Revert "E: modification defectueuse (deja poussee)"
ac360e5 E: modification defectueuse (deja poussee)
0a96f16 D ter: ajout ligne 4 (apres reset --mixed)

PS> git branch feature/experiment


PS> git branch -vv
  feature/experiment 8752390 Revert "E: modification defectueuse (deja poussee)"
* main               8752390 [origin/main] Revert "E: modification defectueuse (deja poussee)"

PS> git branch -m feature/experiment feature/renamed


PS> git switch feature/renamed
Switched to branch 'feature/renamed'

PS> git commit -am 'F: travail sur branche renommee'
[feature/renamed ea02834] F: travail sur branche renommee
 1 file changed, 1 insertion(+)

PS> git push -u origin feature/renamed
To C:\Users\saadd\advanced-git-lab-remote.git
 
 * [new branch]      feature/renamed -> feature/renamed
branch 'feature/renamed' set up to track 'origin/feature/renamed'.

PS> git branch -vv
* feature/renamed ea02834 [origin/feature/renamed] F: travail sur branche renommee
  main            8752390 [origin/main] Revert "E: modification defectueuse (deja poussee)"

PS> git switch main
Switched to branch 'main'
 
Your branch is up to date with 'origin/main'.

PS> git merge --no-ff feature/renamed -m 'merge: integration de feature/renamed'
Merge made by the 'ort' strategy.
 app.txt | 1 +
 1 file changed, 1 insertion(+)

PS> git push origin main
To C:\Users\saadd\advanced-git-lab-remote.git
 
   8752390..06f35c6  main -> main

PS> git branch -d feature/renamed
Deleted branch feature/renamed (was ea02834).

PS> git push origin --delete feature/renamed
To C:\Users\saadd\advanced-git-lab-remote.git
 
 - [deleted]         feature/renamed

PS> git fetch --prune origin


PS> git branch -a
* main
  remotes/origin/main

PS> git switch -c ff-branch
Switched to a new branch 'ff-branch'

PS> git add ff.txt


PS> git commit -m 'FF: ajout ff.txt'
[ff-branch 8c3d0c5] FF: ajout ff.txt
 1 file changed, 1 insertion(+)
 create mode 100644 ff.txt

PS> git switch main
Switched to branch 'main'
 
Your branch is up to date with 'origin/main'.

PS> git merge ff-branch
Updating 06f35c6..8c3d0c5
Fast-forward
 ff.txt | 1 +
 1 file changed, 1 insertion(+)
 create mode 100644 ff.txt

PS> git log --oneline --graph -4
* 8c3d0c5 FF: ajout ff.txt
*   06f35c6 merge: integration de feature/renamed
|\  
| * ea02834 F: travail sur branche renommee
|/  
* 8752390 Revert "E: modification defectueuse (deja poussee)"

PS> git switch -c noff-branch
Switched to a new branch 'noff-branch'

PS> git add noff.txt


PS> git commit -m 'NOFF: ajout noff.txt'
[noff-branch 1cf2c82] NOFF: ajout noff.txt
 1 file changed, 1 insertion(+)
 create mode 100644 noff.txt

PS> git switch main
Switched to branch 'main'
 
Your branch is ahead of 'origin/main' by 1 commit.
  (use "git push" to publish your local commits)

PS> git merge --no-ff noff-branch -m 'merge: integration de noff-branch (--no-ff)'
Merge made by the 'ort' strategy.
 noff.txt | 1 +
 1 file changed, 1 insertion(+)
 create mode 100644 noff.txt

PS> git log --oneline --graph -4
*   439d815 merge: integration de noff-branch (--no-ff)
|\  
| * 1cf2c82 NOFF: ajout noff.txt
|/  
* 8c3d0c5 FF: ajout ff.txt
*   06f35c6 merge: integration de feature/renamed
|\

PS> git switch -c squash-branch
Switched to a new branch 'squash-branch'

PS> git add squash.txt


PS> git commit -m 'SQUASH 1: premiere modification'
[squash-branch 8b37fc9] SQUASH 1: premiere modification
 1 file changed, 1 insertion(+)
 create mode 100644 squash.txt

PS> git commit -am 'SQUASH 2: deuxieme modification'
[squash-branch e61dcb4] SQUASH 2: deuxieme modification
 1 file changed, 1 insertion(+)

PS> git switch main
Switched to branch 'main'
 
Your branch is ahead of 'origin/main' by 3 commits.
  (use "git push" to publish your local commits)

PS> git merge --squash squash-branch
Updating 439d815..e61dcb4
Fast-forward
Squash commit -- not updating HEAD
 squash.txt | 2 ++
 1 file changed, 2 insertions(+)
 create mode 100644 squash.txt

PS> git status --short
A  squash.txt

PS> git commit -m 'SQUASH: fusion des 2 commits de squash-branch en un seul'
[main 1c857f8] SQUASH: fusion des 2 commits de squash-branch en un seul
 1 file changed, 2 insertions(+)
 create mode 100644 squash.txt

PS> git log --oneline --graph -4
* 1c857f8 SQUASH: fusion des 2 commits de squash-branch en un seul
*   439d815 merge: integration de noff-branch (--no-ff)
|\  
| * 1cf2c82 NOFF: ajout noff.txt
|/  
* 8c3d0c5 FF: ajout ff.txt

PS> git switch -c rebase-branch
Switched to a new branch 'rebase-branch'

PS> git add rebase.txt


PS> git commit -m 'REBASE 1: premiere modification'
[rebase-branch e44c254] REBASE 1: premiere modification
 1 file changed, 1 insertion(+)
 create mode 100644 rebase.txt

PS> git commit -am 'REBASE 2: deuxieme modification'
[rebase-branch c5624d6] REBASE 2: deuxieme modification
 1 file changed, 1 insertion(+)

PS> git switch main
Switched to branch 'main'
 
Your branch is ahead of 'origin/main' by 4 commits.
  (use "git push" to publish your local commits)

PS> git add main-only.txt


PS> git commit -m 'MAIN: commit pendant le travail de rebase-branch'
[main 973d2a4] MAIN: commit pendant le travail de rebase-branch
 1 file changed, 1 insertion(+)
 create mode 100644 main-only.txt

PS> git switch rebase-branch
Switched to branch 'rebase-branch'

PS> git rebase main
Rebasing (1/2)
 
Rebasing (2/2)
Successfully rebased and updated refs/heads/rebase-branch.

PS> git log --oneline --graph -6
* f1a6afd REBASE 2: deuxieme modification
* ea9a9c5 REBASE 1: premiere modification
* 973d2a4 MAIN: commit pendant le travail de rebase-branch
* 1c857f8 SQUASH: fusion des 2 commits de squash-branch en un seul
*   439d815 merge: integration de noff-branch (--no-ff)
|\  
| * 1cf2c82 NOFF: ajout noff.txt
|/

PS> git switch main
Switched to branch 'main'
 
Your branch is ahead of 'origin/main' by 5 commits.
  (use "git push" to publish your local commits)

PS> git merge --ff-only rebase-branch
Updating 973d2a4..f1a6afd
Fast-forward
 rebase.txt | 2 ++
 1 file changed, 2 insertions(+)
 create mode 100644 rebase.txt

PS> git log --oneline --graph --all
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

