//
//  axRefArray.h
//  ax_core
//
//  Created by Jason on 2013-10-01.
//
//

#ifndef ax_core_axRefArray_h
#define ax_core_axRefArray_h


template<class T, bool OBJECT_OWNER, size_t LOCAL_BUF_SIZE> class axRefArray;

template<class T, bool OBJECT_OWNER, size_t LOCAL_BUF_SIZE>
class axRefArrayElement : public axRef<T> {
	typedef axRefArray<T, OBJECT_OWNER, LOCAL_BUF_SIZE>	RefArray;
public:
	axRefArrayElement() { _refArray = nullptr; }

	virtual void	onWillRemoveFromList() {
		axSize	idx;
		if( !_refArray->getIndexOf( idx, *this ) ) {
			assert(false);
			return;
		}
		_refArray->remove( idx );
	}
	
protected:
	friend class axRefArray<T, OBJECT_OWNER, LOCAL_BUF_SIZE>;

	axPtr< RefArray >	_refArray;
};

template<class T, bool OBJECT_OWNER, size_t LOCAL_BUF_SIZE = 4>
class axRefArray {
public:
	typedef	axRefArrayElement<T, OBJECT_OWNER, LOCAL_BUF_SIZE>	Element;
	
	~axRefArray() { clear(); }

	axRef<T> &	operator[]( size_t i )	{ return arr_[i]; }

	axStatus	append( T & obj ) {
		axStatus st;
		st = arr_.incSize(1);		if( !st ) return st;
		Element &e = arr_.last();
		e._array = this;
		e.ref( obj );
		return 0;
	}
	
	void clear() {
		if( ! OBJECT_OWNER ) return;
		axSize n = arr_.size();
		if( n == 0 ) return;
		axSize i=n-1;
		for(;;) {
			delete arr_[i];
			if( i == 0 ) break;
			i--;
		}
	}

private:
	axArray< Element, LOCAL_BUF_SIZE >	arr_;
};


#endif
