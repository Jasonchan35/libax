#ifndef __axJsonWriter_h__
#define __axJsonWriter_h__
/*
** 3 method to support axJsonWriter ** 
1: serialize_io function in your class:

template< class S >
axStatus serialize_io( S &s ) {
	axStatus st;
	ax_io( v1 )
	ax_io( v2 )
	return 0;
}

2:  external class global function:

template<class S > inline 
axStatus	ax_json_serialize_object_members ( S &s, vector3 &v ) {	
axStatus st;
	st = s.io( v.x, "x" );	if( !st ) return st;
	st = s.io( v.y, "y" );	if( !st ) return st;
	st = s.io( v.z, "z" );	if( !st ) return st;
return 0;
}



3: inline global function : ( This function only support singale string input and output )

inline axStatus ax_json_serialize_value( axJsonParser &s, vector3 &value ) {
	axStatus st;
	axTempStringA str;
	st = s.getString( str ); if( !st ) return st;
	st = ax_str_to( str, value ); if( !st ) return st;
	return 0;
}

inline axStatus ax_json_serialize_value( axJsonWriter &s, vector3 &value ) {
	axStatus st;
	axTempStringA str;
	st = str.format( "{?} {?} {?}", value.x, value.y, value.z ); if( !st ) return st;
	return ax_json_serialize_value( s, str );
}
*/

#include "../data_structure/axIArray.h"
#include "../data_structure/axDList.h"
#include "../data_structure/axAutoPtr.h"
#include "../data_structure/axPtr.h"
#include "../string/axString.h"
#include "../string/axConstString.h"
#include "../string/axLocalString.h"
#include "../string/axTinyString.h"
#include "../string/ax_utf8.h"
#include "../file_system/axFileSystem.h"
#include "../other/axSerializer.h"

class axJsonWriter;
class axJsonParser;

class axJson {
	axJson(){}
public:
	template<class T> static	axStatus	encode 	( axIStringA &json, const T & v, bool condense=false, const char* indent="\t" );
	template<class T> static	axStatus	decode	( const char* json, T & v, axIStringA* errorLog=NULL );

	template<class T> static 	axStatus	saveFile ( const char* filename, const T & v, bool condense = false, const char* indent="\t" );
	template<class T> static	axStatus	loadFile ( const char* filename, T & v, axIStringA* errorLog=NULL );
};


template<class S, class T> inline	
axStatus ax_json_serialize_value( S &s, T& value );

axStatus	ax_to_json_str		( axIStringA & json, const char*    sz,	bool withQuote );
axStatus	ax_to_json_str		( axIStringA & json, const wchar_t* sz, bool withQuote );

axStatus	ax_from_json_str	( axIStringA & str,  const char* sz, bool withQuote );

class axJsonSerializerBase: public axSerializerBase {
public:
};

class axJsonWriter : public axJsonSerializerBase {
	typedef axJsonSerializerBase B;
public:
	template<class T>	axStatus io_vary( T& value, const char* name )	{ return io(value,name); }
	template<class T>	axStatus io		( T& value, const char* name )	{
		axStatus st;
		if( ! name|| ! name[0] ) return axStatus_Std::JsonParser_format_error;
		st = member( name );		if( !st ) return st;
		st = io_value( value );		if( !st ) return st;
		st = nextElement();			if( !st ) return st;
		return 0;
	}
	
	template<class T>	axStatus io_value( T &value ) {
		return ax_json_serialize_value( *this, value );
	}
	
		axJsonWriter( axIStringA &str, bool condense = true, const char* indent = " " );
		
		axStatus member( const char* name );
		
		axStatus beginObject	( const char* name );
		axStatus endObject 		();

		axStatus beginArray		( const char* name );
		axStatus endArray		();
		
		axStatus beginObjectValue	();
		axStatus endObjectValue 	();

		axStatus beginArrayValue	();
		axStatus endArrayValue		();

		axStatus newline			();
		
	template<class V>
		axStatus numberValue		( V v ) { return io_value(v); }
		axStatus stringValue		( const char* sz );
		axStatus nullValue			();
		
		axStatus write				( const char* sz );	
		axStatus nextElement		();
		
		axStatus writeRawElement( const char* name, const char* value );
		
		void	setCondense( bool b )	{ condense_ = b; }
		bool	isCondense() const		{ return condense_; }
	
	class Scope_JsonCondense : public axNonCopyable {
	public:
		Scope_JsonCondense	( axJsonWriter &w, bool b ) { bind(w,b); }
		~Scope_JsonCondense	() { unbind(); }

		void	bind		( axJsonWriter &w, bool b ) { unbind(); w_=&w; old=w_->isCondense(); w_->setCondense(b); }
		void 	unbind		() { if(w_) { w_->setCondense(old); w_=NULL; } }
		
	private:
		axPtr<axJsonWriter>	w_;
		bool				old;
	};	
	
private:
	axStatus begin_	( const char ch );
	axStatus end_	( const char begin, const char ch );

	axIStringA*		str_;
	bool 			condense_;
	axSize 			depth_;
	bool			ended_;
	axStringA_<8>	indent_;
};


class axJsonParser : public axJsonSerializerBase {
	typedef axJsonSerializerBase B;
public:

	axJsonParser( const char* json, axIStringA* errorLog );
	
	template<class T>	axStatus parse( T& value, const char* name ) {
		axStatus st;
		st = io( value, name );
		if( st.code() == axStatus_Std::JsonParser_member_found ) {
			st = 0;
		}
		
		if( st ) {
			if( checkToken(",") ) return nextToken();
		}
		return 0;
	}
	
	template<class T>	axStatus io_vary( T& value, const char* name )	{ return io(value,name); }	
	template<class T>	axStatus io		( T& value, const char* name )	{
		axStatus st;
		if( ! name ) return axStatus_Std::JsonParser_format_error;
		
		if( ! checkStringToken(name) ) {
			return 1;
		}
		
		st = nextToken();				if( !st ) return st;
		
		st = checkToken( ":" );
		if( !st ) {
			log("expect ':' after member name");
			return st;
		}
		
		st = nextToken();				if( !st ) return st;
		
		st = io_value( value );			if( !st ) return st;
		
		return axStatus_Std::JsonParser_member_found;
	}
	
	template<class T>	axStatus io_value( T& value ) {
		axStatus st;
		st = ax_json_serialize_value( *this, value );	
		if( !st ) return st;
		return 0;
	}
	
	axStatus	nextToken();	
	axStatus	_nextToken();
	axStatus	checkToken( const char* sz );
	axStatus	checkStringToken( const char* sz );
	
	axStatus	beginObject( const char* name );
	axStatus	endObject() ;

	axStatus	beginObjectValue();
	axStatus	endObjectValue();
	
	bool		isObjectEnded();

	axStatus	beginArray( const char* name );
	axStatus	endArray() ;
	
	axStatus	beginArrayValue();
	axStatus	endArrayValue() ;
	
	bool		isArrayEnded();

	axStatus	nextElement();
	
	axStatus	parseMember( const char* name );
	
	axStatus	getMemberName( axIStringA & out );
		
	axStatus 	skipValue();
	axStatus 	skipBlock( char open, char close );
	
	axSize		lineNo() { return lineNo_; }
	axSize		charNo() { return r_ - lineStart_; }
	
	axStatus	log				( const char* msg );
	axStatus	logIgnoreMember	( const char* memberName );
	
	axTempStringA		token;
	bool				tokenIsString;
	
private:
	axSize			lineNo_;
	const char*		start_;
	const char*		r_;
	const char*		lineStart_;
	axIStringA*		errorLog_;
};

//------------------

template<class T> inline axStatus ax_json_serialize_value_primitive( axJsonWriter &s, T &v ) {
	axStatus st;
	axTempStringA tmp;
	st = tmp.convert(v);	if( !st ) return st;
	st = s.write( tmp );	if( !st ) return st;
	return 0;

}

template<class T> inline axStatus ax_json_serialize_value_primitive( axJsonParser &s, T &v ) {
	axStatus st;
	if( s.tokenIsString )	return -1;
	st = ax_str_to( s.token, v );		if( !st ) return st;
	st = s.nextToken();					if( !st ) return st;
	return 0;
}

#define axTYPE_LIST(T) \
	template< class S > inline axStatus ax_json_serialize_value( S &s, T &v ) { return ax_json_serialize_value_primitive( s, v ); }
axTYPE_LIST( int8_t )
axTYPE_LIST( int16_t )
axTYPE_LIST( int32_t )
axTYPE_LIST( int64_t )
axTYPE_LIST( uint8_t )
axTYPE_LIST( uint16_t )
axTYPE_LIST( uint32_t )
axTYPE_LIST( uint64_t )
axTYPE_LIST( float )
axTYPE_LIST( double )
axTYPE_LIST( axSize )
#undef axTYPE_LIST

//------ bool -------
template<> axStatus ax_json_serialize_value( axJsonWriter &s, bool &v );
template<> axStatus ax_json_serialize_value( axJsonParser &s, bool &v );

//----------- DList ----------------

template<class T> inline 
axStatus	ax_json_serialize_value ( axJsonWriter &s, axDList<T> &v ) {
	axStatus st;
	st = s.beginArrayValue();		if( !st ) return st;

	T *p = v.head();
	for( ; p; p=p->next() )  {
		st = s.io_value( *p );		if( !st ) return st;
		st = s.write( "," );		if( !st ) return st;
	}

	st = s.endArrayValue();			if( !st ) return st;
	return 0 ;
}


template<class T> inline 
axStatus	ax_json_serialize_value ( axJsonParser &s, axDList<T> &v ) {	
	axStatus st;

	st = s.beginArrayValue();			if( !st ) return st;
	if( ! s.checkToken("]") ) {
		for(;;) {
			axAutoPtr<T> p(st);			if( !st ) return st;
			st = s.io_value( *p );		if( !st ) return st;
			v.append( p.unref() );
			
			if( s.checkToken("]") ) break;	
			st = s.nextElement();		if( !st ) return st;
		}
	}
	st = s.endArrayValue();				if( !st ) return st;
	return 0;
}

//------ Array -------
template< class T > inline
axStatus ax_json_serialize_value_array( axJsonWriter &s, T *p, size_t n ) {
	axStatus st;
	st = s.beginArrayValue();			if( !st ) return st;
	for(size_t i=0; i<n; i++ ) {
		st = s.io_value( p[i] );		if( !st ) return st;
		st = s.nextElement();			if( !st ) return st;
	}	
	st = s.endArrayValue();				if( !st ) return st;
	return 0;
}

template< class T > inline
axStatus ax_json_serialize_value( axJsonWriter &s, axIArray<T> &v ) {
	return ax_json_serialize_value_array( s, v.ptr(), v.size() );
}

//fixed array
template< class T > inline
axStatus ax_json_serialize_value_array( axJsonParser &s, T *p, size_t n ) {
	axStatus st;
	st = s.beginArrayValue();			if( !st ) return st;
	for(size_t i=0; i<n; i++ ) {
		if( i > 0 ) {
			st = s.nextElement();			if( !st ) return st;
		}
		st = s.io_value( p[i] );		if( !st ) return st;
	}	
	st = s.endArrayValue();				if( !st ) return st;
	return 0;
}

template< class T > inline
axStatus ax_json_serialize_value( axJsonParser &s, axIArray<T> &v ) {
	axStatus st;
	v.resize(0);
	st = s.beginArrayValue();				if( !st ) return st;
	while( ! s.isArrayEnded() ) {
		st = v.incSize( 1 );				if( !st ) return st;
		st = s.io_value( v.last() );		if( !st ) return st;
		
		st = s.nextElement();				if( !st ) return st;
	}
	st = s.endArrayValue();					if( !st ) return st;
	return 0;
}

template< class S, class T > inline
axStatus ax_json_serialize_value( S &s, axExternalArray<T> &v ) {
	return ax_json_serialize_value( s, v.asInterface() );
}
template< class S, class T, size_t N > inline
axStatus ax_json_serialize_value( S &s, axLocalArray<T,N> &v ) {
	return ax_json_serialize_value( s, v.asInterface() );
}

template< class S, class T > inline
axStatus ax_json_serialize_value( S &s, axArray<T> &v ) {
	return ax_json_serialize_value( s, v.asInterface() );
}

template< class S, class T, size_t LOCAL_BUF_SIZE > inline
axStatus ax_json_serialize_value( S &s, axArray<T,LOCAL_BUF_SIZE> &v ) {
	return ax_json_serialize_value( s, v.asInterface() );
}


template< class S, class T, size_t CS, size_t LB > inline
axStatus ax_json_serialize_value( S &s, axChunkArray<T,CS,LB> &v ) {
	return ax_json_serialize_value( s, v.asInterface() );
}

//-------- axAutoPtr ------------

template <class T> inline
axStatus ax_json_serialize_value( axJsonWriter &s, axAutoPtr<T> &v ) {
	if(v) return s.io_value( *v );
	return s.nullValue();
}

template <class T> inline
axStatus ax_json_serialize_value( axJsonParser &s, axAutoPtr<T> &v ) {
	axStatus st = v.newObject();	if( !st ) return st;
	return s.io_value( *v );
}

//---------- String ----------------


template< class T > inline
axStatus ax_json_serialize_value( axJsonWriter &s, axIString_<T> &v ) {
	axStatus st;
	axTempStringA str;
	st = ax_to_json_str( str, v, true );		if( !st ) return st;	
	return s.write( str );
}


template< class T > inline
axStatus ax_json_serialize_value( axJsonParser &s, axIString_<T> &v ) {
	axStatus st;
	if( ! s.tokenIsString ) return -1;
	st = v.set( s.token );		if( !st ) return st;
	st = s.nextToken();			if( !st ) return st;
	return 0;
}

template< class S, class T, size_t N > inline
axStatus ax_json_serialize_value( S &s, axString_<T,N> &v ) { return ax_json_serialize_value( s, v.asInterface() ); }

template< class S, size_t N > inline
axStatus ax_json_serialize_value( S &s, axStringA_<N>  &v ) { return ax_json_serialize_value( s, v.asInterface() ); }

template< class S, size_t N > inline
axStatus ax_json_serialize_value( S &s, axStringW_<N>  &v ) { return ax_json_serialize_value( s, v.asInterface() ); }

template< class S, class T, size_t N > inline
axStatus ax_json_serialize_value( S &s, axLocalString_<T,N> 	&v ) { return ax_json_serialize_value( s, v.asInterface() ); }

template< class S, size_t N > inline
axStatus ax_json_serialize_value( S &s, axLocalStringA<N> 		&v ) { return ax_json_serialize_value( s, v.asInterface() ); }

template< class S, size_t N > inline
axStatus ax_json_serialize_value( S &s, axLocalStringW<N> 		&v ) { return ax_json_serialize_value( s, v.asInterface() ); }

template< class S, class T > inline
axStatus ax_json_serialize_value( S &s, axConstString_<T> 		&v ) {	return ax_json_serialize_value( s, v.asInterface() ); }

template< class S, class T > inline
axStatus ax_json_serialize_value( S &s, axExternalString_<T> 	&v ) { return ax_json_serialize_value( s, v.asInterface() ); }


//===============

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJsonWriter &s, axTinyString_<T,LOCAL_BUF_SIZE> &v ) {
	axStatus st;
	axTempStringA str;
	st = ax_to_json_str( str, v, true );		if( !st ) return st;	
	return s.write( str );
}


template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJsonParser &s, axTinyString_<T,LOCAL_BUF_SIZE> &v ) {
	axStatus st;
	if( ! s.tokenIsString ) return -1;
	st = v.set( s.token );	if( !st ) return st;
	st = s.nextToken();		if( !st ) return st;
	return 0;	
}

template<size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJsonWriter &s, axTinyStringA< LOCAL_BUF_SIZE > &v ) {
	return ax_json_serialize_value( s, (axTinyString_< char, LOCAL_BUF_SIZE > & ) v );
}

template<size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJsonParser &s, axTinyStringA< LOCAL_BUF_SIZE > &v ) {
	return ax_json_serialize_value( s, (axTinyString_< char, LOCAL_BUF_SIZE > & ) v );
}

template<size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJsonWriter &s, axTinyStringW< LOCAL_BUF_SIZE > &v ) {
	return ax_json_serialize_value( s, (axTinyString_< wchar_t, LOCAL_BUF_SIZE > & ) v );
}

template<size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJsonParser &s, axTinyStringW< LOCAL_BUF_SIZE > &v ) {
	return ax_json_serialize_value( s, (axTinyString_< wchar_t, LOCAL_BUF_SIZE > & ) v );
}


// ================================

template<class S, class T> inline	
axStatus ax_json_serialize_object_members( S &s, T &v ) { 
	return v.serialize_io( s ); 
}

template<class S, class T> inline	
axStatus ax_json_serialize_value( S &s, T& value ) { 
	axStatus st;
	st = s.beginObjectValue();							if( !st ) return st;
	st = ax_json_serialize_object_value( s, value );	if( !st ) return st;
	st = s.endObjectValue();							if( !st ) return st;	
	return 0;
}

template< class T> inline	
axStatus ax_json_serialize_object_value( axJsonWriter &s, T &value ) {
	return ax_json_serialize_object_members( s, value );
}

template< class T> inline	
axStatus ax_json_serialize_object_value( axJsonParser &s, T &value ) {
	axStatus st;
	axTempStringA tmp;
	if( s.checkToken("}") ) return 0;
	
	for(;;) {
		st = ax_json_serialize_object_members( s, value );
		if( st.code() == axStatus_Std::JsonParser_member_found ) {
			if( s.checkToken("}") ) break;
			if( s.checkToken(",") ) {
				st = s.nextToken();		if( !st ) return st;
				continue;		
			}
			s.log("expect ',' or '}'");
			return axStatus_Std::JsonParser_expect_comma_or_close_brucket;
		}			
		
		if( s.checkToken("}") ) break;

		st = s.getMemberName( tmp );	if( !st ) return st;
		st = s.logIgnoreMember( tmp );	if( !st ) return st;
		
		st = s.skipValue();				if( !st ) return st;
		
		if( s.checkToken(",") ) {
			st = s.nextToken();		if( !st ) return st;
		}
	}
	return 0;
}


template<class T> inline
axStatus	axJson::encode ( axIStringA &json, const T & v, bool condense, const char* indent ) {
	axJsonWriter	s( json, condense, indent );
	return s.io_value( ax_const_cast(v) );
}

template<class T> inline
axStatus	axJson::decode ( const char* json, T & v, axIStringA* errorLog ) {
	axJsonParser	s( json, errorLog );
	return s.io_value(v);
}

template<class T> inline
axStatus		axJson::saveFile ( const char* filename, const T & v, bool condense, const char* indent ) {
	axStatus st;
	axTempStringA	json;
	st = axJson::encode( json, v, condense, indent );	if( !st ) return st;
	st = axFileSystem::saveFile( json, filename );		if( !st ) return st;
	return 0;
}

template<class T> inline
axStatus		axJson::loadFile ( const char* filename, T & v, axIStringA* errorLog ) {
	axStatus st;
	axTempStringA	json;
	st = axFileSystem::loadFile( json, filename );		if( !st ) return st;
	st = axJson::decode( json, v, errorLog );
	if( !st ) return st;
	return 0;
}


#endif //__axJsonWriter_h__
