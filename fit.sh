RUN=$1
SENPAR=$2
DRAW=1
root -b -l -q scripts/fitblk.C\(\"${RUN}\",${SENPAR}/4,${SENPAR}%4,${DRAW},0\)
root -b -l -q scripts/fitblk.C\(\"${RUN}\",${SENPAR}/4,${SENPAR}%4,${DRAW},0\)
