MAIN=main.c
UTIL=btree.c streamHandler.c student.c publicInterface.c
BINARY=main


all:
	gcc -g -Wall $(MAIN) $(UTIL) -o $(BINARY) -lm

run:
	./$(BINARY)

debug:
	gcc $(MAIN) $(UTIL) -g

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all --show-reachable=yes ./$(BINARY)

clean:
	rm *.o
