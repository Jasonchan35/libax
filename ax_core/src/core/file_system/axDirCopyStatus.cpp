#include <ax/core/file_system/axDirCopyStatus.h>

float	axDirCopyStatus::getProgess() {
	MD md( md_ );
	float ret = 0;

	if( md->isDone ) return 1.0f;	

	float s = 1;

	axSize i;
	for( i=0; i<md->lv.size(); i++ )  {

		Level &l = md->lv[i];

		if( l.nElement > 0 ) {
			s /= l.nElement;
			ret += (float) l.nDone * s ;
		}	
	}
	
 
	return ret;
}

axStatus axDirCopyStatus::pushWithEntryCount( axSize s ) {
	axStatus st ;

	MD md( md_ );

	st = md->lv.incSize( 1 ); if( !st ) return st;

	Level &l = md->lv.last();

	l.nElement = s; 
	l.nDone = 0;
	
	return 0;
}

void axDirCopyStatus::incProgess() {
	MD md( md_ );
	Level &l = md->lv.last();
	l.nDone++;

	//ax_log("getProgess {?}", getProgess() * 1000 );
}

axStatus axDirCopyStatus::pop() {
	axStatus st;
	MD md( md_ );

	Level &l = md->lv.last();

	if( l.nDone != l.nElement ) {
		//return warrning ?
	}

	st = md->lv.decSize(1); if( !st ) return st;

	if( md->lv.size() == 0 ) md->isDone = true; 

	//ax_log("pop {?}", getProgess() * 1000 );
	return 0;
}
