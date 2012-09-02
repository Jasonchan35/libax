#ifndef __axSerializer_Helper_h__
#define __axSerializer_Helper_h__


#include "../../base/other/axSerializer.h"
#include "../file_system/axFileSystem.h"

template<class T> inline 
axStatus ax_serialize_append_to_buf( axIByteArray	&buf, T &v ) {
	axStatus st;
	axSerializer		s(buf);
	st = s.io( v );							if( !st ) return st;
	return 0;
}

template<class T> inline 
axStatus ax_serialize_to_buf( axIByteArray	&buf, T &v ) {
	buf.resize(0);
	return ax_serialize_append_to_buf(buf,v);
}

template<class T> inline 
axStatus ax_serialize_from_buf( axIByteArray &buf, T &v ) {
	axStatus st;
	axDeserializer		s(buf);
	st = s.io( v );							if( !st ) return st;
	return 0;
}

template<class T> inline 
axStatus ax_serialize_to_file( const char* filename, T &v ) {
	axStatus st;
	axByteArray buf;
	st = ax_serialize_to_buf( buf, v ); if( !st ) return st;
	st = axFileSystem::saveFile( buf, filename ); if( !st ) return st;	
	return 0;
}

template<class T> inline 
axStatus ax_serialize_from_file( const char* filename, T &v ) {
	axStatus st;
	axByteArray buf;
	st = axFileSystem::loadFile( buf, filename ); if( !st ) return st;
	st = ax_serialize_to_buf( buf, v ); if( !st ) return st;
	return 0;
}

#endif //axSerializer_Helper


