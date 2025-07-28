crypto:
	g++ crypto.cpp -o crypto.out -lcrypto

server:
	g++ server.cpp -o server.out -lcrypto -lssl
