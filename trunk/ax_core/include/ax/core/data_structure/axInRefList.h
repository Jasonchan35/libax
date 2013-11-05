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
	axInRefNode( B, inA );
};


class A {
public:
	axInRefList( B, inA, list );
};

A a;
B b;

a.list.append( & b.inA );


*/

//Intrusive Reference Node
#define	 axInRefNode( OBJ, NODE ) \
	class NODE : public axDListNode< NODE, false > { \
	public: \
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
		axStatus getArray( axIArray<OBJ*> & objs ) { \
			axStatus st = objs.resize( B::size() ); if(!st) return st;\
			OBJ::NODE* p = B::head();\
			size_t i=0; \
			for( ; p; p=p->next(), i++ ) { \
				objs[i] = &p->obj(); \
			} \
			return 0; \
		} \
	}; \
	axJOIN_WORD( OBJ, NODE )	ListName; \
//-----


#endif
