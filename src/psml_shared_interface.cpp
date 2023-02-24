/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_shared_interface.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		psml_shared_interface::psml_shared_interface() : _mustProtect(false)
		{
		}
		//----------------------------------------------------
		psml_shared_interface::~psml_shared_interface()
		{
		}
		//----------------------------------------------------
		void psml_shared_interface::CallExecute(InOut psml_remote_param_interface *param)
		{

		}
		//----------------------------------------------------
		void psml_shared_interface::SetMustProtect(bool val)
		{
			_mustProtect = val;
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
