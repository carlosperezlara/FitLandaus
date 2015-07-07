RUN=434017_435939
DRAW=1
IDX=$1


let "ARM=${IDX}/768"
let "ONEARM=${IDX}%768"
let "LYR=${ONEARM}/96"
let "ONELYR=${ONEARM}%96"
let "SEN=${ONELYR}/4"
let "ROW=${ONELYR}%4"

echo "******** I AM WORKING ON:  ARM ${ARM} | LYR ${LYR} | SEN ${SEN} | ROW ${ROW}"

root -b -l -q scripts/fitrow.C\(\"${RUN}\",${ARM},${LYR},${SEN},${ROW},${DRAW}\)
root -b -l -q scripts/fitrow.C\(\"${RUN}\",${ARM},${LYR},${SEN},${ROW},${DRAW}\)
