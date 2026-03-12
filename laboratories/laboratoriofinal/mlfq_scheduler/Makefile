CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
TARGET = mlfq_app

SRC = src/main.c \
      src/scheduler/scheduler.c \
      src/utils/utils.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

src/main.o: src/main.c include/scheduler/scheduler.h include/utils/utils.h
src/scheduler/scheduler.o: src/scheduler/scheduler.c include/scheduler/scheduler.h
src/utils/utils.o: src/utils/utils.c include/utils/utils.h include/scheduler/scheduler.h

run: all
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) assets/results.csv
