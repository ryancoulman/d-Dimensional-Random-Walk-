# Random Walk Simulation

This project implements a random walk simulation in multiple cartesian dimensions using C. The code demonstrates proficiency in several fundamental programming concepts, including dynamic memory allocation, use of structs, file I/O, and modular code organization.
This project was part of my final year module 'Programming' and received a mark of 88%. Please see `code_report.pdf` to see extensive testing of the programs capabilities and an explanation of the approach involved.   

## Features

- **Multi-Dimensional Random Walk:** Simulates random walks in configurable dimensions and grid sizes.
- **High-Performance Computation:** Efficiently handles large-scale simulations using dynamically allocated memory.
- **Data Output:** Outputs the results of simulations to CSV files for further analysis.

## Key Concepts Demonstrated

- **Dynamic Memory Allocation:** Utilises `malloc`, `realloc` and `free` to manage memory for simulation structures.
- **Pointers:**
- **Structs:** Defines and manipulates complex data structures to represent the state of the random walk.
- **File I/O:** Writes simulation results to CSV files for visualization and analysis.
- **Random Number Generation:** Uses random functions to simulate the movement of particles in the grid.

## Code Overview

### Main Simulation (`main.c`)

- Initializes the random number generator.
- Allocates memory for the simulation structures.
- Performs the random walk simulation.
- Outputs the results to CSV files.

### Header File (`header.h`)

- Defines the `pos_info` struct to store the position and state of the particle.
- Declares functions for:
  - Random number generation
  - Position updating and resetting
  - Memory allocation and deallocation
  - Printing positions
  - File handling

## Example Output

The simulation generates CSV files with the average number of iterations to visit all points in the grid for different dimensions and grid sizes.

```csv
L,N,Avg_iterations
1,1,1000.0
2,8,1500.0
...
