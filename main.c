#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "quicksort.h"

// return an array where each element contains the chunk size
// assigned to each rank. Used in scatterv function
int * compute_chunk_sizes (int n_elements, int n_cpus) {
    int *ret_arr = malloc(n_cpus * sizeof(int));
    int index;
    int chunk_size;
    int remainder_size;

    // elements can be evenly distributed:
    if (n_elements % n_cpus == 0) {
        for (int i = 0; i < n_cpus; i++) {
            ret_arr[i] = n_elements / n_cpus;
        }
    }
    // elements can't be evenly distributed:
    else {
        chunk_size = (int) n_elements / n_cpus;
        remainder_size = (int) n_elements - ((n_cpus) * chunk_size);
        for (int i = 0; i < n_cpus; i ++) {
            ret_arr[i] = chunk_size;
        }

        // distribute randomly the remainder:
        srand(time(NULL));
        for (int i = 0; i < remainder_size; i ++) {
            index = rand() % n_cpus;
            ret_arr[index] += 1;
        }
    }
    
    return ret_arr;
}

int main (int argc, char **argv) {
    FILE *file = NULL;
    int *data = NULL;
    int *buf;
    int n_elements;
    int n_cpus, rank;
    double timer;
    MPI_Status status;

    if (argc != 3) {
        printf("Input arguments are invalid\n");
        exit(-1);
    }

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &n_cpus);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    // rank 0 is the root process
    if (rank == 0) {
        file = fopen(argv[1], "r");
        // first number in the file represents the number of elements
        fscanf(file, "%d", &n_elements);

        data = malloc(n_elements * sizeof(int));
        for (int i = 0; i < n_elements; i++) {
            fscanf(file, "%d", &data[i]);
        }

        fclose(file);
        file = NULL;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    // broadcast size of data to all other processes
    MPI_Bcast(&n_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // calculate size of individual chunks and displacements array
    // chunk_sizes[i] = size of chunk given to processor with rank i
    int *chunk_sizes;
    chunk_sizes = compute_chunk_sizes(n_elements, n_cpus);

    // local buffer to store the partition of the data
    //buf = malloc(sizeof(chunk_sizes[rank]) * sizeof(int));
    //buf = malloc(sizeof(chunk_sizes[rank]) * sizeof(int));
    buf = malloc(chunk_sizes[rank] * sizeof(int));
    int local_buf_size = chunk_sizes[rank];
    
    // displs[i] = offset from start of data array, from 
    // which processor i has to find its chunk
    int displs[n_cpus];
    displs[0] = 0;
    for (int i = 1; i < n_cpus; i++) {
        displs[i] = displs[i -1] + chunk_sizes[i - 1];
    }
    MPI_Barrier(MPI_COMM_WORLD);
    
    // scatter elements with variable lengths, according to
    // array chunk_sizes and displacements
    MPI_Scatterv(data, chunk_sizes, displs, MPI_INT,
    		 buf, chunk_sizes[rank], MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        free(data);
    }
    data = NULL;

    // start measuring time
    timer -= MPI_Wtime();
    // sort local buffer
    quick_sort(buf, 0, chunk_sizes[rank] - 1);

    MPI_Barrier(MPI_COMM_WORLD);

    // initialize variables for merging tree
    int max_depth = log_2(n_cpus);
    int offset = 1;
    int recv_size = 0;
    for (int level = 1; level <= max_depth; level ++) {
        if (rank % (2 * offset) != 0) {
	    MPI_Send(buf, local_buf_size, MPI_INT,
       		     rank - offset, 0, MPI_COMM_WORLD);
            break;
        } 

        if (rank + offset < n_cpus) {
	    // get the size of receiving buffer using Probe
	    MPI_Probe(rank + offset, 0, MPI_COMM_WORLD, &status);
	    MPI_Get_count(&status, MPI_INT, &recv_size);

	    // receive the actual buffer
	    int *recv_buf = malloc(sizeof(int) * (recv_size));
	    MPI_Recv(recv_buf, recv_size, MPI_INT, rank + offset, 0, 
	             MPI_COMM_WORLD, &status);

	    data = concatenate(buf, recv_buf, local_buf_size, recv_size);
	    free(buf);
	    free(recv_buf);
	    buf = data;
	    local_buf_size += recv_size;
        }
	offset *= 2;
    }

    timer += MPI_Wtime();

    // print out final result
    if (rank == 0) {
    //print_arr(data, n_elements);
	printf("with %d cores it has taken time %f\n", n_cpus, timer);
	//print_arr(chunk_sizes, n_cpus);
	free(data);
    }

    free(chunk_sizes);
    MPI_Finalize();
    return 0;
}
