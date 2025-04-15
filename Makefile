CC = cc
CFLAGS = -Wall -Wextra -Werror

SOURCES = \
	client.c \
	server.c \

SOURCES_BONUS = \
	client_bonus.c \
	server_bonus.c \

OBJ = $(SOURCES:.c=.o)
OBJ_BONUS = $(SOURCES_BONUS:.c=.o)

all: client server

client: $(OBJ)
	$(CC) $(CFLAGS) client.o -o client

server: $(OBJ)
	$(CC) $(CFLAGS) server.o -o server

bonus: client_bonus server_bonus

client_bonus: $(OBJ_BONUS)
	$(CC) $(CFLAGS) client_bonus.o -o client_bonus
	
server_bonus: $(OBJ_BONUS)
	$(CC) $(CFLAGS) server_bonus.o -o server_bonus

clean:
	rm -f $(OBJ) $(OBJ_BONUS)

fclean: clean
	rm -f client server client_bonus server_bonus

re: fclean all
