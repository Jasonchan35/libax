//
//  axIniParser.cpp
//  libax
//
//  Created by Jason on 25/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/parser/axIniParser.h>
#include <ax/core/system/axLog.h>

axStatus axIniParser::Key::onTake( Key & src ) { 
	name.onTake( src.name );
	value.onTake( src.value );
	return 0; 
}		

axStatus axIniParser::Section::onTake( Section & src ) { 
	name.onTake( src.name );
	keys.onTake( src.keys );
	return 0; 
}		


axStatus axIniParser::readFile( const char* filename ) {
	axStatus st;
	axMemMap	mm;
	st = mm.openRead( filename );		if( !st ) return st;
	return readMem( mm );
}

void axIniParser::clear() {
	sections.clear();
}

axStatus	axIniParser::readMem ( const axIByteArray & buf ) {
	clear();

	axStatus st;
	axDeserializer	ds( buf );

	st = sections.incSize(1);		if( !st ) return st;
	
	axSize	line_no = 1;
	axTempStringA	lineBuf;
	for( ;; line_no++ ) {
		st = ds.getLine( lineBuf );		
		if( st.isEOF() ) break;
		if( !st ) return st;
		
		//ax_log( "{?}", lineBuf );
		
		st = lineBuf.trimBoth();			if( !st ) return st;
		if( lineBuf.size() == 0 ) continue;
		switch( lineBuf.charAt(0) ) {
			case ';': continue; // ; is comment
			case '[': {
//				ax_log( "section found {?}", lineBuf );
				if( lineBuf.size() < 2 || lineBuf.lastChar() != ']' ) {
					ax_log( "line {?}: invalid section {?}", line_no, lineBuf );
					return axStatus_Std::IniParser_invalid_section;
				}
				st = sections.incSize(1);	if( !st ) return st;
				Section &s = sections.last();
				s.name.setWithLength( lineBuf.c_str(1), lineBuf.size()-2 );
				continue;
			}break;
		}
			
		axSize pos;
		if( ! lineBuf.findChar('=', pos) ) {
			ax_log( "line {?}: invalid key {?}", line_no, lineBuf );
			return axStatus_Std::IniParser_invalid_key;
		}
		
		st = sections.last().keys.incSize(1);		if( !st ) return st;
		Key & k = sections.last().keys.last();
		
		st = k.name.setWithLength( lineBuf, pos );	if( !st ) return st;
		st = k.name.trimBoth();						if( !st ) return st;
		
		st = k.value.set( lineBuf.c_str(pos+1) );	if( !st ) return st;
		st = k.value.trimBoth();					if( !st ) return st;
	}
	
	return 0;
}

axStatus	axIniParser::toStringFormat( axStringFormat &f ) const {
	for( axSize i=0; i<sections.size(); i++ ) {
		const Section &s = sections[i];
		f.format("\n[{?}]\n", s.name );
		
		for( axSize j=0; j<s.keys.size(); j++ ) {
			const Key &k = s.keys[j];
			f.format("{?}={?}\n", k.name, k.value );
		}
	}
	return 0;
}


axIniParser::Section*	axIniParser::findSection	( const char* section ) {
	for( axSize i=0; i<sections.size(); i++ ) {
		Section &s = sections[i];
		if( ! s.name.equals(section) ) return &s;
	}
	return NULL;
}


axIniParser::Key* axIniParser::findKey  ( const char* section, const char* key ) {
	for( axSize i=0; i<sections.size(); i++ ) {
		Section &s = sections[i];
		if( ! s.name.equals(section) ) continue;
		
		for( axSize j=0; j<s.keys.size(); j++ ) {
			Key & k = s.keys[j];
			if( k.name.equals(key) ) return &k;
		}
	}
	return NULL;
}

const char* axIniParser::value( const char* section, const char* key ) {
	Key* k = findKey( section, key );	if( !k ) return NULL;
	return k->value.c_str();
}

axStatus axIniParser::findKeyArray  ( axIArray<Key*> &keys, const char* section, const char* key ) {
	axStatus st;
	keys.resize(0);
	for( axSize i=0; i<sections.size(); i++ ) {
		Section &s = sections[i];
		if( ! s.name.equals(section) ) continue;
		
		for( axSize j=0; j<s.keys.size(); j++ ) {
			Key & k = s.keys[j];
			if( k.name.equals(key) ) {
				st = keys.append(&k);	if( !st ) return st;
			}
		}
	}

	if( keys.size() == 0 ) return  axStatus_Std::IniParser_key_not_found;
	return 0;
}

axStatus	axIniParser::valueArray	( axIArray<const char*> & values, const char* section, const char* key ) {
	axStatus st;
	values.resize(0);

	axArray<Key*, 64>	keys;
	st = findKeyArray( keys, section, key );	if( !st ) return st;

	for( axSize i=0; i<keys.size(); i++ ) {
		values.append( keys[i]->value.c_str() );
	}

	return 0;
}


