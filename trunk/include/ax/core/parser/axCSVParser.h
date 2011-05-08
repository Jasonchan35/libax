#ifndef __axCSVParser_h__
#define __axCSVParser_h__

#include "../../ax_base.h"

class axCSVParserBase : public axNonCopyable {
public:
	axCSVParserBase();
	virtual	~axCSVParserBase();
	axStatus	getRow( axStringA_Array	&row );

	axStatus	setSeperators( const char* char_list );

protected:
	virtual		axStatus	onReadLine( axStringA &buf ) = 0;
	axSize			line_buf_max_size_;
	axSize			line_buf_offset_;
	axStringA		line_buf_;
	axStringA_<4>	seperators_;
	axStringA_<4>	line_feeds_;

private:
	axStatus	readLine();
	axStatus	getCell( axStringA &cell );
};


class axCSVFileParser : public axCSVParserBase {
	typedef axCSVParserBase B;
public:
	axStatus	open( const wchar_t* filename );

protected:
	virtual		axStatus	onReadLine( axStringA &buf );
private:
	axFile		file_;
};


#endif //__axCSVParser_h__
