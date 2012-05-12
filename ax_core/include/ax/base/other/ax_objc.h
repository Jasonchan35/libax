#ifdef __OBJC__

#ifndef __ax_objc_h__
#define __ax_objc_h__

#include "../common/axStatus.h"
#include "../common/ax_utility.h"

class	axScope_NSAutoreleasePool : public axNonCopyable {
public:
	axScope_NSAutoreleasePool () { p_ = [[NSAutoreleasePool alloc] init]; }
	~axScope_NSAutoreleasePool() { [ p_ release ]; }
private:
	NSAutoreleasePool* p_;
};


template<class T>
class	axNSObject {
public:
	axNSObject	()				{ p_ = nil; }
	axNSObject	( T p )			{ p_ = p; retain(p); }	
	~axNSObject	()				{ release(); }
	
	void	retain	( T p )			{ release(); p_ = p; [p retain]; } 
	void	release	()				{ if(p_) { [p_ release]; p_ = nil; } }
	
			T obj	()				{ return  p_; }
	const	T obj	() const		{ return  p_; }
	
	operator		T ()			{ return  p_; }
	operator const	T () const		{ return  p_; }
	
	void	operator=( T p )		{ retain(p); }
	
	axStatus	onTake ( axNSObject &src ) { operator=(src);	return 0; }
	
private:
	T	p_;
};




#endif //__ax_objc_h__

#endif //__OBJC__
