#include <ax/core/parser/axCSVParser.h>

axCSVParserBase::axCSVParserBase() {
	line_buf_max_size_ = 2048;
	line_buf_offset_ = 0;
	seperators_.set( "\t," );
	line_feeds_.set( "\r\n");
}

axCSVParserBase::~axCSVParserBase() {
}

axStatus axCSVParserBase::setSeperators( const char* char_list ) { 
	return seperators_.set(char_list); 
}

axStatus axCSVParserBase::getRow( axStringA_Array &row ) {
	axStatus st;
	row.clear();

	for(;;) {
		st = readLine();
		if( !st ) return st;

//		ax_print( "{?}", line_buf_ );
		line_buf_offset_ = 0;

		axSize	c;
		for( c=0; ; c++ ) {
			st = row.resize( c+1 );		if( !st ) return st;
			st = getCell( row[c] );
			if( !st ) {
				row.decSize(1);
				return 0;
			}
//			ax_print( "getCell {?}\n", row[c] );
		}

	}
	return 0;
}

axStatus	axCSVParserBase::readLine() {
	axStatus st;
	line_buf_offset_ = 0;
	return onReadLine( line_buf_ );
}

axStatus axCSVParserBase::getCell( axStringA &cell ) {
	axStatus st;
	const char *s = line_buf_.c_str() + line_buf_offset_;
	if( s[0] == 0 ) return -1;
	if( ax_strchr( line_feeds_, s[0] ) ) {
		return -2;
	}

	axSize	slen = 0;
	cell.clear();
	st = cell.reserve( 32 );		if( !st ) return st;

	if( s[0] == '"' ) {
		slen++; //open ["]
		for(;;) {
			if( s[slen] == 0 ) {
				if( ! readLine() ) {
					ax_print("error: unexpected end of line\n" );
					assert( false );
					return axStatus::code_data_corrupted;
				}
				slen = 0;
				s = line_buf_.c_str();
			}

			if( s[slen] == '"' ) { // " ecsapce
				slen++;
				if( s[slen] != '"' ) break;
			}

			st = cell.append( s[slen] );	if( !st ) return st;
			slen++;
		}

		if( ! ax_strchr( seperators_, s[slen] ) ) {
			ax_print("error: expected seperator after close quote\n" );
			assert( false );
			return axStatus::code_data_corrupted;
		}

	}else { //direct copy
		for( slen=0; s[slen]; slen++ ) {
			if( ax_strchr( seperators_, s[slen] ) ) break;
			if( ax_strchr( line_feeds_, s[slen] ) ) break;
		}
		st = cell.set( s, slen );		if( !st ) return st;
	}

	line_buf_offset_ += slen+1;

	return 0;
}


//===== axCSVFileParser

axStatus axCSVFileParser::open( const wchar_t* filename ) { 
	return file_.open( filename, "rb" ); 
}

//virtual		
axStatus axCSVFileParser::onReadLine( axStringA &buf ) {
	return file_.readLine( buf, B::line_buf_max_size_ );
}
