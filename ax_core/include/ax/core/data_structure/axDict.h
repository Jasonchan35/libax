//
//  axDict.h
//  ax_core
//
//  Created by Jason on 2013-11-09.
//
//

#ifndef ax_core_axDict_h
#define ax_core_axDict_h

#include "axHashTable.h"

template<class KEY, class VALUE>
class axDict {
public:
		axStatus	insert	( const KEY & key, const VALUE& value ) {
			axStatus	st;
			axAutoPtr< Node >	p(st);		if( !st ) return st;
			st = ax_copy( p.key, key );		if( !st ) return st;
			st = ax_copy( p.value, value );	if( !st ) return st;
			st = table_.insert( p );		if( !st ) return st;
			p.unref();
			return 0;
		}
			
		const VALUE*	find	( const KEY & key ) {
			Node* p = getListHead( ax_hash_code( key ) );
			for( ; p; p=p->next() ) {
				if( p->key == key ) return & p->value;
			}
			return nullptr;
		}
		
private:

	class Node : public axHashTableNode< Node, true > {
	public:
		KEY		key;
		VALUE	value;
	};

	axHashTable< Node >	table_;
};

template< class VALUE, class CHAR, size_t LOCAL_BUF_SIZE >
class axDict_String : public axDict< axStringWithHash<CHAR, LOCAL_BUF_SIZE > > {
	typedef	axStringWithHash< CHAR, LOCAL_BUF_SIZE >		String;
	typedef	axDict< String, VALUE > B;
public:
	axStatus	insert	( const CHAR* key, const VALUE & value ) {
		String	skey;	skey.set(key);
		return B::insert( skey, value );
	}
	
	const VALUE* find	( const CHAR* key ) {
		String	skey;	skey.set(key);
		return B::find( skey );
	}

};

#endif
