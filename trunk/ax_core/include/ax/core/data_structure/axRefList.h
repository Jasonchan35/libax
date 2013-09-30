//
//  axRefList.h
//  ax_core
//
//  Created by Jason on 2013-10-01.
//
//

#ifndef ax_core_axRefList_h
#define ax_core_axRefList_h

/*

class B {
public:
	axRefNode( B, inA );
};


class A {
public:
	axRefList( B, inA )	b_list;
};

A a;
B b;

a.b_list.append( & b.inA );


*/

#define	 axRefNode( OBJ, T ) \
	class axRefNode_##T : public axDListNode< axRefNode_##T, false > { \
	public: \
/*		friend class axJOIN_WORD3( axRefList_, OBJ, T ); */ \
		operator OBJ&	() { return obj(); } \
		OBJ&	obj		() { return ax_class_of( &OBJ::T, this ); } \
	}; \
	axRefNode_##T	T; \
//-----

#define	axRefList( OBJ, T, ListName ) \
	class axJOIN_WORD3( axRefList_, OBJ, _##T ) : public axDList< OBJ::axRefNode_##T > {\
		typedef axDList< OBJ::axRefNode_##T > B; \
	public: \
		OBJ*	head() { return B::head() ? &( B::head()->obj() ) : NULL; } \
	}; \
	axJOIN_WORD3( axRefList_, OBJ, _##T )	ListName; \
//-----

#endif
