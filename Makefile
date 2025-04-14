CC = cc
CFLAGS = -Wall -Wextra -Werror

SOURCES = \
	client.c \
	server.c \

OBJ = $(SOURCES:.c=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) client.o -o client
	$(CC) $(CFLAGS) server.o -o server

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f client server

re: fclean all