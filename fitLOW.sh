RUN=$1
DRAW=0

for ARM in 0 1
do
    for LYR in 0 1 #2 3 4 5 6 7
    do
	for SEN in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
	do
	    for ROW in 0 1 2 3
	    do
		root -b -l -q fitrowLOW.C\(\"${RUN}\",${ARM},${LYR},${SEN},${ROW},${DRAW}\)
	    done
	done
    done
done
