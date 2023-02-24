/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_interface.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		std::vector<psml_interface *> *psml_interface::__interfaces = null;
		//----------------------------------------------------
		psml_interface::psml_interface() : _ownerConnector(null)
		{
            if(__interfaces == null)
                __interfaces = new std::vector<psml_interface *>;

			__interfaces->push_back(this);
		}
		//----------------------------------------------------
		psml_interface::~psml_interface()
		{
		}
		//----------------------------------------------------
		void psml_interface::before_static_event_elaboration()
		{
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
