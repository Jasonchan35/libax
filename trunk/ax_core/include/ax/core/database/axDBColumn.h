#ifndef __axDBColumn_h__
#define __axDBColumn_h__

#include "axDB_common.h"


class axDBColumn {
public:
	axDBColumn() 
		: type(axDB_c_type_null)
		, pkey(false)
		, pkey_auto_inc(false)
		, data(NULL)
	{}
	
	axStatus	onTake( axDBColumn &src ) { 
		axStatus st;
		ax_take_macro( type );
		ax_take_macro( name );
        ax_take_macro( pkey );
        ax_take_macro( pkey_auto_inc );
        ax_take_macro( data );
		return 0; 
	}

	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("{?} {?} {?} {?}", type, name, data, pkey ? "pkey" : "" );
	}
	
	int					type;
	axStringA_<64>		name;
	bool				pkey;
	bool				pkey_auto_inc;
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
	axStatus	build( const char* pkey, bool pkey_auto_inc ) {
		T	t;
		axStatus st = io( t, NULL );		if( !st ) return st;
		axDBColumn* c = findColumnByName( pkey );
		if( c ) {
			c->pkey = true;
			if( pkey_auto_inc ) c->pkey_auto_inc = true;
		}
		return 0;
	}

	template<class T, class PKey, PKey T::*PKeyMember, bool pkey_auto_inc >
	axStatus	_build( axSize * outPKeyIndex ) {
		T	t;
		axStatus st = io( t, NULL );		if( !st ) return st;
		PKey & p = t.*PKeyMember;

		axDBColumn* c = findColumnByData( &p, outPKeyIndex );
		if( c ) {
			c->pkey = true;
			c->pkey_auto_inc = pkey_auto_inc;
		}
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
		c.type = axDBOutParamType(value);
        c.data = &value;
		return 0;
	}

	axStatus	toStringFormat		( axStringFormat &f ) const;
    axDBColumn* findColumnByData	(		void * p, axSize * outIndex = NULL );
    axDBColumn* findColumnByName	( const char * p, axSize * outIndex = NULL );

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


inline
axStatus	axDBColumnList::toStringFormat( axStringFormat &f ) const {
	return B::toStringFormat(f);
}

inline
axDBColumn* axDBColumnList::findColumnByData( void * p, axSize * outIndex ) {
    for( axSize i=0; i<size(); i++ ) {
        axDBColumn &c = at(i);
		if( c.data == p ) {
			if( outIndex ) *outIndex = i;
			return &c;
		}
    }
    return NULL;
}

inline
axDBColumn* axDBColumnList::findColumnByName( const char * p, axSize * outIndex ) {
	if( !p ) return NULL;

    for( axSize i=0; i<size(); i++ ) {
        axDBColumn &c = at(i);
		if( c.name.equals(p) ) {
			if( outIndex ) *outIndex = i;
			return &c;
		}
    }
    return NULL;
}



#endif // __axDBColumn_h__
