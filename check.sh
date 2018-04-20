echo "Compiling"
g++ -std=c++14 property_test.cpp -D BIT_COUNT=30 -o _check
echo "Checking rec"
./_check rec 100 50 100
echo "Checking iter"
./_check iter 100 50 100
