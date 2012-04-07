#ifndef __axSystem_h__
#define __axSystem_h__

#include <ax/base/string/axIString.h>

//! \ingroup base_platform
//@{

class axSystem {
public:
	static	axSize		numberOfProcessors	();
	
	static	axSize		availableMemory		();
	static	double		availableMemoryInKiloByte()						{ return (double)availableMemory() / 1024.0; }
	static	double		availableMemoryInMegaByte()						{ return (double)availableMemory() / 1024.0 / 1024.0; }
			axStatus	getMachineName		( axIStringA &str );
	static	void*		getCurrentThreadID	();

	static	double		uptime	();
		
		
	enum Endianness {
		Endianness_Unknown,
		Endianness_BigEndian,
		Endianness_LittleEndian,
	};
			Endianness	endianness			();
			bool		isBigEndian			();
			bool		isLittleEndian		();
};

//@}

inline
axSystem::Endianness	axSystem::endianness		() {
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

#endif //__axSystem_h__

