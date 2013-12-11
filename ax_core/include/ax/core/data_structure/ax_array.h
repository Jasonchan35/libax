//
//  ax_array.h
//  ax_core
//
//  Created by Jason on 2012-11-07.
//
//

#ifndef ax_core_ax_array_h
#define ax_core_ax_array_h

#include "../common/ax_common.h"


// ----------
template<class T> axALWAYS_INLINE( void		ax_array_constructor( T* p, size_t n ) );
template<class T> axALWAYS_INLINE( void		ax_array_destructor	( T* p, size_t n ) );

template<class T> axALWAYS_INLINE( bool		ax_array_is_equals	( T* dst, const T* src, size_t n ) );
template<class T> axALWAYS_INLINE( axStatus ax_array_copy		( T* dst, const T* src, size_t n ) );
template<class T> axALWAYS_INLINE( axStatus ax_array_take		( T* dst, const T* src, size_t n ) );

template<class T> axALWAYS_INLINE( T* 		ax_array_min		( T* p, size_t n ) );
template<class T> axALWAYS_INLINE( T* 		ax_array_max		( T* p, size_t n ) );

template<class T> class axIArray;

template< class DST, class SRC > inline
axStatus	ax_array_cast		( axIArray<DST*> & dst, axIArray<SRC*> & src ) {
	axStatus st;
	size_t n = src.size();
	st = dst.reserve( n );			if( !st ) return st;
	for( size_t i=0; i<n; i++ ) {
		DST* t;
		if( ! src[i]->cast(t) ) continue;
		st = dst.append( t );		if( !st ) return st;
	}
	return 0;
}

template< class T > inline
void	ax_array_constructor( T* p, size_t n ) {
	if( axTypeTrait<T>::isPOD ) return;
	T *e = p + n;
	for( ; p<e; p++ )
		::new(p) T;
}

template< class T > inline
void	ax_array_destructor( T* p, size_t n ) {
	if( axTypeTrait<T>::isPOD ) return;
	T *e = p + n;
	for( ; p<e; p++ )
		p->~T();
}

template<class T> inline
bool ax_array_is_equals( const T* dst, const T* src, size_t n ) {
	if( axTypeTrait<T>::isPOD ) {
		return ( 0 == memcmp( dst, src, n * sizeof(T) ) );
	}else{
		const T* end = src + n;
		for( ; src < end; src++, dst++ ) {
			if( *dst != *src ) return false;
		}
	}
	return true;
}

template<class T> inline
axStatus ax_array_copy( T* dst, const T* src, size_t n ) {
	if( dst+n > src && dst < src+n ) {
		assert( false );
		return axStatus_Std::cannot_be_itself;
	}

	axStatus st;
	if( axTypeTrait<T>::isPOD ) {
		memcpy( (void*)dst, (void*)src, n * sizeof(T) );
	}else{
		const T* end = src + n;
		for( ; src < end; src++, dst++ ) {
			ax_copy( *dst, *src );
		}
	}
	return 0;
}

template<class T> inline
axStatus ax_array_take( T* dst, T* src, size_t n ) {
	axStatus st;

	if( dst == src ) return 0;
	if( n == 0 ) return 0;

	if( dst+n > src && dst < src+n ) {
		//overlapped
		if( axTypeTrait<T>::isPOD ) {
			memmove( (void*)dst, (void*)src, n * sizeof(T) );
			return 0;
		}else{
			if( src > dst ) { 
				//move forward
				T* s = src;
				T* e = s + n;
				T* d = dst;
				for( ; s<e; s++, d++ ) {
					st = ax_take( *d, *s );	if( !st ) return st;
				}
			}else{ 
				//move backward
				T* s = src + n - 1;
				T* d = dst + n - 1;
				for( ; s>=src; s--, d--) {
					st = ax_take( *d, *s );	if( !st ) return st;
				}
			}
		}
		return 0;
	}

	if( axTypeTrait<T>::isPOD ) {
		//warning: destination for this 'memcpy' call is a pointer to dynamic class 'axArray<int, 0>'; vtable pointer will be overwritten [-Wnon-pod-memaccess,3]
		memcpy( (void*)dst, (void*)src, n * sizeof(T) );
	}else{
		T* end = src + n;
		for( ; src < end; src++, dst++ ) {
			st = ax_take( *dst, *src );		if( !st ) return st;
		}
	}
	return 0;
}

template<class T> inline
T*	ax_array_min( T* p, size_t n ) {
	if( ! n ) return NULL;
	T* m = p;
	T* s = p + 1;
	T* e = p + n;

	for( ; s<e; s++ ) {
		if( *s < *m ) m = s;
	}
	return m;
}

template<class T> inline
T*	ax_array_max( T* p, size_t n ) {
	if( ! n ) return NULL;
	T* m = p;
	T* s = p + 1;
	T* e = p + n;

	for( ; s<e; s++ ) {
		if( *s > *m ) m = s;
	}
	return m;
}


#endif
