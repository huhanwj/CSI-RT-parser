all: TCPServer TCPClient UDPServer UDPClient
TCPServer: server.o
	g++ -o TCPServer server.o
TCPClient: client.o
	g++ -o TCPClient client.o
UDPServer: UDPServer.o
	gcc -o UDPServer UDPServer.o
UDPClient: UDPClient.o
	gcc -o UDPClient UDPClient.o
server.o: server.cpp
	g++ -c server.cpp
client.o: client.cpp
	g++ -c client.cpp
UDPServer.o: UDPServer.c
	gcc -c UDPServer.c
UDPClient.o: UDPClient.c
	gcc -c UDPClient.c
clean: all
	rm all

