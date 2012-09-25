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
	class Instance {
	public:
				T* operator->()			{ return  getInstance(); }
		const	T* operator->() const	{ return  getInstance(); }
		
				T& operator* ()			{ return *getInstance(); }
		const	T& operator* () const	{ return *getInstance(); }
		
		operator		T*()			{ return  getInstance(); }
		operator const	T*() const		{ return  getInstance(); }

	private:
		T* getInstance() {
			if( ! instance ) { //might init by other EXE/DLL
				static T t;
				instance = &t;
			}
			return instance;
		}

		T* instance;	
	};
	static Instance instance;
private:
};

// *instance will be init to NULL cause static, and also share between DLL/EXE so don't try to init to NULL here
#define axSingleton_BODY( T )	axSingleton<T>::Instance	axSingleton<T>::instance;


//shared symbol between DLL/EXE
//template<class T> axDLL_EXPORT	T* axSingleton<T>::instance;


#endif
