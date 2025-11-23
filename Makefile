CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Wvla
LDFLAGS = -lcriterion

SRC = src/parser.c src/cli.c src/main.c
OBJ = $(SRC:.c=.o)

TESTS_SRC = tests/test_parser.c tests/test_cli.c
TESTS_OBJ = $(TESTS_SRC:.c=.o)
TESTSUITE = testsuite

all: $(OBJ)

check: $(OBJ) $(TESTS_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(TESTS_OBJ) -o $(TESTSUITE)
	./$(TESTSUITE)

clean:
	rm -f $(OBJ) $(TESTS_OBJ) $(TESTSUITE)
