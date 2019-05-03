g++ -std=c++17 -g -o ServerTest main_server.cpp TcpServer.h TcpServer.cpp SocketConfig.h SocketConfig.cpp SocketUtility.h SocketUtility.cpp TcpThreadForEcho.h TcpThreadForEcho.cpp TcpThread.h TcpThread.cpp -levent -pthread
g++ -std=c++17 -g -o ClientTest main_client.cpp -levent -pthread
