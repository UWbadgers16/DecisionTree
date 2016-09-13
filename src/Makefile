CC=gcc
CFLAGS=-g -Os
OBJECTS=DecisionTree.o ARFF.o Classes.o Features.o

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o decision_tree

DecisionTree.o: DecisionTree.c
	$(CC) $(CFLAGS) -c DecisionTree.c
ARFF.o: ARFF.c
	$(CC) $(CFLAGS) -c ARFF.c
Classes.o: Classes.c
	$(CC) $(CFLAGS) -c Classes.c
Features.o: Features.c
	$(CC) $(CFLAGS) -c Features.c

clean:
	$(RM) decision_tree *.o