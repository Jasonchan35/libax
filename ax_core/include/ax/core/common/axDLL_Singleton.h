/*
#ifndef axDLL_Singleton
#define axDLL_Singleton

template<class T>
class axSingleton {
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

template<T>
class axDLL_Singleton {

	T* (*GetInstanceFunc)();

	T* getInstance()	{ 
		if( ! getInstanceFunc ) {
			GetProcAddress( GetMoudleHandle(NULL), 
		}
	}

	axDLL_Singleton()	{ getInstanceFunc = NULL; }
private:
	static GetInstanceFunc getInstanceFunc;
}

#define axDLL_Singleton_Imp(T) \
	T* axDLL_Singleton_getInstnace_##T() { static T t; return &t; } \
	T* T::getInstance()	{ 
		if( ! getInstanceFunc ) {
			getInstanceFunc = (axDLL_Singleton<T>::GetInstanceFunc) GetProcAddress( GetMoudleHandle(NULL), "axDLL_Singleton_getInstnace_"#T );
		}
	}

//======

#endif //axDLL_Singleton
*/

