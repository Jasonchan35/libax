#ifndef __axAndroid_h__
#define __axAndroid_h__

#include "../../core/math/axRect2.h"
#ifdef axOS_Android

#include <jni.h>

class axAndroid : public axNonCopyable {
public:


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
	class JNILocal {
	public:
		JNILocal( T p = NULL ) { o_=p; }
		void operator=( T p ) { o_=p; }
		
		
		T	get() { return o_; }
		operator T	() { return o_; }
		T operator->() { return o_; }

		~JNILocal() {
			if(o_) axAndroid::instance.jni->DeleteLocalRef(o_);
		}
	private:
		T	o_;
	};
	
	static axAndroid instance;
	

	axStatus findClass( jclass &cls, const char *name );
	axStatus getFieldID( jclass cls, jfieldID &fid, const char *name, const char *sig );
	axStatus getMethodID( jclass cls, jmethodID &mth, const char *name, const char *sig );
	axStatus getStaticMethodID( jclass cls, jmethodID &mth, const char *name, const char *sig );
	
private:

};



#endif //axOS_Android

#endif //__axAndroid_h__
