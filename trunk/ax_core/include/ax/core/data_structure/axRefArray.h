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
class axRefArrayElement : public axRef<T> {
	typedef axRefArray<T,LOCAL_BUF_SIZE>	ARRAY;
public:
	axRefArrayElement() { _array = nullptr; }

	virtual void	onWillRemoveFromList() {
		axSize	idx;
		if( !_array->getIndexOf( idx, *this ) ) {
			assert(false);
			return;
		}
		_array->remove( idx );
	}
	
protected:
	friend class axRefArray<T,LOCAL_BUF_SIZE>;

	axPtr< ARRAY >	_array;
};

template<class T, size_t LOCAL_BUF_SIZE = 4>
class axRefArray {
	typedef	axRefArrayElement<T, LOCAL_BUF_SIZE>	E;
public:

	axRef<T> &	operator[]( size_t i )	{ return arr_[i]; }

	axStatus	append( T & obj ) {
		axStatus st;
		st = arr_.incSize(1);		if( !st ) return st;
		E &e = arr_.last();
		e._array = this;
		e.ref( obj );
		return 0;
	}

private:
	axArray< E, LOCAL_BUF_SIZE >	arr_;
};


#endif
