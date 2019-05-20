
numThreads=$1
file=(basename $2)
input="../$2"
output="../results/"$input".speedup.cvs"
elapsedTime=$input".res"

../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $input
SeqSolverElapsedTime=$(cut -c 19-27 <(grep "Elapsed" $elapsedTime))
echo $SeqSolverElapsedTime

for i in $(seq 1 $numThreads)
do
   ./CircuitRouter-ParSolver -t $i $input
   ParSolverElapsedTime=$(cut -c 19-27 <(grep "Elapsed" $elapsedTime))
   speedup=$(echo "scale=6; ${SeqSolverElapsedTime}/${ParSolverElapsedTime}" | bc)
   echo $i $ParSolverElapsedTime ${speedup} >> $output
done

