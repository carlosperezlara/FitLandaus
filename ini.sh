RUN=428211_429133_5s
for SEN in {0..384}
do
    root -b -l -q scripts/xini.C\(\"${RUN}\",${SEN}\)
done
