#include <ax/core/parser/axCSVParser.h>

axCSVParserBase::axCSVParserBase() {
	line_buf_max_size_ = 2048;
	seperators_ = ',';
	line_feeds_.set( "\r\n");
	r_ = NULL;
}

void	axCSVParserBase::setLineBufSize( axSize n ) {
	line_buf_max_size_ = n;
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

axStatus axCSVParserBase::getCell_( axIStringA &cell ) {
	axStatus st;
	if( ! *r_ ) return axStatus_Std::CSVParser_get_cell_error;

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
			
			if( *p == seperators_ ) {
				st = cell.appendWithLength( r_, p-r_ );		if( !st ) return st;
				p++;
				break;
			}
			else if( *p == '\r' ) { //handle Windows Line Feed "\r\n"
				if( p[1] != '\n' ) {
					assert( false );
					return axStatus_Std::CSVParser_get_cell_error;
				}
				st = cell.appendWithLength( r_, p-r_ );		if( !st ) return st;
				p++;
				p++;
				break;
			}else if( *p == '\n' ) { //handle Unix Line Feed  "\n"
				st = cell.appendWithLength( r_, p-r_ );		if( !st ) return st;
				p++;
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
	if( !str ) return axStatus_Std::CSVParser_cannot_open;
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
	if( !e ) return axStatus_Std::CSVParser_readline_error;
	
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
