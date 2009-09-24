template<class LS, class RS>
expr<pipe, args<LS,RS> > 
operator|( LS const&, RS const& )
{
	return expr<pipe, args<LS,RS> >();
}
