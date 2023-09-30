#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm parent_comm;
    MPI_Comm_get_parent(&parent_comm);

    if(parent_comm == NULL){
        printf("Error no parent.\n");
         MPI_Abort(MPI_COMM_WORLD, 1);
    }


    //printf("Process with rank %d has started working!\n", rank);
    
    int num_of_processes = atoi(argv[2]);
    int num_of_intervals = atoi(argv[3]);

    //printf("Number of processes %d\n",num_of_processes);
    //printf("Number of intervals %d\n",num_of_intervals);

    double h;

    h   = 1.0 / (double) num_of_intervals;
	double sum = 0.0;
	for (int i = rank + 1; i <= num_of_intervals; i += num_of_processes) {
	    double x = h * ((double)i - 0.5);
	    sum += 4.0 / (1.0 + x*x);
	}
	double mypi = h * sum;
    
    MPI_Send(&mypi, 1, MPI_DOUBLE, 0, 0, parent_comm);
    MPI_Barrier(parent_comm);
    MPI_Comm_disconnect(&parent_comm);
    MPI_Finalize();
    return 0;
}

