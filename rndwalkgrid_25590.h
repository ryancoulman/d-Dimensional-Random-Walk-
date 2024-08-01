#ifndef RNDWALKGRID_25590_H
#define RNDWALKGRID_25590_H

#include <stdio.h>

// structure to store the position of the particle within the grid
typedef struct {
    int *pos;
    int pos_size;
    int *pos_tracker;
    long tracker_size;
    int superindex;
    int length;
    } pos_info; 

// functions to handle the random walk
void random_int(int *dim, int *direc, int d);
void new_position(pos_info *);
void reset_position(pos_info *);
long ipow(int base, int exponent);
void flatten_array(pos_info *);
int iterations_to_vist_all(pos_info *);
void random_walk(pos_info *, float *, int);
// functions to handle memory allocation and deallocation
void allocate_struct(pos_info *, int L, int D);
void reallocate_struct(pos_info *, int L, int D);
void free_struct(pos_info *);
// functions to print positions 
void print_position(pos_info *);
void print_M_positions(pos_info *, int M);
// function to handle file writing
FILE *open_file(const char *, const char *);

#endif /* RNDWALKGRID_25590_H */
