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
class axDict : public axNonCopyable {
public:

	class Pair : public axHashTableNode< Pair, true > {
	public:
		uint32_t	hashTableValue() { return ax_hash_code( key ); }
		KEY		key;
		VALUE	value;
	};

	typedef	axHashTable< Pair >	HashTable;
	typedef	typename HashTable::iterator	iterator;

	iterator	begin	()	{ return table_.begin(); }
	iterator	end		()	{ return table_.end(); }

	axStatus	set	( const KEY & key, const VALUE& value ) {
		axStatus	st;
		Pair* p = getPair( key, true );
		if( !p ) return axStatus_Std::dict_cannot_add_pair;
		st = ax_copy( p->value, value );	if( !st ) return st;
		return 0;
	}

	axStatus	setByTake ( const KEY & key, VALUE & value ) {
		axStatus	st;
		Pair* p = getPair( key, true );
		if( !p ) return axStatus_Std::dict_cannot_add_pair;
		st = ax_take( p->value, value );	if( !st ) return st;
		return 0;
	}

		
	VALUE*	get	( const KEY & key ) {
		Pair* p = getPair( key, false );
		return p ? & p->value : nullptr;
	}

	Pair*	getPair	( const KEY & key, bool newIfNotFound ) {
		Pair* p = table_.getListHead( ax_hash_code( key ) );
		for( ; p; p=p->next() ) {
			if( p->key == key ) return p;
		}
		
		if( newIfNotFound ) {
			axStatus st;
			axAutoPtr< Pair >	ap(st);			if( !st ) return nullptr;
			st = ax_copy( ap->key, key );		if( !st ) return nullptr;
			st = table_.insert( ap );			if( !st ) return nullptr;
			return ap.unref();
		}
		
		return nullptr;
	}
	
	axStatus	remove	( const KEY & key ) {
		Pair* p = getPair( key );
		if( p ) { delete p; return 0; }
		return axStatus_Std::not_found;
	}
	
	axStatus	getArray( axIArray< VALUE > & arr ) {
		axStatus st;
		st = arr.reserve( count() );		if( !st ) return st;
		for( auto & p : table_ ) {
			st = arr.append( p.value );	if( !st ) return st;
		}
		return 0;
	}
	
	axSize	count	() const	{ return table_.count(); }
	
	void	clear	()			{ table_.clear(); }
		
private:
	HashTable	table_;
};

template< class VALUE, class CHAR, size_t STRING_LOCAL_BUF_SIZE >
class axDict_String : public axDict< axStringWithHash<CHAR, STRING_LOCAL_BUF_SIZE >, VALUE > {
	typedef	axStringWithHash< CHAR, STRING_LOCAL_BUF_SIZE >		String;
	typedef	axDict< String, VALUE > B;
public:
	typedef typename B::Pair	Pair;

	axStatus	set			( const CHAR* key, const VALUE & value ) 	{ String skey; skey.set(key); return B::set		( skey, value ); }
	axStatus	setByTake	( const CHAR* key, const VALUE & value ) 	{ String skey; skey.set(key); return B::setByTake( skey, value ); }
	VALUE* 		get 		( const CHAR* key ) 						{ String skey; skey.set(key); return B::get		( skey ); }
	Pair* 		getPair		( const CHAR* key, bool newIfNotFound ) 	{ String skey; skey.set(key); return B::getPair	( skey, newIfNotFound ); }
	axStatus	remove		( const CHAR* key )							{ String skey; skey.set(key); return B::remove 	( skey ); }	
};

template< class VALUE, size_t STRING_LOCAL_BUF_SIZE=64 >
class axDict_StringA : public axDict_String< VALUE, char, 	 STRING_LOCAL_BUF_SIZE > {};

template< class VALUE, size_t STRING_LOCAL_BUF_SIZE=64 >
class axDict_StringW : public axDict_String< VALUE, wchar_t, STRING_LOCAL_BUF_SIZE > {};


#endif
