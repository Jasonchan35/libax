#ifndef __axDBOutParam_h__
#define __axDBOutParam_h__

#include "axDB_common.h"


class axDBOutParamList;
template<class T> axStatus axDBOutParamList_io( axDBOutParamList & list, T & v );

typedef axStatus (*axDBOutParamList_io_func)( axDBOutParamList &list, void* data );

template<class T> inline
axStatus axDBOutParamList_io_func_T( axDBOutParamList & list, void* data ) {
	return axDBOutParamList_io( list, *(T*) data );
}

class axDBOutParam_CB {
public:
	template<class T> axDBOutParam_CB( T &v ) {
		data = &v;
		func = axDBOutParamList_io_func_T<T>;
	}

	axDBOutParamList_io_func	func;
	void*	data;
};



#define axDB_c_type_list( NAME, C_TYPE, C_ITYPE ) \
	inline int axDBOutParamType( C_ITYPE &v ) { return axDB_c_type_##NAME; }
//----
	#include "axDB_c_type_list.h"
#undef axDB_c_type_list

//for SQL stmt output values
class axDBOutParam {
public:
	axDBOutParam( int _type = axDB_c_type_null, void* _data=NULL ) 
		: type(_type)
		, data(_data) 
	{}
	axStatus	onTake( axDBOutParam &src ) { *this=src; return 0; }
	
	int		type;
	void*	data;
};

class axDBOutParamList : public axArray< axDBOutParam, axDB_kArgListLocalBufSize > {
public:
	axStatus addArg( const axDBOutParam_CB & p ) {
		return p.func( *this, p.data );
	}

	axStatus addParam ( const axDBOutParam &p ) {
		return append( p );
	}

	template<class T>
	axStatus io( T &v, const char* name ) {
		axDBOutParamList_io( *this, v );	return 0;
	}
};


#define axDB_c_type_list( NAME, C_TYPE, C_ITYPE ) \
	inline axStatus axDBOutParamList_io( axDBOutParamList & list, C_ITYPE & v ) { return list.addParam( axDBOutParam( axDB_c_type_##NAME, &v ) ); }
//----
	#include "axDB_c_type_list.h"
#undef axDB_c_type_list

inline axStatus axDBOutParamList_io( axDBOutParamList & list, axStringA	& v ) { return axDBOutParamList_io(list, v.asInterface() ); }
inline axStatus axDBOutParamList_io( axDBOutParamList & list, axStringW	& v ) { return axDBOutParamList_io(list, v.asInterface() ); }
inline axStatus axDBOutParamList_io( axDBOutParamList & list, axByteArray & v ) { return axDBOutParamList_io(list, v.asInterface() ); }


template<class T> inline //for user-define class
axStatus axDBOutParamList_io( axDBOutParamList & list, T & v ) {
	return ax_const_cast(v).db_io( list );
}




#endif //__axDBOutParam_h__

