#ifndef __axDBInParam_h__
#define __axDBInParam_h__

#include "axDB_common.h"

class axDBInParamList;
template<class T> void axDBInParamList_io( axDBInParamList & list, const T & v );

typedef void (*axDBInParamList_io_func)( axDBInParamList &list, const void* data );

template<class T> inline
void axDBInParamList_io_func_T( axDBInParamList & list, const void* data ) {
	axDBInParamList_io( list, *(const T*) data );
}

class axDBInParam_CB {
public:
	template<class T> axDBInParam_CB( const T &v ) {
		data = &v;
		func = axDBInParamList_io_func_T<T>;
	}

	axDBInParamList_io_func	func;
		const void*			data;
};

//for SQL stmt input param
class axDBInParam {
public:
	axDBInParam( int _type = axDB_c_type_null ) : type( _type )
	{}

	union {
		bool				v_bool;
		int8_t				v_int8;
		int16_t				v_int16;
		int32_t				v_int32;
		int64_t				v_int64;
		float				v_float;
		double				v_double;
		const char*			v_strA;
		const wchar_t*		v_strW;
		double				v_TimeStamp;
		const axIByteArray* v_ByteArray;
	};

	axStatus	onTake( axDBInParam &src ) { *this=src; return 0; }

	axStatus	toStringFormat( axStringFormat & f ) const;
	int			type;
};

class axDBInParamList : public axArray< axDBInParam, axDB_kArgListLocalBufSize > {
public:
	axDBInParamList() : skipPkeyAtIndex_(-1) {}

	axDBInParamList&	operator << ( const axDBInParam_CB &p ) {
		p.func( *this, p.data );
		return *this;
	}

	axDBInParamList&	operator << ( const axDBInParam &p ) {
		if( skipPkeyAtIndex_ != curIndex ) {
			axStatus st;
			st = append( p );	assert(st);
		}
		curIndex++;
		return *this;
	}

	template<class T>
	axStatus build( T &v, size_t skipPkeyAtIndex = -1 ) {
		curIndex = 0;
		skipPkeyAtIndex_ = skipPkeyAtIndex;
		return io( v, NULL );
	}

	template<class T>
	axStatus io( T &v, const char* name ) {
		axDBInParamList_io( *this, v );	return 0;
	}

	axSize	curIndex;
	axSize	skipPkeyAtIndex_;
};

inline void axDBInParamList_io( axDBInParamList & list, bool				v ) { axDBInParam p( axDB_c_type_bool		);	p.v_bool	=v;	list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, float				v ) { axDBInParam p( axDB_c_type_float	);	p.v_float	=v;	list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, double				v ) { axDBInParam p( axDB_c_type_double	);	p.v_double	=v;	list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, int8_t				v ) { axDBInParam p( axDB_c_type_int8		);	p.v_int8	=v;	list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, int16_t				v ) { axDBInParam p( axDB_c_type_int16	);	p.v_int16	=v;	list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, int32_t				v ) { axDBInParam p( axDB_c_type_int32	);	p.v_int32	=v;	list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, int64_t				v ) { axDBInParam p( axDB_c_type_int64	);	p.v_int64	=v;	list<<(p); }

inline void axDBInParamList_io( axDBInParamList & list, const char*			v ) { axDBInParam p( axDB_c_type_StringA );	p.v_strA	=v;	list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, const axStringA  &	v ) { axDBInParam p( axDB_c_type_StringA );	p.v_strA	=v;	list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, const axIStringA &	v ) { axDBInParam p( axDB_c_type_StringA );	p.v_strA	=v;	list<<(p); }

inline void axDBInParamList_io( axDBInParamList & list, const wchar_t*		v ) { axDBInParam p( axDB_c_type_StringW );	p.v_strW	=v;	list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, const axStringW  &	v ) { axDBInParam p( axDB_c_type_StringW );	p.v_strW	=v;	list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, const axIStringW &	v ) { axDBInParam p( axDB_c_type_StringW );	p.v_strW	=v;	list<<(p); }

inline void axDBInParamList_io( axDBInParamList & list, const axIByteArray & v ) { axDBInParam p( axDB_c_type_ByteArray ); p.v_ByteArray =&v; list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, const axByteArray  & v ) { axDBInParam p( axDB_c_type_ByteArray ); p.v_ByteArray =&v; list<<(p); }

inline void axDBInParamList_io( axDBInParamList & list, axTimeStamp			 v ) { axDBInParam p( axDB_c_type_TimeStamp ); p.v_TimeStamp = v; list<<(p); }
inline void axDBInParamList_io( axDBInParamList & list, const axDateTime   & v ) { axDBInParamList_io( list, v.toTimeStamp() );  }



template<class T> inline
void axDBInParamList_io( axDBInParamList & list, const T & v ) {
	ax_const_cast(v).serialize_io( list );
}

inline
axStatus	axDBInParam::toStringFormat( axStringFormat & f ) const {
	f.format("\n    ({?})\t", axDB_c_type_name(type) );

	switch( type ) {
		case axDB_c_type_null:			return 0;
		case axDB_c_type_ByteArray:		return f.format("size={?}", v_ByteArray->size() );

		case axDB_c_type_TimeStamp:		return f.format("{?}", axTimeStamp( v_TimeStamp ) );

		case axDB_c_type_StringA:		return f.format("[{?}]", v_strA );
		case axDB_c_type_StringW:		return f.format("[{?}]", v_strW  );

		#define axTYPE_LIST( NAME ) \
			case axDB_c_type_##NAME: return f.format("{?}", v_##NAME ); \
		//-----
			axTYPE_LIST( bool )
			axTYPE_LIST( float )
			axTYPE_LIST( double )
			axTYPE_LIST( int8 )
			axTYPE_LIST( int16 )
			axTYPE_LIST( int32 )
			axTYPE_LIST( int64 )
		#undef axDB_c_type_list
	}
	assert(false);
	return f.out("Unknown");
}

#endif //__axDBInParam_h__
