#include <iostream>
#include <cstdlib> // For atoi, rand, srand
#include <ctime>   // For time
#include <mpi.h>

void walker_process();
void controller_process();

int domain_size;
int max_steps;
int world_rank;
int world_size;

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes and the rank of this process
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (argc != 3)
    {
        if (world_rank == 0)
        {
            std::cerr << "Usage: mpirun -np <p> " << argv[0] << " <domain_size> <max_steps>" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    domain_size = atoi(argv[1]);
    max_steps = atoi(argv[2]);

    if (world_rank == 0)
    {
        // Rank 0 is the controller
        controller_process();
    }
    else
    {
        // All other ranks are walkers
        walker_process();
    }

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}

void walker_process()
{
    // Seed the random number generator.
    // Using rank ensures each walker gets a different sequence of random numbers.
    srand(time(NULL) + world_rank);

    // TODO: Implement the random walk logic for a walker process.
    // 1. Initialize the walker's position to 0.
    // 2. Loop for a maximum of `max_steps`.
    // 3. In each step, randomly move left (-1) or right (+1).
    // 4. Check if the walker has moved outside the domain [-domain_size, +domain_size].
    // 5. If the walk is finished (either out of bounds or max_steps reached):
    //    a. Print a message including the keyword "finished". For example:
    //       "Rank X: Walker finished in Y steps."
    //    b. Send an integer message to the controller (rank 0) to signal completion.
    //    c. Break the loop.
}

void controller_process()
{
    // TODO: Implement the logic for the controller process.
    // 1. Determine the number of walkers (world_size - 1).
    // 2. Loop that many times to receive a message from each walker.
    // 3. Use MPI_Recv to wait for a message. Use MPI_ANY_SOURCE to accept
    //    a message from any walker that finishes.
    // 4. After receiving messages from all walkers, print a final summary message.
    //    For example: "Controller: All X walkers have finished."
}