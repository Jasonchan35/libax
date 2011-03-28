#pragma once
#ifndef __ax_DList_h__
#define __ax_DList_h__

#include "../Common/Status.h"
#include "../Common/NonCopyable.h"

namespace ax {
//! \addtogroup data_algorithm
//@{


template<class T> class DList;

//! double linked list node
template< class T >
class DListNode : public NonCopyable {
public:
	DListNode();
	~DListNode() { removeFromList(); }

	T*			prev() { return prev_; }
	T*			next() { return next_; }
	DList<T>*	list() const				{ return (DList<T>*) list_; }

	Status	getIndex		( Size &idx );
	void	removeFromList	()				{ if( list() ) list()->remove( (T*)this ); }

	void	onDidAddToList()			{}
	void	onWillRemoveFromList()		{}

	void	setOwnedByDList( bool b )		{ ownedByDList_ = b; }
	bool	isOwnedByDList()				{ return ownedByDList_; }

friend class DList<T>;
private:
	T*		next_;
	T*		prev_;
	void*	list_;
	bool	ownedByDList_;
};

//! double linked list template
template <class T>
class DList : public NonCopyable {
public:
	DList();
	~DList();

	T*		head		() const				{ return (T*)head_; }
	T*		tail		() const				{ return (T*)tail_; }
	T*		getNodeByIndex( Size idx ) const;

	T*		popHead	()							{ T* h = head_; if (h) remove( h ); return h; }

	Status	takeOwnership ( DList<T> &src );

	//! ownership of all node in "src" will be taken
	void	merge		( DList<T> &src );

	void	insert		( T* node );
	void	insert		( T* node, T* before );

	void	append		( T* node );
	void	append		( T* node, T* after );
	void	remove		( T* node, bool call_onDListRemove = true );

	void	clear		();

	Size	size() const						{ return size_; }

private:
	T*		head_;
	T*		tail_;
	Size	size_;
};

//===================== inline ==========================

template<class T>
DListNode<T>::DListNode() {
	list_ = NULL;
	prev_ = next_ = NULL;
	ownedByDList_ = true;
}

template<class T>
Status	DListNode<T> :: getIndex( Size &idx ) {
	if( ! list() ) return Status::invalid_param;
	T* n = list()->head();
	Size i = 0;
	for( ; n; n=n->next(), i++ ) {
		if( n == this ) {
			idx = i;
			return 0;
		}
	}
	return Status::not_found;
}

template<class T>
Status DList<T>::takeOwnership( DList<T> &src ) {
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
DList<T>::DList() {
	head_   = NULL;
	tail_   = NULL;
	size_	= 0;
}

template<class T>
DList<T>::~DList() {
	clear();
}

template<class T>
void DList<T>::clear() {
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
void DList<T>::insert( T *node, T *before ) {
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
void DList<T>::insert( T *node ) {
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
void DList<T>::append( T *node ) {
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
void DList<T>::append( T* node, T* after ) {
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
void DList<T>::remove( T *node, bool call_onDListRemove ) {
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
void DList<T>::merge( DList<T> &src ) {
	for(;;) {
		T *n = src.popHead();
		if( !n ) break;
		append( n );
	}
}

template<class T>
T*	 DList<T>::getNodeByIndex( Size idx ) const {
	T* n = head();
	if( idx >= size() ) return NULL;

	Size i = 0;
	for( ; n; n=n->next(), i++ ) {
		if( i == idx ) return n;
	}
	return NULL;
}

//@}
} // namespace ax

#endif //__ax_DList_h__
