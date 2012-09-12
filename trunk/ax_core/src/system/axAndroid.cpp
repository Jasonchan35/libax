#include <ax/core/system/axAndroid.h>
#include <ax/core/system/axLog.h>

#ifdef axOS_Android

axAndroid axAndroid::instance;

axTHREAD_LOCAL	JNIEnv*	 axAndroid::jni = NULL;

axAndroid::axAndroid() {
	jni = NULL;
	jni_Activity = NULL;
}

void axAndroid::_destroy() {
	jni = NULL;
}

const char* axAndroid::resourcePath() {
	return resourcePath_;
}

axStatus	axAndroid::_create( JNIEnv* env, jobject activity ) {
 	axStatus	st;
	
	if( jni != NULL ) {
		ax_log("error axAndroid already init: {?}", jni );
		return axStatus_Std::Java_JNI_not_found;
	}
	jni = env;
	jni_Activity = activity;


#define	FindClass( out, name ) \
	out = jni->FindClass( name );	\
	if( !out ) { \
		ax_log("JNI Class({?}) not found", name ); \
		return axStatus_Std::Java_JNI_not_found; \
	}
#define GetFieldID( cls, out, name, sig ) \
	out = jni->GetFieldID( cls, name, sig ); \
	if( !out ) { \
		ax_log("JNI FieldID({?},{?}) not found", name, sig ); \
		return axStatus_Std::Java_JNI_not_found; \
	}
#define GetMethodID( cls, out, name, sig ) \
	out = jni->GetMethodID( cls, name, sig ); \
	if( !out ) { \
		ax_log("JNI GetMethodID({?},{?}) not found", name, sig ); \
		return axStatus_Std::Java_JNI_not_found; \
	}
#define GetStaticMethodID( cls, out, name, sig ) \
	out = jni->GetStaticMethodID( cls, name, sig ); \
	if( !out ) { \
		ax_log("JNI GetStaticMethodID({?},{?}) not found", name, sig ); \
		return axStatus_Std::Java_JNI_not_found; \
	}
//-------

	FindClass			( jni_SystemClock_class, "android/os/SystemClock");
	GetStaticMethodID	( jni_SystemClock_class, jni_SystemClock_uptimeMillis, 	"uptimeMillis", 	"()J" );
	GetStaticMethodID	( jni_SystemClock_class, jni_SystemClock_elapsedRealtime, "elapsedRealtime", 	"()J" );

	FindClass	( jni_Rect_class, "android/graphics/Rect" );
	GetMethodID	( jni_Rect_class, jni_Rect_init, 	 "<init>", "()V");
	GetFieldID	( jni_Rect_class, jni_Rect_left,	 "left",   "I" );
	GetFieldID	( jni_Rect_class, jni_Rect_right,	 "right",  "I" );
	GetFieldID	( jni_Rect_class, jni_Rect_top, 	 "top",	   "I" );
	GetFieldID	( jni_Rect_class, jni_Rect_bottom, "bottom", "I" );

	FindClass	( jni_Bitmap_class, "android/graphics/Bitmap" );
	GetMethodID	( jni_Bitmap_class, jni_Bitmap_getPixels, "getPixels", "([IIIIIII)V");

	FindClass	( jni_Activity_class, "com/awenix/axcore/axGLApp" );
	GetMethodID	( jni_Activity_class, jni_Activity_finish, "finish", "()V" );
	GetMethodID	( jni_Activity_class, jni_Activity_getPackageResourcePath, "getPackageResourcePath", "()Ljava/lang/String;" );
	

//-------
 	jobject res_path = jni->CallObjectMethod( jni_Activity, jni_Activity_getPackageResourcePath );
 	if( ! res_path ) {
 		ax_log("axApp.getPackageResourcePath() error");
 		return -1;
 	}
	
 	st = getJString( resourcePath_, res_path );
 	if( !st ) {
		ax_log("axApp.getPackageResourcePath() error 2");
 		return -1;
 	}
 	ax_log( "resourcePath_={?}", resourcePath_ );
 	return 0;

#undef	FindClass
#undef	GetFieldID
#undef	GetMethodID
#undef	GetStaticMethodID
}

jstring	axAndroid::toJString	( const char* 	 sz ) {
	return jni->NewStringUTF( sz ? sz : "" );
}

jstring	axAndroid::toJString	( const wchar_t* sz ) {
	if( sz ) {
		axStringA_<256>	a;	a.set(sz);
		return jni->NewStringUTF( a );
	}else{
		return jni->NewStringUTF( "" );
	}
}

axStatus axAndroid::getJString ( axIStringA &out, jstring str ) {
	const char* utf = jni->GetStringUTFChars( str, NULL );
	if( !utf ) return -1;
	axStatus st = out.set( utf );
	jni->ReleaseStringUTFChars( str, utf );
	return st;
}

axStatus axAndroid::getJString ( axIStringW &out, jstring str ) {
	const char* utf = jni->GetStringUTFChars( str, NULL );
	if( !utf ) return -1;
	axStatus st = out.set( utf );
	jni->ReleaseStringUTFChars( str, utf );
	return st;
}

axRect2i	axAndroid::getRect2i	( jobject rect ) {
	int left   = jni->GetIntField( rect, jni_Rect_left   );
	int top    = jni->GetIntField( rect, jni_Rect_top    );
	int right  = jni->GetIntField( rect, jni_Rect_right  );
	int bottom = jni->GetIntField( rect, jni_Rect_bottom );
	return axRect2i( left, top, right-left, bottom-top );
}

jobject		axAndroid::toRect		( const axRect2i& rect ) {
	jobject obj = jni->NewObject( jni_Rect_class, jni_Rect_init );
	jni->SetIntField( obj, jni_Rect_left,   rect.x );
	jni->SetIntField( obj, jni_Rect_top,    rect.y );
	jni->SetIntField( obj, jni_Rect_right,  rect.right()  );
	jni->SetIntField( obj, jni_Rect_bottom, rect.bottom() );
	return obj;
}

extern "C" {

JNIEXPORT void Java_com_awenix_axcore_axAndroid_jniOnInit ( JNIEnv* env, jobject obj, jobject activity ) {
	ax_log("================== axAndroid_jniOnInit =================== ");
	axAndroid::instance._create( env, activity );
}

}//extern "C"




#endif // axOS_Android
