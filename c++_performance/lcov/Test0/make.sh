g++ -std=c++17 -g -o Test main.cpp -fprofile-arcs -ftest-coverage 
./Test 1
/usr/bin/gcov-8 main.cpp
lcov -d ./ -t 'Test' -o 'Test.info' -b . -c --gcov-tool /usr/bin/gcov-8
genhtml -o result Test.info
