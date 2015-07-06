for RUN in 434017_435939
do
    echo "extracting run ${RUN}"
    root -b -l -q scripts/extractHistograms.C\(\"${RUN}\"\)
done
