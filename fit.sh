#ARM=$2
#LYR=$3
#SEN=$4
#ROW=$5
DRAW=0

for ARM in 0 #1
do
    for LYR in 4 #1 2 3 4 5 6 7
    do
	for SEN in 12 #0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
	#for SEN in 15 16 17 18 19 20 21 22 23
	do
	    for ROW in 0 1 2 3
	    do
		RUN=19020_19035
		root -b -l -q fitrow.C\(\"${RUN}\",${ARM},${LYR},${SEN},${ROW},${DRAW}\)
		root -b -l -q fitrow.C\(\"${RUN}\",${ARM},${LYR},${SEN},${ROW},${DRAW}\)
		RUN=19037_19049
		root -b -l -q fitrow.C\(\"${RUN}\",${ARM},${LYR},${SEN},${ROW},${DRAW}\)
		root -b -l -q fitrow.C\(\"${RUN}\",${ARM},${LYR},${SEN},${ROW},${DRAW}\)
		RUN=19052_19057
		root -b -l -q fitrow.C\(\"${RUN}\",${ARM},${LYR},${SEN},${ROW},${DRAW}\)
		root -b -l -q fitrow.C\(\"${RUN}\",${ARM},${LYR},${SEN},${ROW},${DRAW}\)
	    done
	done
    done
done
