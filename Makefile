CC=clang
CFLAGS = -std=c99 -Wall -Wextra -I$(INCDIR)
DFLAGS = -DREADLINE
# si besoin pour le framebuffer...
LDFLAGS = -lSDL -lreadline

INCDIR=./include
SRCDIR=./src
OBJDIR=./obj
TSTDIR=./test

# les objets a ne pas compiler (ni effacer lors du clean!)
OBJ_PROF=elf_reader.o relocation.o framebuffer.o

# ce qui est a vous
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
AS=$(wildcard $(TSTDIR)/*.s)
ELF=$(patsubst %.s, %.o, $(AS))

all: simips compile_tests


$(OBJDIR)/framebuffer_s.o: $(SRCDIR)/framebuffer_s.c $(INCDIR)/framebuffer_s.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/relocation_s.o: $(SRCDIR)/relocation_s.c $(INCDIR)/relocation_s.h $(INCDIR)/elf_reader.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/test_etudiants.o: $(SRCDIR)/test_etudiants.c $(INCDIR)/elf_reader.h
	$(CC) -c $(CFLAGS) $< -o $@
test_etudiants: $(OBJDIR)/elf_reader.o $(OBJDIR)/test_etudiants.o
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/first_test.o: $(SRCDIR)/first_test.c $(INCDIR)/elf_reader.h
	$(CC) -c $(CFLAGS) $< -o $@
first_test: $(OBJDIR)/elf_reader.o $(OBJDIR)/first_test.o $(OBJDIR)/operations.o $(OBJDIR)/mem.o $(OBJDIR)/gpr.o $(OBJDIR)/instructions.o
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/second_test.o: $(SRCDIR)/second_test.c
	$(CC) -c $(CFLAGS) $< -o $@
second_test: $(OBJDIR)/second_test.o $(OBJDIR)/shell.o $(OBJDIR)/mem.o $(OBJDIR)/elf_reader.o $(OBJDIR)/gpr.o $(OBJDIR)/instructions.o $(OBJDIR)/operations.o
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/%.h
	$(CC) -c $(DFLAGS) $(CFLAGS) $< -o $@

# Outils cross-compiles (as, readelf, ...) a l'Ensimag:
# ajouter PATH=$PATH:/opt/mips-tools-cep/bin dans votre .bashrc
exemple:
	mips-elf-as exempleElf.s -o exempleElf.o

# simips
$(OBJDIR)/shell-no-readline.o: $(SRCDIR)/shell.c $(INCDIR)/shell.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/simips.o: $(SRCDIR)/simips.c
	$(CC) -c $(DFLAGS) $(CFLAGS) $< -o $@
simips: $(OBJDIR)/simips.o $(OBJDIR)/gpr.o $(OBJDIR)/mem.o $(OBJDIR)/shell.o $(OBJDIR)/elf_reader.o $(OBJDIR)/instructions.o $(OBJDIR)/operations.o $(OBJDIR)/relocation_s.o $(OBJDIR)/framebuffer_s.o
	$(CC) $(LDFLAGS) $^ -o $@

#simips - without readline
$(OBJDIR)/simips-no-readline.o: $(SRCDIR)/simips.c
	$(CC) -c $(CFLAGS) $< -o $@
simips-no-readline: $(OBJDIR)/simips-no-readline.o $(OBJDIR)/gpr.o $(OBJDIR)/mem.o $(OBJDIR)/shell-no-readline.o $(OBJDIR)/elf_reader.o $(OBJDIR)/instructions.o $(OBJDIR)/operations.o $(OBJDIR)/relocation_s.o $(OBJDIR)/framebuffer.o
	$(CC) $(LDFLAGS) $^ -o $@

compile_tests: $(ELF)

$(TSTDIR)/%.o: $(TSTDIR)/%.s
	mips-elf-as $^ -o $@


clean:
	rm -rf $(OBJ)

cleanall: clean
	rm -rf test_etudiants exempleElf.o $(TSTDIR)/*.o
