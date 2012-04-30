void full_chain_harris(tile const&,tile&)
{
	run( pardo<8>((seq(grad),seq(mul)
				   ,seq(gauss),seq(coarsity)));
		}
		
		void half_chain_harris(tile const&,tile&)
		{
		run( pardo<4>(  (seq(grad),seq(mul)) 
					  | (seq(gauss),seq(coarsity)));
			}
			
			void no_chain_harris(tile const&,tile&)
			{
			run( pardo<2>( seq(grad)  | seq(mul) 
						  | seq(gauss) | seq(coarsity));
}
