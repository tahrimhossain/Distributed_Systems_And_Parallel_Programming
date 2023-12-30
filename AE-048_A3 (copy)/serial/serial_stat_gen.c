#include <stdio.h>
#include <math.h>
#include<mpi.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int done = 0, n, i, countInsideCircle;
    double PI25DT = 3.141592653589793238462643;
    double pi;

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
        for(int j = 1000000; j <= 5000000; j+=1000000){

            n = j;

            if (n == 0) break;

            double start_time, end_time;

            start_time = MPI_Wtime();

            countInsideCircle = 0;
            srand(time(NULL)); 

            for (i = 1; i <= n; ++i)
            {
                double x = (double)rand() / RAND_MAX * 2 - 1;
                double y = (double)rand() / RAND_MAX * 2 - 1;

                if (x * x + y * y <= 1) {
                    countInsideCircle++;
                }
            }

            pi = (double)countInsideCircle / n * 4;

            end_time = MPI_Wtime();

            fprintf(file, "%d %d %.9lf\n", n,1,(end_time-start_time)*1e9);

            printf("pi is approximately %.16f, Error is %.16f\n",pi, fabs(pi - PI25DT));
        }

        fclose(file);
    }
    
    MPI_Finalize();
    return 0;
}
