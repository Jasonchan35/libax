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
#define axDB_c_type_list( T ) \
	axDB_c_type_##T,
//----
	#include "axDB_c_type_list.h"
#undef axDB_c_type_list
};

const char* axDB_c_type_name( int type );

const size_t	axDB_kArgListLocalBufSize = 32;
const size_t	axDB_kRowIdType	= axDB_c_type_int64_t;


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

class axDBParam {
public:
	axDBParam() { type = axDB_c_type_null; }
	axStatus	onTake( axDBParam &src ) { operator=( src ); return 0; }
	
	union {
		bool				bool_;
		int8_t				int8_;
		int16_t				int16_;
		int32_t				int32_;
		int64_t				int64_;
		float				float_;
		double				double_;
		const char*			strA;
		const wchar_t*		strW;
		const axTimeStamp*	p_timeStamp;
		const axDateTime*	p_dateTime;
		const axIByteArray* p_byteArray;
	};
	axStatus	toStringFormat( axStringFormat & f ) const;
	int	type;
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

inline void axDBParamList_io( axDBParamList & list, bool		v ) { axDBParam p; p.type=axDB_c_type_bool;		p.bool_		=v;	list.operator<<(p); }
inline void axDBParamList_io( axDBParamList & list, float		v ) { axDBParam p; p.type=axDB_c_type_float;	p.float_	=v;	list.operator<<(p); }
inline void axDBParamList_io( axDBParamList & list, double	v ) { axDBParam p; p.type=axDB_c_type_double;	p.double_	=v;	list.operator<<(p); }
inline void axDBParamList_io( axDBParamList & list, int8_t	v ) { axDBParam p; p.type=axDB_c_type_int8_t;	p.int8_		=v;	list.operator<<(p); }
inline void axDBParamList_io( axDBParamList & list, int16_t	v ) { axDBParam p; p.type=axDB_c_type_int16_t;	p.int16_	=v;	list.operator<<(p); }
inline void axDBParamList_io( axDBParamList & list, int32_t	v ) { axDBParam p; p.type=axDB_c_type_int32_t;	p.int32_	=v;	list.operator<<(p); }
inline void axDBParamList_io( axDBParamList & list, int64_t	v ) { axDBParam p; p.type=axDB_c_type_int64_t;	p.int64_	=v;	list.operator<<(p); }

inline void axDBParamList_io( axDBParamList & list, const char*	 v ) { axDBParam p; p.type=axDB_c_type_axIStringA;		p.strA=v;	list.operator<<(p); }
inline void axDBParamList_io( axDBParamList & list, const wchar_t* v ) { axDBParam p; p.type=axDB_c_type_axIStringW;		p.strW=v;	list.operator<<(p); }

inline void axDBParamList_io( axDBParamList & list, axIByteArray	& v ) { axDBParam p; p.type=axDB_c_type_axIByteArray;	p.p_byteArray =&v; list.operator<<(p); }

inline void axDBParamList_io( axDBParamList & list, axTimeStamp	& v ) { axDBParam p; p.type=axDB_c_type_axTimeStamp;	p.p_timeStamp=&v; list.operator<<(p); }
inline void axDBParamList_io( axDBParamList & list, axDateTime	& v ) { axDBParam p; p.type=axDB_c_type_axDateTime;		p.p_dateTime =&v; list.operator<<(p); }


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



#define axDB_c_type_list( T ) \
	inline int axDBValueType( T &v) { return axDB_c_type_##T; }
//----
	#include "axDB_c_type_list.h"
#undef axDB_c_type_list

class axDBValue {
public:
	axDBValue( int _type = axDB_c_type_null, void* _data=NULL ) : type(_type), data(_data) {}
	
	axStatus	onTake( axDBValue &src ) { operator=( src ); return 0; }
	
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


#define axDB_c_type_list( T ) \
	inline void axDBValueList_io( axDBValueList & list, T & v ) { list.operator<<( axDBValue( axDB_c_type_##T, &v ) ); }
//----
	#include "axDB_c_type_list.h"
#undef axDB_c_type_list

inline void axDBValueList_io( axDBValueList & list, axStringA & v ) { return axDBValueList_io(list, (axIStringA &)v); }
inline void axDBValueList_io( axDBValueList & list, axStringW & v ) { return axDBValueList_io(list, (axIStringW &)v); }


template<class T> inline
void axDBValueList_io( axDBValueList & list, T & v ) {
	ax_const_cast(v).serialize_io( list );
}

class axDBColumn {
public:
	axDBColumn() { type = axDB_c_type_null; }
	
	axStatus	onTake( axDBColumn &src ) { 
		axStatus st;
		ax_take_macro( type );
		ax_take_macro( name );
		return 0; 
	}

	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("{?} {?}", type, name );
	}
	
	int					type;
	axStringA_<64>		name;
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
		return 0;
	}

	axStatus	toStringFormat( axStringFormat &f ) const {
		return B::toStringFormat(f);
	}

	const char* prefix;
};

#define axDB_c_type_list( T ) \
	template<> inline axStatus axDBColumnList_io( axDBColumnList &s, T & value, const char* name )	{ return s._io( value, name ); }
//----
	#include "axDB_c_type_list.h"
#undef axDB_c_type_list

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


