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

	static T& getInstance() {
		static T t;
		return t;
	}
};


template<class T>
axDLLEXPORT T & axSingleton<T>::instance = axSingleton<T>::getInstance();


#endif
