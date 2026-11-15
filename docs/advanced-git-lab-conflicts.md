
PS> git add commun.txt alpha.txt beta.txt


PS> git commit -m 'base: ajout des fichiers pour le conflit multi-fichiers'
[main 686eae9] base: ajout des fichiers pour le conflit multi-fichiers
 3 files changed, 3 insertions(+)
 create mode 100644 alpha.txt
 create mode 100644 beta.txt
 create mode 100644 commun.txt

PS> git switch -c conflit-multi-A
Switched to a new branch 'conflit-multi-A'

PS> git commit -am 'A: modification de alpha.txt et beta.txt'
[conflit-multi-A e3f2a19] A: modification de alpha.txt et beta.txt
 2 files changed, 2 insertions(+), 2 deletions(-)

PS> git switch main
Switched to branch 'main'
 
Your branch is ahead of 'origin/main' by 8 commits.
  (use "git push" to publish your local commits)

PS> git switch -c conflit-multi-B
Switched to a new branch 'conflit-multi-B'

PS> git commit -am 'B: modification de alpha.txt et beta.txt'
[conflit-multi-B d1d7ce4] B: modification de alpha.txt et beta.txt
 2 files changed, 2 insertions(+), 2 deletions(-)

PS> git switch main
Switched to branch 'main'
 
Your branch is ahead of 'origin/main' by 8 commits.
  (use "git push" to publish your local commits)

PS> git merge --no-ff conflit-multi-B -m 'merge: integration de conflit-multi-B'
Merge made by the 'ort' strategy.
 alpha.txt | 2 +-
 beta.txt  | 2 +-
 2 files changed, 2 insertions(+), 2 deletions(-)

PS> git merge conflit-multi-A
Auto-merging alpha.txt
CONFLICT (content): Merge conflict in alpha.txt
Auto-merging beta.txt
CONFLICT (content): Merge conflict in beta.txt
Automatic merge failed; fix conflicts and then commit the result.

PS> git status --short
UU alpha.txt
UU beta.txt

PS> git checkout --ours alpha.txt
Updated 1 path from the index

PS> git checkout --theirs beta.txt
Updated 1 path from the index

PS> git add alpha.txt beta.txt


PS> git status --short
M  beta.txt

PS> git commit -m 'fix: conflit multi-fichiers resolu (--ours pour alpha.txt, --theirs pour beta.txt)'
[main e323531] fix: conflit multi-fichiers resolu (--ours pour alpha.txt, --theirs pour beta.txt)

PS> Get-Content alpha.txt
alpha - modification B

PS> Get-Content beta.txt
beta - modification A

PS> git log --oneline --graph -6
*   e323531 fix: conflit multi-fichiers resolu (--ours pour alpha.txt, --theirs pour beta.txt)
|\  
| * e3f2a19 A: modification de alpha.txt et beta.txt
* |   d964770 merge: integration de conflit-multi-B
|\ \  
| |/  
|/|   
| * d1d7ce4 B: modification de alpha.txt et beta.txt
|/  
* 686eae9 base: ajout des fichiers pour le conflit multi-fichiers
* f1a6afd REBASE 2: deuxieme modification
