# build
make clean && make -j16

# run
for i in {0..9}; do
    echo "Running case test$i.txt."
    ./verifier ./case/test$i.txt > ./out/test$i.txt
    if grep -q "Error" ./out/test$i.txt; then
        echo "Error found in log file."
        exit 1
    else
        echo "Passed."
    fi
done