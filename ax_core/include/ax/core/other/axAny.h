//
//  axAny.h
//  ax_core
//
//  Created by Jason on 2013-10-02.
//
//

#ifndef ax_core_axAny_h
#define ax_core_axAny_h

#define	axAny_TypeList 	\
	axAny_Type( int,		int		) \
	axAny_Type( int64_t,	int64	) \
	axAny_Type( float,		float	) \
	axAny_Type( double,		double	) \
	axAny_Type( axObject*,	object	) \
//--------------

class axAny {
public:


	union {
		#define	axAny_Type(T,NAME) \
			T		v_##NAME;
		//----
			axAny_TypeList
		#undef axAny_Type
	};

	

};

#endif
