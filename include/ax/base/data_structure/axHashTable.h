#ifndef __axHashTable_h__
#define __axHashTable_h__

#include "axDList.h"
#include "axArray.h"

template< class T > class axHashTable;
template< class T > class axHashTable_List;

template< class T >
class axHashTableNode : public axDListNode<T> {
	typedef axDListNode<T>          B;
    typedef axHashTable_List<T>     List;
public:
	~axHashTableNode()              { removeFromList(); }
    axHashTable_List<T>*  list()    { return (axHashTable_List<T>*) B::list(); }
    void    removeFromList()        { if( list() ) list()->remove( (T*) this ); }
    
	//please add this func in your class
	// uint32_t hashTableValue() = 0;
};

template<class T>
class axHashTable_List : public axDList<T> {
	typedef axDList<T>          B;
    typedef axHashTable<T>      Table;
public:
    axHashTable_List()          { node_.This = this; }
    ~axHashTable_List()         { B::clear(); }
    
friend class axHashTable<T>;
friend class axHashTableNode<T>;
protected:
	void insert ( T* item );
	void append ( T* item );
	void remove ( T* item );
    
    Table*  table_;    
    
    class   Node : public axDListNode< Node > {
        typedef axDListNode< Node > B;
    public:
        Node() { B::setOwnedByList(false); }
        axHashTable_List<T> *This;
    };
    Node    node_;
};

//----------------------------

template < class T >
class axHashTable : public axNonCopyable {
public:
	typedef axHashTable_List<T>	List;
	typedef T	Node;

	axHashTable();
    ~axHashTable();

	T*          getListHead ( uint32_t hash )   { List* list; axStatus st = getList( list, hash ); return st ? list->head() : NULL; }

	axStatus	insert		( T* item )			{ List* list; axStatus st = getList( list, item->hashTableValue() ); if( !st ) return st; list->insert( item ); return 0; }
	axStatus	append		( T* item )			{ List* list; axStatus st = getList( list, item->hashTableValue() ); if( !st ) return st; list->append( item ); return 0; }
	axStatus	remove		( T* item )			{ List* list; axStatus st = getList( list, item->hashTableValue() ); if( !st ) return st; list->remove( item ); return 0; }

	void        clear       ();
    
	axSize      nodeCount   () const			{ return nodeCount_; }

	axStatus	setTableSize( axSize table_size );	
    axSize      tableSize   () const			{ return table_.size(); }


	class Iterator : public axNonCopyable {
		public:

		Iterator ( const axHashTable &table ) : table_(table)		{ reset(); }

		void	reset	();
		bool	goNext	();

			  Node* node()				{ return  p_; }
		const Node* node() const		{ return  p_; }

			  Node* operator->()		{ return  p_; }
		const Node* operator->() const	{ return  p_; }

			  Node& operator* ()		{ return *p_; }
		const Node& operator* () const	{ return *p_; }

		operator Node*()				{ return  p_; }
		operator const Node*() const	{ return  p_; }

	private:
		const axHashTable		&table_;
		axHashTable_List<T>*	list_;
		Node*                   p_;
	};

friend class axHashTable_List<T>;
protected:
	void	_decNodeCount() { nodeCount_--; }
	void	_incNodeCount() { nodeCount_++; }
    
private:
   	axArray< List >  table_;
    axDList< typename axHashTable_List<T>::Node >   nonEmptyList_;
	axSize		nodeCount_;

	axStatus	getList( List* & list, uint32_t hash )   { 
		axStatus st;
		if( table_.size() == 0 ) { 
			st = setTableSize( 64 ); 
			list = NULL;
			if( !st ) return st;
		}
		list = &table_[ hash % table_.size() ]; 
		return 0;
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
            tmpList.append( p );
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
        append( p );
    }
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
	if( B::size() == 0 ) {
        table_->nonEmptyList_.insert( &node_ );
    }
    B::insert(item);
	table_->_incNodeCount();
}

template < class T >
void axHashTable_List<T> :: append( T* item ) {
	if( B::size() == 0 ) {
        table_->nonEmptyList_.append( &node_ );
    }
    B::append(item);
	table_->_incNodeCount();
}

template < class T >
void axHashTable_List<T> :: remove( T* item ) {
    B::remove(item);
	table_->_decNodeCount();
	if( B::size() == 0 ) {//only remove from nonEmptyList when no more node
		table_->nonEmptyList_.remove( &node_ );
    }
}

template< class T >
void axHashTable<T> :: Iterator :: reset() {
	list_ = table_.nonEmptyList_.head()->This; 
	p_ = list_ ? list_->head() : NULL;
}


template< class T >
bool axHashTable<T> :: Iterator :: goNext() {
	if( p_ ) {
		p_ = p_->next();
		if( p_ ) return true;
	}

	if( ! list_ ) return false;
	if( ! list_->node_.next() ) return false;
	list_ = list_->node_.next()->This;
    
	if( ! list_ ) return false;

	p_ = list_->head();
	return true;
}

#endif //__axHashTable_h__
