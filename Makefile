EXEC = asteroid

BINDIR = exe
INCDIR = inc
SRCDIR = src
OBJDIR = obj

CC = gcc
INCL = -I$(INCDIR)
CFLAGS = -g -O3 -Wall

SRCFILES = $(wildcard $(SRCDIR)/*.c)
OBJFILES = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCFILES))

$(BINDIR)/$(EXEC): $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCL) -c $< -o $@

run:
	./$(BINDIR)/$(EXEC)

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/*$(EXEC)  core *~
