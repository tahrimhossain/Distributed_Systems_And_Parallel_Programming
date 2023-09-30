#include <stdio.h>
#include <math.h>
#include<mpi.h>

int main(int argc, char* argv[])
{
    int done = 0, n, i;
    double PI25DT = 3.141592653589793238462643;
    double pi, h, sum, x;

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 1) {
        printf("This program is designed for one process only.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    FILE *file;
    char filename[] = "stat.txt";

    file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    if(rank == 0){
        for(int j = 1; j <= 500; j++){

            n = j;

            if (n == 0) break;

            double start_time, end_time;

            start_time = MPI_Wtime();

            h = 1.0 / (double)n;
            sum = 0.0;

            for (i = 1; i <= n; ++i)
            {
                x = h * ((double)i - 0.5);
                sum += 4.0 / (1.0 + x * x);
            }

            pi = h * sum;

            end_time = MPI_Wtime();

            fprintf(file, "%d %.9lf\n", n,(end_time-start_time)*1e9);

            //printf("pi is approximately %.16f, Error is %.16f\n",pi, fabs(pi - PI25DT));
        }

        fclose(file);
    }
    
    MPI_Finalize();
    return 0;
}
