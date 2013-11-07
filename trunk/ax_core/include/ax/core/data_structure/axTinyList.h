#ifndef __axTinyList_h__
#define __axTinyList_h__

#include "../common/ax_common.h"
#include "../string/axStringFormat.h"

class axStringFormat;
class axSerializer;
class axDeserializer;


template<class T> class axTinyList;

template<class T, bool OwnedByList>
class axTinyListNode : public axNonCopyable {
public:
	typedef	axTinyList<T>	List;

	axTinyListNode();
	~axTinyListNode()		{ removeFromList(); }

	axALWAYS_INLINE(		T*	prev()			)	{ return _prev_; }
	axALWAYS_INLINE(		T*	next()			)   { return _next_; }

	axALWAYS_INLINE( const	T*	prev() const	)	{ return _prev_; }
	axALWAYS_INLINE( const	T*	next() const	)   { return _next_; }

	axTinyList<T>*	list() const				{ return (axTinyList<T>*) _list_; }

	void        removeFromList	()				{ if( list() ) list()->remove( (T*)this ); }

//--- function for override ---
	void        onDidAddToList  ()              {}
	void        onWillRemoveFromList()          {}
	bool       	onIsOwnedByList  ()				{ return OwnedByList; }

friend class axTinyList<T>;
protected:
	T*		_prev_;
	T*		_next_;
	void*	_list_;
};

template<class T>
class axTinyList : public axNonCopyable {
public:
	axTinyList();
	~axTinyList()		{ clear(); }

	class	iterator {
	public:
		iterator( T* p=nullptr ) : p_(p) {}
		T&		operator*	()	{ return *p_; }
		T*		operator->	()	{ return  p_; }
		void	operator++	()	{ p_ = p_ ? p_->next() : nullptr; }
		bool	operator==	( const iterator & rhs )	{ return p_ == rhs.p_; }
		bool	operator!=	( const iterator & rhs )	{ return p_ != rhs.p_; }
	private:
		T*	p_;
	};
	
	iterator	begin	()	{ return iterator( head() ); }
	iterator	end		()	{ return iterator( nullptr ); }

	axALWAYS_INLINE( 		T*	head		() )			{ return _head_; }
	axALWAYS_INLINE( const	T*	head		() const	)	{ return _head_; }


	axALWAYS_INLINE( 		T*	takeHead	() )	{ T* h = _head_; if (h) h->removeFromList(); return h; }

	axALWAYS_INLINE( void		insert	( T* node )	);
	axALWAYS_INLINE( void		remove	( T* node, bool call_onWillRemoveFromList = true ) );
	axALWAYS_INLINE( axStatus	onTake	( axTinyList<T> &src ) );

	void	clear		();

	axStatus	toStringFormat( axStringFormat &f ) const;

private:
	T*	_head_;
};

//---- inline ----
template<class T, bool OwnedByList>
axTinyListNode<T,OwnedByList>::axTinyListNode()	{ 
	_prev_  = NULL;
	_next_  = NULL;
	_list_	= NULL;
}

template<class T>
axTinyList<T>::axTinyList()	{ 
	_head_ = NULL;
}

template<class T>
axStatus axTinyList<T>::onTake( axTinyList<T> &src ) {
	clear();
	_head_ = src._head_;
	src._head_ = NULL;
	
	T* p = _head_;
	for( ;p; p=p->next() ) {
		p->_list_ = this;
	}
	return 0;
}

template<class T>
void axTinyList<T>::clear() {
	T *n;
	for( ;; ) {
		n = _head_;     if( !n ) return;
        n->removeFromList();
		if( n->onIsOwnedByList() )
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

	node->onWillRemoveFromList();
	
	if( node->_prev_ )
		node->_prev_->_next_ = node->_next_;
	else
		_head_ = node->_next_;

	if( node->_next_ )
		node->_next_->_prev_ = node->_prev_;

	node->_prev_ = node->_next_ = NULL;
	node->_list_ = NULL;
}

template<class T>
axStatus axTinyList<T>::toStringFormat( axStringFormat &f ) const {
	const T* p = _head_;
	f.out("[");
	size_t i = 0;
	for( ; p; p=p->next(), i++ ) {
		if( i > 0 ) f.out(", ");
		f.format("{?}", *p );
	}
	f.out("]");
	return 0;
}

#endif //__axTinyList_h__
