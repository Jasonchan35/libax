//
//  axStringWithHash.h
//  ax_core
//
//  Created by Jason on 2013-11-09.
//
//

#ifndef ax_core_axStringWithHash_h
#define ax_core_axStringWithHash_h

#include "../other/axJson.h"

template<class T>
class axIStringWithHash {
public:
	axIStringWithHash( axIString_<T> & istr ) : istr_(istr), hash_(0) {}

	axStatus	set( const T* sz ) {
		axStatus st = istr_.set( sz );	if( !st ) return st;
		recalcHash();
		return 0;
	}
	
	axStatus	setWithLength( const T* sz, size_t len ) {
		axStatus st = istr_.setWithLength( sz, len );	if( !st ) return st;
		recalcHash();
		return 0;
	}

	const T*				c_str	( axSize offset=0 ) const 	{ return istr_.c_str( offset ); }
	
	operator const T*	() const 					{ return c_str(); }

	bool	operator==	( const axIStringWithHash & rhs ) const { return equals(rhs); }

	bool	equals ( const axIStringWithHash & rhs ) const {
		if( hash_ != rhs.hash_ ) return false;
		return istr_.equals( rhs.istr_ );
	}
	
	uint32_t	hash	() const	{ return hash_; }

	axStatus	toStringFormat(	axStringFormat & f ) const { return istr_.toStringFormat(f); }

	axStatus	copy	( const axIStringWithHash & src ) {
		hash_ = src.hash_;
		return ax_copy( istr_, src.istr_ );
	}
	
	axIString_<T> &		internal_str() 		{ return istr_; }
	void				recalcHash	()		{ hash_ = ax_hash_code( istr_.c_str() ); }

private:
	bool operator ==	( const T* sz ) const;	// please using equals() to avoid  "abc" == str will compare the pointer rather than content
	bool operator !=	( const T* sz ) const;	// please using equals()
	bool operator <		( const T* sz ) const;	// please using cmp() < 0
	bool operator >		( const T* sz ) const;	// please using cmp() > 0
	bool operator <=	( const T* sz ) const;	// please using cmp() <= 0
	bool operator >=	( const T* sz ) const;	// please using cmp() >= 0

	axIString_<T> & istr_;
	uint32_t		hash_;
};

template<class T, size_t LOCAL_BUF_SIZE=64 >
class axStringWithHash : public axIStringWithHash<T> {
	typedef axIStringWithHash<T> B;
public:
	axStringWithHash() : B(str_) {}
			B& asInterface() 		{ return (B&)*this; }
	const 	B& asInterface() const 	{ return (B&)*this; }

private:
	axString_<T, LOCAL_BUF_SIZE> str_;
};


//-------------------------------------

template<class T> inline
axStatus	ax_json_serialize_value ( axJsonWriter &s, axIStringWithHash<T> &v ) {
	axStatus st;
	st = s.io_value( v.internal_str() );	if( !st ) return st;
	return 0 ;
}

template<class T> inline
axStatus	ax_json_serialize_value ( axJsonParser &s, axIStringWithHash<T> &v ) {
	axStatus st;
	st = s.io_value( v.internal_str() );	if( !st ) return st;
	v.recalcHash();
	return 0;
}



template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus	ax_json_serialize_value ( axJsonWriter &s, axStringWithHash<T,LOCAL_BUF_SIZE> &v ) {
	return ax_json_serialize_value( s, v.asInterface() );
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus	ax_json_serialize_value ( axJsonParser &s, axStringWithHash<T,LOCAL_BUF_SIZE> &v ) {
	return ax_json_serialize_value( s, v.asInterface() );
}


template<class T> inline
uint32_t ax_hash_code( const axIStringWithHash<T> & v ) {
	return v.hash();
}

template<class T, size_t LOCAL_BUF_SIZE> inline
uint32_t ax_hash_code( const axStringWithHash<T,LOCAL_BUF_SIZE> & v ) {
	return v.hash();
}

template<class T> inline
axStatus ax_copy( axIStringWithHash<T> & dst, const axIStringWithHash<T> & src ) {
	return dst.copy( src );
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus ax_copy( axStringWithHash<T,LOCAL_BUF_SIZE> & dst, const axStringWithHash<T,LOCAL_BUF_SIZE> & src ) {
	return dst.copy( src );
}

#endif
