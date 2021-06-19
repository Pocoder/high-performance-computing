#include <mpi.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

const int BUFFSIZE = 64;

int main(int argc, char* argv[]){
        int num, rank;
        MPI_Init(&argc, &argv);
        MPI_Status status;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &num);
        char recv_buf[BUFFSIZE];
        for (int target = 0; target<num; target++){
		if (target != rank) {
			const char* message = "Hello from ";
	                char buf[BUFFSIZE];
	                strcpy(buf, message);
	                int len = strlen(message);
	                buf[len] = '0'+rank;
	                buf[len+1] = '\0';
			MPI_Send(buf, strlen(buf)+1, MPI_CHAR, target, 0, MPI_COMM_WORLD);
	                MPI_Recv(recv_buf, BUFFSIZE, MPI_CHAR, target, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	                printf("recieved: \"%s\" by process %d\n", recv_buf, rank);
		}
        }
        MPI_Finalize();
        return 0;
}
