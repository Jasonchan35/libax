#include <ax/core/parser/axCSVParser.h>

axCSVParserBase::axCSVParserBase() {
	line_buf_max_size_ = 2048;
	seperators_ = ',';
	line_feeds_.set( "\r\n");
	r_ = NULL;
}

axCSVParserBase::~axCSVParserBase() {
}

void axCSVParserBase::setSeperator( char ch ) { 
	seperators_ = ch;
}

axStatus axCSVParserBase::getRow( axStringA_Array &row ) {
	axStatus st;
	row.clear();

	for(;;) {
		st = readLine_();
		if( !st ) return st;

		axSize	c;
		for( c=0; ; c++ ) {
			st = row.resize( c+1 );		if( !st ) return st;
			st = getCell_( row[c] );
			if( !st ) {
				row.decSize(1);
				return 0;
			}
//			ax_log( "getCell {?}\n", row[c] );
		}

	}
	return 0;
}

axStatus	axCSVParserBase::readLine_() {
	axStatus st;
	st = onReadLine( line_buf_ );		if( !st ) return st;
	r_ = line_buf_.c_str();
	return 0;
}

axStatus axCSVParserBase::getCell_( axStringA &cell ) {
	axStatus st;
	if( ! *r_ ) return -1;

	bool	in_quote = false;
	
	cell.clear();
	const char* p = r_;
	for(;;) {
		if( in_quote ) {
			if( *p == 0 ) { //new line inside quote
				st = cell.appendWithLength( r_, p-r_ );		if( !st ) return st;
				if( ! readLine_() ) {
					ax_log("error: unexpected end of line\n" );
					assert( false );
					return axStatus_Std::data_corrupted;
				}
				p = r_;
				continue;
			}
			
			if( *p == '"' ) {
				if( p[1] == '"' ) { //escape quote
					p++;
					st = cell.appendWithLength( r_, p-r_ );		if( !st ) return st;
					p++;
					r_ = p;
					continue;
				}
				
				st = cell.appendWithLength( r_, p-r_ );		if( !st ) return st;
				p++;
				r_ = p;
				in_quote = false;
				continue;
			}
		}else{			
			if( *p == '"' ) {
				st = cell.appendWithLength( r_, p-r_ );		if( !st ) return st;
				p++;
				r_ = p;
				in_quote = true;
				continue;
			}
			
			if( *p == seperators_ ) break;
			else if( *p == '\r' ) { //handle Windows Line Feed "\r\n"
				if( p[1] != '\n' ) {
					assert( false );
					return -1;
				}
				p++;
				break;
			}else if( *p == '\n' ) { //handle Unix Line Feed  "\n"
				break;
			}
			if( *p == 0 ) {
				st = cell.appendWithLength( r_, p-r_ );		if( !st ) return st;
				r_ = p;
				return 0;
			}			
		}
		p++;
	}

	st = cell.appendWithLength( r_, p-r_ );		if( !st ) return st;
	p++;
	r_ = p;
	return 0;
}


//===== axCSVFileParser

axStatus axCSVFileParser::open( const wchar_t* filename ) { 
	return file_.open( filename, L"rb" ); 
}

//virtual		
axStatus axCSVFileParser::onReadLine( axStringA &buf ) {
	return file_.readLine( buf, B::line_buf_max_size_ );
}

axStatus axCSVFileParser::open( const char* filename ) {
	axStatus st;
	axTempStringW str;
	st = str.set( filename ); if( !st ) return st;
	return open( str );
}	

//===== axCSVByteArrayParser

axStatus axCSVStringAParser::open( const char* str ) { 
	if( !str ) return -1;
	p_ = str;
	return 0; 
}

//virtual		
axStatus axCSVStringAParser::onReadLine( axStringA &buf ) {
	axStatus st;
	const char *e = NULL;

	e = ax_strchr( p_, '\r' );
	if( !e ) e = ax_strchr( p_, '\n' );
	if( !e ) { e = p_ + ax_strlen(p_); }

	for( ; *e ; e++ ) {
		bool is_break=false;
		switch( *e )  {
			case '\r':
			case '\n':
			case ' ': {
				continue;
			}break;
			default: {
				is_break = true;
			}
		}
		if( is_break ) break;
	}

	if( *p_ == 0 ) return -101;

	st = buf.setWithLength( p_, e-p_ );	if( !st ) return st;
	
	p_ = e;

	
	return 0;
}
