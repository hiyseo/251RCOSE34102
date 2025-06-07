CC = gcc
CFLAGS = -Iinclude -Wall
SRC = src/main.c src/process.c src/queue.c src/config.c src/schedule.c src/io.c src/evaluation.c
OBJ = $(SRC:.c=.o)
EXEC = cpu_scheduler

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -f $(EXEC) src/*.o