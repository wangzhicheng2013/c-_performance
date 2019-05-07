g++ -std=c++17 -g -o Test test.cpp
sudo perf record -a -g ./Test
sudo perf report -i perf.data -f
