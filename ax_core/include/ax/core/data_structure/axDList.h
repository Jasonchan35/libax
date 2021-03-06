#pragma once
#ifndef __axDList_h__
#define __axDList_h__

#include "../common/ax_common.h"
#include "axAutoPtr.h"

class axStringFormat;

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

	axALWAYS_INLINE( 		T*	prev() 				) { return _prev_; }
	axALWAYS_INLINE( 		T*	next() 				) { return _next_; }

	axALWAYS_INLINE( const	T*	prev() const 		) { return _prev_; }
	axALWAYS_INLINE( const	T*	next() const 		) { return _next_; }
	
	
	axALWAYS_INLINE( void 		toListHead	() 		) { axDList<T>* a=list(); if(a){ a->remove((T*)this); a->insert((T*)this); } }
	axALWAYS_INLINE( void 		toListTail	() 		) { axDList<T>* a=list(); if(a){ a->remove((T*)this); a->append((T*)this); } }

	axDList<T>*	list() const                    { return (axDList<T>*) _list_; }
	void*		listOwner() const				{ return list() ? list()->owner() : NULL; }

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
	

	class	iterator {
	public:
		iterator( T* p=nullptr ) : p_(p) {}
		operator T*			()  { return p_; }
		T&		operator*	()	{ return *p_; }
		T*		operator->	()	{ return  p_; }
		void	operator++	()	{ if(p_) p_=p_->next(); }
		bool	operator==	( const iterator & rhs )	{ return p_ == rhs.p_; }
		bool	operator!=	( const iterator & rhs )	{ return p_ != rhs.p_; }
	private:
		T*	p_;
	};
	
	iterator	begin	()	{ return iterator( head() ); }
	iterator	end		()	{ return iterator( nullptr ); }
	
	class	const_iterator {
	public:
		const_iterator( const T* p=nullptr ) : p_(p) {}

		operator const T*		()  { return p_; }
		const T&	operator*	()	{ return *p_; }
		const T*	operator->	()	{ return  p_; }
		void		operator++	()	{ if(p_) p_=p_->next(); }
		bool		operator==	( const const_iterator & rhs )	{ return p_ == rhs.p_; }
		bool		operator!=	( const const_iterator & rhs )	{ return p_ != rhs.p_; }
	private:
		const T*	p_;
	};
	
	const_iterator	begin	() const	{ return const_iterator( head() ); }
	const_iterator	end		() const	{ return const_iterator( nullptr ); }
	

	void*	owner		() const				{ return _owner_; }
	void	setOwner	( void* o )				{ _owner_ = o; }

	T*		getNodeByIndex( axSize idx ) const;
	
	axALWAYS_INLINE( 	axSize		size			() const )	{ return _size_; }
	axALWAYS_INLINE( 	T*			head			() const )	{ return _head_; }
	axALWAYS_INLINE( 	T*			tail			() const )	{ return _tail_; }

	axALWAYS_INLINE( 	T*			takeHead		( bool call_onWillRemoveFromList=true )	) { T* h = _head_; if (h) remove(h,call_onWillRemoveFromList); return h; }
	axALWAYS_INLINE( 	T*			takeTail		( bool call_onWillRemoveFromList=true )	) { T* h = _tail_; if (h) remove(h,call_onWillRemoveFromList); return h; }
	
	axALWAYS_INLINE(	void		insert			( T* node )	)				{ append( node, _head_ ); }
	axALWAYS_INLINE(	void		append			( T* node, T* before = NULL ) );
	
	axALWAYS_INLINE(	void		appendByTake	( axDList<T> &src ) );
	axALWAYS_INLINE(	axStatus	appendByCopy	( const axDList<T> &src ) );
	axALWAYS_INLINE(	axStatus	copy			( const axDList<T> &src ) ) { clear(); return appendByCopy( src ); }
		
	axALWAYS_INLINE(	void		remove			( T* node, bool call_onWillRemoveFromList=true ) );
	axALWAYS_INLINE(	void		removeAll		() );
	
	axALWAYS_INLINE(	void		clear			() );
	
	axALWAYS_INLINE(	axStatus	serialize_io	( axSerializer		&se ) );
	axALWAYS_INLINE(	axStatus	serialize_io	( axDeserializer	&se ) );
	
	axALWAYS_INLINE(	axStatus	toStringFormat	( axStringFormat &f ) const );
	axALWAYS_INLINE(	axStatus	onTake			( axDList<T> &src ) );

						void		sort( bool (T::*largerThan)( const T & a ), bool ascending );

private:
	T*		_head_;
	T*		_tail_;
	axSize	_size_;
	void*	_owner_;
};

//===================== inline ==========================

template<class T, bool OwnedByList> inline
axDListNode<T,OwnedByList>::axDListNode() {
	_list_ = NULL;
	_prev_ = _next_ = NULL;
}


template<class T, bool OwnedByList> inline
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

template<class T> inline
axStatus axDList<T>::onTake( axDList<T> &src ) {
	clear();
	_head_ = src._head_;
	_tail_ = src._tail_;
	_size_ = src._size_;
	_owner_ = src._owner_;
	
	src._head_ = NULL;
	src._tail_ = NULL;
	src._size_ = 0;
	
	T* p = _head_;
	for( ;p; p=p->next() ) {
		p->_list_ = this;
	}
	return 0;
}

template<class T> inline
axDList<T>::axDList() {
	_head_   = NULL;
	_tail_   = NULL;
	_owner_	 = NULL;
	_size_	 = 0;
}

template<class T> inline
axDList<T>::~axDList() {
	clear();
}

template<class T> inline
void axDList<T>::clear() {
	T *n;
	for( ;; ) {
		n = _tail_;     if( ! n ) return;
        n->removeFromList();
		if( n->onIsOwnedByList() )
			delete n;
	}
}

template<class T> inline
void axDList<T>::append( T *node, T *before ) {
	if( !node )					{ assert( false ); return; }
	if( node->_list_ )			{ assert( false ); return; } //already in other List ?

	if( before ) {
		if( before->_list_ != this ) { assert(false);	return; }
		node->_prev_ = before->_prev_;
		node->_next_ = before;
		
		if( before->_prev_ ) {
			before->_prev_->_next_ = node;
		}else{
			_head_ = node;
		}
		before->_prev_ = node;
	}else{ //append to end
		node->_next_ = NULL;
		node->_prev_ = _tail_;
		if( _tail_ ) {
			_tail_->_next_ = node;
			_tail_ = node;
		}else{
			_head_ = _tail_ = node;
		}
	}

	_size_++;
	node->_list_ = this;
	node->onDidAddToList();
}

template<class T> inline
void axDList<T>::remove( T *node, bool call_onWillRemoveFromList ) {
	if( !node )	{ assert( false ); return; }
	if( node->list() != this ) {
		assert( false ); return;
	}  // node is not belongs to this list !!

	_size_--;
	
	if( call_onWillRemoveFromList ) {
		node->onWillRemoveFromList();
	}

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

template<class T> inline
void axDList<T>::removeAll() {
	while( _head_ ) {
		remove( _head_ );
	}
}

template<class T> inline
void axDList<T>::appendByTake( axDList<T> &src ) {
	for(;;) {
		T *n = src.takeHead();
		if( !n ) break;
		append( n );
	}
}

template<class T> inline
T*	 axDList<T>::getNodeByIndex( axSize idx ) const {
	T* n = head();
	if( idx >= size() ) return NULL;

	axSize i = 0;
	for( ; n; n=n->next(), i++ ) {
		if( i == idx ) return n;
	}
	return NULL;
}

template<class T> inline
axStatus axDList<T>::appendByCopy( const axDList<T> &src ) {
	axStatus st;
	T *n = src.head();
	for(;n;n=n->next()) {
		axAutoPtr< T > np( st );
		if( !st ) return st;
		st = ax_copy( *np, *n ); if( !st ) return st;
		append( np.unref() );
	}
	return 0;
}


template<class T> inline
void axDList<T>::sort( bool (T::*largerThan)( const T & a ), bool ascending ) {
	axDList<T>	tmp;

	for(;;) {
		T* p = takeHead( false );
		if( !p ) break;
		tmp.append(p);
	}
	
	for(;;) {
		T* p = tmp.takeHead( false );
		if( !p ) break;
		
		T* before = nullptr;
		for( auto & t : tmp ) {
			if( ascending ) {
				if( p.largerThan( t ) ) continue;
			}else{
				if( t.largerThan( p ) ) continue;
			}
			before = t;
		}
		
		tmp.append( p, before );
	}
	
}

//@}

#endif //__axDList_h__

