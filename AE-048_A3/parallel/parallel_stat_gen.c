#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    int done = 0, n, myid, numprocs, i;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    
    FILE *file;
    
    if(myid == 0){

    	char filename[] = "stat.txt";

    	file = fopen(filename, "a");

    	if (file == NULL) {
        	printf("Error opening the file.\n");
        	return 1;
    	}
    }

    for(int j = 1000000; j <= 5000000; j+=1000000){

		if (myid == 0) {

	    	n = j;
		}
		MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (n == 0) break;
  	
  		double start_time, end_time;

    	start_time = MPI_Wtime();

		h   = 1.0 / (double) n;
		sum = 0.0;
		for (i = myid + 1; i <= n; i += numprocs) {
	    	x = h * ((double)i - 0.5);
	    	sum += 4.0 / (1.0 + x*x);
		}
		mypi = h * sum;
    
		MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0,
		   	MPI_COMM_WORLD);
		end_time = MPI_Wtime();
    
		if (myid == 0){
	    	fprintf(file, "%d %d %.9lf\n", n,numprocs,(end_time-start_time)*1e9);
		}
    }

    if(myid == 0){
    	fclose(file);
    }
    
    MPI_Finalize();
    return 0;
}
