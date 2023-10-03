CC = mpicc
MPIRUN = mpirun
CFLAGS = -Wall -O2
LDFLAGS = -lm


all: parallel parallel_stat_gen

parllel: parallel.c
	$(CC) $(CFLAGS) -o parallel parallel.c $(LDFLAGS)

parallel_stat_gen: parallel_stat_gen.c
	$(CC) $(CFLAGS) -o parallel_stat_gen parallel_stat_gen.c $(LDFLAGS)

run_parallel: parallel
	$(MPIRUN) --oversubscribe -np 1 ./parallel

run_stat: parallel_stat_gen
	rm -f stat.txt
	$(MPIRUN) --oversubscribe -np 5 ./parallel_stat_gen
	$(MPIRUN) --oversubscribe -np 6 ./parallel_stat_gen
	$(MPIRUN) --oversubscribe -np 7 ./parallel_stat_gen
	$(MPIRUN) --oversubscribe -np 8 ./parallel_stat_gen
	$(MPIRUN) --oversubscribe -np 9 ./parallel_stat_gen
	$(MPIRUN) --oversubscribe -np 10 ./parallel_stat_gen

clean:
	rm -f parallel parallel_stat_gen	