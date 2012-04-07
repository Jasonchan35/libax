//
//  axDB_PostgreSQL_ParamSet.h
//  axDB_PostgreSQL
//
//  Created by Jason on 18/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef __axDB_PostgreSQL_ParamSet_h__
#define __axDB_PostgreSQL_ParamSet_h__

#include "axDB_PostgreSQL_common.h"

class axDB_PostgreSQL_ParamSet {
public:

	axDB_PostgreSQL_ParamSet();	
	axStatus	resize		( axSize n );
	axSize		size		() { return types.size(); }
	
	axStatus	bind		( axSize index, const axDB_Param & param );
	axStatus	bindList	( const axDB_ParamList & list );

	axStatus	setTypes	( const axDB_ParamList & list );
	bool		isSameTypes	( const axDB_ParamList & list );
		
	static	Oid	c_type_to_Oid( int c );
	
	struct UnionType {
		union {
			int8_t	int8_;
			int16_t int16_;
			int32_t int32_;
			int64_t int64_;
			double	double_;
			float	float_;
		};
		axStatus	onTake( UnionType &src ) { int64_ = src.int64_; return 0; }
	};
		
	enum { BUF_SIZE = 32 };

	axArray< Oid, BUF_SIZE >				types;
	axArray< int, BUF_SIZE >				lengths;
	axArray< int, BUF_SIZE >				formats;
	axArray< const void*, BUF_SIZE >		pData; //point to strData or uniData
	axArray< axStringA_<32>, BUF_SIZE >		strData;	
	axArray< UnionType, BUF_SIZE >			uniData;	
};

#endif //__axDB_PostgreSQL_ParamSet_h__
