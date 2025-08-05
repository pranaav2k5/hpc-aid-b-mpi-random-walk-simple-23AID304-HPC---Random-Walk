#include <iostream>
#include <cstdlib> 
#include <ctime>   
#include <mpi.h>

void walker_process();
void controller_process();

int domain_size;
int max_steps;
int world_rank;
int world_size;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (argc != 3)
    {
        if (world_rank == 0)
        {
            std::cerr << "Usage: mpirun -np <num_processes> " << argv[0]
                      << " <domain_size> <max_steps>" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    domain_size = std::atoi(argv[1]);
    max_steps = std::atoi(argv[2]);

    if (world_rank == 0)
    {
        controller_process();
    }
    else
    {
        walker_process();
    }

    MPI_Finalize();
    return 0;
}

void walker_process()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)) + world_rank);

    int position = 0;
    int steps = 0;

    while (std::abs(position) <= domain_size && steps < max_steps)
    {
        int direction = (std::rand() % 2 == 0) ? -1 : 1;
        position += direction;
        steps++;
    }

    std::cout << "Rank " << world_rank << ": Walker finished in " << steps << " steps." << std::endl;

    int done_signal = 1;
    MPI_Send(&done_signal, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
}

void controller_process()
{
    int completed_walkers = 0;
    int total_walkers = world_size - 1;

    while (completed_walkers < total_walkers)
    {
        MPI_Status status;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        int count;
        MPI_Get_count(&status, MPI_INT, &count);

        if (count == 1)
        {
            int message;
            MPI_Recv(&message, count, MPI_INT,
                     status.MPI_SOURCE, status.MPI_TAG,
                     MPI_COMM_WORLD, &status);
            completed_walkers++;
        }
    }

    std::cout << "Controller: All " << total_walkers << " walkers have finished." << std::endl;
}
