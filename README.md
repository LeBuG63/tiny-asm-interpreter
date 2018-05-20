# tiny-asm-interpreter
(Travail en cours)

Un intépreteur d'un langage d'assembleur fortement simplifié.

Utilisation: ./tinyasminterpreter <nom du fichier>

Il y'a 6 registres (R1-6).

Ajouter un '#' devant un nombre signifie que ce qui suit sera utilisé comme un entier
  - add #10,R1 ajoutera le nombre 10 dans le registre R1

Pour acceder un espace mémoire, il suffit de faire: [x], où x represente l'emplacement mémoire (de 0 à 32768):
  - add #10,[50] ajoutera le nombre 10 à l'emplacement mémoire 50

Pour créer une fonction, il suffit de faire comme un label, puis à la fin de la fonction mettre l'instruction `ret`, qui permettra de revenir à l'appel de la fonction + 1 ligne.

Un commentaire commence par `;`. Il ne peut pas encore être placé la même ligne qu'une instruction.

Commandes:
  - `move <src><dest>` dest prend la valeur src
  - `add <src><dest>` ajoute src dans dest 
  - `sub <src><dest>` soustrait dest à src et le stock dans dest
  - `cmp <arg1><arg2>` compare arg1 à arg2
  - `jge <label>` saute au label indiqué si arg1 est plus grand ou égale à arg2
  - `jg <label>` saute au label indiqué si arg1 est plus grand à arg2
  - `jle <label>` saute au label indiqué si arg1 est plus petit ou égale à arg2
  - `jl <label>` saute au label indiqué si arg1 est plus petit à arg2
  - `jmp <label>` saute au label indiqué
  - `call <func>` saute à la fonction
  - `print <src>` affiche le nombre stocké à l'emplacement src
  - `rets` quitte le programme
  
Les commentaires ne sont pas encore supporés.
  
Exemple d'un programme, la suite de Fibonacci:
``` 
move #0,R1
move #1,R2
move #0,R3
move #0,R4
move #20,R5

call fibo
rets

fibo:
    sub #1,R5
    move R1,R3
    add R2,R3
    move R2,R1
    move R3,R2

    print R1

    cmp R5,#1
    jg fibo
    
    ret

``` 
