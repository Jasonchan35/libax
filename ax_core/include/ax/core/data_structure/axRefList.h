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
	axRefList( B, inA, list );
};

A a;
B b;

a.list.append( & b.inA );


*/

//Intrusive Reference Node
#define	 axInRefNode( OBJ, NODE ) \
	class NODE : public axDListNode< NODE, false > { \
	public: \
/*		friend class axJOIN_WORD3( axRefList_, OBJ, NODE ); */ \
		OBJ&	obj		() { return ax_member_owner( &OBJ::axInRefNode_##NODE, this ); } \
	}; \
	NODE	axInRefNode_##NODE; \
//-----

#define	axInRefList( OBJ, NODE, ListName ) \
	class axJOIN_WORD( OBJ, NODE ) : public axDList< OBJ::NODE > {\
		typedef axDList< OBJ::NODE > B; \
	public: \
		void	insert	( OBJ * obj ) { B::insert( &(obj->axInRefNode_##NODE) ); } \
		void	append	( OBJ * obj, OBJ * before = nullptr ) { \
			B::append( &(obj->axInRefNode_##NODE), before ? &(before->axInRefNode_##NODE) : nullptr ); \
		} \
	}; \
	axJOIN_WORD( OBJ, NODE )	ListName; \
//-----


#endif
