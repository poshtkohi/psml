/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_connector_h__
#define __Parvicursor_psml_psml_connector_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/Guid/Guid.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include <Parvicursor/xSim2/Vector.h>

//#include "psml_port.h"

#include <iostream>
#include <vector>
#include <map>

using namespace System;
using namespace Parvicursor::xSim2::psc;

using namespace std;

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations.
		//class psml_connector;
		class psml_simulator;
		class psml_process;
		class psml_nonshared_interface;
		class psml_pdes_lp;
		class psml_interface;
		class psml_clock;
		class psml_port;
		template <class IF> class psml_proxy;
		template <class IF> class psml_inport;
		template <class IF> class psml_outport;
		template <class IF> class psml_gport;

		class psml_connector : public Object
		{
			friend class psml_simulator;
			friend class psml_nonshared_interface;
			friend class psml_pdes_lp;
			friend class psml_clock;
			friend class psml_process;
			template <class IF> friend class psml_proxy;
			template <class IF> friend class psml_inport;
			template <class IF> friend class psml_outport;
			template <class IF> friend class psml_gport;
			/*---------------------fields-----------------*/
			// Stores the destination processes with respect to this connector instance. This map is used by the elaboration phase
		    // to optimally search the subscribers on static events. New destination processes have to be inserted to the map
			// by the bind() method of psml_inport class.
			// Stores the original interface bound to this connector.
			private: psml_interface *_interface;
			// Tracks in and out ports with respect to this connector instance to indirectly be used by psml framework.
			// _outports come from target processes and _inports come from destination processes.
			// target ===>>> connector ===>>> destination
			private: Vector<psml_port *> *_inports;
			private: std::vector<psml_port *> _outports;
			private: std::vector<psml_port *> _gports;
			private: std::map<psml_process *, psml_process *> _dest_processes;
			// A Singleton pattern to store the whole connector instances during a simulation instance.
			// For DLL/SO support, we must change the static variables to be properly loaded.
			private: static vector<psml_connector *> *__connectors;
			// The name of the connector instance.
			private: String _objName;
			/*---------------------methods----------------*/
			// psml_connector Class constructor.
			public: psml_connector(const String &name = "");
			// psml_connector Class destructor.
			public: ~psml_connector();
			// Gets the name of the connector instance.
			public: const String &GetName();
			public: void SetName(const String &n);
			private: psml_interface *GetOwnerInterface();
			private: void RegisterOwnerInterface(psml_interface *_if);
			private: static void before_end_of_elaboration(const psml_simulator *sim);
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
