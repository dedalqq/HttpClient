

all: client

client: main.o client.o result.o
	g++ -g main.o client.o result.o -pthread -o client

main.o: main.cpp
	g++ -g -c -O2 main.cpp -o main.o

client.o: client.cpp client.h
	g++ -g -O2 -c client.cpp -std=c++11 -o client.o

result.o: result.cpp result.h
	g++ -g -O2 -c result.cpp -o result.o

clean:
	rm main.o
	rm client.o
	rm client
	rm result.o
