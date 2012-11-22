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
    
	axStatus	toStringFormat( axStringFormat &f ) const {
		return B::toStringFormat(f);
	}

friend class axHashTable<T>;
friend class axHashTableNode<T,true>;
friend class axHashTableNode<T,false>;
protected:
	void insert ( T* item );
	void remove ( T* item );
    
    Table*  table_;
    
    class   InTable : public axTinyListNode< InTable, false> {
        typedef axTinyListNode< InTable, false > B;
		typedef	axHashTable_List<T> Owner;
    public:
		Owner* getOwner() { return ax_class_of( &Owner::inTable_, this ); }
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

	axSize      nodeCount   () const			{ return nodeCount_; }

	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("{?}", table_ );
	}

	class Iterator : public axNonCopyable {
		typedef	axHashTable_List<T>	List;
	public:

		Iterator ( axHashTable &table ) : table_(table)		{ reset(); }

		void	reset	();
		bool	goNext	();

		void	operator++()			{ goNext(); }
		void	operator++(int)			{ goNext(); }

			  Node* node()				{ return  p_; }
		const Node* node() const		{ return  p_; }

			  Node* operator->()		{ return  p_; }
		const Node* operator->() const	{ return  p_; }

			  Node& operator* ()		{ return *p_; }
		const Node& operator* () const	{ return *p_; }

		operator Node*()				{ return  p_; }
		operator const Node*() const	{ return  p_; }

	private:
		axHashTable	&table_;
		List*		list_;
		Node*		p_;
	};

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
        for( ;; ) {
            p = table_[i].takeHead();
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
        p = tmpList.takeHead();
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
void axHashTable_List<T> :: remove( T* item ) {
    B::remove(item);
	table_->_decNodeCount();
	if( ! B::head() ) {//only remove from nonEmptyList when no more node
		table_->nonEmptyList_.remove( &inTable_ );
    }
}

template< class T >
void axHashTable<T> :: Iterator :: reset() {
	list_ = table_.nonEmptyList_.head()->getOwner(); 
	p_ = list_ ? list_->head() : NULL;
}

template< class T >
bool axHashTable<T> :: Iterator :: goNext() {
	if( p_ ) {
		p_ = p_->next();
		if( p_ ) return true;
	}

	if( ! list_ ) return false;

	typename List::InTable* p = list_->inTable_.next();
	if( !p ) return false;

	list_ = p->getOwner();
	if( ! list_ ) return false;

	p_ = list_->head();
	return true;
}

#endif //__axHashTable_h__
