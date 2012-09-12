//
//  axStringFormat_Arg.h
//  ax_core
//
//  Created by Jason Chan on 2012-09-12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ax_core_axStringFormat_Arg_h
#define ax_core_axStringFormat_Arg_h

enum { axStringFormat_ArgListMaxSize = 16 };

template<class T> axStatus axStringFormat_out( axStringFormat &f, T & v );

//--------
class axStringFormat_FuncClass {
public:
	axStatus func( axStringFormat &f ) { assert(false); return -1; }
	const void* data_;
};
typedef axStatus (axStringFormat_FuncClass::*StringFormat_Func)( axStringFormat &f );

template< class T >
class axStringFormat_FuncClass_T : public axStringFormat_FuncClass {
public:
	axStatus func( axStringFormat &f ) { return axStringFormat_out( f, *data() ); }
	const T* data() { return (const T*)data_; }
};

class axStringFormat_Arg {
public:
	axStringFormat_Arg() { data_=NULL; callback_=NULL; }

	template<class T> 
	axStringFormat_Arg( const T &v )	{ 
		data_ = &v;
		callback_ = static_cast< StringFormat_Func > ( &axStringFormat_FuncClass_T<T>::func );
	}

	axStatus	call( axStringFormat &f ) const {
		axStringFormat_FuncClass	wrapper;
		wrapper.data_ = data_;
		return (wrapper.*callback_)( f );
	}

	axStatus	onTake( axStringFormat_Arg &src ) { operator=( src ); return 0; }

	const void*	data() const { return data_; }
private:
	const void*			data_;
	StringFormat_Func	callback_;
};

class axStringFormat_ArgList : public axLocalArray< axStringFormat_Arg, axStringFormat_ArgListMaxSize > {
	typedef	axLocalArray< axStringFormat_Arg, axStringFormat_ArgListMaxSize >	B;
public:
	axStringFormat_ArgList&	operator<<( const axStringFormat_Arg &a ) { 
		axStatus st;
		
		st = B::append( a );	assert( st );
		return *this; 
	}

private:
	axStatus	toStringFormat( axStringFormat &f ) const { assert(false); return -1; } // ArgList.toString is not allowed
};

#endif
