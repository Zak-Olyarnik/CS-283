CFLAGS = -g -pthread

client: client.c csapp.h csapp.c
			gcc $(CFLAGS) -o client.o client.c csapp.h csapp.c
			./client.o $(HOST) $(PORT) $(FILE)

server: server.c csapp.h csapp.c
			gcc $(CFLAGS) -o server.o server.c csapp.h csapp.c
			./server.o $(PORT)

clean: 
		-\rm *.o
