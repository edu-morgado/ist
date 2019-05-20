#!/bin/bash

numThreads=$1
file=`basename $2`
input="../$2"
output="../results/"$file".speedup.cvs"
elapsedTime=$input".res"

echo "#threads,exec_time,speedup" >> $output
../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $input
SeqSolverElapsedTime=$(cut -c 19-26 <(grep "Elapsed" $elapsedTime))
echo "1S,$SeqSolverElapsedTime,1" >> $output
for i in $(seq 1 $numThreads)
do
   ./CircuitRouter-ParSolver -t $i $input
   ParSolverElapsedTime=$(cut -c 19-26 <(grep "Elapsed" $elapsedTime))
   speedup=$(echo "scale=6; ${SeqSolverElapsedTime}/${ParSolverElapsedTime}" | bc)
   echo "$i,$ParSolverElapsedTime,${speedup}" >> $output
done