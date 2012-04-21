#include <ax/core/lang_table/axLangTable.h>
#include <ax/core/file_system/axFileSystem.h>
 

	
const char* axLangTable::get_short_name( axLangTableID n ) {
	switch( n ) {
#define	ax_enum_define(a,b) case lt_##a: return #a; break;
#include <ax/core/lang_table/axLocation_enum.h>
#undef ax_enum_define
	}
	return "unknown";
}
	
const char* axLangTable::get_name( axLangTableID n ) {
	switch( n ) {
#define	ax_enum_define(a,b) case lt_##a: return #b; break;
#include <ax/core/lang_table/axLocation_enum.h>
#undef ax_enum_define
	}
	return "unknown";
}


axLangTableID axLangTable::get_id( const char* sz ) {
	
#define	ax_enum_define(a,b) if( ax_strcasecmp( sz, #a ) == 0 ) return (lt_##a);
#include <ax/core/lang_table/axLocation_enum.h>
#undef ax_enum_define
	
	return 0;
	
}
	
const wchar_t* axLangTable::k_main_file_name = L"axLangTable";

axStatus axLangTable::load( const wchar_t* file_dir ) {
	axStatus st;
	
	st = _file_dir.set( file_dir ); if( !st ) return st;
	
	axStringW file_path;
	st = file_path.format( L"{?}/{?}", file_dir, k_main_file_name );
	
	
	axByteArray buf;	
	st = axFileSystem::loadFile( buf, file_path ); if( !st ) return st;	
	
	//tony
	//axDeserializer ds(buf);
	//st = ds.io( *this ); if( !st ) return st;
	
		
	ax_log( L"v={?} n={?} l={?}", curr_version, name, lang_list );
	
	return 0;
}
	
axStatus axLangTable::set_lang( axLangTableID _id ) {
	axStatus st;
	
	axStringW file_path;
	st = file_path.format( L"{?}/{?}", _file_dir,get_short_name(_id) ); if( !st ) return st;

	
	st = lt.load( file_path ); if( !st ) return st;

	
	return 0;
}
	
	
axLangTable::axLangTable () {
	curr_version = k_version;
}
	
//static
axStatus axLangTable::make( const wchar_t *in_file, const wchar_t *out_path ) {
	
	//size_t i;
	
	axLangTable tbl;
	
	axArray< axLangTable_File > f;
		
	axStatus st;
	 
	//todo
	/*
	axByteArray buf;	
	st = axFileSystem::loadFile( buf, in_file ); if( !st ) return st;	
	
	axDeserializer ds(buf);
	
	axCSVParserA p(ds);
	
	//log_info( L"=> {?}" )% buf;

	if( !p.next_line() ) return -100;
	if( !p.next_token() ) return -101; // the 1st note
	st = tbl.name.set( p.token ); if( !st ) return st;
	
	
	while( p.next_token() ) {
		
		axLangTableID lang_id = get_id( p.token );
		//log_info(L"Lang: [{?}][{?}]" ) % lang_id % p.token.sz();
		
		if( lang_id == 0 ) return -110;
		st = tbl.lang_list.append( lang_id ); if( !st ) return st;
	}
	
	Size_t n = tbl.lang_list.size();
	
	st = f.resize( n ); if( !st ) return st;
	
	//log_info(L"n = [{?}]" ) % n;	
	
	for( i=0; i<n; i++ ) {
		f[i].lang_id = tbl.lang_list[i];
		st = f[i].nodes.init( axLangTable_File::k_hash_tbl_size ); if( !st ) return st;
		
	}
		
	while( p.next_line() ) {
		i = 0;
		
		//log_info(L"=[{?}]" ) % p.line;	
		
		if( ! p.next_token() ) { return -120; } //no key
		StringA key;
		st = key.set( p.token ); if( !st ) return st;
		
		while( p.next_token() ) {
			
			st = f[i].add( key, p.token ); if( !st ) return st;
			
			i++;

		}
		
		//printf("\n");
	}
	
	 */
	
	/*
	axStringW out_file;
	
	
	for( i=0; i<n; i++ ) {		
		st = out_file.format( L"{?}/{?}") % out_path % get_short_name( f[i].lang_id ); if( !st ) return st;
		log_info( L"=> Genterating {?}" ) % out_file;
		st = f[i].save( out_file ); if( !st ) return st;
		
	}
	
		
	st = out_file.format( L"{?}/{?}") % out_path % k_main_file_name; if( !st ) return st;
	
	//log_info( L"=> {?}" ) % out_file;

	st = serialize_to_file( out_file, tbl ); if( !st ) return st;
	
	 */
	return 0;
}
	
// ====================
	
	
axLangTable_File::axLangTable_File() {
	reset();
}

void axLangTable_File::reset() {
	lang_id = 0;
	nodes.clear();
}

axStatus axLangTable_File::get( const char* key, axIStringA &str  ) {
	axStatus st;

	TextNode *n = nodes.getListHead( ax_string_hash( key ) );

	for( ;n;n=n->next() ) {
		if( n->key.compareTo( key ) == 0 ) {
			st = str.set( n->str ); if( !st ) return st;
			return 0;
		}
		
	}

	return -2000;
}
	
	
axStatus axLangTable_File::add( const char* key, const char* str ) {
	//todo check double

	axStatus st;
	axAutoPtr< TextNode > n;
	st = n.newObject(); if( !st ) return st;
	st = n->key.set( key ); if( !st ) return st;
	st = n->str.set( str ); if( !st ) return st;
	n->xor_this();
	nodes.append( n.unref() );

	return 0;
}

axStatus axLangTable_File::save( const wchar_t* file_path ) {
	return 0;//serialize_to_file( file_path, *this );
}
	
axStatus axLangTable_File::load( const wchar_t* file_path ) {
	axStatus st;
	reset();

	
	st = nodes.setTableSize( k_hash_tbl_size ); if( !st ) return st;
	
	//log_info( L"axLangTable_File::load {?}") % file_path;
	
	/*
	axByteArray buf;	
	st = File::load( file_path, buf ); if( !st ) return st;	
	axDeserializer ds(buf);
	st = ds.io( *this ); if( !st ) return st;
	*/
	/*	
	log_info( L"axLangTable_File lang_id [{?}] c={?}") % lang_id % (int)nodes.count();
	List::It it( nodes ) ;
	for( ; it.node(); it.go_next() ) {	 
		log_info(L"[{?}] " ) % *it;
	}
	*/
	
	return 0;
}


template< class S >
inline axStatus axLangTable_File::on_serialize( S &s  ) {
	

	axStatus st;
	st = s.io( lang_id );	if( !st ) return st;
	
	axSize n = nodes.nodeCount();
	st = s.io( n );	if( !st ) return st;
	
	/*
	if( s.mode == SerializerCommon::m_len || s.mode == SerializerCommon::m_serialize ) {
		List::It it( nodes ) ;
		
		for( ; it.node(); it.go_next() ) {
			st = s.io( *it ); if( !st ) return st;
			
			//log_info(L"[{?}] [{?}]" ) % it->key.sz() % it->str.sz();
		}
	}
	
	if( s.mode == SerializerCommon::m_deserialize ) {
		uint32_t i;
		
		for( i=0; i<n; i++ ) {		
			AutoPtr< axLangTable_File::TextNode > n;
			st = n.init(); if( !st ) return st;
			st = s.io( *n ); if( !st ) return st;
			n->xor_this();
			nodes.append( n.unref() );
		}
	}
	 */

	return 0;
}

	
void axLangTable_File::TextNode::xor_this() {
//tony
/*
	ax_xor_str( key, axLangTable_File::k_xor_key );
	ax_xor_str( str, axLangTable_File::k_xor_key );	
	
	)
	
	*/
	
	
	

}

////////////////////////////////////////
	 

