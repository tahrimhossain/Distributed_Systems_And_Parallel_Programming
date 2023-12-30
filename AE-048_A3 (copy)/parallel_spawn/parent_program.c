#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <unistd.h>

double PI25DT = 3.141592653589793238462643;

int main(int argc, char* argv[]) {
    int rank, size;
    double pi;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 1) {
        printf("This program is designed for one parent and multiple child processes only.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {
        
        while(1){

        
            int num_child_processes;
            printf("Enter number of processes: \n");
            scanf("%d", &num_child_processes);

            if(num_child_processes == 0){
                printf("Number of processes needs to be greater than 0\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }

            char num_child_processes_str[20]; // Assuming a maximum of 20 characters for the integer string representation
            snprintf(num_child_processes_str, sizeof(num_child_processes_str), "%d", num_child_processes);

            int num_of_samples;
            printf("Enter number of samples: \n");
            scanf("%d", &num_of_samples);

            if(num_of_samples == 0){
                printf("Number of samples needs to be greater than 0\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }

            char num_of_samples_str[20]; // Assuming a maximum of 20 characters for the integer string representation
            snprintf(num_of_samples_str, sizeof(num_of_samples_str), "%d", num_of_samples);


            char* child_argv[] = {"child_program", num_child_processes_str, num_of_samples_str,NULL};
        
            MPI_Comm child_comm;
            int spawn_error;

        

            // Spawn multiple child processes
            spawn_error = MPI_Comm_spawn("./child_program", child_argv, num_child_processes, MPI_INFO_NULL, 0, MPI_COMM_SELF, &child_comm, MPI_ERRCODES_IGNORE);

            if (spawn_error != MPI_SUCCESS) {
                printf("Error spawning child processes. Exiting.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }

            int total_inside = 0;
        

            // Print child ranks
            for (int i = 0; i < num_child_processes; i++) {
                int val;
                MPI_Recv(&val, 1, MPI_INT, i, 0, child_comm, MPI_STATUS_IGNORE);
                //printf("Received child process rank from child %d: %lf\n", i, val);
                total_inside += val;
            }
            pi = (double)total_inside / num_of_samples * 4;
            printf("pi is approximately %.16f, Error is %.16f\n",pi, fabs(pi - PI25DT));
            MPI_Barrier(child_comm);
            MPI_Comm_disconnect(&child_comm);
        }

    }

    MPI_Finalize();
    return 0;
}
