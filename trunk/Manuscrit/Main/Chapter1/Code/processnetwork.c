network< int_<0>, int_<2>
	, list< process< int_<0>
		, desc< pid<-1>, pid<1>
			, instrs<Call<&A>,Send>
			>
		>
	, process< int_<1>
		, desc< pid<0>, pid<2>
			, instrs<Recv,Call<&B>,Send>
			>
		>
	, process< int_<2>
		, desc< pid<1>, pid<-1>
			, instrs<Recv,Call<&C> >
			>
		>
	>
>
