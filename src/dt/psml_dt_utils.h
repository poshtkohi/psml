/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_dt_utils_h__
#define __Parvicursor_psml_psml_dt_utils_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include <iostream>
#include <string>
#include <limits.h>

//#if defined WIN32 || WIN64
#include <stdlib.h>
//#endif

#include <cryptopp/integer.h>

using namespace System;
using namespace std;
using namespace CryptoPP;
//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		class psml_dt_utils : public Object
		{
			// Returns string representation of num in base rad as new string (or NULL)
			public: static std::string Int64toBaseString(Int64 num, Int16 rad);
			// Finds the integer number based on the specified radix (base).
			public: static Int64 BaseStringToInt64(const std::string &num,  Int16 rad);
			public: static Int64 BaseStringToInt64(const char *num, Int16 rad);
            public: static UInt64 BaseStringToUInt64(const std::string &num,  Int16 rad);
			public: static UInt64 BaseStringToUInt64(const char *num, Int16 rad);
			// The following two methods can be unified within a single template-based function.
			public: static Int64 GetBits(Int64 value, unsigned offset, unsigned n);
			public: static const CryptoPP::Integer GetBits(const CryptoPP::Integer &value, unsigned offset, unsigned n);
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//
#endif
