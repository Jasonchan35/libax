#ifndef __axJSONSerializer_h__
#define __axJSONSerializer_h__
/*
** 3 method to support axJSONSerializer ** 
1: onStringSerialize function in your class:

template< class S >
axStatus onStringSerialize( S &s ) {
	axStatus st;
	ax_string_serialize( v1 )
	ax_string_serialize( v2 )
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

inline axStatus ax_json_serialize_value( axJSONDeserializer &s, vector3 &value ) {
	axStatus st;
	axTempStringA str;
	st = s.getString( str ); if( !st ) return st;
	st = ax_str_to( str, value ); if( !st ) return st;
	return 0;
}

inline axStatus ax_json_serialize_value( axJSONSerializer &s, vector3 &value ) {
	axStatus st;
	axTempStringA str;
	st = str.format( "{?} {?} {?}", value.x, value.y, value.z ); if( !st ) return st;
	return ax_json_serialize_value( s, str );
}
*/

#include "../../base/data_structure/axIArray.h"
#include "../../base/data_structure/axDList.h"
#include "../../base/data_structure/axAutoPtr.h"
#include "../../base/string/axIString.h"
#include "../../base/string/ax_utf8.h"

class axJSONSerializer;
class axJSONDeserializer;

template<class S, class T> inline	
axStatus ax_json_serialize_value( S &s, T& value, const char* name );

axStatus	ax_to_json_str		( axIStringA & str, const char* sz, bool withQuote );
axStatus	ax_from_json_str	( axIStringA & str, const char* sz, bool withQuote );

class axJSONSerializerBase: public axNonCopyable {
public:
	static const char *skip_chr() { return " \t\r\n"; }
	
	enum { k_name_mismatch = 4445 };
};

class axJSONSerializer : public axJSONSerializerBase {
	typedef axJSONSerializerBase B;
public:

	axJSONSerializer( axIStringA &str, bool condense = true ) { 
		str_ = & str;
		depth_ = 0;
		ended_ = false;
		condense_ = condense;
	}

	template<class T>	axStatus io	( T& value, const char* name )	{ 
		axStatus st;
		if( name && name[0] ) { assert( false ); return axStatus_Std::JSON_deserialize_format_error; } //first value must without name
		st = member( name );			if( !st ) return st;
		st = io_value( value );			if( !st ) return st;			
		st = str_->append( "," );		if( !st ) return st;
		return 0;
	}
	
	template<class T>	axStatus io_value( T &value ) {
		return ax_json_serialize_value( *this, value );
	}
	
	axStatus member( const char* name ) {
		if( ! name || ! name[0] ) return 0;
		
		axStatus st;
		st = newline();			if( !st ) return st;		
		axTempStringA	tmp;

		st = ax_to_json_str( tmp, name, true );		if( !st ) return st;
		st = str_->append( tmp );					if( !st ) return st;
		st = str_->append( ":" );					if( !st ) return st;
		return 0;
	}
	
	axStatus beginObject	( const char* name ) 	{ 
		axStatus st;
		st = member( name );		if( !st ) return st;
		st = begin_('{'); 			if( !st ) return st;
		return 0;
	}
	
	axStatus endObject 		() 						{ 
		axStatus st;
		st = end_('{','}'); 		if( !st ) return st;
		st = str_->append( "," );	if( !st ) return st;
		return 0;
	}

	axStatus beginArray		( const char* name ) 	{ 
		axStatus st;
		st = member( name );		if( !st ) return st;
		st = begin_('['); 			if( !st ) return st;
		return 0;
	}
	axStatus endArray		() 						{ 
		axStatus st;
		st = end_('[',']'); 	if( !st ) return st;
		st = str_->append( "," );	if( !st ) return st;
		return 0;
	}
	
	axStatus beginObjectValue	()					{ return begin_('{'); }
	axStatus endObjectValue 	()					{ return end_('{','}'); }

	axStatus beginArrayValue	() 					{ return begin_('['); }
	axStatus endArrayValue		() 					{ return end_('[',']'); }

	axStatus begin_( const char ch ) { 
		if( ended_ ) return axStatus_Std::JSON_deserialize_format_error;
	
		axStatus st;
		st = str_->append(ch);		if( !st ) return st;
		depth_++;
		return 0;
	};

	axStatus end_( const char begin, const char ch ) {
		axStatus st;
		if( depth_ == 0 )  		return axStatus_Std::JSON_deserialize_format_error;
		depth_--;

		if( depth_ == 0 ) ended_ = true;
		
		if( str_->size() == 0 ) return axStatus_Std::JSON_deserialize_format_error;
		
		char e = str_->lastChar(0);
		if( e == ',' ) {
			str_->decSize(1);
			st = newline();				if( !st ) return st;
		}else{
			if( e != begin ) 	return axStatus_Std::JSON_deserialize_format_error;
		}
		st = str_->append(ch);			if( !st ) return st;		
		return 0;
	}

	axStatus newline() {
		if( condense_ ) return 0;
		
		axStatus st;
		st = str_->append( "\n" );			if( !st ) return st;
		axSize i=0;
		for( i=0; i<depth_; i++ ) {
			st = str_->append( "\t" );		if( !st ) return st;
		}
		return 0;
	}
	
	axIStringA*	str_;
	axSize 		depth_;
	bool 		condense_;
	bool		ended_;
};


class axJSONDeserializer : public axJSONSerializerBase {
	typedef axJSONSerializerBase B;
public:

	axJSONDeserializer( const axIStringA &str )	;
	
	template<class T>	axStatus parse( T& value, const char* name ) {
		axStatus st = io( value, name );
		if( st.code() == axStatus_Std::JSON_deserialize_internal_found ) return 0;
		return st;
	}
	
	template<class T>	axStatus io	( T& value, const char* name )	{ 
		axStatus st;
		if( ! name ) return axStatus_Std::JSON_deserialize_format_error;
		
		if( ! checkStringToken(name) ) return k_name_mismatch;
		st = nextToken();				if( !st ) return st;
		
		st = checkToken( ":" );			if( !st ) return st;
		st = nextToken();				if( !st ) return st;
		
		st = io_value( value );			if( !st ) return st;
		return axStatus_Std::JSON_deserialize_internal_found;
	}
	
	template<class T>	axStatus io_value( T& value ) {
		return ax_json_serialize_value( *this, value );
	}
	
	axStatus	nextToken();	
	axStatus	_nextToken();
	axStatus	checkToken( const char* sz );
	axStatus	checkStringToken( const char* sz );
	
	
	axStatus beginObject( const char* name );

	axStatus endObject() ;

	axStatus beginObjectValue();

	axStatus endObjectValue();	
	axStatus beginArrayValue();
	axStatus endArrayValue() ;


	axTempStringA		token;
	bool				tokenIsString;
private:
	const axIStringA*	str_;	
	const char *		r_;	
};


template<class T> inline
axStatus	ax_to_json  ( axIStringA &json, const T & v ) {
	axJSONSerializer	s(json);
	return s.io_value( *const_cast<T*>(&v) );
}

template<class T> inline
axStatus	ax_from_json( const char* json, T & v ) {
	axConstStringA		str( json );
	axJSONDeserializer	s(str);
	return s.io_value(v);
}

template<class T> inline
axStatus	ax_to_json_file( const char* filename, const T & v ) {
	axStatus st;
	axTempStringA	json;
	st = ax_to_json( json, v );						if( !st ) return st;
	st = axFileSystem::saveFile( json, filename );	if( !st ) return st;
	return 0;
}

template<class T> inline
axStatus	ax_from_json_file( const char* filename, T & v ) {
	axStatus st;
	axTempStringA	json;
	st = axFileSystem::loadFile( json, filename );	if( !st ) return st;
	st = ax_from_json( json, v );					if( !st ) return st;
	return 0;
}

//------------------

template<class T> inline axStatus ax_json_serialize_value_primitive( axJSONSerializer &s, T &v ) {
	axStatus st;
	st = s.str_->appendFormat( "{?}", v );		if( !st ) return st;
	return 0;

}

template<class T> inline axStatus ax_json_serialize_value_primitive( axJSONDeserializer &s, T &v ) {
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
#undef axTYPE_LIST

//------ bool -------

inline axStatus ax_json_serialize_value( axJSONSerializer &s, bool &v )	{
	return s.str_->appendFormat( v ? "true" : "false" );
}

inline axStatus ax_json_serialize_value( axJSONDeserializer &s, bool &v ) {
	axStatus st;	
	if( s.tokenIsString )	return -1;

	if( s.token.equalsNoCase( "true" ) ) {
		v = true;
	}else if( s.token.equalsNoCase( "false" ) ) {
		v = false;
	}else {
		return axStatus_Std::JSON_deserialize_bool_format_error;
	}
	
	st = s.nextToken();		if( !st ) return st;
	return 0;
}



//----------- DList ----------------

template<class T> inline 
axStatus	ax_json_serialize_value ( axJSONSerializer &s, axDList<T> &v ) {
	axStatus st;
	st = s.beginArrayValue();	if( !st ) return st;

	T *p = v.head();
	for( ; p; p=p->next() )  {
		st = ax_json_serialize_value ( s, (T&)*p );	if( !st ) return st;
	}

	st = s.endArrayValue();		if( !st ) return st;
	return 0 ;
}


template<class T> inline 
axStatus	ax_json_serialize_value ( axJSONDeserializer &s, axDList<T> &v ) {	
	axStatus st;

	st = s.beginArrayValue();			if( !st ) return st;
	for(;;) {
		st = s.nextToken();				if( !st ) return st;
		if( ! s.token.equals(",") ) break;	

		axAutoPtr<T> p;
		st = p.newObject();				if( !st ) return st;
		st = s.io( *p );				if( !st ) return st;
		v.append( p.unref() );
	}
	st = s.endArrayValue();				if( !st ) return st;

	return 0;
}

//------ Array -------
template< class T > inline
axStatus ax_json_serialize_value( axJSONSerializer &s, axIArray<T> &v ) {
	axStatus st;
	st = s.beginArrayValue();		if( !st ) return st;
	for( axSize i=0; i<v.size(); i++ ) {
		st = ax_json_serialize_value ( s, v[i] );	if( !st ) return st;
	}
	st = s.endArrayValue();			if( !st ) return st;
	return 0;
}


template< class T > inline
axStatus ax_json_serialize_value( axJSONDeserializer &s, axIArray<T> &v ) {
	axStatus st;
	st = s.beginArrayValue();			if( !st ) return st;
	for(;;) {
		st = s.nextToken();				if( !st ) return st;
		if( ! s.token.equals(",") ) break;	

		st = v.incSize( 1 );			if( !st ) return st;
		st = s.io( v.last() );			if( !st ) return st;
	}	
	st = s.endArrayValue();				if( !st ) return st;
	return 0;
}

template< class S, class T > inline
axStatus ax_json_serialize_value( S &s, axExternalArray<T> &v ) {
	return ax_json_serialize_value( s, (axIArray<T>&) v );
}
template< class S, class T, size_t N > inline
axStatus ax_json_serialize_value( S &s, axLocalArray<T,N> &v ) {
	return ax_json_serialize_value( s, (axIArray<T>&) v );
}

template< class S, class T > inline
axStatus ax_json_serialize_value( S &s, axArray<T> &v ) {
	return ax_json_serialize_value( s, (axIArray<T>&) v );
}

template< class S, class T, size_t LOCAL_BUF_SIZE > inline
axStatus ax_json_serialize_value( S &s, axArray<T,LOCAL_BUF_SIZE> &v ) {
	return ax_json_serialize_value( s, (axIArray<T>&) v );
}


template< class S, class T, size_t CS, size_t LB > inline
axStatus ax_json_serialize_value( S &s, axChunkArray<T,CS,LB> &v ) {
	return ax_json_serialize_value( s, (axIArray<T>&) v );
}

//---------- String ----------------


template< class T > inline
axStatus ax_json_serialize_value( axJSONSerializer &s, axIString_<T> &v ) {
	axStatus st;
	axTempStringA str;
	st = ax_to_json_str( str, v, true );		if( !st ) return st;	
	return s.str_->append( str );
}


template< class T > inline
axStatus ax_json_serialize_value( axJSONDeserializer &s, axIString_<T> &v ) {
	axStatus st;
	if( ! s.tokenIsString ) return -1;
	st = v.set( s.token );		if( !st ) return st;
	st = s.nextToken();			if( !st ) return st;
	return 0;
}

template< class S, class T, size_t N > inline
axStatus ax_json_serialize_value( S &s, axString_<T,N> &v ) {
	return ax_json_serialize_value( s, (axIString_<T>&) v );
}

template< class S, class T, size_t N > inline
axStatus ax_json_serialize_value( S &s, axLocalString_<T,N> 	&v ) { return ax_json_serialize_value( s, (axIString_<T>&) v ); }

template< class S, size_t N > inline
axStatus ax_json_serialize_value( S &s, axLocalStringA<N> 		&v ) { return ax_json_serialize_value( s, (axIStringA &) v ); }

template< class S, class T > inline
axStatus ax_json_serialize_value( S &s, axConstString_<T> 		&v ) {	return ax_json_serialize_value( s, (axIString_<T>&) v ); }

template< class S, class T > inline
axStatus ax_json_serialize_value( S &s, axExternalString_<T> 	&v ) { return ax_json_serialize_value( s, (axIString_<T>&) v ); }


//===============

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJSONSerializer &s, axTinyString_<T,LOCAL_BUF_SIZE> &v ) {
	axStatus st;
	axTempStringA str;
	st = ax_to_json_str( str, v, true );		if( !st ) return st;	
	return s.str_->append( str );
}


template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJSONDeserializer &s, axTinyString_<T,LOCAL_BUF_SIZE> &v ) {
	axStatus st;
	st = s.nextToken();			if( !st ) return st;
	if( ! s.tokenIsString ) return -1;
	return v.set( s.token );
}

template<size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJSONSerializer &s, axTinyStringA< LOCAL_BUF_SIZE > &v ) {
	return ax_json_serialize_value( s, (axTinyString_< char, LOCAL_BUF_SIZE > & ) v );
}

template<size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJSONDeserializer &s, axTinyStringA< LOCAL_BUF_SIZE > &v ) {
	return ax_json_serialize_value( s, (axTinyString_< char, LOCAL_BUF_SIZE > & ) v );
}

template<size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJSONSerializer &s, axTinyStringW< LOCAL_BUF_SIZE > &v ) {
	return ax_json_serialize_value( s, (axTinyString_< wchar_t, LOCAL_BUF_SIZE > & ) v );
}

template<size_t LOCAL_BUF_SIZE> inline
axStatus ax_json_serialize_value( axJSONDeserializer &s, axTinyStringW< LOCAL_BUF_SIZE > &v ) {
	return ax_json_serialize_value( s, (axTinyString_< wchar_t, LOCAL_BUF_SIZE > & ) v );
}


// ================================


template<class S, class T> inline	
axStatus ax_json_serialize_value( S &s, T& value ) { 
	axStatus st;
	st = s.beginObjectValue();							if( !st ) return st;
	st = ax_json_on_string_serialize( s, value );		if( !st ) return st;
	st = s.endObjectValue();							if( !st ) return st;	
	return 0;
}

template<class S, class T> inline	
axStatus ax_json_serialize_object_members( S &s, T &v ) { 
	return v.onStringSerialize( s ); 
}


template< class T> inline	
axStatus ax_json_on_string_serialize( axJSONSerializer &s, T &value ) {
	return ax_json_serialize_object_members( s, value );
}


template< class T> inline	
axStatus ax_json_on_string_serialize( axJSONDeserializer &s, T &value ) {
	axStatus st;	
	if( s.checkToken("}") ) return 0;
	
	for(;;) {
		st = ax_json_serialize_object_members( s, value );	
		if( st.code() == axStatus_Std::JSON_deserialize_internal_found ) {
			if( s.checkToken(",") ) {
				st = s.nextToken();		if( !st ) return st;
				continue;		
			}
			if( s.checkToken("}") ) break;
			
			return axStatus_Std::JSON_deserialize_format_error;
		}else if( !st ) return st;
	}
	return 0;
}





#define ax_string_serialize( n ) st = s.io( n, #n ); if( !st ) return st;

#endif //__axJSONSerializer_h__
