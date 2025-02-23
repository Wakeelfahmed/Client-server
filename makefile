CC = gcc
CFLAGS = -static
LIBS = -lpthread -lrt

SERVER_SRC = Server.c
CLIENT_SRC = Client.c

SERVER_BIN = server
CLIENT_BIN = client

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN) *.o *~
