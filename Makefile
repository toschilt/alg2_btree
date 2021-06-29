MAIN=main.c
UTIL=bTreeUtils.c bTreeSearch.c bTreeInsert.c streamHandler.c student.c publicInterface.c
PONTOH=bTreeUtils.h bTreeSearch.h bTreeInsert.h streamHandler.h student.h publicInterface.h
BINARY=main


all:
	gcc -g -Wall $(MAIN) $(UTIL) -o $(BINARY) -lm

run:
	./$(BINARY)

debug:
	gcc $(MAIN) $(UTIL) -g

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all --show-reachable=yes ./$(BINARY)

zip:
	zip BTreeGrupo7.zip Makefile $(MAIN) $(UTIL) $(PONTOH)

clean:
	rm *.o