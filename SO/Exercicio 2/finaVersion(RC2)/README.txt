**Estrutura das directorias**
- CircuitRouter-ParSolver: cont�m os ficheiros da solu��o COM paralelismo.
- CircuitRouter-SeqSolver: cont�m os ficheiros da solu��o SEM paralelismo.
- lib: cont�m ficheiros auxiliares.
- inputs: cont�m ficheiros de input para o programa.
- results: pasta de output dos resultados do shell script.

**Passos de compila��o/execu��o**
- correr "make" nas pastas do ParSolver e SeqSolver
- correr "./doTest.sh <n_tarefas> <input file>" na pasta principal do projecto
OU
- correr "./CircuitRouter-ParSolver <flags> <input file>" nas pasta principal do projecto

exemplos:
./doTest.sh 5 ../inputs/random-x32-y32-z3-n64.txt
./CircuitRouter-ParSolver -t 5 ../inputs/random-x32-y32-z3-n64.txt

**CPU**
- modelo:
- cores:
- clock rate:

