/**
	#define meta ...
	printf("%s\n", meta);
**/

/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_globals_h__
#define __Parvicursor_psml_psml_globals_h__

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
        #define __zero_copy_enabled_for_psml_nonshared_interfaces__
        //#define __profile_mode_enabled_for_psml_pdes_lp__
        //#define __cycles_enabled__
#if defined WIN32 || WIN64
        const UInt64 UINT64_ZERO   = 0i64;
        const UInt64 UINT64_ONE    = 1i64;
        const UInt64 UINT64_32ONES = 0x00000000ffffffffi64;
#else
        const UInt64 UINT64_ZERO   = 0ULL;
        const UInt64 UINT64_ONE    = 1ULL;
        const UInt64 UINT64_32ONES = 0x00000000ffffffffULL;
#endif
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
