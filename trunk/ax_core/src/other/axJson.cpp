//
//  axJson.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-03.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/other/axJson.h>

axStatus	ax_to_json_str	( axIStringA & str, const char* sz, bool withQuote ) {
	axStatus st;

	axSize 	offset = 0;
	str.resize(0);
	if( withQuote ) {
		st = str.set("\"");		if( !st ) return st;
		offset++;
	}
	st = str.append( sz ); 	if( !st ) return st;
	
	
	st = str.replaceString( "\\", "\\\\", offset ); if( !st ) return st;
	st = str.replaceString( "\"", "\\\"", offset ); if( !st ) return st;
	st = str.replaceString( "/",  "\\/",  offset ); if( !st ) return st;
	st = str.replaceString( "\r", "\\r",  offset ); if( !st ) return st;
	st = str.replaceString( "\n", "\\n",  offset ); if( !st ) return st;
	st = str.replaceString( "\t", "\\t",  offset ); if( !st ) return st;
	st = str.replaceString( "\b", "\\b",  offset ); if( !st ) return st;
	st = str.replaceString( "\f", "\\f",  offset ); if( !st ) return st;
	
	if( withQuote ) {
		st = str.append("\"");		if( !st ) return st;
	}	
	return 0;
}

axStatus	ax_from_json_str	( axIStringA & str, const char* sz, bool withQuote ) {
	axStatus st;
	
	if( withQuote ) {
		if( !sz || sz[0] != '\"' ) return axStatus_Std::JSON_deserialize_format_error;
		size_t n = ax_strlen( sz );
		if( n < 2 || sz[n-1] != '\"' ) return axStatus_Std::JSON_deserialize_format_error;
	
		st = str.setWithLength( sz+1, n-2 );		if( !st ) return st;
	}else{
		st = str.set( sz );							if( !st ) return st;
	}

	st = str.replaceString( "\\\\", "\\" ); if( !st ) return st;
	st = str.replaceString( "\\\"", "\"" ); if( !st ) return st;
	st = str.replaceString( "\\/",  "/"  ); if( !st ) return st;
	st = str.replaceString( "\\r",  "\r" ); if( !st ) return st;
	st = str.replaceString( "\\n",  "\n" ); if( !st ) return st;
	st = str.replaceString( "\\t",  "\t" ); if( !st ) return st;
	st = str.replaceString( "\\b",  "\b" ); if( !st ) return st;
	st = str.replaceString( "\\f",  "\f" ); if( !st ) return st;

	return 0;
}

//========================


axJsonWriter::axJsonWriter( axIStringA &str, bool condense, const char* indent ) { 
	str_ = & str;
	depth_ = 0;
	ended_ = false;
	condense_ = condense;
	indent_.set( indent );
}

axStatus axJsonWriter::write( const char* sz ) {
	return str_->append( sz );
}

axStatus axJsonWriter::nextElement() {
	return str_->append( "," );
}

axStatus axJsonWriter::member( const char* name ) {
	axStatus st;
	st = newline();			if( !st ) return st;		
	axTempStringA	tmp;

	st = ax_to_json_str( tmp, name, true );		if( !st ) return st;
	st = str_->append( tmp );					if( !st ) return st;
	st = str_->append( ":" );					if( !st ) return st;
	return 0;
}

axStatus axJsonWriter::beginObject	( const char* name ) 	{ 
	axStatus st;
	st = member( name );		if( !st ) return st;
	st = begin_('{'); 			if( !st ) return st;
	return 0;
}

axStatus axJsonWriter::endObject () { 
	axStatus st;
	st = end_('{','}'); 		if( !st ) return st;
	st = str_->append( "," );	if( !st ) return st;
	return 0;
}

axStatus axJsonWriter::beginArray( const char* name ) { 
	axStatus st;
	st = member( name );		if( !st ) return st;
	st = begin_('['); 			if( !st ) return st;
	return 0;
}
axStatus axJsonWriter::endArray	() 	{ 
	axStatus st;
	st = end_('[',']'); 	if( !st ) return st;
	st = str_->append( "," );	if( !st ) return st;
	return 0;
}

axStatus axJsonWriter::beginObjectValue	()	{ return begin_('{'); }
axStatus axJsonWriter::endObjectValue 	()	{ return end_('{','}'); }

axStatus axJsonWriter::beginArrayValue	() 	{ return begin_('['); }
axStatus axJsonWriter::endArrayValue	() 	{ return end_('[',']'); }

axStatus axJsonWriter::begin_( const char ch ) { 
	if( ended_ ) return axStatus_Std::JSON_deserialize_format_error;

	axStatus st;
	st = str_->append(ch);		if( !st ) return st;
	depth_++;
	return 0;
};

axStatus axJsonWriter::end_( const char begin, const char ch ) {
	axStatus st;
	if( depth_ == 0 )  		return axStatus_Std::JSON_deserialize_format_error;
	depth_--;

	if( depth_ == 0 ) ended_ = true;
	
	if( str_->size() == 0 ) return axStatus_Std::JSON_deserialize_format_error;
	
	char e = str_->lastChar(0);
	if( e == ',' ) {
		str_->decSize(1);
		st = newline();				if( !st ) return st;
	}else{
		if( e != begin ) 	return axStatus_Std::JSON_deserialize_format_error;
	}
	st = str_->append(ch);			if( !st ) return st;		
	return 0;
}

axStatus axJsonWriter::newline() {
	if( condense_ ) return 0;
	
	axStatus st;
	st = str_->append( "\n" );			if( !st ) return st;
	
	if( indent_.size() > 0 ) {	
		for( axSize i=0; i<depth_; i++ ) {
			st = str_->append(indent_);		if( !st ) return st;
		}
	}
	return 0;
}
	

//=======================
axJsonParser::axJsonParser( const axIStringA &str, bool memberMustInOrder )	{
	str_ = & str;
	r_ = str_->c_str();
	ignoreUnknownMemeber_ = true;
	memberMustInOrder_ = memberMustInOrder;
	nextToken();
}

void axJsonParser::setIgnoreUnknownMemeber( bool b ) {
	ignoreUnknownMemeber_ = b;
}

static int hex( char ch ) {
	if( ch >= '0' && ch <= '9' ) return (int)(ch - '0');
	if( ch >= 'A' && ch <= 'F' ) return (int)(ch - 'A' + 10);
	return -1;
}

/*
axStatus	axJsonParser::nextToken() {
	axStatus st = _nextToken();
	if( st ) ax_log( "json token = {?}", token );
	return st;
}
*/
axStatus	axJsonParser::nextToken() {
	axStatus st;

	const char* singleCharTokens = ":,{}[]";

	token.resize(0);
	if( ! r_ ) return 0;
	
	tokenIsString = false;
	
	for( ;*r_; r_++ ) {
		if( ax_strchr( skip_chr(), *r_ ) ) continue;
		break;
	}

	//---------------
	for( ; *r_; r_++) {
		if( tokenIsString ) {
			if( *r_ == '\\' ) {
				r_++;
				switch( *r_ ) {
					case 0: 	return -1;
				//-----
					case '/':	
					case '\\':	
						break;
				//------
					case 'b': { r_++; st = token.append('\b'); if( !st ) return st; continue; }
					case 'n': { r_++; st = token.append('\n'); if( !st ) return st; continue; }
					case 'r': { r_++; st = token.append('\r'); if( !st ) return st; continue; }
					case 't': { r_++; st = token.append('\t'); if( !st ) return st; continue; }
				//-------
					case 'u': {
						r_++;
						int u = 0;
						int v;
						v = hex(*r_++);	if(v<0) return -1;	u <<= 4; u+=v;	
						v = hex(*r_++);	if(v<0) return -1;	u <<= 4; u+=v;	
						v = hex(*r_++);	if(v<0) return -1;	u <<= 4; u+=v;	
						v = hex(*r_++);	if(v<0) return -1;	u <<= 4; u+=v;
						
						wchar_t w;
						st = ax_safe_assign( w, v );			if( !st ) return st;
						st = token.append( w );		if( !st ) return st;
					}continue;
				//--------	
					default: { //unknown escape
						return -1;
					}
				}
			}
		
			if( *r_ == '\"' ) {
				r_++;
				return 1;
			}
										
			st = token.append( *r_ );		if( !st ) return st;						
		}else{
			if( ax_strchr( singleCharTokens, *r_ ) ) {
				if( token.size() == 0 ) {		
					st = token.append( *r_ );		if( !st ) return st;
					r_++;
				}
				return 0;
			}
		
			switch( *r_ ) {
				case 0: 	return 0;
			//-----
				case '\"':	{ 
					tokenIsString = true;
				} continue;

				default:	{ 
					st = token.append( *r_ ); if( !st ) return st;						
				}continue;
				
			}								
		}
	}

	if( tokenIsString ) {
		assert(false);	// quota excepted for close string
		return -1;
	}
	return 0;	
}

axStatus	axJsonParser::checkToken( const char* sz ) {
	axStatus st;
	if( tokenIsString )		return axStatus_Std::JSON_deserialize_format_error;
	if( !token.equals(sz) )	return axStatus_Std::JSON_deserialize_format_error;
	return 0;
}

axStatus	axJsonParser::checkStringToken( const char* sz ) {
	axStatus st;
	if( !tokenIsString )	return axStatus_Std::JSON_deserialize_format_error;
	if( !token.equals(sz) )	return axStatus_Std::JSON_deserialize_format_error;
	return 0;
}

axStatus axJsonParser::parseMember ( const char* name ) {
	axStatus st;
	st = checkStringToken( name );	if( !st ) return st;
	st = nextToken();				if( !st ) return st;
	st = checkToken(":");			if( !st ) return st;
	st = nextToken();				if( !st ) return st;
	return 0;
}

axStatus	axJsonParser::skipValue() {
	if( tokenIsString ) return nextToken();
	double tmp;
	axStatus st = ax_str_to( token, tmp );
	if( st ) return nextToken();
	
	if( checkToken("null") ) return nextToken();
	
	if( checkToken("{") ) return skipBlock( '{', '}' );
	if( checkToken("[") ) return skipBlock( '[', ']' );
	
	return axStatus_Std::JSON_deserialize_format_error;
}

axStatus axJsonParser::skipBlock( char open, char close ) {
	int lv = 0;
	for( ; *r_; r_++ ) {
		if( *r_ == open ) { lv++; continue; }
		if( *r_ == close ) {
			if( lv == 0 ) {
				r_++;
				return nextToken();
			}
			lv--;
		}
	}
	return axStatus_Std::JSON_deserialize_format_error;
}

axStatus axJsonParser::beginObject( const char* name ) {
	return 0;
}

axStatus axJsonParser::endObject() {
	return 0;
}

axStatus axJsonParser::beginObjectValue() { 
	axStatus st;		
	st = checkToken("{");		if( !st ) return st;
	st = nextToken();			if( !st ) return st;
	return 0;
}

axStatus axJsonParser::endObjectValue() { 
	axStatus st;
	st = checkToken("}");		if( !st ) return st;
	st = nextToken();			if( !st ) return st;
	return 0;
}

axStatus axJsonParser::beginArrayValue() {
	return 0;
}

axStatus axJsonParser::endArrayValue() {
	return 0;
}
