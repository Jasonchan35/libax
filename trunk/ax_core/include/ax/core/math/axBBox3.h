#ifndef __axBBox3_h__
#define __axBBox3_h__

#include "axVec3.h"

template< class T >
class axBBox3 {
public:
	axBBox3()				{ clear(); }
	void		clear()		{ minPoint_.set(-1,-1,-1); maxPoint_.set( 1, 1, 1); }
	void		set			( const axVec3<T> &minPoint, const axVec3<T> &maxPoint )	{ minPoint_ = minPoint; maxPoint_ = maxPoint; }
	void		setMinPoint ( const axVec3<T> &v )			{ minPoint_ = v; }
	void		setMaxPoint ( const axVec3<T> &v )			{ maxPoint_ = v; }
	
	void		expand		( const axVec3<T>	&pt );
	void		expand		( const axBBox3		&b  );
	
	axVec3<T>	center		() const		{ return ( maxPoint_ + minPoint_ ) / (T)2.0; }
	axVec3<T>	size		() const		{ return maxPoint_ - minPoint_; }
	T			sizeX		() const		{ return maxPoint_.x - minPoint_.x; }
	T			sizeY		() const		{ return maxPoint_.y - minPoint_.y; }
	T			sizeZ		() const		{ return maxPoint_.z - minPoint_.z; }

	bool		isEmpty		() const		{ return minPoint_.x > maxPoint_.x; }
	bool		isInside	( const axVec3<T>  &pt ) const;
	bool		isOverlap	( const axBBox3<T> &bb ) const;
	
	axBBox3<T>	operator*	( const axMatrix4x4<T>& m ) const;

	const axVec3<T>&	minPoint() const { return minPoint_; }
	const axVec3<T>&	maxPoint() const { return maxPoint_; }

	template<class S>	axStatus	serialize_io	( S &se );
						axStatus	toStringFormat	( axStringFormat &f ) const;
private:
	axVec3<T>	minPoint_;
	axVec3<T>	maxPoint_;
};

typedef axBBox3<float>	axBBox3f;
typedef axBBox3<double>	axBBox3d;


template<class T>
axBBox3<T>	axBBox3<T>::operator*( const axMatrix4x4<T>& m ) const {
	axBBox3<T>	ret;
	if( isEmpty() ) return ret;
	ret.minPoint_ = minPoint_ * m;
	ret.maxPoint_ = maxPoint_ * m;
	return ret;
}


template<class T>
template<class S>
inline axStatus axBBox3<T>::serialize_io( S &s ) {
	axStatus st;
	st = s.io( minPoint_ );	if( !st ) return st;
	st = s.io( maxPoint_ );	if( !st ) return st;
	return 0;
}

template<class T> inline 
axStatus axBBox3<T>::toStringFormat( axStringFormat &f ) const{
	if( isEmpty() ) {
		return f.put( "axBBox3[empty]" );
	}else{
		return f.format( "axBBox3[{?},{?}]", minPoint_, maxPoint_ );
	}
}

//---------- inline ---------------
template<class T>
bool axBBox3<T>::isInside( const axVec3<T> &pt ) const {
	return ( pt >= minPoint_ && pt <= maxPoint_ );
}

template<class T>
bool axBBox3<T>::isOverlap	( const axBBox3<T> &bb ) const {
	if( isEmpty() ) return false;
	if( bb.isEmpty() ) return false;
	if( minPoint_.x > bb.maxPoint_.x ) return false;
	if( maxPoint_.x < bb.minPoint_.x ) return false;
	if( minPoint_.y > bb.maxPoint_.y ) return false;
	if( maxPoint_.y < bb.minPoint_.y ) return false;
	if( minPoint_.z > bb.maxPoint_.z ) return false;
	if( maxPoint_.z < bb.minPoint_.z ) return false;
	return true;
}

template<class T>
void axBBox3<T>::expand( const axVec3<T> &pt ) {
	if( isEmpty() ) {
		minPoint_ = pt;
		maxPoint_ = pt;
	}else{
		minPoint_ = ax_min( minPoint_, pt );
		maxPoint_ = ax_max( maxPoint_, pt );
	}
}

template<class T>
void axBBox3<T>::expand( const axBBox3 &box ) {
	if( isEmpty() ) {
		*this = box;
	}else{
		if( box.isEmpty() ) return;
		minPoint_ = ax_min( minPoint_, box._minPoint_ );
		maxPoint_ = ax_max( maxPoint_, box._minPoint_ );
	}
}


#endif //__axBBox3_h__

