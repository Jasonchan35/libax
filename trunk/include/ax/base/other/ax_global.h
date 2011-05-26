#ifndef __ax_global_h__
#define __ax_global_h__

#include "ax_log.h"

class axGlobal {
public:
    axLog        log;
};

inline	axGlobal& ax_global() {
    static	axGlobal	g;
    return g;
}
	

inline 
void ax_log( const char* fmt ) {
    axStringFormat::ArgList list;
    ax_global().log.log_ArgList( axLog::lv_info, fmt, list );
}

#endif //__ax_global_h__
