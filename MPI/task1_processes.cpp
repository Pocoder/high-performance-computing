#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
	int ProcNum, ProcRank, RecvRank;
	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	cout << "In group of " << ProcNum << " processes, Hello from process " << ProcRank << endl;
	MPI_Finalize();
	return 0;
}
