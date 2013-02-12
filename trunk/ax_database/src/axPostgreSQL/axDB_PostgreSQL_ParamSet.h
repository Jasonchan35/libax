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
	
	axStatus	bind		( axSize index, const axDBInParam & param );
	axStatus	bindList	( const axDBInParamList & list );

	axStatus	_bindValue	( axSize index, axTimeStamp ts );

	axStatus	setTypes	( const axDBInParamList & list );
	bool		isSameTypes	( const axDBInParamList & list );
		
	static	Oid	c_type_to_Oid( int c );
	
	struct UnionType {
		union {
			int8_t		bool_as_int8;	//1:true, 0:false in pgSQL
			int16_t		int16_;
			int32_t		int32_;
			int64_t		int64_;
			double		double_;
			float		float_;
		};
		axStatus	onTake( UnionType &src ) { *this = src; return 0; }
	};
		
	enum { BUF_SIZE = 32 };

	axArray< Oid,			BUF_SIZE >		types;
	axArray< int,			BUF_SIZE >		lengths;
	axArray< int,			BUF_SIZE >		formats;
	axArray< const void*,	BUF_SIZE >		pData; //point to strData or uniData
	axArray< axStringA,		BUF_SIZE >		strData;	
	axArray< UnionType,		BUF_SIZE >		uniData;	
};

#endif //__axDB_PostgreSQL_ParamSet_h__
