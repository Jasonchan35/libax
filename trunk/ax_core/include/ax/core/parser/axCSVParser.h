#ifndef __axCSVParser_h__
#define __axCSVParser_h__

#include "../file_system/axFileStream.h"
#include "../system/axLog.h"

class axCSVParserBase : public axNonCopyable {
public:
	axCSVParserBase();
	virtual	~axCSVParserBase();
	axStatus	getRow( axStringA_Array	&row );

	void		setSeperator( char s );
	void		setLineBufSize( axSize n );

protected:
	virtual		axStatus	onReadLine( axStringA &buf ) = 0;
	const char*		r_; //current line_buf read ptr
	axSize			line_buf_max_size_;
	axStringA		line_buf_;
	char			seperators_;
	axStringA_<4>	line_feeds_;

private:
	axStatus	readLine_	();
	axStatus	getCell_	( axStringA &cell );
};


class axCSVFileParser : public axCSVParserBase {
	typedef axCSVParserBase B;
public:

	axStatus	open( const wchar_t* filename );
	axStatus	open( const char* filename );	

protected:
	virtual		axStatus	onReadLine( axStringA &buf );
private:
	axFileStream		file_;
};

class axCSVStringAParser : public axCSVParserBase {
	typedef axCSVParserBase B;
public:

	axCSVStringAParser() {
		p_ = NULL;
	}
	axStatus	open( const char* str );

protected:
	virtual		axStatus	onReadLine( axStringA &buf );
private:
	const char* p_;

};

#endif //__axCSVParser_h__
