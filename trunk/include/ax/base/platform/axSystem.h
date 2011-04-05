#ifndef __axSystem_h__
#define __axSystem_h__

#include "../string/axIString.h"

//! \ingroup base_platform
//@{

class axSystem {
public:
	static	axSize		numberOfProcessors	();
	static	axSize		availableMemory		();
	static	double		availableMemoryInKiloByte()						{ return (double)availableMemory() / 1024.0; }
	static	double		availableMemoryInMegaByte()						{ return (double)availableMemory() / 1024.0 / 1024.0; }
			axStatus	getMachineName		( axIStringA &str );

	enum Endianness {
		Endianness_Unknown,
		Endianness_BigEndian,
		Endianness_LittleEndian,
	};
			Endianness	getEndianness		();
			bool		isBigEndian			();
			bool		isLittleEndian		();
};

//@}

inline
axSystem::Endianness	axSystem::getEndianness		() {
	#if axCPU_LITTLE_ENDIAN
		return Endianness_LittleEndian;
	#elif axCPU_BIG_ENDIAN
		return Endianness_BigEndian;
	#else
		return Endianness_Unknown;
	#endif
}

inline
bool		axSystem::isBigEndian			() {
	#if axCPU_BIG_ENDIAN
		return true;
	#else
		return false;
	#endif
}

inline
bool		axSystem::isLittleEndian		() {
	#if axCPU_LITTLE_ENDIAN
		return true;
	#else
		return false;
	#endif
}


#if 0
#pragma mark ================= MS Win ====================
#endif
#ifdef axOS_WIN

inline
axSize	axSystem::numberOfProcessors() {
	SYSTEM_INFO	info;
	GetSystemInfo( &info );
	return info.dwNumberOfProcessors;
}	

#endif
	
#if axOS_UNIX
	#if axOS_iOS	
		#if 0
		#pragma mark ================= iOS ====================
		#endif
		
		inline
		axSize axSystem::availableMemory() {
			vm_statistics_data_t vmStats;
			mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
			kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
			if(kernReturn != KERN_SUCCESS) {
				return -1;
			}
			return vm_page_size * vmStats.free_count;
		}	

		inline
		axStatus axSystem::getMachineName( axIStringA &str ) {
			Status st;
			axSize size;
			//get string size
			sysctlbyname("hw.machine", NULL, &size, NULL, 0);
			
			st = str.resize( size, false );			
			if( !st ) return st;
			
			sysctlbyname("hw.machine", str.buf.ptr(), &size, NULL, 0);			
			return 0;
		}
	
	#elif axOS_MacOSX
		#if 0
		#pragma mark ================= Mac OS X ====================
		#endif
		inline axSize	axSystem::numberOfProcessors() { return MPProcessors(); }
	#else
		#if 0
		#pragma mark ================= UNIX ====================
		#endif

		inline axSize	axSystem::numberOfProcessors() { return sysconf( _SC_NPROCESSORS_CONF ); }
	
	#endif
	
#endif	

#endif //__axSystem_h__

