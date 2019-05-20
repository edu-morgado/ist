for i in $(seq 10 110); do 
    echo "Calculating $(( $i )) 5 $(( $i )) $(( $i / 2 )) 1" 
    ./gerador $(( $i )) 5 $(( $i )) $(( $i / 2 )) 1 > test$(( $i - 9 )).in 
    done
