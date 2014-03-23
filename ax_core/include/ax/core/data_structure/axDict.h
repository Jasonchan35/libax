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
#include "../other/axJson.h"

template<class KEY, class VALUE>
class axDict : public axNonCopyable {
public:

	class Pair : public axHashTableNode< Pair, true > {
	public:
		virtual ~Pair() {};
		
		uint32_t	hashTableValue() {
			return ax_hash_code( key );
		}

		template<class S>
		axStatus	serialize_io( S & s ) {
			axStatus st;
			ax_io( key );
			ax_io( value );
			return 0;
		}
/*
		virtual	axStatus toStringFormat( axStringFormat &f ) const {
			return f.format("Pair<{?},{?}>",key,value);
		}
*/
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

	VALUE*  addUnique ( const KEY & key ) {
		if( get(key) ) return nullptr; //already exist !!
		return _createPair( key );
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
		
		if( newIfNotFound ) return _createPair( key );
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
	
	axALWAYS_INLINE(	axStatus	serialize_io	( axSerializer		&se ) );
	axALWAYS_INLINE(	axStatus	serialize_io	( axDeserializer	&se ) );
	
private:
	HashTable	table_;

	
	Pair*	_createPair( const KEY & key ) {
		axStatus st;
		axAutoPtr< Pair >	ap(st);			if( !st ) return nullptr;
		st = ax_copy( ap->key, key );		if( !st ) return nullptr;
		st = table_.insert( ap );			if( !st ) return nullptr;
		return ap.unref();
	}	
};

template< class VALUE, class CHAR = char, size_t STRING_LOCAL_BUF_SIZE = 64 >
class axDict_String : public axDict< axStringWithHash<CHAR, STRING_LOCAL_BUF_SIZE >, VALUE > {
	typedef	axStringWithHash< CHAR, STRING_LOCAL_BUF_SIZE >		String;
	typedef	axDict< String, VALUE > B;
	typedef typename B::Pair	Pair;
public:
			B & asDict()		{ return (B&)*this; }
	const	B & asDict() const	{ return (B&)*this; }
	
	axStatus	set			( const CHAR* key, const VALUE & value ) 	{ String skey; skey.set(key); return B::set			( skey, value ); }
	axStatus	setByTake	( const CHAR* key, const VALUE & value ) 	{ String skey; skey.set(key); return B::setByTake	( skey, value ); }
	VALUE*		addUnique	( const CHAR* key )							{ String skey; skey.set(key); return B::addUnique	( skey ); }
	VALUE* 		get 		( const CHAR* key ) 						{ String skey; skey.set(key); return B::get			( skey ); }
	Pair* 		getPair		( const CHAR* key, bool newIfNotFound ) 	{ String skey; skey.set(key); return B::getPair		( skey, newIfNotFound ); }
	axStatus	remove		( const CHAR* key )							{ String skey; skey.set(key); return B::remove 		( skey ); }
	
	template<class S>	axStatus	serialize_io ( S &s ) { return B::serialize_io(s); }
	
};

//-----------------------------------


template<class KEY, class VALUE> inline 
axStatus	ax_json_serialize_value ( axJsonWriter &s, axDict<KEY, VALUE> &v ) {
	axStatus st;
	st = s.beginArrayValue();		if( !st ) return st;

	for( auto & p : v )  {
		st = s.io( p.key, 	"key" );	if( !st ) return st;
		st = s.io( p.value, "value" );	if( !st ) return st;
	}
	st = s.endArrayValue();			if( !st ) return st;
	return 0 ;
}


template<class KEY, class VALUE> inline 
axStatus	ax_json_serialize_value ( axJsonParser &s, axDict<KEY, VALUE> &dict ) {
	axStatus st;

	st = s.beginArrayValue();			if( !st ) return st;
	if( ! s.checkToken("]") ) {
		for(;;) {
			axAutoPtr< typename axDict<KEY, VALUE> :: Pair >	ap(st);		if( !st ) return st;
			st = s.io_value( *ap );								if( !st ) return st;
			st = dict.table_.insert( ap );						if( !st ) return st;
			ap.unref();
		
			if( s.checkToken("]") ) break;	
			st = s.nextElement();		if( !st ) return st;
		}
	}
	st = s.endArrayValue();				if( !st ) return st;
	return 0;
}



template<class KEY, class VALUE> inline 
axStatus	axDict<KEY, VALUE>::serialize_io	( axSerializer		&se ) {
	axSize c = count();
	axStatus st = se.io_vary( c );		if( !st ) return st;
		
	for( auto &p : *this ) {
        st = se.io( *p );    			if( !st ) return st;
    }	
	return 0;
}

template<class KEY, class VALUE> inline 
axStatus	axDict<KEY, VALUE>::serialize_io	( axDeserializer	&se ) {
	axSize  c = 0;
	axStatus st = se.io_vary( c ); 		if( !st ) return st;
	
    for( axSize i=0; i<c; i++ ) {
		axAutoPtr< Pair >	ap(st);			if( !st ) return st;
		st =se.io( *ap );					if( !st ) return st;
		st = table_.insert( ap );			if( !st ) return st;
        ap.unref();
    }
	return 0;
}


template< class VALUE, class CHAR, size_t STRING_LOCAL_BUF_SIZE > inline
axStatus	ax_json_serialize_value ( axJsonWriter &s, axDict_String< VALUE, CHAR, STRING_LOCAL_BUF_SIZE > &v ) {
	return ax_json_serialize_value( s, v.asDict() );
}


template< class VALUE, class CHAR, size_t STRING_LOCAL_BUF_SIZE > inline
axStatus	ax_json_serialize_value ( axJsonParser &s, axDict_String< VALUE, CHAR, STRING_LOCAL_BUF_SIZE > &v ) {	
	return ax_json_serialize_value( s, v.asDict() );
}

#endif
