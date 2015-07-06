RUN=434017_435939
DRAW=1
ARM=$1
LYR=$2
SEN=$3
ROW=$4

root -b -l -q scripts/fitrow.C\(\"${RUN}\",${ARM},${LYR},${SEN},${MPD},${DRAW},6.5,82.5,50\)
root -b -l -q scripts/fitrow.C\(\"${RUN}\",${ARM},${LYR},${SEN},${MPD},${DRAW},6.5,82.5,50\)
