#pragma once
#ifndef __axDList_h__
#define __axDList_h__

#include "../common/ax_common.h"

class axStringFormat;

class axLenSerializer;
class axSerializer;
class axDeserializer;



//! \ingroup base_data_structure
//@{
template<class T> class axDList;

//! double linked list node
template< class T, bool OwnedByList >
class axDListNode : public axNonCopyable {
public:
	typedef	axDList<T>	List;

	axDListNode();
	~axDListNode()                              { removeFromList(); }

	axALWAYS_INLINE( T*	prev() )                { return _prev_; }
	axALWAYS_INLINE( T*	next() )                { return _next_; }
	
	axALWAYS_INLINE( void toListHead	() )	{ axDList<T>* a=list(); if(a){ a->remove((T*)this); a->insert((T*)this); } }
	axALWAYS_INLINE( void toListTail	() )	{ axDList<T>* a=list(); if(a){ a->remove((T*)this); a->append((T*)this); } }

	axDList<T>*	list() const                    { return (axDList<T>*) _list_; }

	axStatus	getIndex        ( axSize &idx );
	void        removeFromList	()				{ if( list() ) list()->remove( (T*)this ); }

//--- function for override ---
	void        onDidAddToList  ()              {}
	void        onWillRemoveFromList()          {}
	bool        onIsOwnedByList  ()				{ return OwnedByList; }

friend class axDList<T>;
protected:
	T*		_next_;
	T*		_prev_;
	void*	_list_;
};








//! double linked list template
template <class T>
class axDList : public axNonCopyable {
public:
	axDList();
	~axDList();

	void*	owner		() const				{ return _owner_; }
	void	setOwner	( void* o )				{ _owner_ = o; }

	T*		getNodeByIndex( axSize idx ) const;
	
	axALWAYS_INLINE( axSize	size	() const )	{ return _size_; }
	axALWAYS_INLINE( T*		head	() const )	{ return _head_; }
	axALWAYS_INLINE( T*		tail	() const )	{ return _tail_; }

	axALWAYS_INLINE( T*		takeHead	()	)		{ T* h = _head_; if (h) h->removeFromList(); return h; }

	axALWAYS_INLINE(	void		insert			( T* node ) );
	axALWAYS_INLINE(	void		insert			( T* node, T* before ) );

	axALWAYS_INLINE(	void		append			( T* node ) );
	axALWAYS_INLINE(	void		append			( T* node, T* after ) );
	axALWAYS_INLINE(	void		appendByTake	( axDList<T> &src ) );
	
	axALWAYS_INLINE(	void		remove			( T* node, bool call_onWillRemoveFromList = true ) );
	axALWAYS_INLINE(	void		clear			() );
	
	axALWAYS_INLINE(	axStatus	serialize_io	( axSerializer		&se ) );
	axALWAYS_INLINE(	axStatus	serialize_io	( axDeserializer	&se ) );
	axALWAYS_INLINE(	axStatus	serialize_io	( axLenSerializer	&se ) );
	
	axALWAYS_INLINE(	axStatus	toStringFormat	( axStringFormat &f ) const );
	axALWAYS_INLINE(	axStatus	onTake			( axDList<T> &src ) );

private:
	T*		_head_;
	T*		_tail_;
	axSize	_size_;
	void*	_owner_;
};

//===================== inline ==========================

template<class T, bool OwnedByList>
axDListNode<T,OwnedByList>::axDListNode() {
	_list_ = NULL;
	_prev_ = _next_ = NULL;
}


template<class T, bool OwnedByList>
axStatus	axDListNode<T,OwnedByList> :: getIndex( axSize &idx ) {
	if( ! list() ) return axStatus_Std::invalid_parameter;
	T* n = list()->head();
	axSize i = 0;
	for( ; n; n=n->next(), i++ ) {
		if( n == this ) {
			idx = i;
			return 0;
		}
	}
	return axStatus_Std::not_found;
}

template<class T>
axStatus axDList<T>::onTake( axDList<T> &src ) {
	clear();
	_head_ = src._head_;
	_tail_ = src._tail_;
	_size_ = src._size_;
	_owner_ = _owner_;
	
	src._head_ = NULL;
	src._tail_ = NULL;
	src._size_ = 0;
	
	T* p = _head_;
	for( ;p; p=p->next() ) {
		p->_list_ = this;
	}
	return 0;
}

template<class T>
axDList<T>::axDList() {
	_head_   = NULL;
	_tail_   = NULL;
	_owner_	 = NULL;
	_size_	 = 0;
}

template<class T>
axDList<T>::~axDList() {
	clear();
}

template<class T>
void axDList<T>::clear() {
	T *n;
	for( ;; ) {
		n = _tail_;     if( ! n ) return;
        n->removeFromList();
		if( n->onIsOwnedByList() )
			delete n;
	}
}

template<class T>
void axDList<T>::insert( T *node, T *before ) {
	if( !node )					{ assert( false ); return; }
	if( node->_list_ )			{ assert( false ); return; } //already in other List ?

	if( ! before )				{ assert( false ); return; }
	if( before->_list_ != this ) {
		assert( false ); return;
	} // before is not node of this list

	_size_++;

	if( _head_ == before ) {
		_head_ = node ;
	}

	if( before->_prev_ ) {
		before->_prev_->_next_ = node;
	}

	node->_prev_ = before->prev_;
	node->_next_ = before;
	node->_list_ = this;

	before->_prev_ = node;

	node->onDidAddToList();
}

template<class T>
void axDList<T>::insert( T *node ) {
	if( !node )		   { assert( false ); return; }
	if( node->_list_ ) { assert( false ); return; } //node already in list

	_size_++;
	node->_next_ = _head_;
	node->_prev_ = NULL;
	node->_list_ = this;
	if( !_head_ ) {
		_tail_ = _head_ = node;
	}else{
		_head_->_prev_ = node;
		_head_ = node;
	}
	node->onDidAddToList();
}

template<class T>
void axDList<T>::append( T *node ) {
	if( !node )		   { assert( false ); return; }
	if( node->_list_ ) { assert( false ); return; } //node already in list

	_size_++;
	node->_next_ = NULL;
	node->_prev_ = _tail_;
	node->_list_ = this;
	if( !_tail_ ) {
		_tail_ = _head_ = node;
	}else{
		_tail_->_next_ = node;
		_tail_ = node;
	}
	node->onDidAddToList();
}

template<class T>
void axDList<T>::append( T* node, T* after ) {
	if( !node )					{ assert( false ); return; }
	if( node->_list_ )			{ assert( false ); return; } //already in other List ?

	if( ! after )				{ assert( false ); return; }
	if( after->_list_ != this )  {
		assert( false ); return;
	} // after is not node of this list

	_size_++;


	if( _tail_ == after ) {
		_tail_ = node;
	}

	node->_next_ = after->_next_;
	node->_prev_ = after;
	after->_next_ = node;
	node->_list_ = this;

	node->onDidAddToList();
}

template<class T>
void axDList<T>::remove( T *node, bool call_onWillRemoveFromList ) {
	if( !node )	{ assert( false ); return; }
	if( node->list() != this ) {
		assert( false ); return;
	}  // node is not belongs to this list !!

	_size_--;
	if( call_onWillRemoveFromList ) node->onWillRemoveFromList();

	if( node->_prev_ )
		node->_prev_->_next_ = node->_next_;
	else
		_head_ = node->_next_;

	if( node->_next_ )
		node->_next_->_prev_ = node->_prev_;
	else
		_tail_ = node->_prev_;

	node->_prev_ = node->_next_ = NULL;
	node->_list_ = NULL;
}

template<class T>
void axDList<T>::appendByTake( axDList<T> &src ) {
	for(;;) {
		T *n = src.popHead();
		if( !n ) break;
		append( n );
	}
}

template<class T>
T*	 axDList<T>::getNodeByIndex( axSize idx ) const {
	T* n = head();
	if( idx >= size() ) return NULL;

	axSize i = 0;
	for( ; n; n=n->next(), i++ ) {
		if( i == idx ) return n;
	}
	return NULL;
}

//@}

#endif //__axDList_h__

