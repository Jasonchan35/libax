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


class axDB_ParamList;
template<class T> void axDB_ParamList_io( axDB_ParamList & list, const T & v );

typedef void (*axDB_ParamList_io_func)( axDB_ParamList &list, const void* data );

template<class T> inline
void axDB_ParamList_io_func_T( axDB_ParamList & list, const void* data ) {
	axDB_ParamList_io( list, *(const T*) data );
}

class axDBParam_CB {
public:
	template<class T> axDBParam_CB( const T &v ) {
		data = &v;
		func = axDB_ParamList_io_func_T<T>;
	}

	axDB_ParamList_io_func	func;
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

class axDB_ParamList : public axArray< axDBParam, axDB_kArgListLocalBufSize > {
public:
	axDB_ParamList&	operator << ( const axDBParam_CB &p ) {
		p.func( *this, p.data );
		return *this;
	}

	axDB_ParamList&	operator << ( const axDBParam &p ) {
		axStatus st = append( p );	assert(st);
		return *this;
	}

	template<class T>
	axStatus io( T &v, const char* name ) {
		axDB_ParamList_io( *this, v );	return 0;
	}
};

inline void axDB_ParamList_io( axDB_ParamList & list, bool		v ) { axDBParam p; p.type=axDB_c_type_bool;		p.bool_		=v;	list.operator<<(p); }
inline void axDB_ParamList_io( axDB_ParamList & list, float		v ) { axDBParam p; p.type=axDB_c_type_float;	p.float_	=v;	list.operator<<(p); }
inline void axDB_ParamList_io( axDB_ParamList & list, double	v ) { axDBParam p; p.type=axDB_c_type_double;	p.double_	=v;	list.operator<<(p); }
inline void axDB_ParamList_io( axDB_ParamList & list, int8_t	v ) { axDBParam p; p.type=axDB_c_type_int8_t;	p.int8_		=v;	list.operator<<(p); }
inline void axDB_ParamList_io( axDB_ParamList & list, int16_t	v ) { axDBParam p; p.type=axDB_c_type_int16_t;	p.int16_	=v;	list.operator<<(p); }
inline void axDB_ParamList_io( axDB_ParamList & list, int32_t	v ) { axDBParam p; p.type=axDB_c_type_int32_t;	p.int32_	=v;	list.operator<<(p); }
inline void axDB_ParamList_io( axDB_ParamList & list, int64_t	v ) { axDBParam p; p.type=axDB_c_type_int64_t;	p.int64_	=v;	list.operator<<(p); }

inline void axDB_ParamList_io( axDB_ParamList & list, const char*	 v ) { axDBParam p; p.type=axDB_c_type_axIStringA;		p.strA=v;	list.operator<<(p); }
inline void axDB_ParamList_io( axDB_ParamList & list, const wchar_t* v ) { axDBParam p; p.type=axDB_c_type_axIStringW;		p.strW=v;	list.operator<<(p); }

inline void axDB_ParamList_io( axDB_ParamList & list, axIByteArray	& v ) { axDBParam p; p.type=axDB_c_type_axIByteArray;	p.p_byteArray =&v; list.operator<<(p); }

inline void axDB_ParamList_io( axDB_ParamList & list, axTimeStamp	& v ) { axDBParam p; p.type=axDB_c_type_axTimeStamp;	p.p_timeStamp=&v; list.operator<<(p); }
inline void axDB_ParamList_io( axDB_ParamList & list, axDateTime	& v ) { axDBParam p; p.type=axDB_c_type_axDateTime;		p.p_dateTime =&v; list.operator<<(p); }


template<class T> inline
void axDB_ParamList_io( axDB_ParamList & list, const T & v ) {
	ax_const_cast(v).serialize_io( list );
}

//================



class axDB_ValueList;
template<class T> void axDB_ValueList_io( axDB_ValueList & list, T & v );

typedef void (*axDB_ValueList_io_func)( axDB_ValueList &list, void* data );

template<class T> inline
void axDB_ValueList_io_func_T( axDB_ValueList & list, void* data ) {
	axDB_ValueList_io( list, *(T*) data );
}

class axDBValue_CB {
public:
	template<class T> axDBValue_CB( T &v ) {
		data = &v;
		func = axDB_ValueList_io_func_T<T>;
	}

	axDB_ValueList_io_func	func;
	void*					data;
};



#define axDB_c_type_list( T ) \
	inline int axDB_ValueType( T &v) { return axDB_c_type_##T; }
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

class axDB_ValueList : public axArray< axDBValue, axDB_kArgListLocalBufSize > {
public:
	axDB_ValueList& operator << ( const axDBValue_CB & p ) {
		p.func( *this, p.data );
		return *this;
	}

	axDB_ValueList&	operator << ( const axDBValue &p ) {
		axStatus st;
		st = append( p );	assert(st);
		return *this;
	}

	template<class T>
	axStatus io( T &v, const char* name ) {
		axDB_ValueList_io( *this, v );	return 0;
	}
};


#define axDB_c_type_list( T ) \
	inline void axDB_ValueList_io( axDB_ValueList & list, T & v ) { list.operator<<( axDBValue( axDB_c_type_##T, &v ) ); }
//----
	#include "axDB_c_type_list.h"
#undef axDB_c_type_list

inline void axDB_ValueList_io( axDB_ValueList & list, axStringA & v ) { return axDB_ValueList_io(list, (axIStringA &)v); }
inline void axDB_ValueList_io( axDB_ValueList & list, axStringW & v ) { return axDB_ValueList_io(list, (axIStringW &)v); }


template<class T> inline
void axDB_ValueList_io( axDB_ValueList & list, T & v ) {
	ax_const_cast(v).serialize_io( list );
}






class axDB_Column {
public:
	axDB_Column() { type = axDB_c_type_null; }
	
	axStatus	onTake( axDB_Column &src ) { 
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

class axDB_ColumnList;
template<class T> axStatus	axDB_ColumnList_io( axDB_ColumnList &s, T & value, const char* name );

class axDB_ColumnList : public axArray< axDB_Column, axDB_kArgListLocalBufSize > {
	typedef	axArray< axDB_Column, axDB_kArgListLocalBufSize > B;
public:
	axDB_ColumnList() {
		prefix = NULL;
	}

	template<class T>
	axStatus	io	( T &value, const char* name ) {
		return axDB_ColumnList_io( *this, value, name );
	}

	template<class T>
	axStatus	_io	( T & value, const char* name ) {
		axStatus st;
		st = incSize(1);		if( !st ) return st;
		axDB_Column & c = last();

		if( prefix && prefix[0] ) {
			st = c.name.format("{?}_{?}", prefix, name );
		}else{
			st = c.name.set( name );
		}
		c.type = axDB_ValueType(value);
		return 0;
	}

	axStatus	toStringFormat( axStringFormat &f ) const {
		return B::toStringFormat(f);
	}

	const char* prefix;
};

#define axDB_c_type_list( T ) \
	template<> inline axStatus axDB_ColumnList_io( axDB_ColumnList &s, T & value, const char* name )	{ return s._io( value, name ); }
//----
	#include "axDB_c_type_list.h"
#undef axDB_c_type_list

template<class T> inline //for user-define structure
axStatus	axDB_ColumnList_io( axDB_ColumnList &s, T & value, const char* name ) {
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


