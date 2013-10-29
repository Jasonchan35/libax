//
//  axConnection.h
//  ax_core
//
//  Created by Jason on 2012-09-29.
//
//

#ifndef ax_core_axConnection_h
#define ax_core_axConnection_h

template<class T> class axConnection;
template<class T> class axConnector;

//---------------------------

template<class T>
class axConnector {
public:
	class Input : public axNonCopyable {
	public:
		Input() : head_(NULL) {}
	
				T*		head	()			{ return head_; }
		const	T*		head	() const	{ return head_; }
		
				void	insert	( T* conn );
				void	remove	( T* conn );
				void	clear	()			{ while( head_ ) { delete head_; } }
	private:
				T* 		head_;
	};

	class Output : public axNonCopyable {
	public:
		Output() : head_(NULL) {}
						
				T*		head	()			{ return head_; }
		const	T*		head	() const	{ return head_; }
		
				void 	insert	( T* conn );
				void	remove	( T* conn );
				void	clear	()			{ while( head_ ) { delete head_; } }
	private:
				T* 		head_;
	};

				void	clear() 	{ input.clear(); output.clear(); }

	Input		input;
	Output		output;
};


template<class T>
class axConnection {
public:
	typedef	axConnector<T>		List;
	~axConnection();
	
	void removeFromList();
	bool isInList();
	
	class Input : public axNonCopyable {
	public:
		Input() : prev_(NULL), next_(NULL), list_(NULL) {}

				T*	prev() 			{ return prev_; }
		const	T*	prev() const	{ return prev_; }

				T*	next()			{ return next_; }
		const	T*	next() const	{ return next_; }


	friend class List::Input;
	friend class axConnection<T>;
	protected:
				T*	prev_;
				T*	next_;
		typename List::Input*	list_;
	};

	class Output : public axNonCopyable {
	public:
		Output() : prev_(NULL), next_(NULL), list_(NULL) {}

				T*	prev() 			{ return prev_; }
		const	T*	prev() const	{ return prev_; }

				T*	next()			{ return next_; }
		const	T*	next() const	{ return next_; }

		
	friend class List::Output;
	friend class axConnection<T>;
	protected:
				T*	prev_;
				T*	next_;
		typename List::Output*	list_;
	};	
	

	Input		input;
	Output		output;
	
protected:
	void 		connect ( typename List::Input & inList, typename List::Output & outList );
};


//------ inline -------

template<class T> inline
axConnection<T> :: ~axConnection() {
	removeFromList();
}

template<class T> inline
void axConnection<T> :: removeFromList() {
	if( input.list_  ) input.list_ ->remove( (T*)this );
	if( output.list_ ) output.list_->remove( (T*)this );
}

template<class T> inline
bool axConnection<T> :: isInList() {
	if( input.list_ || output.list_ ) return true;
	return false;
}



template<class T> inline
void axConnector<T>::Output :: insert( T* conn ) {
	assert( conn->output.prev_ == NULL );
	assert( conn->output.next_ == NULL );

	if( head_ ) head_->output.prev_ = conn;
	
	conn->output.prev_ = NULL;
	conn->output.next_ = head_;
	conn->output.list_ = this;
	head_ = conn;
}

template<class T> inline
void axConnector<T>::Output :: remove( T* conn ) {
	assert( conn->output.list_ == this );
	
	if( conn->output.prev_ ) {
		conn->output.prev_->output.next_ = conn->output.next_;
	}else{
		head_ = conn->output.next_;
	}

	if( conn->output.next_ ) {
		conn->output.next_->output.prev_ = conn->output.prev_;
	}
	conn->output.next_ = NULL;
	conn->output.prev_ = NULL;
	conn->output.list_ = NULL;
}

template<class T> inline
void axConnector<T>::Input :: insert( T* conn ) {
	assert( conn->input.prev_ == NULL );
	assert( conn->input.next_ == NULL );
	
	if( head_ ) head_->input.prev_ = conn;
	
	conn->input.prev_ = NULL;
	conn->input.next_ = head_;
	conn->input.list_ = this;
	head_ = conn;
}

template<class T> inline
void axConnector<T>::Input :: remove( T* conn ) {
	assert( conn->input.list_ == this );
	
	if( conn->input.prev_ ) {
		conn->input.prev_->input.next_ = conn->input.next_;
	}else{
		head_ = conn->input.next_;
	}

	if( conn->input.next_ ) {
		conn->input.next_->input.prev_ = conn->input.prev_;
	}
	
	conn->input.next_ = NULL;
	conn->input.prev_ = NULL;
	conn->input.list_ = NULL;	
}



#endif
