#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

//create random 2d array
int** fillRandom2dArray(int n) {
    int** randomArray = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        randomArray[i] = (int*)malloc(n * sizeof(int));
    }

    srand(time(NULL));  // Seed for random number generation

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            randomArray[i][j] = rand() % 10 + 1;  // Generates random number between 1 and 10
        }
    }

    return randomArray;
}

int** read2dArrayFromFile(const char* filename, int n) {
    int** newArray = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        newArray[i] = (int*)malloc(n * sizeof(int));
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (fscanf(file, "%d", &newArray[i][j]) != 1) {
                perror("Error reading from file");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);

    return newArray;
}

void write2dArrayToFile(int** arr, int n){

    FILE* file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fprintf(file, "%d ", arr[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

//prints a 2d array
void print2dArray(int** array, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}
//sequential floyd warshall
void floydWarshall(int numNodes, int** graph) {
    // Allocate memory for the distance matrix
    int** dist = (int**)malloc(numNodes * sizeof(int*));
    for (int i = 0; i < numNodes; i++) {
        dist[i] = (int*)malloc(numNodes * sizeof(int));
    }

    // Initialize the distance matrix
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    // Perform the Floyd-Warshall algorithm
    for (int k = 0; k < numNodes; k++) {
        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < numNodes; j++) {
                if (dist[i][k] != -1 && dist[k][j] != -1 &&
                    (dist[i][j] == -1 || dist[i][k] + dist[k][j] < dist[i][j])) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Print the shortest distances
    printf("Shortest distances between every pair of nodes:\n");
    print2dArray(dist,numNodes);

    // Free the allocated memory
    for (int i = 0; i < numNodes; i++) {
        free(dist[i]);
    }
    free(dist);
}

//returns a copy of the provided source 2d array
int** copy2dArray(int n, int** source) {
    int** destination = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        destination[i] = (int*)malloc(n * sizeof(int));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            destination[i][j] = source[i][j];
        }
    }

    return destination;
}

//flattens a 2d array to a 1d array
int* flattenArray(int** array, int n) {
    int* flatArray = (int*)malloc(n * n * sizeof(int));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            flatArray[i * n + j] = array[i][j];
        }
    }

    return flatArray;
}

//prints a flat 1d array
void printFlatArray(int* flatArray, int n) {
    for (int i = 0; i < n * n; ++i) {
        printf("%d ", flatArray[i]);
    }
    printf("\n");
}

//transforms a 1d array into a 2d array
int** reshapeArray(int* flatArray, int n) {
    int** newArray = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i < n; ++i) {
        newArray[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            newArray[i][j] = flatArray[i * n + j];
        }
    }

    return newArray;
}

//checks if a number is a perfect square
int isPerfectSquare(int num) {
    int sqrt_num = sqrt(num);
    return (sqrt_num * sqrt_num == num);
}


int main(int argc, char** argv) {
    int stat_gen = 1;
    if (argc < 2) {

        stat_gen = 0;
    }

    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (!isPerfectSquare(size)){
        printf("Number of processes needs to be perfect square");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    int numberOfNodes = 0;

    double start_time, end_time;

    if(stat_gen == 1){
        numberOfNodes = atoi(argv[1]);
    }

    if(rank == 0 && stat_gen == 1){
        start_time = MPI_Wtime();
    }
    
    int** random2dArray = NULL;
    int* flatArray = NULL;

    if(rank == 0){

        if(stat_gen == 0){

            printf("Enter number of nodes:\n");
            scanf("%d",&numberOfNodes);
        }
        

        if(numberOfNodes%((int)sqrt(size)) != 0){
            MPI_Abort(MPI_COMM_WORLD,1);
        }

        random2dArray = read2dArrayFromFile("input.txt", numberOfNodes);
        //int** copiedMatrix = copy2dArray(numberOfNodes,random2dArray);
        //floydWarshall(numberOfNodes,copiedMatrix);
        flatArray = flattenArray(random2dArray,numberOfNodes);
    }


    MPI_Bcast(&numberOfNodes, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(rank != 0){

        flatArray = (int*)malloc(numberOfNodes * numberOfNodes * sizeof(int));
    }

    MPI_Bcast(flatArray, numberOfNodes * numberOfNodes, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank != 0){

        random2dArray = reshapeArray(flatArray,numberOfNodes);
    }
    
    int dims[2] = {sqrt(size), sqrt(size)};
    MPI_Dims_create(size, 2, dims);
    int periods[2] = {0,0};

    MPI_Comm comm_cart;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm_cart);


    int coords[2];
    MPI_Cart_coords(comm_cart, rank, 2, coords);

    MPI_Comm row_comm;
    MPI_Comm_split(MPI_COMM_WORLD, coords[0], rank, &row_comm);
    int row_rank;
    MPI_Comm_rank(row_comm, &row_rank);

    MPI_Comm col_comm;
    MPI_Comm_split(MPI_COMM_WORLD, coords[1]+dims[0], rank, &col_comm);
    int col_rank;
    MPI_Comm_rank(col_comm, &col_rank);

    int rowIndexStart = coords[0]*(numberOfNodes/((int)sqrt(size)));
    int rowIndexEnd = rowIndexStart + (numberOfNodes/((int)sqrt(size))) - 1;

    int colIndexStart = coords[1]*(numberOfNodes/((int)sqrt(size)));
    int colIndexEnd = colIndexStart + (numberOfNodes/((int)sqrt(size))) - 1;

    for(int k = 0; k < numberOfNodes; k++){

        int row[(numberOfNodes)/((int)sqrt(size))];
        int row_root_coords[2];
        row_root_coords[0] = k/(numberOfNodes/((int)sqrt(size)));
        row_root_coords[1] = coords[1];
        
        if(k >= rowIndexStart && k <= rowIndexEnd){

            int index = 0;
            for(int i = colIndexStart; i <= colIndexEnd; i++){

                row[index] = random2dArray[k][i];
                index++;
            }
            //send to all processes in the same column
        }

        
        MPI_Bcast(row, colIndexEnd - colIndexStart +1, MPI_INT, row_root_coords[0], col_comm);
        
        int col[(numberOfNodes)/((int)sqrt(size))];
        int col_root_coords[2];
        col_root_coords[0] = coords[0];
        col_root_coords[1] = k/(numberOfNodes/((int)sqrt(size)));
        
        if(k >= colIndexStart && k <= colIndexEnd){

            int index = 0;
            for(int i = rowIndexStart; i <= rowIndexEnd; i++){

                col[index] = random2dArray[i][k];
                index++;
            }
            //send to all processes in the same row
        }

        MPI_Bcast(col, rowIndexEnd - rowIndexStart +1, MPI_INT, col_root_coords[1], row_comm);
        
        if(k < rowIndexStart || k > rowIndexEnd){
            
            int index = 0;
            for(int i = colIndexStart; i <= colIndexEnd; i++){

                random2dArray[k][i] = row[index];
                index++;
            }
        }
        
        if(k < colIndexStart || k > colIndexEnd){
            
            int index = 0;
            for(int i = rowIndexStart; i <= rowIndexEnd; i++){

                random2dArray[i][k] = col[index];
                index++;
            }
        }

        for(int i = rowIndexStart; i <= rowIndexEnd; i++){

            for(int j = colIndexStart; j <= colIndexEnd; j++){

                if (random2dArray[i][k] != INT_MAX && random2dArray[k][j] != INT_MAX && random2dArray[i][k] + random2dArray[k][j] < random2dArray[i][j]) {
                    random2dArray[i][j] = random2dArray[i][k] + random2dArray[k][j];
                }
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    if(rank != 0){
        
        int* flatArray = flattenArray(random2dArray,numberOfNodes);
        MPI_Send(flatArray, numberOfNodes*numberOfNodes, MPI_INT, 0, 69420, MPI_COMM_WORLD);
        
    }else{

        for(int i = 1; i < size; i++){
            int receivedArray[numberOfNodes*numberOfNodes];
            MPI_Recv(receivedArray, numberOfNodes*numberOfNodes, MPI_INT, i, 69420, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int** reshapedArray = reshapeArray(receivedArray,numberOfNodes);
            int from_coords[2];
            MPI_Cart_coords(comm_cart, i, 2, from_coords);
    
            int curRowIndexStart = from_coords[0]*(numberOfNodes/((int)sqrt(size)));
            int curRowIndexEnd = curRowIndexStart + (numberOfNodes/((int)sqrt(size))) - 1;

            int curColIndexStart = from_coords[1]*(numberOfNodes/((int)sqrt(size)));
            int curColIndexEnd = curColIndexStart + (numberOfNodes/((int)sqrt(size))) - 1;

            for(int m = curRowIndexStart; m <= curRowIndexEnd; m++){
                
                for(int n = curColIndexStart; n <= curColIndexEnd; n++){

                    random2dArray[m][n] = reshapedArray[m][n];
                }
            }
        }

        write2dArrayToFile(random2dArray,numberOfNodes);
    }

    //MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0 && stat_gen == 1){
        end_time = MPI_Wtime();
        
        FILE *file = fopen("stat.txt", "a");

        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        fprintf(file, "%d %d %.9lf\n", numberOfNodes,size,((end_time-start_time)*1e9));

        fclose(file);
    }
    
    MPI_Comm_free(&col_comm);
    MPI_Comm_free(&row_comm);
    MPI_Comm_free(&comm_cart);
    MPI_Finalize();

    return 0;
}

