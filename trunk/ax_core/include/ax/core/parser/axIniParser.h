//
//  axIniParser.h
//  libax
//
//  Created by Jason on 25/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axIniParser_h__
#define __axIniParser_h__

#include "../file_system/axMemMap.h"

class axIniParser {
public:
	class Key {
	public:
		axStringA	name;
		axStringA	value;
		
		axStatus onTake( Key & src );
	};

	class Section {
	public:
		axStringA		name;
		axArray< Key >	keys;
		axStatus onTake( Section & src );
	};
	axArray< Section >	sections;
	
	axStatus	readFile( const char* filename );
	axStatus	readMem ( const axIByteArray & buf );
	
	void		clear	();
	
	Section*	findSection	( const char* section );
	Key*		findKey	( const char* section, const char* key );
	const char* value	( const char* section, const char* key );

	axStatus	findKeyArray( axIArray<Key*> &keys,				const char* section, const char* key );
	axStatus	valueArray	( axIArray<const char*> & values,	const char* section, const char* key );

	template<class T> axStatus	getValueArray	( axIArray<T> & out,	const char* section, const char* key );
	template<class T> axStatus	getValue		( T & out,				const char* section, const char* key );


	axStatus	toStringFormat( axStringFormat &f ) const;
};


template<class T> inline
axStatus axIniParser::getValue( T & out, const char* section, const char* key ) {
	const char* sz = value(section,key);
	if( !sz ) return axStatus_Std::IniParser_key_not_found;
	return ax_str_to( sz, out );
}

template<class T>
axStatus axIniParser::getValueArray( axIArray<T> & out, const char* section, const char* key ) {
	axStatus st;
	axArray<const char*> v;
	st = valueArray( v, section, key );		if( !st ) return st;
	st = out.resize( v.size() );			if( !st ) return st;

	for( axSize i=0; i<v.size(); i++ ) {
		st = ax_str_to( v[i], out[i] );		if( !st ) return st;
	}
	return 0;
}



#endif //__axIniParser_h__
