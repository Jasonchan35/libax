#ifndef __axDirCopyStatus__
#define __axDirCopyStatus__

#include "ax/ax_base.h"
#include "../thread/axMutex.h"

class axDirCopyStatus {
public:


	float			getProgess();

	axStatus		pushWithEntryCount( axSize s );
	void			incProgess();

	axStatus		pop();

private:

	class Level {
	public:
		Level() { 
			nElement = 0 ;
			nDone = 0;
		}

		axSize nElement;
		axSize nDone;

		axStatus onTake( Level &src ) {
			nElement = src.nElement;
			nDone = src.nDone;
			return 0;
		}
	};
	
	class MD_  {
	public:

		MD_ () { isDone = false; }
		axArray< Level > lv;
		bool isDone;

	};

	typedef axMutexProtected< MD_ >			MD;
	MD::Data md_;


};


#endif //__axDirCopyStatus__
