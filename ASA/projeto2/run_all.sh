for INPUT in $(ls *.in)
do
    FILENAME=$(echo $INPUT | cut -d '.' -f 1)
    echo "-------- RUNNING ${FILENAME} --------"
    time ./proj < $FILENAME.in > $FILENAME.try
done