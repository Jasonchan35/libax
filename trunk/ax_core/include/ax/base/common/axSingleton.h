//
//  axSingleton.h
//  ax_core
//
//  Created by Jason Chan on 2012-05-13.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ax_core_axSingleton_h
#define ax_core_axSingleton_h

template<class T>
class axSingleton {
	static T& instance;

	T* getInstance() {
		if( !f ) {
#ifdef axOS_WIN
			f = (func)GetProcAddress(GetModuleHandle(NULL),"dummy"); //always get function from .EXE even in .DLL
#endif
		}
		if( f )	return f();
	}
private:
	T* (*func)();
	func f;

	axDLL_EXPORT T* dummy() {
		static T s;
		return &s;
	}
};

#endif
