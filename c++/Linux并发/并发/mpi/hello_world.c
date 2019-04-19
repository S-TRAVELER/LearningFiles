#include <stdio.h>
#include <mpi.h>

int main(int argc,char** argv){
	MPI_Init(NULL,NULL);
	int np_size;
	MPI_Comm_size(MPI_COMM_WORLD,&np_size);

	int np_rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&np_rank);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name,&name_len);

	printf("Hello wrold from processor %s, rank %d out of %d processors\n",processor_name,np_rank,np_size);
	MPI_Finalize();

	return 0;
}
