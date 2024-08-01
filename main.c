#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"


int main() {

    // seed the random number generator with the current time (typically in seconds)
    srandom(time(NULL));

    // for Q1 set D = 1, and Q2 set D = 3
    int L = 10, D = 3;
    pos_info particle;

    // allocate memory for the structure for L and D, and initialsie
    allocate_struct(&particle, L, D);
    reset_position(&particle);

    // print first M positions of the random walk
    int M = 20;
    print_M_positions(&particle, M);

    // vary total iterations accordingly 
    int total_iterations = 1e4;
    float average_iterations;

    // average iterations to visit all points in grid for L, D 
    random_walk(&particle, &average_iterations, total_iterations);

    // average iterations for L = 1 to Lmax, D = D
    int Lmax = 100;
    FILE *fp1 = open_file("filename.csv", "L,N,Avg_iterations");
    for (L = 1, D = D; L <= Lmax; L++) {
        reallocate_struct(&particle, L, D);
        random_walk(&particle, &average_iterations, total_iterations);
        fprintf(fp1, "%d,%ld,%f\n", L, ipow(L, D), average_iterations);
    }
    fclose(fp1);

    // average iterations for L = 3, D = 1 to Dmax
    int Dmax = 13;
    FILE *fp2 = open_file("filename2.csv", "D,N,Avg_iterations");
    for (L = 3, D = 1; D <= Dmax; D++) {
        reallocate_struct(&particle, L, D);
        random_walk(&particle, &average_iterations, total_iterations);
        fprintf(fp2, "%d,%ld,%f\n", D, ipow(L, D), average_iterations);

    }
    fclose(fp2);

    // average iterations for L = 1 to Lmax, D = 1 to Dmax
    FILE *fp3 = open_file("filename3.csv", "L,D,N,Avg_iterations");
    Lmax = 13, Dmax = 4;
    for (D = 1; D <= Dmax; D++) {
        for (L = 1; L <= Lmax; L++) {
            reallocate_struct(&particle, L, D);
            random_walk(&particle, &average_iterations, total_iterations);
            fprintf(fp3, "%d,%d,%ld,%f\n", L, D, ipow(L, D), average_iterations);
        }
    }
    fclose(fp3);

    free_struct(&particle);
    return 0;

}


// functions to handle the random walk //

// generates a random dimension and direction
void random_int(int *dim, int *direc, int d) {

    int maxn = 1; // when called random() will return an integer in domian [0, maxn]
    *dim = random() % d; // pick random dimension
    *direc = random() % (maxn + 1); // pick random direction

    // if direction is 0, set to -1 so the partiicle either moves forward or backward
    if (*direc == 0) {
        *direc = -1;
    }

}


// generates a new position for the particle
void new_position(pos_info *p) {

    int dimension, direction;
    random_int(&dimension, &direction, p->pos_size); // generate random dimension and direction
    p->pos[dimension] += direction; // increment or decrement the position in the dimension  

    // implement periodic boundary conditions
    if (p->pos[dimension] < 1) {
        p->pos[dimension] = p->length;
    } else if (p->pos[dimension] > p->length) {
        p->pos[dimension] = 1;
    }

}


// resets the position of the particle and position tracker
void reset_position(pos_info *p) {
 
    // set start position to 1 in all dimensions
    for (int i = 0; i < p->pos_size; i++) {
        p->pos[i] = 1;
    }
    for (int i = 0; i < p->tracker_size; i++) {
        p->pos_tracker[i] = 0;
    }

}    


// slightly faster than <math.h> pow function as designed only to handle integers 
long ipow(int base, int exponent) {
    long result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}


// flattens the position array to a single index
void flatten_array(pos_info *p) {

    p->superindex = 0; 
    for (int i = 0; i < p->pos_size; i++) {
        p->superindex += (p->pos[i] - 1) * ipow(p->length, i);
    }

}


// returns the number of iterations to visit all points in the grid
int iterations_to_vist_all(pos_info *p) {

    int count = 0, iterations = 0;

    reset_position(p);

    // only increment count if the position has not been visited before
    while (count < p->tracker_size) {

        new_position(p);
        flatten_array(p);

        // check the position tracker at the flattened index of the current position
        if (p->pos_tracker[p->superindex] == 0) {
            p->pos_tracker[p->superindex] = 1;
            count++;
        } 
        iterations++;
    }

    return iterations;
}


// performs the random walk and returns the average number of iterations to visit all points
void random_walk(pos_info *p, float *average_iterations, int total_iterations) {

    long total = 0;
    *average_iterations = 0;

    for (int i = 0; i < total_iterations; i++) {
        total +=  iterations_to_vist_all(p);
    }

    *average_iterations = total / (float) total_iterations;
    printf("Average iterations = %f, for (L = %i, D = %i)\n", *average_iterations, p->length, p->pos_size);

}

// functions to handle memory allocation and deallocation //

// allocates memory for the structure and initilaise other variables 
void allocate_struct(pos_info *p, int l, int d) {

    p->length = l;
    p->pos_size = d;
    p->tracker_size = ipow(p->length, p->pos_size);

    // assign a block of memory of size D to the position pointer 
    p->pos = malloc(p->pos_size * sizeof(int));
    if (p->pos == NULL) {
       printf("Memory allocation failed\n");
        exit(1);
    }
    // assign a block of memory of size L^D to the position tracker pointer
    p->pos_tracker = malloc(p->tracker_size * sizeof(int));
    if (p->pos_tracker == NULL) {
        printf("Memory allocation failed\n");
        exit(2);
    }

}


// reallocates memory for the structure when L or D changes
void reallocate_struct(pos_info *p, int l, int d) {

    int realloc_pos;
    // if D is unchanged then the position array remains fixed in size 
    if (p->pos_size == d) {
        realloc_pos = 0;
    } else {
        realloc_pos = 1;
    }

    p->length = l;
    p->pos_size = d;
    p->tracker_size = ipow(p->length, p->pos_size);

    // reallocate memory for the position tracker
    p->pos_tracker = realloc(p->pos_tracker, p->tracker_size * sizeof(int));
    if (p->pos_tracker == NULL) {
        printf("Memory reallocation failed\n");
        exit(3);
    }
    // reallocate memory for the position array if D changes
    if (realloc_pos) {
        p->pos = realloc(p->pos, p->pos_size * sizeof(int));
        if (p->pos == NULL) {
            printf("Memory reallocation failed\n");
            exit(4);
        }
    }

}


// frees memory allocated for the structure
void free_struct(pos_info *p) {

    free(p->pos);
    free(p->pos_tracker);

}

// functions to print positions //

// prints the current position of the particle, in pareentheses if more than one dimension
void print_position(pos_info *p) {

    if (p->pos_size > 1) {printf("(");}
    for (int i = 0; i < p->pos_size; i++) {
        printf("%d", p->pos[i]);
        if (i < p->pos_size - 1) {
            printf(", ");
        }
    }
    if (p->pos_size > 1) {printf(") ");} else {printf(" ");}
}


// prints the first M positions of the random walk
void print_M_positions(pos_info *p, int M) {

    printf("[ ");
    print_position(p);
    for (int i = 0; i < M; i++) {
        new_position(p);
        print_position(p);
    }
    printf("]\n");

}

// function to open file for writing //

// returns a pointer the file opened for writing and writes the header
FILE *open_file(const char *filename, const char *header) {

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(5);
    }
    fprintf(fp, "%s\n", header);
    return fp;
}

