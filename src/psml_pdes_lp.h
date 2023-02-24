/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_pdes_lp_h__
#define __Parvicursor_psml_psml_pdes_lp_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System.IO/IOException/IOException.h>
#include <System.Threading/Fiber/Fiber.h>

#include <Parvicursor/xSim2/LogicalProcess.h>
#include <Parvicursor/xSim2/Vector.h>

#include "psml_globals.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace System;
using namespace System::IO;
using namespace System::Threading;
using namespace Parvicursor::xSim2;
using namespace Parvicursor::xSim2::psc;

using std::string;
using namespace std;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations.
		class psml_process;
		class psml_event;
		class psml_time;
		class psml_port;
		class psml_wait_internal;
		class psml_nonshared_interface;
		class psml_pdes_event;

		class psml_pdes_lp : public LogicalProcess
		{
			friend class psml_process;
			friend class psml_event;
			friend class psml_time;
			friend class psml_wait_internal;
			friend class psml_nonshared_interface;
			/*---------------------fields-----------------*/
		    // A reference to the main PSML process.
			private: psml_process *_p;
			private: Vector<psml_port *> *_inports;
			private: bool _first_execution;
			//private: Vector<psml_port *> _uncommitted_insensitive_inports;
			//private: UInt64 numEventProcessed;
			//private: ServerState *myState;
#ifdef __Parvicursor_xSim2_Debug_Enable__
			private: ofstream *logFile;
#endif
			/*---------------------methods----------------*/
			/// psml_pdes_lp Class constructor.
			public: psml_pdes_lp(psml_process *p);
			/// psml_pdes_lp Class destructor.
			public: ~psml_pdes_lp();

			/// Overridden methods
			public: void Elaborate();
			public: void Initialize();
			public: void Finalize();
			public: void ExecuteProcess();
			public: const String &GetName();
			public: State *GetState();
			public: State *AllocateState();
			public: void DeallocateState(const State *state);
			public: inline void DeallocateEvent(const EventInterface *e);

			/// Other methods
			public: void PrintInformation();
			private: bool ProcessPeekedNextEvents();
			private: bool ProcessAggregatedNsiEvent(psml_pdes_event *e);
			//private: void CommitInsenistivePorts();
			/*--------------------------------------------*/

		};
		//----------------------------------------------------
	}
};
//**************************************************************************************************************//

#endif

