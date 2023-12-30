#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

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
    int total_num_of_samples = atoi(argv[3]);

    //printf("Number of processes %d\n",num_of_processes);
    //printf("Number of samples %d\n",total_num_of_samples);

    int number_of_samples = 0;

    number_of_samples = total_num_of_samples/num_of_processes;
    
    if(rank < total_num_of_samples%num_of_processes){
        number_of_samples++;
    }

    srand(time(NULL)); 

    int my_count_inside_circle = 0;

    for (int i = 1; i <= number_of_samples; ++i)
    {
        double x = (double)rand() / RAND_MAX * 2 - 1;
        double y = (double)rand() / RAND_MAX * 2 - 1;

        if (x * x + y * y <= 1) {
            my_count_inside_circle++;
        }
    }
    
    MPI_Send(&my_count_inside_circle, 1, MPI_INT, 0, 0, parent_comm);
    MPI_Barrier(parent_comm);
    MPI_Comm_disconnect(&parent_comm);
    MPI_Finalize();
    return 0;
}

