all: server client

server: server.cpp
	g++ server.cpp -o pa4_server -lpthread

client: client.cpp
	g++ client.cpp -o pa4_client

clean:
	rm -f pa4_server pa4_client
