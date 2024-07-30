#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
    int count = 0;
    double x, y, z, pi;
    int my_rank;
    int my_size;
    int tot_count = 0;
    int part_count = 0;
    double start, end;
    MPI_Status status;

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&my_size);

    start = MPI_Wtime();
    srand(SEED * my_rank); // Important: Multiply SEED by "rank" when you introduce MPI!

    
    int tot_it = NUM_ITER / my_size;
    // Calculate PI following a Monte Carlo method
    for (int iter = 0; iter < tot_it; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));
        
        // Check if point is in unit circle
        if (z <= 1.0)
        {
            count++;
        }
    }
    

    MPI_Reduce(&count, &part_count,1,MPI_INT, MPI_SUM,0,MPI_COMM_WORLD);
    if(my_rank == 0){
        tot_count += part_count;
        pi = ((double)tot_count / (double)NUM_ITER) * 4.0;

        printf("The result is %f\n", pi);
        end = MPI_Wtime();
        double tot_time = end - start;

        printf("The program took %f seconds to run\n", tot_time);
        
    }
    // Estimate Pi and display the result
    MPI_Finalize();
    
    return 0;
}

