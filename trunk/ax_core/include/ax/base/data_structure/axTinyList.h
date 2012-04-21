#ifndef __axTinyList_h__
#define __axTinyList_h__

#include "../common/ax_common.h"

class axStringFormat;
class axLenSerializer;
class axSerializer;
class axDeserializer;


template<class T> class axTinyList;

template<class T>
class axTinyListNode : public axNonCopyable {
public:
	axTinyListNode();
	~axTinyListNode()		{ removeFromList(); }

	T*			prev()		{ return _prev_; }
	T*			next()      { return _next_; }

	axTinyList<T>*	list() const				{ return (axTinyList<T>*) _list_; }

	void        removeFromList	()				{ if( list() ) list()->remove( (T*)this ); }

	void        onDidAddToList  ()              {}
	void        onWillRemoveFromList()          {}

	void        setOwnedByList ( bool b )		{ _ownedByList_ = b; }
	bool        isOwnedByList  ()				{ return _ownedByList_; }

friend class axTinyList<T>;
protected:
	T*		_prev_;
	T*		_next_;
	void*	_list_;
	bool	_ownedByList_;
};

template<class T>
class axTinyList : public axNonCopyable {
public:
	axTinyList();
	~axTinyList()		{ clear(); }

	void	clear	();

	T*		head	()	{ return _head_; }

	void	insert	( T* node );
	void	remove	( T* node, bool call_onWillRemoveFromList = true );

private:
	T*	_head_;
};

//---- inline ----
template<class T>
axTinyListNode<T>::axTinyListNode()	{ 
	_prev_  = NULL;
	_next_  = NULL;
	_list_	= NULL;
	_ownedByList_ = true;
}

template<class T>
axTinyList<T>::axTinyList()	{ 
	_head_ = NULL;
}

template<class T>
void axTinyList<T>::clear() {
	T *n;
	for( ;; ) {
		n = _head_;     if( !n ) return;
        n->removeFromList();
		if( n->_ownedByList_ )
			delete n;
	}
}

template<class T>
void axTinyList<T>::insert( T* node ) {
	if( !node )		   { assert( false ); return; }
	if( node->_list_ ) { assert( false ); return; } //node already in list

	node->_next_ = _head_;
	node->_prev_ = NULL;
	node->_list_ = this;
	if( !_head_ ) {
		_head_ = node;
	}else{
		_head_->_prev_ = node;
		_head_ = node;
	}	
	node->onDidAddToList();
}

template<class T>
void axTinyList<T>::remove( T* node, bool call_onWillRemoveFromList ) {
	if( !node )				  { assert( false ); return; }
	if( node->list() != this ) {
		assert( false ); return;
	}  // node is not belongs to this list !!

	if( node->_prev_ )
		node->_prev_->_next_ = node->_next_;
	else
		_head_ = node->_next_;

	if( node->_next_ )
		node->_next_->_prev_ = node->_prev_;

	node->_prev_ = node->_next_ = NULL;
	node->_list_ = NULL;
	node->onWillRemoveFromList();
}

#endif //__axTinyList_h__
