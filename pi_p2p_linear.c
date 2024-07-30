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
    double x_rec;
    double y_rec;
    int my_rank;
    int my_size;
    int tot_count = 0;
    int tag = 0;
    double start, end;
    MPI_Status status;
    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&my_size);
    start = MPI_Wtime();

    srand((SEED * my_rank)); // Important: Multiply SEED by "rank" when you introduce MPI!
   
    int total_it = NUM_ITER / my_size;
    // Calculate PI following a Monte Carlo method
    for (int iter = 0; iter < total_it; iter++)
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
    if(my_rank !=0){
        MPI_Send(&count, 1, MPI_INT, 0,tag, MPI_COMM_WORLD);
    }
    else{
        tot_count += count;
        for(int i = 1; i < my_size; i++){
            MPI_Recv(&count,1, MPI_INT,i,tag,MPI_COMM_WORLD, &status);
            tot_count +=count;
        }
        pi = ((double)tot_count / (double)NUM_ITER) * 4.0;
        printf("The result is %f\n", pi);
        end = MPI_Wtime();
        double tot_time = end - start;

        printf("The total excecution time is: %f seconds\n", tot_time);
    }
    MPI_Finalize();
    return 0;
}

