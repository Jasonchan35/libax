//
//  axDB_common.h
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDB_common_h__
#define __axDB_common_h__

#include "../data_structure/axSharedPtr.h"
#include "../time/axTimeStamp.h"
#include "../time/axDateTime.h"

enum {
	axDB_c_type_null = 0,

	#define axDB_c_type_list( NAME, C_TYPE, C_ITYPE ) \
		axDB_c_type_##NAME,
	//----
		#include "axDB_c_type_list.h"
	#undef axDB_c_type_list
};

const char* axDB_c_type_name( int type );

const size_t	axDB_kArgListLocalBufSize = 32;
const size_t	axDB_kRowIdType	= axDB_c_type_int64;


class axDBParamList;
template<class T> void axDBParamList_io( axDBParamList & list, const T & v );

typedef void (*axDBParamList_io_func)( axDBParamList &list, const void* data );

template<class T> inline
void axDBParamList_io_func_T( axDBParamList & list, const void* data ) {
	axDBParamList_io( list, *(const T*) data );
}

class axDBParam_CB {
public:
	template<class T> axDBParam_CB( const T &v ) {
		data = &v;
		func = axDBParamList_io_func_T<T>;
	}

	axDBParamList_io_func	func;
		const void*			data;
};

//for SQL stmt input param
class axDBParam {
public:
	axDBParam( int _type = axDB_c_type_null ) : type( _type )
	{}

	union {
		bool				p_bool;
		int8_t				p_int8;
		int16_t				p_int16;
		int32_t				p_int32;
		int64_t				p_int64;
		float				p_float;
		double				p_double;
		const char*			p_strA;
		const wchar_t*		p_strW;
		const axTimeStamp*	p_TimeStamp;
		const axDateTime*	p_DateTime;
		const axIByteArray* p_ByteArray;
	};

	axStatus	onTake( axDBParam &src ) { *this=src; return 0; }

	axStatus	toStringFormat( axStringFormat & f ) const;
	int			type;
};

class axDBParamList : public axArray< axDBParam, axDB_kArgListLocalBufSize > {
public:
	axDBParamList&	operator << ( const axDBParam_CB &p ) {
		p.func( *this, p.data );
		return *this;
	}

	axDBParamList&	operator << ( const axDBParam &p ) {
		axStatus st = append( p );	assert(st);
		return *this;
	}

	template<class T>
	axStatus io( T &v, const char* name ) {
		axDBParamList_io( *this, v );	return 0;
	}
};

inline void axDBParamList_io( axDBParamList & list, bool				v ) { axDBParam p( axDB_c_type_bool		);	p.p_bool	=v;	list<<(p); }
inline void axDBParamList_io( axDBParamList & list, float				v ) { axDBParam p( axDB_c_type_float	);	p.p_float	=v;	list<<(p); }
inline void axDBParamList_io( axDBParamList & list, double				v ) { axDBParam p( axDB_c_type_double	);	p.p_double	=v;	list<<(p); }
inline void axDBParamList_io( axDBParamList & list, int8_t				v ) { axDBParam p( axDB_c_type_int8		);	p.p_int8	=v;	list<<(p); }
inline void axDBParamList_io( axDBParamList & list, int16_t				v ) { axDBParam p( axDB_c_type_int16	);	p.p_int16	=v;	list<<(p); }
inline void axDBParamList_io( axDBParamList & list, int32_t				v ) { axDBParam p( axDB_c_type_int32	);	p.p_int32	=v;	list<<(p); }
inline void axDBParamList_io( axDBParamList & list, int64_t				v ) { axDBParam p( axDB_c_type_int64	);	p.p_int64	=v;	list<<(p); }

inline void axDBParamList_io( axDBParamList & list, const char*			v ) { axDBParam p( axDB_c_type_StringA );	p.p_strA	=v;	list<<(p); }
inline void axDBParamList_io( axDBParamList & list, const axStringA  &	v ) { axDBParam p( axDB_c_type_StringA );	p.p_strA	=v;	list<<(p); }
inline void axDBParamList_io( axDBParamList & list, const axIStringA &	v ) { axDBParam p( axDB_c_type_StringA );	p.p_strA	=v;	list<<(p); }

inline void axDBParamList_io( axDBParamList & list, const wchar_t*		v ) { axDBParam p( axDB_c_type_StringW );	p.p_strW	=v;	list<<(p); }
inline void axDBParamList_io( axDBParamList & list, const axStringW  &	v ) { axDBParam p( axDB_c_type_StringW );	p.p_strW	=v;	list<<(p); }
inline void axDBParamList_io( axDBParamList & list, const axIStringW &	v ) { axDBParam p( axDB_c_type_StringW );	p.p_strW	=v;	list<<(p); }

inline void axDBParamList_io( axDBParamList & list, const axIByteArray & v ) { axDBParam p( axDB_c_type_ByteArray ); p.p_ByteArray =&v; list<<(p); }
inline void axDBParamList_io( axDBParamList & list, const axByteArray  & v ) { axDBParam p( axDB_c_type_ByteArray ); p.p_ByteArray =&v; list<<(p); }

inline void axDBParamList_io( axDBParamList & list, const axTimeStamp  & v ) { axDBParam p( axDB_c_type_TimeStamp ); p.p_TimeStamp =&v; list<<(p); }
inline void axDBParamList_io( axDBParamList & list, const axDateTime   & v ) { axDBParam p( axDB_c_type_DateTime  ); p.p_DateTime  =&v; list<<(p); }



template<class T> inline
void axDBParamList_io( axDBParamList & list, const T & v ) {
	ax_const_cast(v).serialize_io( list );
}

//================



class axDBValueList;
template<class T> void axDBValueList_io( axDBValueList & list, T & v );

typedef void (*axDBValueList_io_func)( axDBValueList &list, void* data );

template<class T> inline
void axDBValueList_io_func_T( axDBValueList & list, void* data ) {
	axDBValueList_io( list, *(T*) data );
}

class axDBValue_CB {
public:
	template<class T> axDBValue_CB( T &v ) {
		data = &v;
		func = axDBValueList_io_func_T<T>;
	}

	axDBValueList_io_func	func;
	void*					data;
};



#define axDB_c_type_list( NAME, C_TYPE, C_ITYPE ) \
	inline int axDBValueType( C_ITYPE &v ) { return axDB_c_type_##NAME; }
//----
	#include "axDB_c_type_list.h"
#undef axDB_c_type_list

//for SQL stmt output values
class axDBValue {
public:
	axDBValue( int _type = axDB_c_type_null, void* _data=NULL ) 
		: type(_type)
		, data(_data) 
	{}
	axStatus	onTake( axDBValue &src ) { *this=src; return 0; }
	
	int		type;
	void*	data;
};

class axDBValueList : public axArray< axDBValue, axDB_kArgListLocalBufSize > {
public:
	axDBValueList& operator << ( const axDBValue_CB & p ) {
		p.func( *this, p.data );
		return *this;
	}

	axDBValueList&	operator << ( const axDBValue &p ) {
		axStatus st;
		st = append( p );	assert(st);
		return *this;
	}

	template<class T>
	axStatus io( T &v, const char* name ) {
		axDBValueList_io( *this, v );	return 0;
	}
};


#define axDB_c_type_list( NAME, C_TYPE, C_ITYPE ) \
	inline void axDBValueList_io( axDBValueList & list, C_ITYPE & v ) { list.operator<<( axDBValue( axDB_c_type_##NAME, &v ) ); }
//----
	#include "axDB_c_type_list.h"
#undef axDB_c_type_list

inline void axDBValueList_io( axDBValueList & list, axStringA	& v ) { return axDBValueList_io(list, v.asInterface() ); }
inline void axDBValueList_io( axDBValueList & list, axStringW	& v ) { return axDBValueList_io(list, v.asInterface() ); }
inline void axDBValueList_io( axDBValueList & list, axByteArray & v ) { return axDBValueList_io(list, v.asInterface() ); }


template<class T> inline
void axDBValueList_io( axDBValueList & list, T & v ) {
	ax_const_cast(v).serialize_io( list );
}

class axDBColumn {
public:
	axDBColumn() 
		: type(axDB_c_type_null)
		, pkey(false)
		, data(NULL)
	{}
	
	axStatus	onTake( axDBColumn &src ) { 
		axStatus st;
		ax_take_macro( type );
		ax_take_macro( name );
        ax_take_macro( pkey );
        ax_take_macro( data );
		return 0; 
	}

	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("{?} {?} {?} {?}", type, name, data, pkey ? "pkey" : "" );
	}
	
	int					type;
	axStringA_<64>		name;
	bool				pkey;
    void*               data;
};

class axDBColumnList;
template<class T> axStatus	axDBColumnList_io( axDBColumnList &s, T & value, const char* name );

class axDBColumnList : public axArray< axDBColumn, axDB_kArgListLocalBufSize > {
	typedef	axArray< axDBColumn, axDB_kArgListLocalBufSize > B;
public:
	axDBColumnList() {
		prefix = NULL;
	}

	template<class T>
	axStatus	build( const char* pkey ) {
		T	t;
		axStatus st = io( t, NULL );		if( !st ) return st;
		axDBColumn* c = findColumnByName( pkey );
		if( c ) c->pkey = true;
		return 0;
	}

	template<class T, class PKey, PKey T::*PKeyMember >
	axStatus	_build() {
		T	t;
		axStatus st = io( t, NULL );		if( !st ) return st;
		axDBColumn* c = findColumnByData( &(t.*PKeyMember) );
		if( c ) c->pkey = true;
		return 0;
	}


	template<class T>
	axStatus	io	( T &value, const char* name ) {
		return axDBColumnList_io( *this, value, name );
	}

	template<class T>
	axStatus	_io	( T & value, const char* name ) {
		axStatus st;
		st = incSize(1);		if( !st ) return st;
		axDBColumn & c = last();

		if( prefix && prefix[0] ) {
			st = c.name.format("{?}_{?}", prefix, name );
		}else{
			st = c.name.set( name );
		}
		c.type = axDBValueType(value);
        c.data = &value;
		return 0;
	}

	axStatus	toStringFormat		( axStringFormat &f ) const;
    axDBColumn* findColumnByData	( void * p );
    axDBColumn* findColumnByName	( const char * p );

	const char* prefix;
};

#define axDB_c_type_list( NAME, C_TYPE, C_ITYPE ) \
	inline axStatus axDBColumnList_io( axDBColumnList &s, C_ITYPE & value, const char* name )	{ return s._io( value, name ); }
//----
	#include "axDB_c_type_list.h"
#undef axDB_c_type_list

inline axStatus axDBColumnList_io( axDBColumnList &s, axStringA				& value, const char* name )	{ return s._io( value.asInterface(), name ); }
inline axStatus axDBColumnList_io( axDBColumnList &s, axStringW				& value, const char* name )	{ return s._io( value.asInterface(), name ); }

template<size_t N> 
inline axStatus axDBColumnList_io( axDBColumnList &s, axStringA_<N>			& value, const char* name )	{ return s._io( value.asInterface(), name ); }
template<size_t N> 
inline axStatus axDBColumnList_io( axDBColumnList &s, axStringW_<N>			& value, const char* name )	{ return s._io( value.asInterface(), name ); }

template<size_t N> 
inline axStatus axDBColumnList_io( axDBColumnList &s, axLocalStringA<N>		& value, const char* name )	{ return s._io( value.asInterface(), name ); }
template<size_t N> 
inline axStatus axDBColumnList_io( axDBColumnList &s, axLocalStringW<N>		& value, const char* name )	{ return s._io( value.asInterface(), name ); }

inline axStatus axDBColumnList_io( axDBColumnList &s, axExternalStringA		& value, const char* name )	{ return s._io( value.asInterface(), name ); }
inline axStatus axDBColumnList_io( axDBColumnList &s, axExternalStringW		& value, const char* name )	{ return s._io( value.asInterface(), name ); }

inline axStatus axDBColumnList_io( axDBColumnList &s, axByteArray			& value, const char* name )	{ return s._io( value.asInterface(), name ); }
inline axStatus axDBColumnList_io( axDBColumnList &s, axExternalByteArray	& value, const char* name )	{ return s._io( value.asInterface(), name ); }

template<class T> inline //for user-define structure
axStatus	axDBColumnList_io( axDBColumnList &s, T & value, const char* name ) {
	axStatus st;
	axScopeValue<const char*>	old( s.prefix );

	axStringA_<64>	prefix;
	if( s.prefix && s.prefix[0] ) {
		st = prefix.format( "{?}_{?}", s.prefix, name );
	}else{
		st = prefix.set( name );
	}
	s.prefix = prefix.c_str();

	st = value.serialize_io( s );	if( !st ) return st;
	return 0;
}


#endif //__axDB_common_h__


