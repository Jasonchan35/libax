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
			T* operator->()			{ return & instance; }
	const	T* operator->() const	{ return & instance; }
	
			T& operator* ()			{ return instance; }
	const	T& operator* () const	{ return instance; }
	
	operator		T*()			{ return & instance; }
	operator const	T*() const		{ return & instance; }

	static T& instance;	
private:
	static T& getInstance() {
		static T t;
		return t;
	}
};


//template<class T> axDLL_EXPORT T & axSingleton<T>::instance = axSingleton<T>::getInstance();
template<class T>  T & axSingleton<T>::instance = axSingleton<T>::getInstance();


#endif
