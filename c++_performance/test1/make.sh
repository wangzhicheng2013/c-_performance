g++ -std=c++17 -g -o Test main.cpp -pthread
g++ -std=c++17 -g -o TcmallocTest main_tcmalloc.cpp -ltcmalloc -pthread
g++ -std=c++17 -g -o NoTcmallocTest main_tcmalloc.cpp -pthread
