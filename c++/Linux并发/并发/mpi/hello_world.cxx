#include <stdio.h>
#include <mpi.h>

int main(int argc,char** argv){
	MPI::Init(&argc,&argv);
	int np_size=MPI::COMM_WORLD.Get_size();

	int np_rank=MPI::COMM_WORLD.Get_rank();

	char processor_name[MPI::MAX_PROCESSOR_NAME];
	int name_len;
	MPI::COMM_WORLD.Get_processor_name(processor_name,&name_len);

	printf("Hello wrold from processor %s, rank %d out of %d processors\n",processor_name,np_rank,np_size);
	MPI::Finalize();

	return 0;
}
