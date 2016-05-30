CC=clang
CFLAGS = -std=c99 -Wall -Wextra -I$(INCDIR)
LDFLAGS = # -lSDL    # si besoin pour le framebuffer...

INCDIR=./include
SRCDIR=./src
OBJDIR=./obj

# les objets a ne pas compiler (ni effacer lors du clean!)
OBJ_PROF=elf_reader.o relocation.o framebuffer.o

# ce qui est a vous
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))


all: test_etudiants

$(OBJDIR)/test_etudiants.o: $(SRCDIR)/test_etudiants.c $(INCDIR)/elf_reader.h
	$(CC) -c $(CFLAGS) $< -o $@
test_etudiants: $(OBJDIR)/elf_reader.o $(OBJDIR)/test_etudiants.o
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/first_test.o: $(SRCDIR)/first_test.c $(INCDIR)/elf_reader.h
	$(CC) -c $(CFLAGS) $< -o $@
first_test: $(OBJDIR)/elf_reader.o $(OBJDIR)/first_test.o $(OBJDIR)/operations.o $(OBJDIR)/mem.o $(OBJDIR)/gpr.o $(OBJDIR)/instructions.o
	$(CC) $(LDFLAGS) $^ -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/%.h
	$(CC) -c $(CFLAGS) $< -o $@




# Outils cross-compiles (as, readelf, ...) a l'Ensimag:
# ajouter PATH=$PATH:/opt/mips-tools-cep/bin dans votre .bashrc
exemple:
	mips-elf-as exempleElf.s -o exempleElf.o

# TEST PROC
$(OBJDIR)/gpr.o: $(SRCDIR)/gpr.c $(INCDIR)/gpr.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/operations.o: $(SRCDIR)/operations.c $(INCDIR)/operations.h
	$(CC) -c $(CFLAGS) $< -o $@

proc.o: proc.c
	$(CC) -c $(CFLAGS) $< -o $@

proc: proc.o $(OBJDIR)/operations.o $(OBJDIR)/gpr.o
	$(CC) $(LDFLAGS) $^ -o $@
# FIN TEST PROC

clean:
	rm -rf $(OBJ)

cleanall: clean
	rm -rf test_etudiants
