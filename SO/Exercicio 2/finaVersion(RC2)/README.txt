**Estrutura das directorias**
- CircuitRouter-ParSolver: contém os ficheiros da solução COM paralelismo.
- CircuitRouter-SeqSolver: contém os ficheiros da solução SEM paralelismo.
- lib: contém ficheiros auxiliares.
- inputs: contém ficheiros de input para o programa.
- results: pasta de output dos resultados do shell script.

**Passos de compilação/execução**
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

