#ifndef __axDBColumn_h__
#define __axDBColumn_h__

#include "axDB_common.h"
#include "axDBOutParam.h"


class axDBColumn {
public:
	axDBColumn() 
		: type(axDB_c_type_null)
		, data(NULL)
	{}
	
	axStatus	onTake( axDBColumn &src ) { 
		axStatus st;
		ax_take_macro( type );
		ax_take_macro( name );
        ax_take_macro( data );
		return 0; 
	}

	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("{?} {?} {?}", type, name, data );
	}
	
	int				type;
	axStringA		name;
	const void*		data;
};

class axDBColumnList;
template<class T> axStatus	axDBColumnList_io( axDBColumnList &s, T & value, const char* name );

class axDBColumnList : public axArray< axDBColumn, axDB_kArgListLocalBufSize > {
	typedef	axArray< axDBColumn, axDB_kArgListLocalBufSize > B;
public:
	axDBColumnList() {
		_tmpPrefix		= NULL;
		pkeyIndex_		= -1;
		pkeyAutoInc_	= false;
	}

	template<class T>
	axStatus	create() {
		return create<T, void, NULL, false>();
	}

	template<class T, class PKeyType, PKeyType T::*PKeyMember, bool PKeyAutoInc >
	axStatus	create() {
		pkeyIndex_	 = -1;
		pkeyAutoInc_ = PKeyAutoInc;

		T	t;
		axStatus st = io( t, NULL );			if( !st ) return st;
		pkeyIndex_ = findColumnByData( &(t.*PKeyMember) );
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

		if( _tmpPrefix && _tmpPrefix[0] ) {
			st = c.name.format("{?}_{?}", _tmpPrefix, name );	if( !st ) return st;
		}else{
			st = c.name.set( name );	if( !st ) return st;
		}
		c.type = axDBOutParamType(value);
		c.data = &value;
		return 0;
	}

			axStatus	toStringFormat		( axStringFormat &f ) const;
			size_t		findColumnByName	( const char* name );
			size_t		findColumnByData	( const void* data );

	const	axDBColumn*	pkeyColumn			() const	{ return inBound( pkeyIndex_ ) ? &at(pkeyIndex_) : NULL; }
			size_t		pkeyIndex			() const	{ return pkeyIndex_;	}
			bool		pkeyAutoInc			() const	{ return pkeyAutoInc_;	}

	const char* _tmpPrefix;

private:
	size_t		pkeyIndex_;
	bool		pkeyAutoInc_;
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
	axScopeValue<const char*>	old( s._tmpPrefix );

	axStringA_<128>	tmp;

	if( s._tmpPrefix && s._tmpPrefix[0] ) {
		st = tmp.format( "{?}_{?}", s._tmpPrefix, name );		if( !st ) return st;
		s._tmpPrefix = tmp.c_str(); //change the prefix
	}else{
		s._tmpPrefix = name;
	}
	st = value.serialize_io( s );		if( !st ) return st;
	return 0;
}


inline
axStatus	axDBColumnList::toStringFormat( axStringFormat &f ) const {
	return B::toStringFormat(f);
}

inline
size_t axDBColumnList::findColumnByName ( const char* name ) {
	if( !name ) return -1;

    for( size_t i=0; i<size(); i++ ) {
		if( at(i).name.equalsNoCase( name ) ) return i;
    }
	return -1;
}

inline
size_t	axDBColumnList::findColumnByData ( const void* data )  {
	if( !data ) return -1;

    for( size_t i=0; i<size(); i++ ) {
		if( at(i).data == data ) return i;
    }
	return -1;
}




#endif // __axDBColumn_h__
