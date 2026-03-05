CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I./include
SRC = src/main.c src/ticket/ticket.c src/utils/utils.c
OBJ = $(SRC:.c=.o)
EXEC = ticket_app

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)
