#include <ax/ax_core.h>

class Obj : public axHashTableNode< Obj, true > {
public:
	Obj( size_t i ) {
		value = i;
	};

	uint32_t hashTableValue() { return value; }

	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.format("{?}", value );
	}	

	size_t value;
};

class HashTable : public axHashTable< Obj > {
public:

};


axStatus test_hashTable() {

	HashTable	table;
	for( size_t i=0; i<10; i++ ) {
		table.insert( new Obj(i) );
	}

	HashTable::Iterator	it(table);
	for( ; it; it++ ) {
		ax_log("it = {?}", *it );
	}

	ax_log_var( table );
	return 0;
}

int main() {
	test_hashTable();

	printf("======== End ==========\n");
	getchar();
}