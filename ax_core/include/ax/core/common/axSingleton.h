//
//  axSingleton.h
//  ax_core
//
//  Created by Jason Chan on 2012-05-13.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ax_core_axSingleton_h
#define ax_core_axSingleton_h

//cross EXE/DLL singleton
template<class T>
class axSingleton {
public:
	static	T*		instance 		()			{ return instance_.get(); }
	static	T*		instanceNoInit	()			{ return instance_.getNoInit(); }
	static	void	setInstance		( T* p )	{ instance_.set( p ); }
private:
	class Instance {
	public:
		void	set( T* p )		{ p_ = p; }
		T*		getNoInit ()	{ return p_; }
		T*		get() {
			if( ! p_ ) { //might init by other EXE/DLL
				static T t;
				p_ = &t;
			}
			return p_;
		}
	private:
		T* volatile p_; // will be init to zero cause static, and also share between DLL/EXE so don't try to init to NULL here
	};
	static 	Instance 	instance_;
};

template<class T> typename axSingleton<T>::Instance 		axSingleton<T>::instance_;



#endif
