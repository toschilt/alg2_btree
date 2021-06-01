MAIN=main.c
UTIL=btree.c streamHandler.c
BINARY=main


all:
	gcc -Wall $(MAIN) $(UTIL) -o $(BINARY)

run:
	./$(BINARY)

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all --show-reachable=yes ./$(BINARY)

clean:
	rm *.o
