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
				T* operator->()			{ return  get(); }
		const	T* operator->() const	{ return  get(); }
		
				T& operator* ()			{ return *get(); }
		const	T& operator* () const	{ return *get(); }
		
		operator		T*()			{ return  get(); }
		operator const	T*() const		{ return  get(); }

	private:
		T* get() const {
			if( ! p ) { //might init by other EXE/DLL
				printf( "%s\n", axPRETTY_FUNC_NAME);
				static T t;
				const_cast<T*&>(p) = &t;
			}
			return p;
		}

		T* p; // will be init to zero cause static, and also share between DLL/EXE so don't try to init to NULL here
	};
	static Instance instance;
private:
};

#if axCOMPILER_VC
	#define axSingleton_BODY( T )	axSingleton<T>::Instance	axSingleton<T>::instance;
#else
	template<class T> typename axSingleton<T>::Instance axSingleton<T>::instance;
	#define axSingleton_BODY( T )  //
//	#define axSingleton_BODY( T )	template<class T> typename axSingleton<T>::Instance axSingleton<T>::instance;
#endif

//shared symbol between DLL/EXE
//template<class T> axDLL_EXPORT	T* axSingleton<T>::instance;


#endif
