//
//  axDB_common.h
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDB_common_h__
#define __axDB_common_h__

#include "../../ax_base.h"
#include "../data_structure/axSharedPtr.h"

enum {
	axDB_c_type_null = 0,
	axDB_c_type_bool,
	axDB_c_type_int8,
	axDB_c_type_int16,
	axDB_c_type_int32,
	axDB_c_type_int64,
	axDB_c_type_float,
	axDB_c_type_double,
	axDB_c_type_StringA,
	axDB_c_type_StringW,
	axDB_c_type_ByteArray,
	axDB_c_type_TimeStamp,
	axDB_c_type_DateTime,
};

class axDB_Param {
public:
	axDB_Param() { type = axDB_c_type_null; }
	
	template<class T> axDB_Param( const T &v );
	axStatus	onTake( axDB_Param &src ) { operator=( src ); return 0; }
	
	union {
		bool			bool_;
		int8_t			int8_;
		int16_t			int16_;
		int32_t			int32_;
		int64_t			int64_;
		float			float_;
		double			double_;
		const char*		strA;
		const wchar_t*	strW;
		double			timestamp_;
		const void*		ptr;
	};
	int	type;
};

inline void axDB_ParamType( axDB_Param &p, const bool			&v) { p.bool_	= v; p.type = axDB_c_type_bool; }
inline void axDB_ParamType( axDB_Param &p, const float			&v) { p.float_	= v; p.type = axDB_c_type_float; }
inline void axDB_ParamType( axDB_Param &p, const double			&v) { p.double_	= v; p.type = axDB_c_type_double; }

inline void axDB_ParamType( axDB_Param &p, const int8_t			&v) { p.int8_	= v; p.type = axDB_c_type_int8; }
inline void axDB_ParamType( axDB_Param &p, const int16_t		&v) { p.int16_	= v; p.type = axDB_c_type_int16; }
inline void axDB_ParamType( axDB_Param &p, const int32_t		&v) { p.int32_	= v; p.type = axDB_c_type_int32; }
inline void axDB_ParamType( axDB_Param &p, const int64_t		&v) { p.int64_	= v; p.type = axDB_c_type_int64; }

inline void axDB_ParamType( axDB_Param &p, const char*			 v) { p.strA	= v; p.type = axDB_c_type_StringA; }
inline void axDB_ParamType( axDB_Param &p, const wchar_t*		 v) { p.strW	= v; p.type = axDB_c_type_StringW; }

inline void axDB_ParamType( axDB_Param &p, const axIByteArray	&v) { p.ptr		= &v; p.type = axDB_c_type_ByteArray; }
inline void axDB_ParamType( axDB_Param &p, const axTimeStamp	&v) { p.timestamp_ = v; p.type = axDB_c_type_TimeStamp; }
inline void axDB_ParamType( axDB_Param &p, const axDateTime		&v) { p.timestamp_ = v.toTimeStamp(); p.type = axDB_c_type_TimeStamp; }

template<class T> axDB_Param::axDB_Param( const T &v ) { axDB_ParamType( *this, v ); }



class axDB_ParamList : public axArray< axDB_Param, 32 > {
public:
	axDB_ParamList() { setCapacityIncrement( 32 ); }
	axDB_ParamList&	operator << ( const axDB_Param &p ) {
		axStatus st;
		st = append( p );	assert(st);
		return *this;
	}
};

//================

inline int axDB_ValueType( bool			&v) { return axDB_c_type_bool; }
inline int axDB_ValueType( float		&v) { return axDB_c_type_float; }
inline int axDB_ValueType( double		&v) { return axDB_c_type_double; }

inline int axDB_ValueType( int8_t		&v) { return axDB_c_type_int8; }
inline int axDB_ValueType( int16_t		&v) { return axDB_c_type_int16; }
inline int axDB_ValueType( int32_t		&v) { return axDB_c_type_int32; }
inline int axDB_ValueType( int64_t		&v) { return axDB_c_type_int64; }

inline int axDB_ValueType( axIStringA	&v) { return axDB_c_type_StringA; }
inline int axDB_ValueType( axIStringW	&v) { return axDB_c_type_StringW; }

inline int axDB_ValueType( axIByteArray	&v) { return axDB_c_type_ByteArray; }
inline int axDB_ValueType( axTimeStamp	&v) { return axDB_c_type_TimeStamp; }

inline int axDB_ValueType( axDateTime	&v) { return axDB_c_type_DateTime; }

class axDB_Value {
public:
	axDB_Value() { data_=NULL; type_ = axDB_c_type_null; }
	
	template<class T> 
	axDB_Value( T &v )	{ 
		data_ = &v;
		type_ = axDB_ValueType( v );
	}
	axStatus	onTake( axDB_Value &src ) { operator=( src ); return 0; }
	
	void*	data() const { return data_; }
	int		type() const { return type_; }
	
private:
	void*	data_;
	int		type_;
};

class axDB_ValueList : public axArray< axDB_Value, 32 > {
public:
	axDB_ValueList() { setCapacityIncrement( 32 ); }
	axDB_ValueList&	operator << ( const axDB_Value &p ) {
		axStatus st;
		st = append( p );	assert(st);
		return *this;
	}
};


#endif //__axDB_common_h__


