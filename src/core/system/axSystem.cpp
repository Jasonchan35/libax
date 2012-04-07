#include <ax/core/system/axSystem.h>
#include <ax/core/system/axAndroid.h>

#if 0
#pragma mark ================= Windows ====================
#endif
#ifdef axOS_WIN

axSize	axSystem::numberOfProcessors() {
	SYSTEM_INFO	info;
	GetSystemInfo( &info );
	return info.dwNumberOfProcessors;
}	

void* axSystem::getCurrentThreadID() {
	return (void*) GetCurrentThreadId();
}

#endif //axOS_WIN

#if axOS_Android
#if 0
#pragma mark ================= Android ====================
#endif

double axSystem::uptime() {
	axAndroid *an = axAndroid::getInstance();
	jlong t = an->jni->CallStaticLongMethod( an->jni_SystemClock_class, an->jni_SystemClock_elapsedRealtime );
	return t/1000.0;
}

#endif //axOS_Android

#if axOS_iOS	
#if 0
#pragma mark ================= iOS ====================
#endif

axSize	axSystem::numberOfProcessors() { 
	return [[NSProcessInfo processInfo] processorCount];	
}

axSize axSystem::availableMemory() {
	vm_statistics_data_t vmStats;
	mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
	kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
	if(kernReturn != KERN_SUCCESS) {
		return -1;
	}
	return vm_page_size * vmStats.free_count;
}

axStatus axSystem::getMachineName( axIStringA &str ) {
	UIDevice* dev = [UIDevice currentDevice];
	str.convert( dev.name );
	return 0;
}

double axSystem::uptime() {
	return [[NSProcessInfo processInfo] systemUptime];	
}



	
#endif //axOS_iOS

#if axOS_MacOSX
#if 0
#pragma mark ================= Mac OS X ====================
#endif

axSize	axSystem::numberOfProcessors() { 
	return [[NSProcessInfo processInfo] processorCount];	
}

axStatus axSystem::getMachineName( axIStringA &str ) {
	axStatus st;
	size_t size;
	//get string size
	sysctlbyname("hw.machine", NULL, &size, NULL, 0);
	
	st = str.resize( size, false );			
	if( !st ) return st;
	
	sysctlbyname("hw.machine", str._getInternalBufferPtr(), &size, NULL, 0);			
	return 0;
}

double axSystem::uptime() {
	return [[NSProcessInfo processInfo] systemUptime];	
}

#endif //axOS_MacOSX

#if axOS_Linux
#if 0
#pragma mark ================= Linux ====================
#endif

axSize	axSystem::numberOfProcessors() { 
	return sysconf( _SC_NPROCESSORS_CONF ); 
}

axSize	axSystem::availableMemory() {
	/*
	struct sysinfo a;
	sysinfo( &a );
	return a.freeram;
	*/
	return 0;
}

#endif //axOS_Linux

#if axUSE_PTHREAD
#if 0
#pragma mark ================= PThread ====================
#endif

void* axSystem::getCurrentThreadID() {
    return (void*)pthread_self();
}

#endif	//axUSE_PTHREAD

