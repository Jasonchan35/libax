#pragma once
#ifndef __axDList_h__
#define __axDList_h__

#include "../common/axStatus.h"
#include "../common/axNonCopyable.h"

//! \addtogroup data_algorithm
//@{
template<class T> class axDList;

//! double linked list node
template< class T >
class axDListNode : public axNonCopyable {
public:
	axDListNode();
	~axDListNode() { removeFromList(); }

	T*			prev() { return prev_; }
	T*			next() { return next_; }
	axDList<T>*	list() const				{ return (axDList<T>*) list_; }

	axStatus	getIndex		( axSize &idx );
	void	removeFromList	()				{ if( list() ) list()->remove( (T*)this ); }

	void	onDidAddToList()			{}
	void	onWillRemoveFromList()		{}

	void	setOwnedByDList( bool b )		{ ownedByDList_ = b; }
	bool	isOwnedByDList()				{ return ownedByDList_; }

friend class axDList<T>;
private:
	T*		next_;
	T*		prev_;
	void*	list_;
	bool	ownedByDList_;
};

//! double linked list template
template <class T>
class axDList : public axNonCopyable {
public:
	axDList();
	~axDList();

	T*		head		() const				{ return (T*)head_; }
	T*		tail		() const				{ return (T*)tail_; }
	T*		getNodeByIndex( axSize idx ) const;

	T*		popHead	()							{ T* h = head_; if (h) remove( h ); return h; }

	axStatus	takeOwnership ( axDList<T> &src );

	//! ownership of all node in "src" will be taken
	void	merge		( axDList<T> &src );

	void	insert		( T* node );
	void	insert		( T* node, T* before );

	void	append		( T* node );
	void	append		( T* node, T* after );
	void	remove		( T* node, bool call_onDListRemove = true );

	void	clear		();

	axSize	size() const						{ return size_; }

private:
	T*		head_;
	T*		tail_;
	axSize	size_;
};

//===================== inline ==========================

template<class T>
axDListNode<T>::axDListNode() {
	list_ = NULL;
	prev_ = next_ = NULL;
	ownedByDList_ = true;
}

template<class T>
axStatus	axDListNode<T> :: getIndex( axSize &idx ) {
	if( ! list() ) return axStatus::invalid_param;
	T* n = list()->head();
	axSize i = 0;
	for( ; n; n=n->next(), i++ ) {
		if( n == this ) {
			idx = i;
			return 0;
		}
	}
	return axStatus::not_found;
}

template<class T>
axStatus axDList<T>::takeOwnership( axDList<T> &src ) {
	clear();
	T* item;
	for( ;; ) {
		item = src.head();
		if( !item ) break;
		src.remove( item );
		append( item );
	}
	return 0;
}

template<class T>
axDList<T>::axDList() {
	head_   = NULL;
	tail_   = NULL;
	size_	= 0;
}

template<class T>
axDList<T>::~axDList() {
	clear();
}

template<class T>
void axDList<T>::clear() {
	T *n;
	for( ;; ) {
		n = head_;
		if( ! n ) return;
		remove( n );
		if( n->ownedByDList_ )
			delete n;
	}
}

template<class T>
void axDList<T>::insert( T *node, T *before ) {
	if( !node )					{ assert( false ); return; }
	if( node->_list )			{ assert( false ); return; } //already in other List ?

	if( ! before )				{ assert( false ); return; }
	if( before->_list != this ) {
		assert( false ); return;
	} // before is not node of this list

	size_++;

	if( head_ == before ) {
		head_ = node ;
	}
	
	if( before->prev_ ) {
		before->prev_->next_ = node;
	}
	
	node->prev_ = before->prev_;
	node->next_ = before;
	node->_list = this;

	before->prev_ = node;

	node->onDidAddToList();
}

template<class T>
void axDList<T>::insert( T *node ) {
	if( !node )		  { assert( false ); return; }
	if( node->_list ) { assert( false ); return; } //node already in list

	size_++;
	node->next_ = head_;
	node->prev_ = NULL;
	node->_list = this;
	if( !head_ ) {
		tail_ = head_ = node;
	}else{
		head_->prev_ = node;
		head_ = node;
	}
	node->onDidAddToList();
}

template<class T>
void axDList<T>::append( T *node ) {
	if( !node )		  { assert( false ); return; }
	if( node->list_ ) { assert( false ); return; } //node already in list

	size_++;
	node->next_ = NULL;
	node->prev_ = tail_;
	node->list_ = this;
	if( !tail_ ) {
		tail_ = head_ = node;
	}else{
		tail_->next_ = node;
		tail_ = node;
	}
	node->onDidAddToList();
}

template<class T>
void axDList<T>::append( T* node, T* after ) {
	if( !node )					{ assert( false ); return; }
	if( node->_list )			{ assert( false ); return; } //already in other List ?

	if( ! after )				{ assert( false ); return; }
	if( after->_list != this )  {
		assert( false ); return;
	} // after is not node of this list

	size_++;

	
	if( tail_ == after ) {
		tail_ = node;
	}
	
	node->next_ = after->next_;
	node->prev_ = after;
	after->next_ = node;	
	node->_list = this;

	node->onDidAddToList();
}

template<class T>
void axDList<T>::remove( T *node, bool call_onDListRemove ) {
	if( !node )				  { assert( false ); return; }
	if( node->list() != this ) {
		assert( false ); return;
	}  // node is not belongs to this list !!
	
	size_--;
	if( call_onDListRemove ) node->onWillRemoveFromList();

	if( node->prev_ )
		node->prev_->next_ = node->next_;
	else
		head_ = node->next_;

	if( node->next_ )
		node->next_->prev_ = node->prev_;
	else
		tail_ = node->prev_;

	node->prev_ = node->next_ = NULL;
	node->list_ = NULL;
}

template<class T>
void axDList<T>::merge( axDList<T> &src ) {
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

