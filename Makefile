SRCDIR = src
OBJ = main.o todolist.o cmd.o getch.o
CC = gcc
OUT = mtodo

# Windows:
# gcc -c src/main.c src/todolist.c src/cmd.c src/getch.c;gcc main.o todolist.o cmd.o getch.o -o todo.exe; ./todo.exe

output: $(OBJ)
	$(CC) $(OBJ) -o $(OUT)

todolist.o: $(SRCDIR)/todolist.c
	$(CC) -c $(SRCDIR)/todolist.c

main.o: $(SRCDIR)/main.c
	$(CC) -c $(SRCDIR)/main.c

cmd.o: $(SRCDIR)/cmd.c
	$(CC) -c $(SRCDIR)/cmd.c

getch.o: $(SRCDIR)/getch.c
	$(CC) -c $(SRCDIR)/getch.c

clean:
	rm *.o $(OUT)