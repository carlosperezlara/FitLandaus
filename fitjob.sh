RUN=$1
BLK=$2
DRAW=1

let "INI=${BLK}*32"
let "FIN=(${BLK}+1)*32"

echo "******** I AM WORKING ON:  [${INI} - ${FIN}]  DRAW ${DRAW}"

root -b -l -q scripts/fitblk.C\(\"${RUN}\",${INI},${FIN},${DRAW}\)
root -b -l -q scripts/fitblk.C\(\"${RUN}\",${INI},${FIN},${DRAW}\)
