//
//  axFunctor.h
//  ax_core
//
//  Created by Jason on 2013-09-30.
//
//

#ifndef ax_core_axFunctor_h
#define ax_core_axFunctor_h

template<				 class R=void, class P0=void, class P1=void, class P2=void > class axIFunctor;
template<class OBJ=void, class R=void, class P0=void, class P1=void, class P2=void > class axFunctor;


template<class R, class P0, class P1, class P2 >
class axIFunctor {
public:
	virtual ~axIFunctor() {}
	virtual	R call( P0 p0, P1 p1, P2 p2 ) = 0;
};

// Member function
template<class OBJ, class R, class P0, class P1, class P2>
class axFunctor : public axIFunctor<R, P0, P1, P2> {
public:
	typedef R (OBJ::*Func)( P0, P1, P2 );

	axFunctor( OBJ* obj=nullptr, Func func=nullptr ) : obj_(obj), func_(func) {}

private:
	OBJ* 	obj_;
	Func	func_;
	
	virtual R call( P0 p0, P1 p1, P2 p2 ) { return (obj_->*func_)(p0, p1, p2); }
};

// static function
template< class R, class P0, class P1, class P2 >
class axFunctor<void, R, P0, P1, P2> : public axIFunctor<R, P0, P1, P2> {
public:
	typedef R (*Func)( P0, P1, P2 );
	
	axFunctor( Func func=nullptr ) : func_(func) {}
	Func	func_;
	
	virtual R call( P0 p0, P1 p1, P2 p2 ) { return (*func_)(p0, p1, p2); }
	
};

//============= P0, P1 ===================

template<class R, class P0, class P1 >
class axIFunctor<R,P0,P1,void> {
public:
	virtual ~axIFunctor() {}
	virtual	R call( P0 p0, P1 p1 ) = 0;
};

// Member function
template<class OBJ, class R, class P0, class P1>
class axFunctor<OBJ, R, P0, P1, void> : public axIFunctor<R, P0, P1, void> {
public:
	typedef R (OBJ::*Func)( P0, P1 );

	axFunctor( OBJ* obj=nullptr, Func func=nullptr ) : obj_(obj), func_(func) {}

private:
	OBJ* 	obj_;
	Func	func_;
	
	virtual R call( P0 p0, P1 p1 ) { return (obj_->*func_)(p0, p1); }
};

// static function
template< class R, class P0, class P1 >
class axFunctor<void, R, P0, P1, void> : public axIFunctor<R, P0, P1, void> {
public:
	typedef R (*Func)( P0, P1 );
	
	axFunctor( Func func=nullptr ) : func_(func) {}
	Func	func_;
	
	virtual R call( P0 p0, P1 p1 ) { return (*func_)(p0, p1); }
	
};

//=============== P0 =================

template<class R, class P0>
class axIFunctor<R,P0,void,void> {
public:
	virtual ~axIFunctor() {}
	virtual	R call( P0 p0 ) = 0;
};

// Member function
template<class OBJ, class R, class P0>
class axFunctor<OBJ, R, P0, void, void> : public axIFunctor<R, P0, void, void> {
public:
	typedef R (OBJ::*Func)( P0 );

	axFunctor( OBJ* obj=nullptr, Func func=nullptr ) : obj_(obj), func_(func) {}

private:
	OBJ* 	obj_;
	Func	func_;
	
	virtual R call( P0 p0 ) { return (obj_->*func_)(p0); }
};

// static function
template< class R, class P0 >
class axFunctor<void, R, P0, void, void > : public axIFunctor<R, P0, void, void> {
public:
	typedef R (*Func)( P0 );
	
	axFunctor( Func func=nullptr ) : func_(func) {}
	Func	func_;
	
	virtual R call( P0 p0 ) { return (*func_)(p0); }
	
};

//============= PARAM = void ====================

template<class R>
class axIFunctor<R,void,void,void> {
public:
	virtual ~axIFunctor() {}
	virtual	R call() = 0;
};

// Member function
template<class OBJ, class R>
class axFunctor<OBJ, R, void, void, void> : public axIFunctor<R, void, void, void > {
public:
	typedef R (OBJ::*Func)();

	axFunctor( OBJ* obj=nullptr, Func func=nullptr ) : obj_(obj), func_(func) {}

private:
	OBJ* 	obj_;
	Func	func_;
	
	virtual R call() { return (obj_->*func_)(); }
};

// static function
template< class R >
class axFunctor<void, R, void, void, void> : public axIFunctor<R, void, void, void > {
public:
	typedef R (*Func)();
	
	axFunctor( Func func=nullptr ) : func_(func) {}
	Func	func_;
	
	virtual R call() { return (*func_)(); }
};


#endif
