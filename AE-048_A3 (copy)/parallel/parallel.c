#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int done = 0, n, myid, numprocs, i, myCountInsideCircle, countInsideCircle, numberOfSamples;
    double PI25DT = 3.141592653589793238462643;
    double pi;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    while (!done)
    {
	if (myid == 0) {
	    printf("Enter the number of samples: (0 quits)\n");
	    scanf("%d",&n);
	}
	
	countInsideCircle = 0;

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (n == 0) break;

  	myCountInsideCircle = 0;
  	
	numberOfSamples = 0;

	numberOfSamples = n/numprocs;
	
	if(myid < n%numprocs){
		numberOfSamples++;
	}

	srand(time(NULL)); 

    for (i = 1; i <= numberOfSamples; ++i)
    {
        double x = (double)rand() / RAND_MAX * 2 - 1;
        double y = (double)rand() / RAND_MAX * 2 - 1;

        if (x * x + y * y <= 1) {
            myCountInsideCircle++;
        }
    }
    
	MPI_Reduce(&myCountInsideCircle, &countInsideCircle, 1, MPI_INT, MPI_SUM, 0,
		   MPI_COMM_WORLD);
    

    pi = (double)countInsideCircle / n * 4;
	if (myid == 0)
	    printf("pi is approximately %.16f, Error is %.16f\n",
		   pi, fabs(pi - PI25DT));
    }
    MPI_Finalize();
    return 0;
}
