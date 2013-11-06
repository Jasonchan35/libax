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
	class axInRefNode_##NODE : public axDListNode< axInRefNode_##NODE, false > { \
	public: \
		OBJ&	obj		() { return ax_member_owner( &OBJ::NODE, this ); } \
	}; \
	axInRefNode_##NODE	NODE; \
	\
	class axJOIN_WORD( NODE, List ) : public axDList< axInRefNode_##NODE > {\
		typedef axDList< axInRefNode_##NODE > B; \
	public: \
		class	iterator { \
		public: \
			iterator( axInRefNode_##NODE* p=nullptr ) : p_( p ) {} \
			iterator( OBJ* obj ) : p_( obj ? &obj->NODE : nullptr ) {} \
			OBJ*	operator*	()	{ return & p_->obj(); } \
			void	operator++	()	{ p_ = p_ ? p_->next() : nullptr; } \
			bool	operator==	( const iterator & rhs )	{ return p_ == rhs.p_; } \
			bool	operator!=	( const iterator & rhs )	{ return p_ != rhs.p_; } \
		private: \
			axInRefNode_##NODE*	p_; \
		}; \
		\
		iterator	begin	()	{ return iterator( B::head() ); } \
		iterator	end		()	{ return iterator(); } \
		\
		void	insert	( OBJ * obj ) { B::insert( &(obj->NODE) ); } \
		void	append	( OBJ * obj, OBJ * before = nullptr ) { \
			B::append( &(obj->NODE), before ? &(before->NODE) : nullptr ); \
		} \
		axStatus getArray( axIArray<OBJ*> & objs ) { \
			axStatus st = objs.resize( B::size() ); if(!st) return st;\
			axInRefNode_##NODE* p = B::head();\
			size_t i=0; \
			for( ; p; p=p->next(), i++ ) { \
				objs[i] = &p->obj(); \
			} \
			return 0; \
		} \
	}; \
//-----


#endif
