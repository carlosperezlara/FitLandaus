for RUN in 432637_432999  433000_433930  434017_434984  435022_435939  436010_436939  437005_437944  438026_428422  438026_438422
do
    echo "extracting run ${RUN}"
    root -b -l -q scripts/extractHistograms.C\(\"${RUN}\"\)
done
