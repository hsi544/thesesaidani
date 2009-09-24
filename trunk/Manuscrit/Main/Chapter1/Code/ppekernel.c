#include <skell.hpp>
SKELL_KERNEL(sample,(2,(float const*,float*)));

int main(int argc, char** argv)
{
	float in[256], out[256];
	
	skell::environment(argc,argv);
	sample(in,out);
	
	return 0;
}
