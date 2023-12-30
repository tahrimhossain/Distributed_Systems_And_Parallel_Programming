CC = mpicc
MPIRUN = mpirun
CFLAGS = -Wall -O2
LDFLAGS = -lm

all: parent_program child_program parent_stat_gen

parent_program: parent_program.c
	$(CC) $(CFLAGS) -o parent_program parent_program.c $(LDFLAGS)

child_program: child_program.c
	$(CC) $(CFLAGS) -o child_program child_program.c $(LDFLAGS)

parent_stat_gen: parent_stat_gen.c
	$(CC) $(CFLAGS) -o parent_stat_gen parent_stat_gen.c $(LDFLAGS)

run_parent: parent_program
	$(MPIRUN) --oversubscribe -np 1 ./parent_program

run_stat: parent_stat_gen
	$(MPIRUN) --oversubscribe -np 1 ./parent_stat_gen

clean:
	rm -f parent_program child_program parent_stat_gen

