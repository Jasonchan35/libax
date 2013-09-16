#pragma once

#ifndef __axArray_types_h__
#define __axArray_types_h__

#include "axArray.h"
#include "axExternalArray.h"
#include "axLocalArray.h"
#include "axAutoPtr.h"

//! \ingroup base_data_structure
//@{

typedef axIArray<uint8_t>			axIByteArray;
typedef	axArray<uint8_t>			axByteArray;
typedef axExternalArray<uint8_t>	axExternalByteArray;

/* typedef template does not work on VC 2008 yet
template<size_t LOCAL_BUF_SIZE > typedef axArray<uint8_t, LOCAL_BUF_SIZE>	axByteArray_;
*/




//@}

#endif //__axArray_types_h__

