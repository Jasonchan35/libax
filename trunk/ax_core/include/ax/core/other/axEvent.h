#ifndef __axEvent_h__
#define __axEvent_h__
#include "../data_structure/axTinyList.h" 

/* !!!! 

This file generated by tools/axEvent_generator.html ( java script ) 

*/



 template<            class RET, class P0=void, class P1=void, class P2=void, class P3=void > class axIFunctor;
 template< class OBJ, class RET, class P0=void, class P1=void, class P2=void, class P3=void > class axFunctor;
 template<            class RET, class P0=void, class P1=void, class P2=void, class P3=void > class axDelegate;
 template<            class RET, class P0=void, class P1=void, class P2=void, class P3=void > class axEvent;


//==== 4 Param(s) ======================================

	template< class RET, class P0, class P1, class P2, class P3 >
	class axIFunctor {
	public:
		virtual ~axIFunctor() {}
		virtual	RET call( P0 p0, P1 p1, P2 p2, P3 p3 ) = 0;
	};
	
//---- Functor 4-----------------------------------

	// Member function
	template< class OBJ, class RET, class P0, class P1, class P2, class P3 >
	class axFunctor : public axIFunctor< RET, P0, P1, P2, P3 > {
	public:
		typedef RET (OBJ::*Func)( P0 p0, P1 p1, P2 p2, P3 p3 );
		axFunctor( OBJ* obj=nullptr, Func func=nullptr ) : obj_(obj), func_(func) {}
	private:
		OBJ* 	obj_;
		Func	func_;
		virtual RET call( P0 p0, P1 p1, P2 p2, P3 p3 ) { return (obj_->*func_)(p0, p1, p2, p3); }
	};
	
	
	// static function
	template< class RET, class P0, class P1, class P2, class P3 >
	class axFunctor< void, RET, P0, P1, P2, P3 > : public axIFunctor< RET, P0, P1, P2, P3 > {
	public:
		typedef RET (*Func)( P0 p0, P1 p1, P2 p2, P3 p3 );
		axFunctor( Func func=nullptr ) : func_(func) {}
	private:
		Func	func_;
		virtual RET call( P0 p0, P1 p1, P2 p2, P3 p3 ) { return (*func_)(p0, p1, p2, p3); }
	};

//---- Delegate 4-----------------------------------

	template< class RET, class P0, class P1, class P2, class P3 >
	class axDelegate : public axReferred, public axTinyListNode< axDelegate< RET, P0, P1, P2, P3 >, false > {
		typedef	axEvent		< RET, P0, P1, P2, P3 > Event;
		typedef	axDelegate	< RET, P0, P1, P2, P3 > Delegate;
		typedef	axTinyListNode< Delegate, false > ListNode;
	public:

		class ObjRef : public axRef< axReferred > {
		public:
			virtual void	onWillRemoveFromList() { delete & owner(); }
			Delegate & owner() { return ax_member_owner( & Delegate::objRef, this ); } 
		};

		ObjRef  objRef;
		axPtr< axIFunctor< RET, P0, P1, P2, P3 > > functor;
		char buf[ sizeof( axFunctor< axUnused, RET, P0, P1, P2, P3> ) ];
	};

//----- Event 4----------------------------------

	template< class RET, class P0, class P1, class P2, class P3 >
	class axEvent {
	public:
		typedef axDelegate< RET, P0, P1, P2, P3 > Delegate; 

		template< class OBJ >
		axStatus	connect ( OBJ* obj, RET (OBJ::*method)(P0 p0, P1 p1, P2 p2, P3 p3) ) {
			axStatus st;
			axPtr< Delegate >	d(st);	if( !st ) return st; 
			d->functor = new(d->buf) axFunctor< OBJ, RET, P0, P1, P2, P3>( obj, method );
			d->objRef.ref( obj ); 
			list.insert(d);
			return 0;
		}

		axStatus	connect ( RET (*func)(P0 p0, P1 p1, P2 p2, P3 p3) ) {
			axStatus st;
			axPtr< Delegate >	d(st);	if( !st ) return st; 
			d->functor = new(d->buf) axFunctor< void, RET, P0, P1, P2, P3>( func );
			list.insert(d);
			return 0;
		}

		Delegate*	head	()	{ return list.head(); }
		Delegate*	tail	()	{ return list.tail(); }

		void		operator()	(P0 p0, P1 p1, P2 p2, P3 p3)	{ call(p0, p1, p2, p3); }
		operator	bool	()					{ return list.head() != nullptr; }

		void	call (P0 p0, P1 p1, P2 p2, P3 p3) {
			axRefIterator< Delegate >	d( list.head() );
			for( ; d; ++d ) {
				d->functor->call(p0, p1, p2, p3);
			}
		}
		axTinyList< Delegate >	list;

	};



//==== 3 Param(s) ======================================

	template< class RET, class P0, class P1, class P2 >
	class axIFunctor< RET, P0, P1, P2, void > {
	public:
		virtual ~axIFunctor() {}
		virtual	RET call( P0 p0, P1 p1, P2 p2 ) = 0;
	};
	
//---- Functor 3-----------------------------------

	// Member function
	template< class OBJ, class RET, class P0, class P1, class P2 >
	class axFunctor< OBJ, RET, P0, P1, P2, void > : public axIFunctor< RET, P0, P1, P2, void > {
	public:
		typedef RET (OBJ::*Func)( P0 p0, P1 p1, P2 p2 );
		axFunctor( OBJ* obj=nullptr, Func func=nullptr ) : obj_(obj), func_(func) {}
	private:
		OBJ* 	obj_;
		Func	func_;
		virtual RET call( P0 p0, P1 p1, P2 p2 ) { return (obj_->*func_)(p0, p1, p2); }
	};
	
	
	// static function
	template< class RET, class P0, class P1, class P2 >
	class axFunctor< void, RET, P0, P1, P2, void > : public axIFunctor< RET, P0, P1, P2, void > {
	public:
		typedef RET (*Func)( P0 p0, P1 p1, P2 p2 );
		axFunctor( Func func=nullptr ) : func_(func) {}
	private:
		Func	func_;
		virtual RET call( P0 p0, P1 p1, P2 p2 ) { return (*func_)(p0, p1, p2); }
	};

//---- Delegate 3-----------------------------------

	template< class RET, class P0, class P1, class P2 >
	class axDelegate< RET, P0, P1, P2, void > : public axReferred, public axTinyListNode< axDelegate< RET, P0, P1, P2, void >, false > {
		typedef	axEvent		< RET, P0, P1, P2, void > Event;
		typedef	axDelegate	< RET, P0, P1, P2, void > Delegate;
		typedef	axTinyListNode< Delegate, false > ListNode;
	public:

		class ObjRef : public axRef< axReferred > {
		public:
			virtual void	onWillRemoveFromList() { delete & owner(); }
			Delegate & owner() { return ax_member_owner( & Delegate::objRef, this ); } 
		};

		ObjRef  objRef;
		axPtr< axIFunctor< RET, P0, P1, P2, void > > functor;
		char buf[ sizeof( axFunctor< axUnused, RET, P0, P1, P2, void> ) ];
	};

//----- Event 3----------------------------------

	template< class RET, class P0, class P1, class P2 >
	class axEvent< RET, P0, P1, P2, void > {
	public:
		typedef axDelegate< RET, P0, P1, P2, void > Delegate; 

		template< class OBJ >
		axStatus	connect ( OBJ* obj, RET (OBJ::*method)(P0 p0, P1 p1, P2 p2) ) {
			axStatus st;
			axPtr< Delegate >	d(st);	if( !st ) return st; 
			d->functor = new(d->buf) axFunctor< OBJ, RET, P0, P1, P2, void>( obj, method );
			d->objRef.ref( obj ); 
			list.insert(d);
			return 0;
		}

		axStatus	connect ( RET (*func)(P0 p0, P1 p1, P2 p2) ) {
			axStatus st;
			axPtr< Delegate >	d(st);	if( !st ) return st; 
			d->functor = new(d->buf) axFunctor< void, RET, P0, P1, P2, void>( func );
			list.insert(d);
			return 0;
		}

		Delegate*	head	()	{ return list.head(); }
		Delegate*	tail	()	{ return list.tail(); }

		void		operator()	(P0 p0, P1 p1, P2 p2)	{ call(p0, p1, p2); }
		operator	bool	()					{ return list.head() != nullptr; }

		void	call (P0 p0, P1 p1, P2 p2) {
			axRefIterator< Delegate >	d( list.head() );
			for( ; d; ++d ) {
				d->functor->call(p0, p1, p2);
			}
		}
		axTinyList< Delegate >	list;

	};



//==== 2 Param(s) ======================================

	template< class RET, class P0, class P1 >
	class axIFunctor< RET, P0, P1, void, void > {
	public:
		virtual ~axIFunctor() {}
		virtual	RET call( P0 p0, P1 p1 ) = 0;
	};
	
//---- Functor 2-----------------------------------

	// Member function
	template< class OBJ, class RET, class P0, class P1 >
	class axFunctor< OBJ, RET, P0, P1, void, void > : public axIFunctor< RET, P0, P1, void, void > {
	public:
		typedef RET (OBJ::*Func)( P0 p0, P1 p1 );
		axFunctor( OBJ* obj=nullptr, Func func=nullptr ) : obj_(obj), func_(func) {}
	private:
		OBJ* 	obj_;
		Func	func_;
		virtual RET call( P0 p0, P1 p1 ) { return (obj_->*func_)(p0, p1); }
	};
	
	
	// static function
	template< class RET, class P0, class P1 >
	class axFunctor< void, RET, P0, P1, void, void > : public axIFunctor< RET, P0, P1, void, void > {
	public:
		typedef RET (*Func)( P0 p0, P1 p1 );
		axFunctor( Func func=nullptr ) : func_(func) {}
	private:
		Func	func_;
		virtual RET call( P0 p0, P1 p1 ) { return (*func_)(p0, p1); }
	};

//---- Delegate 2-----------------------------------

	template< class RET, class P0, class P1 >
	class axDelegate< RET, P0, P1, void, void > : public axReferred, public axTinyListNode< axDelegate< RET, P0, P1, void, void >, false > {
		typedef	axEvent		< RET, P0, P1, void, void > Event;
		typedef	axDelegate	< RET, P0, P1, void, void > Delegate;
		typedef	axTinyListNode< Delegate, false > ListNode;
	public:

		class ObjRef : public axRef< axReferred > {
		public:
			virtual void	onWillRemoveFromList() { delete & owner(); }
			Delegate & owner() { return ax_member_owner( & Delegate::objRef, this ); } 
		};

		ObjRef  objRef;
		axPtr< axIFunctor< RET, P0, P1, void, void > > functor;
		char buf[ sizeof( axFunctor< axUnused, RET, P0, P1, void, void> ) ];
	};

//----- Event 2----------------------------------

	template< class RET, class P0, class P1 >
	class axEvent< RET, P0, P1, void, void > {
	public:
		typedef axDelegate< RET, P0, P1, void, void > Delegate; 

		template< class OBJ >
		axStatus	connect ( OBJ* obj, RET (OBJ::*method)(P0 p0, P1 p1) ) {
			axStatus st;
			axPtr< Delegate >	d(st);	if( !st ) return st; 
			d->functor = new(d->buf) axFunctor< OBJ, RET, P0, P1, void, void>( obj, method );
			d->objRef.ref( obj ); 
			list.insert(d);
			return 0;
		}

		axStatus	connect ( RET (*func)(P0 p0, P1 p1) ) {
			axStatus st;
			axPtr< Delegate >	d(st);	if( !st ) return st; 
			d->functor = new(d->buf) axFunctor< void, RET, P0, P1, void, void>( func );
			list.insert(d);
			return 0;
		}

		Delegate*	head	()	{ return list.head(); }
		Delegate*	tail	()	{ return list.tail(); }

		void		operator()	(P0 p0, P1 p1)	{ call(p0, p1); }
		operator	bool	()					{ return list.head() != nullptr; }

		void	call (P0 p0, P1 p1) {
			axRefIterator< Delegate >	d( list.head() );
			for( ; d; ++d ) {
				d->functor->call(p0, p1);
			}
		}
		axTinyList< Delegate >	list;

	};



//==== 1 Param(s) ======================================

	template< class RET, class P0 >
	class axIFunctor< RET, P0, void, void, void > {
	public:
		virtual ~axIFunctor() {}
		virtual	RET call( P0 p0 ) = 0;
	};
	
//---- Functor 1-----------------------------------

	// Member function
	template< class OBJ, class RET, class P0 >
	class axFunctor< OBJ, RET, P0, void, void, void > : public axIFunctor< RET, P0, void, void, void > {
	public:
		typedef RET (OBJ::*Func)( P0 p0 );
		axFunctor( OBJ* obj=nullptr, Func func=nullptr ) : obj_(obj), func_(func) {}
	private:
		OBJ* 	obj_;
		Func	func_;
		virtual RET call( P0 p0 ) { return (obj_->*func_)(p0); }
	};
	
	
	// static function
	template< class RET, class P0 >
	class axFunctor< void, RET, P0, void, void, void > : public axIFunctor< RET, P0, void, void, void > {
	public:
		typedef RET (*Func)( P0 p0 );
		axFunctor( Func func=nullptr ) : func_(func) {}
	private:
		Func	func_;
		virtual RET call( P0 p0 ) { return (*func_)(p0); }
	};

//---- Delegate 1-----------------------------------

	template< class RET, class P0 >
	class axDelegate< RET, P0, void, void, void > : public axReferred, public axTinyListNode< axDelegate< RET, P0, void, void, void >, false > {
		typedef	axEvent		< RET, P0, void, void, void > Event;
		typedef	axDelegate	< RET, P0, void, void, void > Delegate;
		typedef	axTinyListNode< Delegate, false > ListNode;
	public:

		class ObjRef : public axRef< axReferred > {
		public:
			virtual void	onWillRemoveFromList() { delete & owner(); }
			Delegate & owner() { return ax_member_owner( & Delegate::objRef, this ); } 
		};

		ObjRef  objRef;
		axPtr< axIFunctor< RET, P0, void, void, void > > functor;
		char buf[ sizeof( axFunctor< axUnused, RET, P0, void, void, void> ) ];
	};

//----- Event 1----------------------------------

	template< class RET, class P0 >
	class axEvent< RET, P0, void, void, void > {
	public:
		typedef axDelegate< RET, P0, void, void, void > Delegate; 

		template< class OBJ >
		axStatus	connect ( OBJ* obj, RET (OBJ::*method)(P0 p0) ) {
			axStatus st;
			axPtr< Delegate >	d(st);	if( !st ) return st; 
			d->functor = new(d->buf) axFunctor< OBJ, RET, P0, void, void, void>( obj, method );
			d->objRef.ref( obj ); 
			list.insert(d);
			return 0;
		}

		axStatus	connect ( RET (*func)(P0 p0) ) {
			axStatus st;
			axPtr< Delegate >	d(st);	if( !st ) return st; 
			d->functor = new(d->buf) axFunctor< void, RET, P0, void, void, void>( func );
			list.insert(d);
			return 0;
		}

		Delegate*	head	()	{ return list.head(); }
		Delegate*	tail	()	{ return list.tail(); }

		void		operator()	(P0 p0)	{ call(p0); }
		operator	bool	()					{ return list.head() != nullptr; }

		void	call (P0 p0) {
			axRefIterator< Delegate >	d( list.head() );
			for( ; d; ++d ) {
				d->functor->call(p0);
			}
		}
		axTinyList< Delegate >	list;

	};



//==== 0 Param(s) ======================================

	template< class RET >
	class axIFunctor< RET, void, void, void, void > {
	public:
		virtual ~axIFunctor() {}
		virtual	RET call(  ) = 0;
	};
	
//---- Functor 0-----------------------------------

	// Member function
	template< class OBJ, class RET >
	class axFunctor< OBJ, RET, void, void, void, void > : public axIFunctor< RET, void, void, void, void > {
	public:
		typedef RET (OBJ::*Func)(  );
		axFunctor( OBJ* obj=nullptr, Func func=nullptr ) : obj_(obj), func_(func) {}
	private:
		OBJ* 	obj_;
		Func	func_;
		virtual RET call(  ) { return (obj_->*func_)(); }
	};
	
	
	// static function
	template< class RET >
	class axFunctor< void, RET, void, void, void, void > : public axIFunctor< RET, void, void, void, void > {
	public:
		typedef RET (*Func)(  );
		axFunctor( Func func=nullptr ) : func_(func) {}
	private:
		Func	func_;
		virtual RET call(  ) { return (*func_)(); }
	};

//---- Delegate 0-----------------------------------

	template< class RET >
	class axDelegate< RET, void, void, void, void > : public axReferred, public axTinyListNode< axDelegate< RET, void, void, void, void >, false > {
		typedef	axEvent		< RET, void, void, void, void > Event;
		typedef	axDelegate	< RET, void, void, void, void > Delegate;
		typedef	axTinyListNode< Delegate, false > ListNode;
	public:

		class ObjRef : public axRef< axReferred > {
		public:
			virtual void	onWillRemoveFromList() { delete & owner(); }
			Delegate & owner() { return ax_member_owner( & Delegate::objRef, this ); } 
		};

		ObjRef  objRef;
		axPtr< axIFunctor< RET, void, void, void, void > > functor;
		char buf[ sizeof( axFunctor< axUnused, RET, void, void, void, void> ) ];
	};

//----- Event 0----------------------------------

	template< class RET >
	class axEvent< RET, void, void, void, void > {
	public:
		typedef axDelegate< RET, void, void, void, void > Delegate; 

		template< class OBJ >
		axStatus	connect ( OBJ* obj, RET (OBJ::*method)() ) {
			axStatus st;
			axPtr< Delegate >	d(st);	if( !st ) return st; 
			d->functor = new(d->buf) axFunctor< OBJ, RET, void, void, void, void>( obj, method );
			d->objRef.ref( obj ); 
			list.insert(d);
			return 0;
		}

		axStatus	connect ( RET (*func)() ) {
			axStatus st;
			axPtr< Delegate >	d(st);	if( !st ) return st; 
			d->functor = new(d->buf) axFunctor< void, RET, void, void, void, void>( func );
			list.insert(d);
			return 0;
		}

		Delegate*	head	()	{ return list.head(); }
		Delegate*	tail	()	{ return list.tail(); }

		void		operator()	()	{ call(); }
		operator	bool	()					{ return list.head() != nullptr; }

		void	call () {
			axRefIterator< Delegate >	d( list.head() );
			for( ; d; ++d ) {
				d->functor->call();
			}
		}
		axTinyList< Delegate >	list;

	};





#endif //__axExpandArgList_h__
