#ifndef __axLinkedPtrArray_h__
#define __axLinkedPtrArray_h__
/*
template<class T > class axLinkedPtrArray;

template<class T>
class axLinkedPtrArrayNode {
public:
	axLinkedPtrArrayNode() : ptrArr_(NULL), indexInArray_(-1) {}
	~axLinkedPtrArrayNode()		{ removeFromPtrArray(); }

	void removeFromPtrArray() {
		if( ! ptrArr_ ) return;
		ptrArr_->remove( indexInArray_ );
	}

friend class axLinkedPtrArray<T>;
protected:
	axLinkedPtrArray<T>*	ptrArr_;
	size_t	indexInArray_;
};

template<class T>
class axLinkedPtrArray {
public:
	~axLinkedPtrArray() {
		clear();
	}

	void clear() {
		while( size() ) {
			remove(0);
		}
	}

	T*		operator[]	( size_t i )	{ return arr_[i]; }
	size_t	size		()				{ return arr_.size(); }

	void	append	( T &p ) {
		if( p.ptrArr_ == this ) return; //already in list
		p.removeFromPtrArray();
		p.ptrArr_ = this;
		p.indexInArray_ = arr_.size();
		arr_.append( &p );
	}

	void	remove	( size_t i )		{
		T *p = arr_[i];
		if( p->ptrArr_ != this ) 	{ assert(false); return; }
		if( p->indexInArray_ == i ) { assert(false); return; }

		//swap with the last one
		arr_[i] = arr_.last();
		arr_[i]->indexInArray_ = i;
		arr_.decSize(1);

		p->ptrArr_ = NULL;
		p->indexInArray_ = -1;
	}

	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("{?}", (const axIArray<void*>&) arr_ );
	}

private:
	axArray<T*>	arr_;
};
*/
#endif //__axLinkedPtrArray_h__
