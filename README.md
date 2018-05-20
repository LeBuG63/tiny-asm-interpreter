# tiny-asm-interpreter
(Travail en cours)

Un intépreteur d'un langage d'assembleur fortement simplifié.

Utilisation: ./a.out <nom du fichier>

Il y'a 7 registres (R1-7), mais le dernier ne doit pas être utilisé car c'est celui qui est utilisé pour faire des comparaisons.

Ajouter un '#' devant un nombre signifie que ce qui suit sera utilisé comme un entier
  - add #10,R1 ajoutera le nombre 10 dans le registre R1

Pour acceder un espace mémoire, il suffit de faire: [x], où x represente l'emplacement mémoire (de 0 à 32768):
  - add #10,[50] ajoutera le nombre 10 à l'emplacement mémoire 50

  
Commandes:
  - `stock <src><dest>` dest prend la valeur src
  - `add <src><dest>` ajoute src dans dest 
  - `sub <src><dest>` soustrait dest à src et le stock dans dest
  - `cmp <arg1><arg2>` compare arg1 à arg2
  - `jge <label>` saute au label indiqué si arg1 est plus petit ou égale à arg2
  - `jra <label>` saute au label indiqué
  - `print <src>` affiche le nombre stocké à l'emplacement src
  - `ret` quitte le programme (pour l'instant, ce n'est pas sa version définitive)
  
Les commentaires ne sont pas encore supporés.
  
Exemple d'un programme, la suite de Fibonacci:
``` 
store #0,R1 
store #1,R2
store #0,R3
store #20,R4
store #0,R5

fibo:
cmp R3,R4
jge label1
jra label4

label2:
add R1,R5
add R2,R5
store R2,R1
store R5,R2
print R1
store #0,R5
jra fibo

label1:
add #1,R3
jra label2

label4:
ret
``` 
