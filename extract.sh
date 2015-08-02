for RUN in 428211_429133_5s  429351_430594_3s  430595_431736_3s
do
    echo "extracting run ${RUN}"
    root -b -l -q scripts/extractHistograms.C\(\"${RUN}\"\)
done
