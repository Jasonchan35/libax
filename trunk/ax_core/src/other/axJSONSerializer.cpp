//
//  axJSONSerializer.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-03.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/other/axJSONSerializer.h>

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

axJSONDeserializer::axJSONDeserializer( const axIStringA &str )			{ 
	str_ = & str;
	r_ = str_->c_str();
	
	nextToken();
}

static int hex( char ch ) {
	if( ch >= '0' && ch <= '9' ) return (int)(ch - '0');
	if( ch >= 'A' && ch <= 'F' ) return (int)(ch - 'A' + 10);
	return -1;
}

axStatus	axJSONDeserializer::nextToken() {
	axStatus st = _nextToken();
	if( st ) ax_log( "token = {?}", token );
	return st;
}

axStatus	axJSONDeserializer::_nextToken() {
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

axStatus	axJSONDeserializer::checkToken( const char* sz ) {
	axStatus st;
	if( tokenIsString )		return axStatus_Std::JSON_deserialize_format_error;
	if( !token.equals(sz) )	return axStatus_Std::JSON_deserialize_format_error;
	return 0;
}

axStatus	axJSONDeserializer::checkStringToken( const char* sz ) {
	axStatus st;
	if( !tokenIsString )	return axStatus_Std::JSON_deserialize_format_error;
	if( !token.equals(sz) )	return axStatus_Std::JSON_deserialize_format_error;
	return 0;
}


axStatus axJSONDeserializer::beginObject( const char* name ) {
	return 0;
}

axStatus axJSONDeserializer::endObject() {
	return 0;
}

axStatus axJSONDeserializer::beginObjectValue() { 
	axStatus st;		
	st = checkToken("{");		if( !st ) return st;
	st = nextToken();			if( !st ) return st;
	return 0;
}

axStatus axJSONDeserializer::endObjectValue() { 
	axStatus st;
	st = checkToken("}");		if( !st ) return st;
	st = nextToken();			if( !st ) return st;
	return 0;
}

axStatus axJSONDeserializer::beginArrayValue() {
	return 0;
}

axStatus axJSONDeserializer::endArrayValue() {
	return 0;
}
