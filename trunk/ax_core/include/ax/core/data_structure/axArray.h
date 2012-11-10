#ifndef __axArray_h__
#define __axArray_h__

#include "axIArray.h"
#include "axLocalBuf.h"

//! \ingroup base_data_structure
//@{


//! Array Class
/*!
	Feature:
	Performance:
	- support local buffer
	- reserve memory by capacity increment when resize
	- auto memory grow by 1.5x of orginal size ( when capacity increment = 0 )
*/
template<class T, size_t LOCAL_BUF_SIZE = 0, size_t CAP_INC = 0 >
class axArray : public axIArray<T>, private axLocalBuf<T, LOCAL_BUF_SIZE > {
	typedef axIArray<T>	B;
	typedef	axLocalBuf<T, LOCAL_BUF_SIZE > BUF;
public:
	axArray() 											{ _ctor(); }
//	axArray( const axArray<T, LOCAL_BUF_SIZE> & src ) 	{ _ctor(); B::copy(src); }

	virtual	~axArray()	{ B::clear(); }


	template<class S>	axStatus	serialize_io	 ( S &s )	{ return B::serialize_io(s); }
	template<class S>	axStatus	serialize_io_vary( S &s )	{ return B::serialize_io_vary(s); }

	virtual	axSize		capacityIncrement		() const		{ return CAP_INC; }
	
			bool		usingLocalBuffer() const { return (void*)B::ptr() == (void*)BUF::_localBufPtr(); }
	
protected:
	virtual	axStatus	onMalloc	( axSize req_size, void* &newPtr, axSize &newCapacity );
	virtual void		onFree		( void* p );

	virtual	bool		_canBeTakenDirectly	 () const { return ! usingLocalBuffer(); }
	virtual bool		_canTakeOtherDirectly() const { return true; }

private:
	void	_ctor();
};


template<class T, size_t N>  inline
axStatus	ax_copy( axArray<T,N> &dst, const axArray<T,N> &src ) {
	return dst.copy( src );
}

// -----------

template<class T, size_t LOCAL_BUF_SIZE, size_t CAP_INC> inline
void axArray< T, LOCAL_BUF_SIZE, CAP_INC >::_ctor() {
	if( LOCAL_BUF_SIZE ) {
		B::_init( BUF::_localBufPtr(), 0, LOCAL_BUF_SIZE );
	}else{
		B::_init( NULL, 0, 0 );
	}
}

template<class T, size_t LOCAL_BUF_SIZE, size_t CAP_INC> inline
axStatus	axArray< T, LOCAL_BUF_SIZE, CAP_INC >::onMalloc( axSize req_size, void* &newPtr, axSize &newCapacity ) {
	if( req_size <= LOCAL_BUF_SIZE ) {
		newPtr = BUF::_localBufPtr();
		newCapacity = LOCAL_BUF_SIZE;
		return 0;
	}

	axSize	n = 0;
	newCapacity = 0;
	if( capacityIncrement() == 0 ) {
		n = ax_max( axSize( B::size() + B::size() / 2 ), req_size ); //auto resize to 1.5x
	}else{
		n = ax_align_multiple( req_size, capacityIncrement() );
	}

	newPtr = ax_malloc( sizeof(T) * n );
	if( ! newPtr ) return axStatus_Std::not_enough_memory;

	newCapacity = n;
	return 0;
}

template<class T, size_t LOCAL_BUF_SIZE, size_t CAP_INC> inline
void axArray< T, LOCAL_BUF_SIZE, CAP_INC >::onFree( void* p ) {
	if( ! usingLocalBuffer() ) {
		ax_free( p );
	}
}

//@}

#endif //__axArray_h__

