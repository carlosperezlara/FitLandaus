RUN=434017_435939
DRAW=1

for ARM in 0 1
do
    for LYR in 2
    do
	for SEN in 4 5 9 10 13 14
	do
	    for MPD in 31 96
	    do
		#root -b -l -q scripts/fit.C\(\"${RUN}\",${ARM},${LYR},${SEN},${MPD},${DRAW},6.5,82.5,50\)
		root -b -l -q scripts/fit.C\(\"${RUN}\",${ARM},${LYR},${SEN},${MPD},${DRAW},4.5,62.5,50\)
	    done
	done
    done
done
