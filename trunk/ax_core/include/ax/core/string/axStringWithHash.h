//
//  axStringWithHash.h
//  ax_core
//
//  Created by Jason on 2013-11-09.
//
//

#ifndef ax_core_axStringWithHash_h
#define ax_core_axStringWithHash_h

template<class T, size_t LOCAL_BUF_SIZE=8 >
class axStringWithHash {
public:

	axStringWithHash( const T* sz ) : hash_(0) {}

	axStatus	set( const T* sz ) {
		hash_ = ax_string_hash( sz );
		return str_.set( sz );
	}

	const char*	c_str	( axSize offset=0 ) const 	{ return str_.c_str( offset ); }
	operator const T*	() const 					{ return c_str(); }

	bool	operator == ( const axStringWithHash & rhs ) {
		if( hash_ != rhs.hash_ ) return false;
		return str_.equals( rhs.str_ );
	}
	
	uint32_t	hash	() const	{ return hash_; }

private:
	axString_<T, LOCAL_BUF_SIZE> str_;
	uint32_t hash_;
};

template<> inline uint32_t ax_hash_code( const int8_t   & v ) { return (uint32_t) v.hash(); }


#endif
