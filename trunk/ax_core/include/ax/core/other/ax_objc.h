#ifdef __OBJC__

#ifndef __ax_objc_h__
#define __ax_objc_h__

#include "../common/axStatus.h"
#include "../common/ax_utility.h"
#include "../string/ax_string.h"

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

template<class T> inline 
axStatus axStringFormat_out( axStringFormat &f, const axNSObject<T>& value ) { return axStringFormat_out( f, value.obj() ); }

inline
NSString* ax_toNSString( const char *sz ) {
	if( sz == NULL ) return @"";
	return [NSString stringWithUTF8String: sz ];
}

inline
NSString* ax_toNSString( const wchar_t *sz ) {
	if( sz == NULL ) return @"";
	axTempStringA lstr;	
	if( ! lstr.set( sz ) ) return nil;
	return ax_toNSString( lstr );
}

inline
NSString* ax_toNSString( const axUChar *sz ) {
	if( sz == NULL ) return @"";
	axTempStringA lstr;	
	if( ! lstr.set( sz ) ) return nil;
	return ax_toNSString( lstr );
}

#endif //__ax_objc_h__

#endif //__OBJC__
