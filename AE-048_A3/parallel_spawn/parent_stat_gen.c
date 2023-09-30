#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <unistd.h>

double PI25DT = 3.141592653589793238462643;

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 1) {
        printf("This program is designed for one parent and multiple child processes only.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {

        FILE *file;
        char filename[] = "stat.txt";

        file = fopen(filename, "w");

        if (file == NULL) {
            printf("Error opening the file.\n");
            return 1;
        }

        for ( int k = 20; k <= 25; k++)
        {
            fprintf(file, "%d\n",k);

            for (int j = 2; j <= 20; j++)
            {
                int num_child_processes;
                num_child_processes = j; 

                if(num_child_processes == 0){
                    printf("Number of processes needs to be greater than 0\n");
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }

                char num_child_processes_str[20]; // Assuming a maximum of 20 characters for the integer string representation
                snprintf(num_child_processes_str, sizeof(num_child_processes_str), "%d", num_child_processes);

                int num_of_intervals;
                num_of_intervals = k;

                if(num_of_intervals == 0){
                    printf("Number of intervals needs to be greater than 0\n");
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }

                char num_of_intervals_str[20]; // Assuming a maximum of 20 characters for the integer string representation
                snprintf(num_of_intervals_str, sizeof(num_of_intervals_str), "%d", num_of_intervals);


                char* child_argv[] = {"child_program", num_child_processes_str, num_of_intervals_str,NULL};
        
                MPI_Comm child_comm;
                int spawn_error;

        
                double start_time, end_time;

                start_time = MPI_Wtime();
                // Spawn multiple child processes
                spawn_error = MPI_Comm_spawn("./child_program", child_argv, num_child_processes, MPI_INFO_NULL, 0, MPI_COMM_SELF, &child_comm, MPI_ERRCODES_IGNORE);

                if (spawn_error != MPI_SUCCESS) {
                    printf("Error spawning child processes. Exiting.\n");
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }

                double pi = 0.0;
        

                // Print child ranks
                for (int i = 0; i < num_child_processes; i++) {
                    double val;
                    MPI_Recv(&val, 1, MPI_DOUBLE, i, 0, child_comm, MPI_STATUS_IGNORE);
                    //printf("Received child process rank from child %d: %lf\n", i, val);
                    pi += val;
                }

                printf("pi is approximately %.16f, Error is %.16f\n",pi, fabs(pi - PI25DT));
                MPI_Barrier(child_comm);
                end_time = MPI_Wtime();
                fprintf(file, "%d %.9lf\n", num_child_processes,(end_time-start_time)*1e9);
                MPI_Comm_disconnect(&child_comm); 
            }
        }
        
        fclose(file);
    }

    MPI_Finalize();
    return 0;
}
