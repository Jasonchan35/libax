//
//  axStringWithHash.h
//  ax_core
//
//  Created by Jason on 2013-11-09.
//
//

#ifndef ax_core_axStringWithHash_h
#define ax_core_axStringWithHash_h

template<class T>
class axIStringWithHash {
public:
	axIStringWithHash( axIString_<T> & istr ) : istr_(istr), hash_(0) {}

	axStatus	set( const T* sz ) {
		hash_ = ax_hash_code( sz );
		return istr_.set( sz );
	}

	const T*	c_str	( axSize offset=0 ) const 	{ return istr_.c_str( offset ); }
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
	
	template<class S>
	axStatus	serialize_io( S & s ) {
		axStatus st;
		ax_io( istr_ );
		ax_io( hash_ );
		return 0;
	}

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

private:
	axString_<T, LOCAL_BUF_SIZE> str_;
};


//-------------------------------------

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
