//
//  axJson.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-03.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/other/axJson.h>
#include <ax/core/system/axLog.h>

axStatus	ax_to_json_str	( axIStringA & json, const wchar_t* sz, bool withQuote ) {
	axStringA_<2048>	tmp;
	axStatus st = tmp.set(sz);	if( !st ) return st;
	return ax_to_json_str( json, tmp, withQuote );
}

axStatus	ax_to_json_str	( axIStringA & json, const char* sz, bool withQuote ) {
	axStatus st;
	json.resize(0);

	if( withQuote ) {
		st = json.set("\"");		if( !st ) return st;
	}
	
	const char* p = sz;
	if( p ) {
		for( ; *p; p++ ) {
			switch( *p ) {
				case '\\': { st = json.append("\\\\");	if( !st ) return st; continue; }
				case '\"': { st = json.append("\\\"");	if( !st ) return st; continue; }
				case '/' : { st = json.append("\\/");	if( !st ) return st; continue; }
				case '\r': { st = json.append("\\r");	if( !st ) return st; continue; }
				case '\n': { st = json.append("\\n");	if( !st ) return st; continue; }
				case '\t': { st = json.append("\\t");	if( !st ) return st; continue; }
				case '\b': { st = json.append("\\b");	if( !st ) return st; continue; }
				case '\f': { st = json.append("\\f");	if( !st ) return st; continue; }
				default: {
					st = json.append(*p);	if( !st ) return st;
				}
			}
		}
	}	
	
	if( withQuote ) {
		st = json.append("\"");		if( !st ) return st;
	}	
	return 0;
}

axStatus	ax_from_json_str	( axIStringA & str, const char* json, bool withQuote ) {
	axStatus st;	
	axJsonParser	parse( json, NULL );
	parse.nextToken();
	if( withQuote != parse.tokenIsString ) return axStatus_Std::JsonParser_bool_format_error;
	return str.set( parse.token );
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

axStatus axJsonWriter::writeRawElement( const char* name, const char* value ) {
	axStatus st;
	st = member( name );	if( !st ) return st;
	st = write( value );	if( !st ) return st;
	st = nextElement();		if( !st ) return st;
	return 0;
}

axStatus axJsonWriter::nullValue() {
	return write("null");
}

axStatus axJsonWriter::stringValue( const char* sz ) {
	axStatus st;
	axTempStringA tmp;
	st = ax_to_json_str( tmp, sz, true );	if( !st ) return st;
	return str_->append( tmp );
}

axStatus axJsonWriter::member( const char* name ) {
	axStatus st;
	st = newline();				if( !st ) return st;		
	st = stringValue( name );	if( !st ) return st;	
	st = str_->append( ":" );	if( !st ) return st;
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
	st = endObjectValue();		if( !st ) return st;
	st = nextElement();			if( !st ) return st;
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
	st = endArrayValue();		if( !st ) return st;
	st = nextElement();			if( !st ) return st;
	return 0;
}

axStatus axJsonWriter::beginObjectValue	()	{ return begin_('{'); }
axStatus axJsonWriter::endObjectValue 	()	{ return end_('{','}'); }

axStatus axJsonWriter::beginArrayValue	() 	{ return begin_('['); }
axStatus axJsonWriter::endArrayValue	() 	{ return end_('[',']'); }

axStatus axJsonWriter::begin_( const char ch ) { 
	if( ended_ ) {
		assert(false);
		return axStatus_Std::JsonParser_format_error;
	}

	axStatus st;
	st = str_->append(ch);		if( !st ) return st;
	depth_++;
	return 0;
};

axStatus axJsonWriter::end_( const char begin, const char ch ) {
	axStatus st;
	if( depth_ == 0 ) {
		assert(false);
		return axStatus_Std::JsonParser_format_error;
	}
	depth_--;

	if( depth_ == 0 ) ended_ = true;
	
	if( str_->size() == 0 ) {
		assert(false);
		return axStatus_Std::JsonParser_format_error;
	}
	
	char e = str_->lastChar();
	if( e == ',' ) {
		str_->decSize(1);
		st = newline();			if( !st ) return st;
	}else{
		if( e != begin ) {
			ax_log("Json missing ',' end of {?} error", ch );
			assert(false);
			return axStatus_Std::JsonParser_format_error;
		}
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
axJsonParser::axJsonParser( const char* json, axIStringA* errorLog )	{
	lineNo_ = 1;
	start_ = json;
	r_ = json;
	lineStart_ = r_;
	errorLog_ = errorLog;
	nextToken();
}

static int hex( char ch ) {
	if( ch >= '0' && ch <= '9' ) return (int)(ch - '0');
	if( ch >= 'A' && ch <= 'F' ) return (int)(ch - 'A' + 10);
	return -1;
}

template<>
axStatus ax_json_serialize_value( axJsonWriter &s, bool &v )	{
	return s.write( v ? "true" : "false" );
}

template<> 
axStatus ax_json_serialize_value( axJsonParser &s, bool &v ) {
	axStatus st;	
	if( s.tokenIsString )	return -1;

	if( s.checkToken( "true" ) ) {
		v = true;
	}else if( s.checkToken( "false" ) ) {
		v = false;
	}else {
		return axStatus_Std::JsonParser_bool_format_error;
	}
	
	st = s.nextToken();		if( !st ) return st;
	return 0;
}

axStatus	axJsonParser::log ( const char* msg ) {
	axStatus st;
	axTempStringA	tmp;
	st = tmp.format("Json({?}:{?}) {?}\n  Token:[{?}] {?}\n",
					 lineNo(), charNo(), msg, token, tokenIsString ? "isString":"" );
	if( !st ) return st;
	if( errorLog_ ) {
		return errorLog_->append( tmp );
	}else{
		ax_log( tmp );
	}
	return 0;
}

axStatus axJsonParser::logIgnoreMember( const char* name ) {
	axStatus st;
	axTempStringA	tmp;
	st = tmp.format("Json({?}:{?})  ignore member [{?}]\n", lineNo(), charNo(), name );
	if( !st ) return st;
	
	if( errorLog_ ) {
		return errorLog_->append( tmp );
	}else{
		ax_log( tmp );
	}
	return 0;
}



axStatus	axJsonParser::nextToken() {
	axStatus st = _nextToken();
	if( !st ) return st;

//	ax_log("Debug: Json({?}:{?}) Token:[{?}]", lineNo(), charNo(), token );
	return 0;
}

axStatus	axJsonParser::_nextToken() {
	axStatus st;
	const char* singleCharTokens = ":,{}[]";

	token.resize(0);
	if( ! r_ ) return 0;
	
	tokenIsString = false;
	
	for( ;*r_; r_++ ) {
		if( *r_ == '\n' ) { lineNo_++; lineStart_ = r_; continue; }	
		if( ax_strchr( " \t\r", *r_ ) ) continue;
		break;
	}

	//---------------
	for( ; *r_; r_++) {
		if( *r_ == '\n' ) { lineNo_++; lineStart_ = r_; }
		if( tokenIsString ) {
			if( *r_ == '\\' ) {
				r_++;
				switch( *r_ ) {
					case 0: {
						log("expect '\"' ");
						return axStatus_Std::JsonParser_expect_close_quota;
					}break;
				//------
					case '\\': { st = token.append('\\'); if( !st ) return st; continue; }
					case '\"': { st = token.append('\"'); if( !st ) return st; continue; }
					case '/':  { st = token.append('/');  if( !st ) return st; continue; }
					case 'b':  { st = token.append('\b'); if( !st ) return st; continue; }
					case 'n':  { st = token.append('\n'); if( !st ) return st; continue; }
					case 'r':  { st = token.append('\r'); if( !st ) return st; continue; }
					case 't':  { st = token.append('\t'); if( !st ) return st; continue; }
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
						st = ax_safe_assign( w, v );	if( !st ) return st;
						st = token.append( w );			if( !st ) return st;
					}continue;
				//--------	
					default: { //unknown escape
						log("unknown escape in string");
						return axStatus_Std::JsonParser_unknown_escape_in_string;
					}
				}
			}

			if( *r_ == '\"' ) { r_++; return 1; }
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
				case '\n':	{ r_++; return 0; }
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
		log("expect '\"' ");
		return axStatus_Std::JsonParser_expect_close_quota;
	}
	return 0;
}

axStatus	axJsonParser::checkToken( const char* sz ) {
	axStatus st;
	if( tokenIsString )		return axStatus_Std::JsonParser_format_error;
	if( !token.equals(sz) )	return axStatus_Std::JsonParser_format_error;
	return 0;
}

axStatus	axJsonParser::checkStringToken( const char* sz ) {
	axStatus st;
	if( !tokenIsString ) 	return axStatus_Std::JsonParser_format_error;
	if( !token.equals(sz) )	return axStatus_Std::JsonParser_format_error;
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

axStatus axJsonParser::getMemberName( axIStringA & out ) {
	axStatus st;
	if( ! tokenIsString ) return -1;
	st = out.set( token );			if( !st ) return st;
	st = nextToken();				if( !st ) return st;	
	st = checkToken(":");			if( !st ) return st;
	st = nextToken();				if( !st ) return st;	
	return 0;
}

axStatus axJsonParser::nextElement() {
	axStatus st;
	
	if( checkToken("]") ) return 0;
	if( checkToken("}") ) return 0;
	
	st = checkToken(",");			if( !st ) return st;
	st = nextToken();				if( !st ) return st;
	return 0;
}

axStatus	axJsonParser::skipValue() {
	if( tokenIsString ) return nextToken();

	if( checkToken("null")  ) return nextToken();
	if( checkToken("true")  ) return nextToken();
	if( checkToken("false") ) return nextToken();
	
	if( checkToken("{") ) return skipBlock( '{', '}' );
	if( checkToken("[") ) return skipBlock( '[', ']' );

//number
	double tmp;
	axStatus st = ax_str_to( token, tmp );
	if( st ) return nextToken();
	
	assert(false);
	return axStatus_Std::JsonParser_format_error;
}

axStatus axJsonParser::skipBlock( char open, char close ) {
	int lv = 0;
	for( ; *r_; r_++ ) {
		if( *r_ == '\n' ) { lineNo_++; lineStart_ = r_; }
		if( *r_ == open  ) { lv++; continue; }
		if( *r_ == close ) {
			if( lv == 0 ) {
				r_++;
				return nextToken();
			}
			lv--;
		}
	}
	assert(false);
	return axStatus_Std::JsonParser_format_error;
}

axStatus axJsonParser::beginObject( const char* name ) {
	axStatus st;
	st = parseMember( name );		if( !st ) return st;
	st = beginObjectValue();		if( !st ) return st;
	return 0;
}

axStatus axJsonParser::endObject() {
	axStatus st;
	st = endObjectValue();		if( !st ) return st;
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
	st = checkToken(",");
	if( st ) {
		axTempStringA	tmp;
		
		for(;;) {
			st = nextToken();				if( !st ) return st;
			st = getMemberName( tmp );		if( !st ) return st;
			st = logIgnoreMember( tmp );	if( !st ) return st;
			st = skipValue();				if( !st ) return st;
			if( checkToken("}") ) break;
		}
	}
	
	st = checkToken("}");		if( !st ) return st;
	st = nextToken();			if( !st ) return st;
	return 0;
}

bool axJsonParser::isObjectEnded() {
	return checkToken("}");
}

axStatus axJsonParser::beginArray( const char* name ) {
	axStatus st;
	st = parseMember( name );	if( !st ) return st;
	st = beginArrayValue();		if( !st ) return st;
	return 0;
}

axStatus axJsonParser::endArray() {
	axStatus st;
	st = endArrayValue();		if( !st ) return st;
	return 0;
}

axStatus axJsonParser::beginArrayValue() {
	axStatus st;
	st = checkToken("[");		if( !st ) return st;
	st = nextToken();			if( !st ) return st;
	return 0;
}

axStatus axJsonParser::endArrayValue() {
	axStatus st;
	st = checkToken("]");		if( !st ) return st;
	st = nextToken();			if( !st ) return st;
	return 0;
}

bool axJsonParser::isArrayEnded() {
	return checkToken("]");
}
