//
//  axStringWithHash.h
//  ax_core
//
//  Created by Jason on 2013-11-09.
//
//

#ifndef ax_core_axStringWithHash_h
#define ax_core_axStringWithHash_h

template<class T, size_t LOCAL_BUF_SIZE=64 >
class axStringWithHash {
public:

	axStringWithHash() : hash_(0) {}

	axStatus	set( const T* sz ) {
		hash_ = ax_hash_code( sz );
		return str_.set( sz );
	}

	const char*	c_str	( axSize offset=0 ) const 	{ return str_.c_str( offset ); }
	operator const T*	() const 					{ return c_str(); }

	bool	operator == ( const axStringWithHash & rhs ) {
		if( hash_ != rhs.hash_ ) return false;
		return str_.equals( rhs.str_ );
	}
	
	uint32_t	hash	() const	{ return hash_; }

	axStatus	toStringFormat(	axStringFormat & f ) const { return str_.toStringFormat(f); }

	axStatus	copy	( const axStringWithHash & src ) {
		hash_ = src.hash_;
		return ax_copy( str_, src.str_ );
	}

private:
	axString_<T, LOCAL_BUF_SIZE> str_;
	uint32_t hash_;
};

template<class T, size_t LOCAL_BUF_SIZE> inline
uint32_t ax_hash_code( const axStringWithHash<T,LOCAL_BUF_SIZE> & v ) {
	return (uint32_t) v.hash();
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus ax_copy( axStringWithHash<T,LOCAL_BUF_SIZE> & dst, const axStringWithHash<T,LOCAL_BUF_SIZE> & src ) {
	return dst.copy( src );
}

#endif
