#ifndef __axLangTable_h__
#define __axLangTable_h__

#include <ax/ax_base.h>
#include <ax/core/parser/axCSVParser.h>
	
enum {
	lt_null = 0, 	
#define ax_enum_define(a,b) lt_##a,
#include "axLocation_enum.h"
#undef ax_enum_define
	lt_end,	
};
	
typedef uint32_t axLangTableID;	
	

class axLangTable_File : public axNonCopyable {
public:
	
	static const unsigned char k_xor_key = 0x0e;
	static const uint32_t k_hash_tbl_size = 256;
	
	axLangTable_File();
	
	
	axStatus init( axLangTableID _id, uint32_t _hash_tbl_size );
				
	axStatus load( const wchar_t* file_path );
	axStatus save( const wchar_t* file_path );	
	
	
	axStatus add( const char* key, const char* str );
	axStatus get( const char* key, axIStringA &str );
		
	void reset();
		
	template< class S > axStatus on_serialize( S &s  );
	
	class TextNode: public axHashTableNode< TextNode > {
	typedef TextNode B;	
	public:
		uint32_t hashTableValue() { return ax_string_hash( key.c_str() ); }
		
		axStringA key;
		axStringA str;
		
		void xor_this();
		
		axStatus toStringFormat( axStringFormat &f ) const{
			return f.format( L"key:[{?}] str:[{?}]", key, str );
		}

		
		template<class S> axStatus	serialize_io( S &s ) {
			axStatus st;
			st = s.io( key );	if( !st ) return st;
			st = s.io( str );	if( !st ) return st;
			return 0;	
		}		
		
		inline	axStatus onTake(  TextNode &b )  { 
			axStatus st;	
			st = key.set( b.key );	if( !st ) return st;
			st = str.set( b.str );	if( !st ) return st;

			return 0; 
		}

		
	};
	
	typedef axHashTable< TextNode > List;
	
	axLangTableID lang_id;
	List nodes;
		
	axStatus	_take( axLangTable_File &src ) { return -1; }
	inline	axStatus	onTake( axLangTable_File &b )  { return -1; }

	template<class S> axStatus	serialize_io( S &s ) {
		axStatus st;
		st = s.io( lang_id ); if( !st ) return st;		
		st = s.io( nodes ); if( !st ) return st;
		
		return 0;	
	}		
};



/*
template<> inline axStatus take( axLangTable_File &a, axLangTable_File &b ) { return a._take(b); }
	
 */

	
class axLangTable : public axNonCopyable {
public:

	static const uint32_t k_version = 0x0100;
	
	
	static const wchar_t *k_main_file_name;
	
	axLangTable();
	
	axStatus load( const wchar_t* file_dir );
	axStatus set_lang( axLangTableID _id );
	
	
	axArray< axLangTableID > lang_list;
	axStringA name;
	uint32_t curr_version;

	axStatus get( const char* key, axIStringA &str ) { return lt.get( key, str ); }
	
	
	axStringW _file_dir;
	
	//bool is_lang_ava( axLangTableID );
	axLangTable_File lt;
	//axStatus get( const char* key, PxString &str );
		
	static axLangTableID get_id( const char* sz );
	static const char* get_name( axLangTableID n );
	static const char* get_short_name( axLangTableID n );

	static axStatus make( const wchar_t *in_file, const wchar_t *out_path );
		
	
	template<class S> axStatus	serialize_io( S &s ) {
		axStatus st;
		st = s.io( curr_version );	if( !st ) return st;
		if( curr_version < axLangTable::k_version ) return -10001;
		
		st = s.io( name );			if( !st ) return st;
		st = s.io( lang_list);		if( !st ) return st;
		return 0;
	}
	
};
 



#endif //__axLangTable_h__
