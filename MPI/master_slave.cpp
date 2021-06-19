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
        if (rank == 0) {
		for (int i = 1; i<num;i++){
	                MPI_Recv(recv_buf, BUFFSIZE, MPI_CHAR, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			printf("recieved: \"%s\" by master\n", recv_buf);
			const char* message = "Hello from master, slave process ";
		        char buf[BUFFSIZE];
		        strcpy(buf, message);
		        int len = strlen(message);
		        buf[len] = '0'+i;
		        buf[len+1] = '\0';
			MPI_Send(buf, strlen(buf)+1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}
        } else {
		int target = 0;
		int source = 0;
		const char* message = "Message from slave process ";
	        char buf[BUFFSIZE];
	        strcpy(buf, message);
	        int len = strlen(message);
	        buf[len] = '0'+rank;
	        buf[len+1] = '\0';
                MPI_Send(buf, strlen(buf)+1, MPI_CHAR, target, 0, MPI_COMM_WORLD);
		MPI_Recv(recv_buf, BUFFSIZE, MPI_CHAR, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("recieved ans from master: \"%s\" by slave process %d\n", recv_buf, rank);
        }
        MPI_Finalize();
        return 0;
}
