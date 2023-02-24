/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_pdes_event_h__
#define __Parvicursor_psml_psml_pdes_event_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>

#include <Parvicursor/xSim2/EventInterface.h>
#include <Parvicursor/xSim2/LogicalProcess.h>

#include "psml_event.h"
#include "psml_globals.h"

#include <iostream>

using namespace System;
using namespace Parvicursor::xSim2;
using namespace std;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations.
		class psml_wait_internal;
		class psml_pdes_lp;
		class psml_nonshared_interface;
		//class psml_event;
		class psml_clock;
		class psml_process;
		class psml_simulator;
		class psml_pdes_memory_manager;
		class psml_pdes_heap;

		class psml_pdes_event : public EventInterface
		{
			friend class psml_wait_internal;
			friend class psml_pdes_lp;
			friend class psml_event;
			friend class psml_nonshared_interface;
			friend class psml_clock;
			friend class psml_simulator;
			friend class psml_pdes_memory_manager;
			friend class psml_pdes_heap;
			friend class psml_process;
			/*---------------------fields-----------------*/
			// PSML event type.
			private: psml_event_type _type;
			// The data to be transfered by this event.
			private: UInt32 _data_size;
#if defined __cycles_enabled__
			private: UInt32 cycle;
#endif
			/*---------------------methods----------------*/
			/// psml_pdes_event Class constructor.
			/*private*/public: psml_pdes_event(UInt64 sendTime, UInt64 receiveTime, UInt32 sender, UInt32 receiver);
			/// psml_pdes_event Class destructor.
			/*private*/public: ~psml_pdes_event();
			public: void print();
			/*--------------------------------------------*/

		};
		//----------------------------------------------------
	}
};
//**************************************************************************************************************//

#endif

