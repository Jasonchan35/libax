#ifndef __axAndroid_h__
#define __axAndroid_h__

#include "../../ax_base.h"
#include "../../core/math/axRect2.h"
#ifdef axOS_Android

#include <jni.h>

class axAndroid : public axNonCopyable {
public:
	static	axAndroid*	getInstance	();

	axAndroid();
	axStatus	_create		( JNIEnv* env, jobject activity );
	void		_destroy	();

	jstring		toJString	( const char* 	 sz );
	jstring		toJString	( const wchar_t* sz );

	axStatus	getJString	( axIStringA &out, jstring str );
	axStatus	getJString	( axIStringA &out, jobject str ) { return getJString( out, (jstring)str ); }

	axStatus	getJString	( axIStringW &out, jstring str );
	axStatus	getJString	( axIStringW &out, jobject str ) { return getJString( out, (jstring)str ); }

	axRect2i	getRect2i	( jobject rect );
	jobject		toRect		( const axRect2i& rect );

	const char*	resourcePath();

	static		axTHREAD_LOCAL	JNIEnv*		jni;

	jclass		jni_Activity_class;
	jobject		jni_Activity;
	jmethodID	jni_Activity_finish;
	jmethodID	jni_Activity_getPackageResourcePath;


	jclass		jni_SystemClock_class;
	jmethodID	jni_SystemClock_uptimeMillis;
	jmethodID	jni_SystemClock_elapsedRealtime;

	jclass		jni_Bitmap_class;
	jmethodID	jni_Bitmap_getPixels;

	jclass		jni_Rect_class;
	jmethodID	jni_Rect_init;
	jfieldID	jni_Rect_left;
	jfieldID	jni_Rect_right;
	jfieldID	jni_Rect_top;
	jfieldID	jni_Rect_bottom;

	axStringA	resourcePath_;

	template<class T>
	class	JNILocal<T> {
	public:
		Local( T p = NULL ) { o_=p; }
		void operator=( T p ) { o_=p; }
		operator T	() { return o_; }
		T operator->() { return o_; }

		~Local() {
			if(o_) getInstance()->jni->DeleteLocalRef(o_);
		}
	private:
		T	o_;
	};
};


#endif //axOS_Android

#endif //__axAndroid_h__
