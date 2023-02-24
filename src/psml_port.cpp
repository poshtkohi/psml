/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_port.h"
#include "psml_clock.h"
#include "psml_process.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		std::vector<psml_port *> *psml_port::__ports = null;
		//----------------------------------------------------
		psml_port::psml_port() : _portID(0), _sensitive(false), _ownerProcess(null), _connector(null), _interface(null)
		{
            if(__ports == null)
                __ports = new std::vector<psml_port *>;

			__ports->push_back(this);
		}
		//----------------------------------------------------
		psml_port::~psml_port()
		{
		}
		//----------------------------------------------------
		/*void psml_port::SetSensitive(bool sensitive)
		{
			_sensitive = sensitive;
		}
		//----------------------------------------------------
		bool psml_port::GetSensitive()
		{
			return _sensitive;
		}*/
		//----------------------------------------------------
		void psml_port::SetEdgeTriggerType(psml_edge_trigger_type type)
		{
		}
		//----------------------------------------------------
		psml_edge_trigger_type psml_port::GetEdgeTriggerType()
		{
			return PSML_UNKNOW_EDGE;
		}
		//----------------------------------------------------
		psml_pdes_event *psml_port::GetEvent()
		{
			return null;
		}
		//----------------------------------------------------
		void psml_port::SetEvent(psml_pdes_event *e)
		{

		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
