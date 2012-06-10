#ifndef __anEventHook_h__
#define __anEventHook_h__

#include "../data_structure/axTinyList.h"

/*
example:

class MyEvent {
public:
	int param1;
	int param2;
};

typedef axEventCaster< MyEvent > MyEventCaster;
template<class OBJ> class MyEventFunc : public axEventFunc<OBJ, MyEvent> {};

just using "MyEventFunc<void>" for non-member function

*/

template< class EV > class axEventCaster;
template< class OBJ, class EV > class axEventFunc;

template< class EV >
class axEventHookBase : public axTinyListNode< axEventHookBase<EV> > {
	typedef	axTinyListNode< axEventHookBase<EV> > B;
public:
	virtual	void	call	( EV &ev ) = 0;
			void	unhook	()				{ B::removeFromList(); }

	virtual ~axEventHookBase() {}
	axEventHookBase() { B::setOwnedByList( false ); }
};

template< class EV >
class axEventCaster : private axTinyList< axEventHookBase<EV> > {
	typedef axTinyList< axEventHookBase<EV> > B;
	typedef axEventHookBase<EV> HOOK;
public:
	void	operator()	( EV &ev  )	{ _call(ev); }
	void	_call		( EV &ev  )	{ 
		HOOK* p=B::head();
		HOOK* next;
		for( ; p; p=next ) { 
			next = p->next(); // *p might be delete after call
			p->call( ev ); 
		} 
	}
	bool	hasHook		()			{ return B::head() != NULL; }
	void	_hook		( HOOK *h )	{ B::insert(h); }
};

//class member function callback
template< class OBJ, class EV >
class axEventFunc : public axEventHookBase<EV> {
	typedef	axEventHookBase<EV>		B;
	typedef	axEventCaster<EV>		Caster;
	typedef void (OBJ::*Func)( EV& );
public:
			void	hook	( Caster &caster, OBJ *obj, Func func ) { B::unhook(); obj_=obj; func_=func; caster._hook(this); }	
	virtual	void	call	( EV &ev )								{ (obj_->*func_)(ev); }
	virtual ~axEventFunc	() {}
private:
	OBJ*	obj_;
	Func	func_;
};

//static function callback

template< class EV >
class axEventFunc<void, EV> : public axEventHookBase<EV> {
	typedef	axEventHookBase<EV>		B;
	typedef	axEventCaster<EV>		Caster;
	typedef void (*Func)( EV& );
public:
			void	unhook	()	{ B::removeFromList(); }
			void	hook	( Caster &caster, Func func )	{ B::unhook(); func_=func; caster._hook( this ); }	
	virtual	void	call	( EV &ev )						{ func_(ev); }
	virtual ~axEventFunc() {}
private:
	Func	func_;
};

//=========  axEventCaster<void>  specialist =======

template<> class axEventCaster<void>;

template<>
class axEventHookBase<void> : public axTinyListNode< axEventHookBase<void> > {
	typedef	axTinyListNode< axEventHookBase<void> > B;
public:
	virtual	void	call	() = 0;
			void	unhook	()		{ B::removeFromList(); }

	virtual ~axEventHookBase() {}
	axEventHookBase() { B::setOwnedByList( false ); }
};

template<>
class axEventCaster<void> : private axTinyList< axEventHookBase<void> > {
	typedef axTinyList< axEventHookBase<void> > B;
	typedef axEventHookBase<void>	HOOK;
public:
	void	operator()()		{ _call(); }
	void	_call	()	{ 
		HOOK* p=B::head();
		HOOK* next;
		for( ; p; p=next ) { 
			next = p->next(); // *p might be delete after call
			p->call(); 
		} 
	}
	bool	hasHook		()			{ return B::head() != NULL; }
	void	_hook	( HOOK *h )	{ B::insert(h); }
};

//class member funcion callback
template< class OBJ >
class axEventFunc<OBJ,void> : public axEventHookBase<void> {
	typedef	axEventHookBase<void>	B;
	typedef	axEventCaster<void>		Caster;
	typedef void (OBJ::*Func)();
public:
			void	hook	( Caster &caster, OBJ *obj, Func func ) { unhook(); obj_=obj; func_=func; caster._hook(this); }	
	virtual	void	call	()										{ (obj_->*func_)(); }
	virtual ~axEventFunc	() {}
private:
	OBJ*	obj_;
	Func	func_;
};

//static function callback
template<>
class axEventFunc<void, void> : public axEventHookBase<void> {
	typedef	axEventHookBase<void>		B;
	typedef	axEventCaster<void>		Caster;
	typedef void (*Func)();
public:
			void	hook	( Caster &caster, Func func )	{ unhook(); func_=func; caster._hook( this ); }	
	virtual	void	call	()								{ func_(); }
	virtual ~axEventFunc() {}
private:
	Func	func_;
};


#endif //__anEventHook_h__
