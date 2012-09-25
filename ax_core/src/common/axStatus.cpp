//
//  axStatus.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-24.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/common/axStatus.h>
#include <ax/core/system/axLog.h>

axSingleton_BODY( axStatus_ModuleList );

axStatus_Std	axStatus_Std::inst;

const char* axStatus :: c_str() const { 
	if( code_ == k_undefine ) return "undefine";

	const char* sz = axStatus_ModuleList::instance->c_str(code_);
	if( sz ) return sz;
	
	return code_ >= 0 ? "OK" : "Error";
}

//======================

axStatus_Module::axStatus_Module( int start ) {
	axStatus_ModuleList::instance->registerModule( this, start );
}

//======================
axStatus_ModuleList::axStatus_ModuleList() {
// init by static
//	memset( module, 0, sizeof(axStatus_Module) * kModuleMax );
}

const char* axStatus_ModuleList::c_str( int code ) {
	int m = moduleIdByCode( code );
	if( m < 0 || m >= kModuleMax ) {
		assert( false );
	}else{
		if( module[m] ) return module[m]->c_str(code);
	}
	return code >= 0 ? "OK" : "Error";
}

int axStatus_ModuleList::moduleIdByCode( int code ) { 
	return -code / kModuleInterval; 
}

void axStatus_ModuleList::registerModule( axStatus_Module* mod, int moduleStart ) {
	int m = moduleIdByCode( moduleStart );
	if( m < 0 || m >= kModuleMax ) {
		assert( false );
	}else{
		//axStatus_Module* p =  module[m];
		if( module[m] ) {
			 return; //duplicated ?
		}
		module[m] = mod;
	}
}
