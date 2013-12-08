//
//  axInDList.h
//  ax_core
//
//  Created by Jason on 2013-10-01.
//
//

#ifndef ax_core_axInDList_h
#define ax_core_axInDList_h

/*

class B {
public:
	axInDListNode( B, inA );
};


class A {
public:
	axInList( B, inA, list );
};

A a;
B b;

a.list.append( & b.inA );


*/

//Intrusive Reference Node
#define	 axInDListNode( OBJ, NODE ) \
	class axInDListNode_##NODE : public axDListNode< axInDListNode_##NODE, false > { \
	public: \
		OBJ&	obj		() { return ax_member_owner( &OBJ::NODE, this ); } \
	}; \
	axInDListNode_##NODE	NODE; \
	\
	class axJOIN_WORD( NODE, DList ) : public axDList< axInDListNode_##NODE > {\
		typedef axDList< axInDListNode_##NODE > B; \
	public: \
		class	iterator { \
		public: \
			iterator( axInDListNode_##NODE* p=nullptr ) : p_( p ) {} \
			iterator( OBJ* obj ) : p_( obj ? &obj->NODE : nullptr ) {} \
			OBJ&	operator*	()	{ return p_ ?  p_->obj() : *(OBJ*)nullptr; } \
			OBJ*	operator->	()	{ return p_ ? &p_->obj() : nullptr ; } \
			void	operator++	()	{ p_ = p_ ? p_->next() : nullptr; } \
			bool	operator==	( const iterator & rhs )	{ return p_ == rhs.p_; } \
			bool	operator!=	( const iterator & rhs )	{ return p_ != rhs.p_; } \
		private: \
			axInDListNode_##NODE*	p_; \
		}; \
		\
		iterator	begin	()	{ return iterator( B::head() ); } \
		iterator	end		()	{ return iterator(); } \
		\
		void	remove	( OBJ * obj ) { B::remove( &(obj->NODE) ); } \
		void	insert	( OBJ * obj ) { B::insert( &(obj->NODE) ); } \
		void	append	( OBJ * obj, OBJ * before = nullptr ) { \
			B::append( &(obj->NODE), before ? &(before->NODE) : nullptr ); \
		} \
		void	append	( axIArray<OBJ*> & objs, OBJ * before = nullptr ) { \
			for( auto & p: objs ) {  		\
				append( p, before ); 		\
			} \
		} \
		axStatus getArray( axIArray<OBJ*> & objs ) { \
			axStatus st = objs.resize( B::size() ); if(!st) return st;\
			axInDListNode_##NODE* p = B::head();\
			size_t i=0; \
			for( ; p; p=p->next(), i++ ) { \
				objs[i] = &p->obj(); \
			} \
			return 0; \
		} \
	}; \
//-----


#endif
