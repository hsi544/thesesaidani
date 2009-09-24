#include <skell.hpp>

void sqr()
{
	float in[32],out[32];
	
	pull(arg0_,in);
	for(int i=0;i<32;++i) out[i] = in[i]*in[i];
	push(arg1_,out);
	
	terminate();
}

SKELL_KERNEL(sample,(2,(float const*,float*)))
{
	run( pardo<8>( seq(sqr) );
}
