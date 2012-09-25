#ifndef __axLocalBuf_h__
#define __axLocalBuf_h__

//! local buffer to overcome zero size local buf error in Visual C++ 
template<class T, size_t LOCAL_BUF_SIZE = 0>
class axLocalBuf :  public axNonCopyable {
public:
			T& _localBuf	( size_t i )		{ return *(T*) &localByteBuf_[i*sizeof(T)]; }
	const	T& _localBuf	( size_t i ) const	{ return *(T*) &localByteBuf_[i*sizeof(T)]; }
			T* _localBufPtr	()					{ return  (T*)  localByteBuf_; }
	const	T* _localBufPtr	() const			{ return  (T*)  localByteBuf_; }
private:
	char	localByteBuf_[ LOCAL_BUF_SIZE * sizeof(T) ];
};

template<class T>
class axLocalBuf<T,0> {
public:
			T& _localBuf	( size_t i )		{ return *(T*)this; }
	const	T& _localBuf	( size_t i ) const	{ return *(T*)this; }
			T* _localBufPtr	()					{ return  (T*)this; }
	const	T* _localBufPtr	() const			{ return  (T*)this; }
};


#endif //__axLocalBuf_h__

