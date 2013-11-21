#ifndef __axHashTable_h__
#define __axHashTable_h__

#include "axDList.h"
#include "axArray.h"

template< class T > class axHashTable;
template< class T > class axHashTable_List;

template< class T, bool OwnedByList >
class axHashTableNode : public axTinyListNode<T,OwnedByList> {
	typedef axTinyListNode<T,OwnedByList>    B;
    typedef axHashTable_List<T>     List;
public:
	~axHashTableNode()              { removeFromList(); }
    axHashTable_List<T>*  list()    { return (axHashTable_List<T>*) B::list(); }
    void    removeFromList()        { if( list() ) list()->remove( (T*) this ); }
    
	//please add this func in your class
	// uint32_t hashTableValue() = 0;
};

template<class T>
class axHashTable_List : public axTinyList<T> {
	typedef axTinyList<T>       B;
    typedef axHashTable<T>      Table;
public:
    axHashTable_List()          {}
    ~axHashTable_List()         { B::clear(); }
    
	axStatus	onTake	( axHashTable_List & src )	{
		assert( B::head() == nullptr ); //list must be empty when HashTable resize List Array
		return 0;
	}
	
	axStatus	toStringFormat( axStringFormat &f ) const {
		return B::toStringFormat(f);
	}

friend class axHashTable<T>;
friend class axHashTableNode<T,true>;
friend class axHashTableNode<T,false>;
protected:
	void	insert 		( T* item );
	void	remove 		( T* item, bool call_onWillRemoveFromList = true );
	T*		takeHead	( bool call_onWillRemoveFromList = true )		{ T* h = B::head(); if (h) remove(h,call_onWillRemoveFromList); return h; }
	
	
    Table*  table_;
    
    class   InTable : public axTinyListNode< InTable, false> {
        typedef axTinyListNode< InTable, false > B;
		typedef	axHashTable_List<T> List;
    public:
		List & getOwner() { return ax_member_owner( &List::inTable_, this ); }
    };
    InTable   inTable_;
};

//----------------------------

template < class T >
class axHashTable : public axNonCopyable {
public:
	typedef axHashTable_List<T>	List;
	typedef T	Node;

	axHashTable();
    ~axHashTable();

	T*          getListHead ( uint32_t hash )   { List* list = getList( hash ); return list ? list->head() : NULL; }

	axStatus	insert		( T* item );
	axStatus	remove		( T* item )			{ List* list = getList( item->hashTableValue() ); if( ! list ) return -1; list->remove( item ); return 0; }

	void        clear       ();

	axStatus	setTableSize( axSize table_size );	
    axSize      tableSize   () const			{ return table_.size(); }

	axSize      count	   () const				{ return nodeCount_; }

	axStatus	toStringFormat( axStringFormat &f ) const { return f.format("{?}", table_ ); }

	class iterator {
		typedef	axHashTable_List<T>	List;
	public:
		iterator ( T* p=nullptr ) : p_(p) {}

		operator T*()				{ return  p_; }
		T& 		operator* ()		{ return *p_; }
		T* 		operator->()		{ return  p_; }
		void	operator++();
		bool	operator==	( const iterator & rhs )	{ return p_ == rhs.p_; }
		bool	operator!=	( const iterator & rhs )	{ return p_ != rhs.p_; }
			  
	private:
		T*				p_;
	};
	
	iterator	begin	();
	iterator	end		();

friend class axHashTable_List<T>;
protected:
	void	_decNodeCount() { nodeCount_--; }
	void	_incNodeCount() { nodeCount_++; }
    
private:

   	axArray< List, 1 >  table_;
    axTinyList< typename axHashTable_List<T>::InTable >   nonEmptyList_;
	axSize		nodeCount_;

	List*	getList( uint32_t hash )   {
		if( table_.size() == 0 ) return NULL;
		return &table_[ hash % table_.size() ];
	}
};

//@}

//------------- inline -----------------


template < class T >
axHashTable<T>::axHashTable() {
	nodeCount_ = 0;
}

template < class T >
axHashTable<T>::~axHashTable() {
    clear();
}

template < class T >
axStatus axHashTable<T>::setTableSize( axSize table_size ) {
	axStatus st;
    T* p;
    List  tmpList;
	tmpList.table_ = this;

	//move all node to tmpList
	for( axSize i=0; i<table_.size(); i++ ) {
		List & list = table_[i];
        for( ;; ) {
            p = list.takeHead( false );
            if( !p ) break;
            tmpList.insert( p );
        }
    }    
    
	st = table_.resize( table_size );	if( !st ) return st;
	for( axSize i=0; i<table_.size(); i++ ) {
        table_[i].table_ = this;
    }

	//re-insert all node to new list
    for(;;) {
        p = tmpList.takeHead( false );
        if( !p ) break;
        insert( p );
    }
	return 0;
}

template < class T > inline
axStatus axHashTable<T>::insert	 ( T* item ) {
	axStatus st;
	if( tableSize() == 0 ) {
		st = setTableSize(1);		if( !st ) return st;
	}else{
		if( tableSize() < ( nodeCount_ >> 3 ) + 1 ) {
			st = setTableSize( nodeCount_ >> 2 );	if( !st ) return st;
		}
	}

	List* list = getList( item->hashTableValue() );
	list->insert( item );
	
	return 0;
}


template < class T >
void axHashTable<T>::clear() {
	for( axSize i=0; i<table_.size(); i++ ) {
		table_[i].clear();
    }
    nonEmptyList_.clear();
}

template < class T >
void axHashTable_List<T> :: insert( T* item ) {
	if( ! B::head() ) {
        table_->nonEmptyList_.insert( &inTable_ );
    }
    B::insert(item);
	table_->_incNodeCount();
}

template < class T >
void axHashTable_List<T> :: remove( T* item, bool call_onWillRemoveFromList ) {
    B::remove(item, call_onWillRemoveFromList);
	table_->_decNodeCount();
	if( ! B::head() ) {//only remove from nonEmptyList when no more node
		table_->nonEmptyList_.remove( &inTable_ );
    }
}


template< class T > inline
typename axHashTable<T>::iterator axHashTable<T> :: begin () {
	if( nonEmptyList_.head() ) {
		return iterator( nonEmptyList_.head()->getOwner().head() );
	}
	
	return iterator( nullptr );
}

template< class T > inline
typename axHashTable<T>::iterator axHashTable<T> :: end	() {
	return iterator( nullptr );
}


template< class T > inline
void axHashTable<T> :: iterator :: operator++() {
	axHashTable_List<T>* list;
	if( p_ ) {
		list = p_->list();	//current list
		p_ = p_->next();
		if( p_ ) return;
	}

	if( list && list->inTable_.next() ) {
		//try next list
		list = & ( list->inTable_.next()->getOwner() );
		p_ = list->head();
		return;
	}
	
	p_ = nullptr;
}

#endif //__axHashTable_h__
