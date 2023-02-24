/**
	#define meta ...
	printf("%s\n", meta);
**/

/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_wait_types_h__
#define __Parvicursor_psml_psml_wait_types_h__

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------

		enum psml_wait_types
		{
			__WAIT,				// wait()
			__WAIT_N,		    // wait(n)
			__WAIT_E,			// wait(e)
			__WAIT_T,			// wait(t)
			//__WAIT_ANDLIST,		// wait(e1 & e2 & e3)
			//__WAIT_ORLIST		// wait(e1 | e2 | e3)
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
