if(rank() == 0)
{
	result_of<A>::type out;
	do
	{
		call<A>(out); DMA_send(out,1);
	} while( status() );
}

if(rank() == 1)
{
	parameters<B>::type in; 
	result_of<B>::type  out;
	do
	{
		DMA_recv(in,0); call<B>(in,out); DMA_send(out,1);
	} while( status() );
}

if(rank() == 2)
{
	parameters<C>::type in;
	do
	{
		DMA_recv(in,1); call<C>(in);
	} while( status() );
}
