#ifndef __axJSONSerializer_h__
#define __axJSONSerializer_h__

#include "../data_structure/axIArray.h"
#include "../data_structure/axDList.h"
#include "../data_structure/axAutoPtr.h"
#include "../string/axIString.h"
#include "../string/ax_utf8.h"

class axJSONSerializer;
class axJSONDeserializer;



template<class S, class T> inline	
axStatus ax_json_serialize_io( S &s, T& value, const char* name );

class axJSONSerializerBase: public axNonCopyable {
public:

	static const char *skip_chr() { return " \t\r\n"; }

};

class axJSONSerializer : public axJSONSerializerBase {
	typedef axJSONSerializerBase B;
public:

	axJSONSerializer( axIStringA &str )	{ 
		set( str ); 
		depth_ = 0;
		isPrintFormat = false;
	}

	void set( axIStringA &str ) {
		str_ = &str;
	}

	bool isStartValue() {
		if( str_->size() == 0 ) return true;

		const char* s = str_->c_str();
		const char* e = str_->c_str( str_->size()-1 );

		for( ; s<e; e-- ) {

			const char c = *e;
			if( !ax_strchr( skip_chr(), c ) ) {
				if( c == '{' || c == '[' ) {
					return true;
				}else {
					return false;
				}
			}
		}
		return true;
	}

	template<class T>	axStatus io	( T& value, const char* name = "" )	{ 
		axStatus st;
		//if( !str_ ) { assert( false ); return axStatus::JSON_serialize_not_init; }

		bool is_first = false;

		if( str_->size() == 0 ) { // print start {
			if( ax_strlen( name ) > 0 ) {
				st = beginStruct_();	if( !st ) return st;
				is_first = true;
			}
		}

		if( !isStartValue() ) {
			st = str_->appendFormat( "," );	if( !st ) return st;	
		}

		if( isPrintFormat ){
			st = str_->appendFormat( "\n" );	if( !st ) return st;
		}
		st = printDepth_();						if( !st ) return st;

		if( ax_strlen( name ) > 0 ) {
			st = str_->appendFormat( "\"{?}\": ", name );	if( !st ) return st;	
		}

		st = ax_json_serialize_io ( *this, value );		if( !st ) return st;	

		if( is_first ) {
			st = endStruct_();							if( !st ) return st;
		}
		return 0;
	}

	axStatus beginStruct_() { 
		axStatus st;
		st = str_->appendFormat( "{{" );	if( !st ) return st;
		depth_++;
		return 0;

	};

	axStatus endStruct_() {
		axStatus st;
		if( depth_ > 0 ) depth_--;

		if( isPrintFormat ) {
			st = str_->appendFormat( "\n" );				if( !st ) return st;
		}

		st = printDepth_();								if( !st ) return st;
		st = str_->appendFormat( "}" );					if( !st ) return st;

		return 0;
	}

	axStatus printDepth_() {
		if( !isPrintFormat ) return 0;
		axStatus st;
		axSize i=0;
		for( i=0; i<depth_; i++ ) {
			st = str_->appendFormat( "\t" );	if( !st ) return st;
		}
		return 0;
	}

	axIStringA*	str_;
	axSize depth_;

	bool isPrintFormat;
};


class axJSONDeserializer : public axJSONSerializerBase {
	typedef axJSONSerializerBase B;
public:

	axJSONDeserializer( const axIStringA &str )			{ set( str ); }

	void set( const axIStringA &str ) {
		str_ = &str;
		r_ = str.c_str();
		if( str.size() == 0 ) {
			e_ = str.c_str();
		}else {
			e_ = str.c_str( str.size()-1 );
		}
	}

	const char *fineEndToken_() {
		const char *r = r_;
		if( r == NULL ) return NULL;
		if( *r == 0 )   return NULL;


		const char *ax_json_end_token =  " },]";

		for( ; r <= e_; r++ ) {
			const char c = *r;
			if( ax_strchr( ax_json_end_token, c ) ) {
				return r;
			}
		}
		return NULL;

	}

	axStatus skipColon_() {
		axStatus st;
		st = skipSpace_();	if( !st ) return st;
		if( *r_ != ':' ) return axStatus::JSON_deserialize_cannot_find_colon; 
		r_++;
		return 0;
	}

	axStatus skipSpace_() {
		if( r_ == NULL ) return axStatus::JSON_deserialize_format_error;
		if( *r_ == 0 )   return axStatus::JSON_deserialize_format_error;

		for( ; r_ < e_; r_++ ) {
			const char c = *r_;
			if( !ax_strchr( skip_chr(), c ) ) {
				return 0;
			}
		}
		return 0;
	}


	bool  isHaveElement_() {
		axStatus st;
		st = skipSpace_(); if( !st ) return false;
		const char *next_token = fineEndToken_();
		if( next_token == r_ ) return false;
		return true;
	}


	bool  nextElement_() {
		axStatus st;
		st = skipSpace_(); if( !st ) return false;
		if( *r_ == ',' ) { r_++; return true; }
		return false;
	}

	bool  isNextString_() {
		axStatus st;
		st = skipSpace_(); if( !st ) return false;
		if( *r_ == '\"' ) return true;
		return false;

	}

	axStatus getString( axIStringA &str ) {
		axStatus st;

		st = skipSpace_();	if( !st ) return st;

		if( *r_ != '\"' ) return axStatus::JSON_deserialize_cannot_find_string;
		r_++;

		const char *e = ax_strchr( r_, '\"' );
		if( !e ) return axStatus::JSON_deserialize_cannot_find_string;

		st = str.setWithLength( r_, e-r_ );	if( !st ) return st;
		r_ = e+1;

		return 0;
	}

	axStatus getToken_( axIStringA &str ) {
		axStatus st;
		st = skipSpace_();	if( !st ) return st;
		const char *c = fineEndToken_();
		if( !c ) return axStatus::JSON_deserialize_cannot_find_end_token;
		st = str.setWithLength( r_, c-r_ );	if( !st ) return st;
		r_ = c;
		return 0;
	}



	template<class T>	axStatus io	( T& value, const char* name = "" )	{ 
		axStatus st;
		axTempStringA str;
		bool is_first = false;

		if( r_ == str_->c_str() ) {

			if( ax_strlen( name ) > 0 ) {
				is_first = true;
				st = beginStruct_();	if( !st ) return st;

				st = getString( str ); if( !st ) return st;
				if( !str.equalsIgnoreCase( name ) ) { 
					return axStatus::JSON_deserialize_name_not_equal;			
				}

				st = skipColon_(); if( !st ) return st;
			}

		}


		if( targetName.size() > 0 ) {
			if( targetName.equals( name ) ) {
				targetName.clear();
				st =  ax_json_serialize_io	( *this, value );  if( !st ) return st;
				return axStatus::JSON_deserialize_internal_found;

			}else {
				return 0;
			}
		}

		st =  ax_json_serialize_io	( *this, value );  if( !st ) return st;


		if( is_first ) {
			st = endStruct_();		if( !st ) return st;
		}

		return 0;
	}

	axStatus beginStruct_() { 
		axStatus st;
		st = skipSpace_(); if( !st ) return st;
		if( *r_ != '{' ) return axStatus::JSON_deserialize_format_error;
		r_++;
		return 0;
	}

	axStatus endStruct_() { 
		axStatus st;
		st = skipSpace_(); if( !st ) return st;
		if( *r_ != '}' ) return axStatus::JSON_deserialize_format_error;
		r_++;
		return 0;
	}

	const char *r_, *e_;	
	const axIStringA*	str_;

	axTempStringA	targetName;
};



template<class T> inline axStatus ax_json_serialize_io_primitive( axJSONSerializer &s, T &v ) {
	axStatus st;
	st = s.str_->appendFormat( "{?}", v );		if( !st ) return st;
	return 0;

}

template<class T> inline axStatus ax_json_serialize_io_primitive( axJSONDeserializer &s, T &v ) {
	axStatus st;
	axTempStringA str;
	st = s.getToken_( str );			if( !st ) return st;
	st = ax_str_to( str, v );		if( !st ) return st;
	return 0;
}

#define axTYPE_LIST(T) \
	template< class S > inline axStatus ax_json_serialize_io( S &s, T &v ) { return ax_json_serialize_io_primitive( s, v ); }
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

inline axStatus ax_json_serialize_io( axJSONSerializer &s, bool &v )	{
	axStatus st;
	if( v ) {
		st = s.str_->appendFormat( "true" );		if( !st ) return st;
	}else {
		st = s.str_->appendFormat( "false" );		if( !st ) return st;
	}
	return 0;
}

inline axStatus ax_json_serialize_io( axJSONDeserializer &s, bool &v ) {

	axStatus st;
	axTempStringA str;

	st = s.getToken_( str );			if( !st ) return st;

	if( str.equalsIgnoreCase( "true" ) ) {
		v = true;
	}else if( str.equalsIgnoreCase( "false" ) ) {
		v = false;
	}else {
		return axStatus::JSON_deserialize_bool_format_error;
	}

	return 0;
}



//----------- DList ----------------

template<class T> inline 
axStatus	ax_json_serialize_io ( axJSONSerializer &s, axDList<T> &v ) {
	axStatus st;
	T *p = v.head();

	st = s.str_->appendFormat( "[" );	if( !st ) return st;

	if( s.isPrintFormat ) {
		st = s.str_->appendFormat( "\n" );	if( !st ) return st;
	}

	s.depth_++;
	st = s.printDepth_();				if( !st ) return st;
	for( ;p;p=p->next() )  {

		st = ax_json_serialize_io ( s, (T&)*p );	if( !st ) return st;
		if( p->next() != NULL ) {
			st = s.str_->appendFormat( "," );		if( !st ) return st;

			if( s.isPrintFormat ) {
				st = s.str_->appendFormat( "\n" );		if( !st ) return st;
			}

			st = s.printDepth_();					if( !st ) return st;
		}
	}

	if( s.isPrintFormat ) {
		st = s.str_->appendFormat( "\n" );	if( !st ) return st;
	}

	s.depth_--;
	st = s.printDepth_();				if( !st ) return st;
	st = s.str_->appendFormat( "]" );	if( !st ) return st;

	return 0 ;
}


template<class T> inline 
axStatus	ax_json_serialize_io ( axJSONDeserializer &s, axDList<T> &v ) {	
	axStatus st;

	st = s.skipSpace_();	if( !st ) return st;
	if( *s.r_ != '[' ) return axStatus::JSON_deserialize_format_error;
	s.r_++;


	while( s.isHaveElement_() ) { 
		axAutoPtr<T> p;
		st = p.newObject();			if( !st ) return st;
		st = s.io( *p.ptr() );		if( !st ) return st;
		v.append( p.unref() );		if( !st ) return st;
		s.nextElement_();
	};


	st = s.skipSpace_();	if( !st ) return st;
	if( *s.r_ != ']' ) return axStatus::JSON_deserialize_format_error;
	s.r_++;

	return 0;
}

//------ Array -------
template< class T > inline
axStatus ax_json_serialize_io( axJSONSerializer &s, axIArray<T> &v ) {
	axStatus st;
	axSize i;

	//T *p = v.head();
	st = s.str_->appendFormat( "[" );	if( !st ) return st;

	if( s.isPrintFormat ) {
		st = s.str_->appendFormat( "\n" );	if( !st ) return st;
	}

	s.depth_++;
	st = s.printDepth_();				if( !st ) return st;

	for( i=0; i<v.size(); i++ ) {

		st = ax_json_serialize_io ( s, v[i] );		if( !st ) return st;

		if( i != v.size()-1 ) {

			st = s.str_->appendFormat( "," );		if( !st ) return st;
			if( s.isPrintFormat ) {
				st = s.str_->appendFormat( "\n" );	if( !st ) return st;
			}
			st = s.printDepth_();					if( !st ) return st;
		}
	}

	if( s.isPrintFormat ) {
		st = s.str_->appendFormat( "\n" );	if( !st ) return st;
	}

	s.depth_--;
	st = s.printDepth_();				if( !st ) return st;
	st = s.str_->appendFormat( "]" );	if( !st ) return st;

	return 0;
}


template< class T > inline
axStatus ax_json_serialize_io( axJSONDeserializer &s, axIArray<T> &v ) {

	axStatus st;

	st = s.skipSpace_();	if( !st ) return st;
	if( *s.r_ != '[' ) return axStatus::JSON_deserialize_format_error;
	s.r_++;

	while( s.isHaveElement_() ){ 
		st = v.incSize( 1 );		if( !st ) return st;
		st = s.io( v.last() );		if( !st ) return st;
		s.nextElement_();
	}

	st = s.skipSpace_();	if( !st ) return st;
	if( *s.r_ != ']' ) return axStatus::JSON_deserialize_format_error;
	s.r_++;

	return 0;
}

template< class S, class T > inline
axStatus ax_json_serialize_io( S &s, axExternalArray<T> &v ) {
	return ax_json_serialize_io( s, (axIArray<T>&) v );
}
template< class S, class T, size_t N > inline
axStatus ax_json_serialize_io( S &s, axLocalArray<T,N> &v ) {
	return ax_json_serialize_io( s, (axIArray<T>&) v );
}

template< class S, class T > inline
axStatus ax_json_serialize_io( S &s, axArray<T> &v ) {
	return ax_json_serialize_io( s, (axIArray<T>&) v );
}

template< class S, class T, size_t LOCAL_BUF_SIZE > inline
axStatus ax_json_serialize_io( S &s, axArray<T,LOCAL_BUF_SIZE> &v ) {
	return ax_json_serialize_io( s, (axIArray<T>&) v );
}


template< class S, class T, size_t CS, size_t LB > inline
axStatus ax_json_serialize_io( S &s, axChunkArray<T,CS,LB> &v ) {
	return ax_json_serialize_io( s, (axIArray<T>&) v );
}

//---------- String ----------------


template< class T > inline
axStatus ax_json_serialize_io( axJSONSerializer &s, axIString_<T> &v ) {
	axStatus st;
	st = s.str_->appendFormat( "\"{?}\"", v.c_str() );		if( !st ) return st;
	return 0;
}


template< class T > inline
axStatus ax_json_serialize_io( axJSONDeserializer &s, axIString_<T> &v ) {
	axStatus st;
	axTempStringA str;
	st = s.getString( str );	if( !st ) return st;
	st = v.set( str );			if( !st ) return st;

	return 0;
}

template< class S, class T, size_t N > inline
axStatus ax_json_serialize_io( S &s, axString_<T,N> &v ) {
	return ax_json_serialize_io( s, (axIString_<T>&) v );
}

template< class S, class T, size_t N > inline
axStatus ax_json_serialize_io( S &s, axLocalString_<T,N> &v ) {
	return ax_json_serialize_io( s, (axIString_<T>&) v );
}

template< class S, class T > inline
axStatus ax_json_serialize_io( S &s, axConstString_<T> &v ) {
	return ax_json_serialize_io( s, (axIString_<T>&) v );
}

template< class S, class T > inline
axStatus ax_json_serialize_io( S &s, axExternalString_<T> &v ) {
	return ax_json_serialize_io( s, (axIString_<T>&) v );
}

template< class S, class T > inline
axStatus ax_json_serialize_io( S &s, axString_Array<T> &v ) {
	return ax_json_serialize_io( s, (axIArray<axString_<T>>&) v );
}


// ================================


template<class S, class T> inline	
axStatus ax_json_serialize_io( S &s, T& value ) { 
	axStatus st;
	st = s.beginStruct_();								if( !st ) return st;
	st = ax_json_on_string_serialize( s, value );		if( !st ) return st;
	st = s.endStruct_();								if( !st ) return st;	
	return 0;
}


template< class T> inline	
axStatus ax_json_on_string_serialize( axJSONSerializer &s, T &value ) {
	return value.onStringSerialize( s );
}


template< class T> inline	
axStatus ax_json_on_string_serialize( axJSONDeserializer &s, T &value ) {
	axStatus st;
	axTempStringA str;

	while( s.isHaveElement_() ) { 
		st = s.getString( str );		if( !st ) return st;
		st = s.targetName.set( str );	if( !st ) return st;
		st = s.skipColon_();			if( !st ) return st;
		st = value.onStringSerialize( s );
		if( st.code() == 0 )  {
			return axStatus::JSON_deserialize_element_not_found;
		}else if( st.code() != axStatus::JSON_deserialize_internal_found ) {
			return st;
		}

		s.nextElement_();
	}

	return 0;
}



#define ax_string_serialize( n ) st = s.io( n, #n ); if( !st ) return st;


#endif //__axJSONSerializer_h__