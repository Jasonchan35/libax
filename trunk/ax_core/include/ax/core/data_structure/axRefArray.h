//
//  axRefArray.h
//  ax_core
//
//  Created by Jason on 2013-10-01.
//
//

#ifndef ax_core_axRefArray_h
#define ax_core_axRefArray_h


template<class T, size_t LOCAL_BUF_SIZE> class axRefArray;

template<class T, size_t LOCAL_BUF_SIZE>
class axRefArrayElement : protected axRef<T> {
	typedef axRefArray<T, LOCAL_BUF_SIZE>	RefArray;
	typedef	axRef<T>	B;
public:
	axRefArrayElement( RefArray* arr=nullptr, T* p=nullptr ) { _refArray = arr; B::ref(p); }

	virtual void	onWillRemoveFromList() {
		axSize	idx;
		if( !_refArray->arr_.getIndexOf( idx, *this ) ) { assert(false); return; }
		_refArray->arr_.remove( idx );
	}
	
	axStatus	onTake( axRefArrayElement &src ) { _refArray=src._refArray; return B::onTake(src); }
	
protected:
	void	setFromArray( T* p, RefArray* arr ) {
		B::ref( p );
		_refArray = arr;
	}

	friend class axRefArray<T, LOCAL_BUF_SIZE>;
	axPtr< RefArray >	_refArray;
};

template<class T, size_t LOCAL_BUF_SIZE = 4>
class axRefArray {
public:
	typedef	axRefArrayElement<T, LOCAL_BUF_SIZE>	Element;
	
	~axRefArray() { clear(); }

	T*	operator[]( size_t i )						{ return arr_[i].ptr(); }

	axStatus	setCapacity		( axSize n ) 		{ return arr_.setCapacity(n); }
	axStatus	incReserve		( axSize n ) 		{ return arr_.incReserve(n); }

	axStatus	append			( T* p ) 			{ Element e(this,p); return arr_.appendByTake(e); }
	axStatus	remove			( axSize index ) 	{ return arr_.remove( index ); }
	void		clear			() 					{ arr_.clear(); }

friend class axRefArrayElement<T, LOCAL_BUF_SIZE>;
protected:
	axArray< Element, LOCAL_BUF_SIZE >	arr_;
};

#endif
