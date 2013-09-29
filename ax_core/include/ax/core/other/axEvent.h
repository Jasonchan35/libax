#ifndef __anEventHook_h__
#define __anEventHook_h__

#include "../data_structure/axTinyList.h"
#include "axFunctor.h"
/*
example:

class MyEventParam {
public:
	int param1;
	int param2;
};

axEvent< MyEvent >	ev;

axDelegate< MyEvent >	de;

de.hook( ev, obj, &OBJ::func );

MyEventParam p;
ev.call( p );

*/

template< class P0=void, class P1=void, class P2=void > 	class axEvent;
template< class P0=void, class P1=void, class P2=void > 	class axDelegate;


template< class P0, class P1, class P2 >
class axDelegate : public axTinyListNode< axDelegate<P0,P1,P2>, false > {
	typedef	axEvent		<P0,P1,P2>	EV;
	typedef axDelegate	<P0,P1,P2> 	D;
	
	typedef	axTinyListNode< D, false > ListNode;
public:
			
	template<class OBJ>
	void	connect		( EV & ev, OBJ* obj, void (OBJ::*method)(P0,P1,P2) );
	void	connect		( EV & ev, void (*func)(P0,P1,P2) );
	void	disconnect	()		{ ListNode::removeFromList(); }
	
	EV*		event		()		{ return (EV*) ListNode::list();   }

	axStatus	onTake( axDelegate & src );

	void	_call		( P0 p0, P1 p1, P2 p2 ) { if( f ) f->call(p0,p1,p2); }

private:
	axPtr< axIFunctor<void,P0,P1,P2> >	f;
	char buf[ sizeof( axFunctor< axUnused,void,P0,P1,P2> ) ];
};

template< class P0, class P1, class P2 >
class axEvent : private axTinyList< axDelegate<P0,P1,P2> > {
	typedef axDelegate<P0,P1,P2> D;
	typedef axTinyList< D > B;
public:
	void	operator()	( P0 p0, P1 p1, P2 p2 )	{ _call(p0,p1,p2); }
	
	template<class OBJ >
	void	connect	( D & d, OBJ* obj, void (OBJ::*method)(P0,P1,P2) )	{ d.connect( *this, obj, method ); }
	void	connect	( D & d, void (*func)(P0,P1,P2) ) 					{ d.connect( *this, func ); }
	
	bool	hasDelegate	()			{ return B::head() != NULL; }
	void	addDelegate	( D *d )	{ B::insert(d); }
	
	void	_call		( P0 p0, P1 p1, P2 p2 );
};

//=========  axEvent<P0,P1,void>  specialist =======

template< class P0, class P1>
class axDelegate<P0,P1,void> : public axTinyListNode< axDelegate<P0, P1, void>, false > {
	typedef	axTinyListNode< axDelegate<P0,P1,void>, false > ListNode;

public:
	typedef	axEvent<P0,P1,void>		EV;
			
	template<class OBJ>
	void	connect		( EV & ev, OBJ* obj, void (OBJ::*method)(P0,P1) );
	void	connect		( EV & ev, void (*func)(P0,P1) );
	void	disconnect	()		{ ListNode::removeFromList(); }
	
	EV*		event		()		{ return (EV*) ListNode::list();   }

	axStatus	onTake( axDelegate & src );

	void	_call		( P0 p0, P1 p1 ) { if( f ) f->call(p0,p1); }

private:
	axPtr< axIFunctor<void,P0,P1,void> >	f;
	char buf[ sizeof( axFunctor< axUnused,void,P0,P1,void> ) ];
};

template< class P0, class P1 >
class axEvent<P0,P1,void> : private axTinyList< axDelegate<P0,P1,void> > {
	typedef axTinyList< axDelegate<P0,P1,void> > B;
	typedef axDelegate<P0,P1,void> D;
public:
	void	operator()	( P0 p0, P1 p1 )	{ _call(p0,p1); }
	
	template<class OBJ >
	void	connect	( D & d, OBJ* obj, void (OBJ::*method)(P0,P1) ) 	{ d.connect( *this, obj, method ); }
	void	connect	( D & d, void (*func)(P0,P1) ) 						{ d.connect( *this, func ); }
	
	bool	hasDelegate	()			{ return B::head() != NULL; }
	void	addDelegate	( D *d )	{ B::insert(d); }
	
	void	_call		( P0 p0, P1 p1 );
};



//=========  axEvent<P0,void,void>  specialist =======


template< class P0>
class axDelegate<P0,void,void> : public axTinyListNode< axDelegate<P0,void,void>, false > {
	typedef	axEvent		<P0,void,void>	EV;
	typedef axDelegate	<P0,void,void> 	D;
	
	typedef	axTinyListNode< D, false > ListNode;
public:
			
	template<class OBJ>
	void	connect		( EV & ev, OBJ* obj, void (OBJ::*method)(P0) );
	void	connect		( EV & ev, void (*func)(P0) );
	void	disconnect	()		{ ListNode::removeFromList(); }
	
	EV*		event		()		{ return (EV*) ListNode::list();   }

	axStatus	onTake( axDelegate & src );

	void	_call		( P0 p0 ) { if( f ) f->call(p0); }

private:
	axPtr< axIFunctor<void,P0,void,void> >	f;
	char buf[ sizeof( axFunctor< axUnused,void,P0,void,void> ) ];
};

template< class P0 >
class axEvent<P0,void,void> : private axTinyList< axDelegate<P0,void,void> > {
	typedef axDelegate<P0,void,void> D;
	typedef axTinyList< D > B;
public:
	void	operator()	( P0 p0 )	{ _call(p0); }
	
	template<class OBJ >
	void	connect	( D & d, OBJ* obj, void (OBJ::*method)(P0) ) 	{ d.connect( *this, obj, method ); }
	void	connect	( D & d, void (*func)(P0) ) 					{ d.connect( *this, func ); }
	
	bool	hasDelegate	()			{ return B::head() != NULL; }
	void	addDelegate	( D *d )	{ B::insert(d); }
	
	void	_call		( P0 p0 );
};




//=========  axEvent<void,void,void>  specialist =======

template<>
class axDelegate<void,void,void> : public axTinyListNode< axDelegate<void,void,void>, false > {
	typedef	axEvent		<void,void,void>	EV;
	typedef axDelegate	<void,void,void>	D;
	
	typedef	axTinyListNode< D, false > ListNode;
public:
	
	template<class OBJ>
	void	connect		( EV & ev, OBJ* obj, void (OBJ::*method)() );
	void	connect		( EV & ev, void (*func)() );
	
	void	disconnect	()		{ ListNode::removeFromList(); }
	EV*		event		();
	
	axStatus	onTake( axDelegate & src );
	
	void	_call		() { if( f ) f->call(); }

private:
	axPtr< axIFunctor<void,void,void,void> >	f;
	char buf[ sizeof( axFunctor< axUnused,void,void,void,void> ) ];
};

template<>
class axEvent<void,void,void> : private axTinyList< axDelegate<void,void,void> > {
	typedef axDelegate<void,void,void>	D;
	typedef axTinyList< D > B;
public:
	void	operator()()		{ _call(); }
	
	template<class OBJ >
	void	connect	( D & d, OBJ* obj, void (OBJ::*method)() )	{ d.connect( *this, obj, method ); }
	void	connect	( D & d, void (*func)() ) 					{ d.connect( *this, func ); }
		
	bool	hasDelegate	()			{ return B::head() != NULL; }
	void	addDelegate	( D *d )	{ B::insert(d); }
	
	void	_call	();
};

//=================

template<class P0, class P1, class P2> inline
void	axEvent<P0, P1, P2>::_call ( P0 p0, P1 p1, P2 p2 ) {
	D* p=B::head();
	D* next;
	for( ; p; p=next ) { 
		next = p->next(); // *p might be delete after call
		p->_call( p0, p1, p2 );
	} 
}

template<class P0, class P1> inline
void	axEvent<P0, P1, void>::_call ( P0 p0, P1 p1 ) {
	D* p=B::head();
	D* next;
	for( ; p; p=next ) { 
		next = p->next(); // *p might be delete after call
		p->_call( p0, p1 );
	} 
}

template<class P0> inline
void	axEvent<P0, void, void>::_call	( P0 p0 ) {
	D* p=B::head();
	D* next;
	for( ; p; p=next ) { 
		next = p->next(); // *p might be delete after call
		p->_call( p0 );
	} 
}

inline
void	axEvent<void,void,void>::_call () {
	D* p=B::head();
	D* next;
	for( ; p; p=next ) { 
		next = p->next(); // *p might be delete after call
		p->_call();
	} 
}

//--------

template<class P0, class P1, class P2>
template<class OBJ> inline
void	axDelegate<P0,P1,P2>::connect	( EV & ev, OBJ* obj, void (OBJ::*method)(P0,P1,P2) ) {
	disconnect();
	f = new(buf) axFunctor< OBJ, void, P0, P1, P2 >( obj, method );
	ev.addDelegate( this );
}

template<class P0, class P1>
template<class OBJ> inline
void	axDelegate<P0,P1,void>::connect	( EV & ev, OBJ* obj, void (OBJ::*method)(P0,P1) ) {
	disconnect();
	f = new(buf) axFunctor< OBJ, void, P0, void, void >( obj, method );
	ev.addDelegate( this );
}

template<class P0>
template<class OBJ> inline
void	axDelegate<P0,void,void>::connect	( EV & ev, OBJ* obj, void (OBJ::*method)(P0) ) {
	disconnect();
	f = new(buf) axFunctor< OBJ, void, P0, void, void >( obj, method );
	ev.addDelegate( this );
}

template<class OBJ> inline
void	axDelegate<void,void,void>::connect	( EV & ev, OBJ* obj, void (OBJ::*method)() ) {
	disconnect();
	f = new(buf) axFunctor< OBJ, void, void, void, void >( obj, method );
	ev.addDelegate( this );
}


//-----------

template<class P0> inline
void	axDelegate<P0,void,void>::connect	( EV & ev, void (*func)(P0) ) {
	disconnect();
	f = new(buf) axFunctor< void, void, P0, void, void >( func );
	ev.addDelegate( this );
}


inline
void	axDelegate<void,void,void>::connect	( EV & ev, void (*func)() ) {
	disconnect();
	f = new(buf) axFunctor< void, void, void, void, void >( func );
	ev.addDelegate( this );
}
//----------

template<class P0> inline
axStatus	axDelegate<P0,void,void>::onTake( axDelegate & src ) {
	if( src.event() ) {
		src.event()->addDelegate( this );
	}else{
		disconnect();
	}
	return 0;
}

inline
axStatus	axDelegate<void,void,void>::onTake( axDelegate & src ) {
	if( src.event() ) {
		src.event()->addDelegate( this );
	}else{
		disconnect();
	}
	return 0;
}

//-----
inline
axEvent<void,void,void>*	axDelegate<void,void,void>::event		()		{ return (EV*) ListNode::list();   }


#endif //__anEventHook_h__
