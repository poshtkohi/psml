/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_component_h__
#define __Parvicursor_psml_psml_component_h__


#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/DateTime/DateTime.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/Guid/Guid.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/ArgumentException/ArgumentException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include <Parvicursor/xSim2/Vector.h>

//#include "psml_inport.h"
#include "psml_port.h"
#include "psml_process.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace System;
using namespace Parvicursor::xSim2::psc;

using namespace std;

//**************************************************************************************************************/

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations.
		class psml_pdes_lp;
		class psml_process;
		class psml_proxy_interface;
		class psml_simulator;
		class psml_clock;
		template <class IF> class psml_inport;
		template <class IF> class psml_outport;
		template <class IF> class psml_gport;

		class psml_component : public Object
		{
			friend class psml_pdes_lp;
			friend class psml_process;
			friend class psml_simulator;
			friend class psml_clock;
			template <class IF> friend class psml_inport;
			template <class IF> friend class psml_outport;
			template <class IF> friend class psml_gport;
			/*---------------------fields-----------------*/
			// Tracks in and out ports to indirectly be used by psml framework.
			private: Vector<psml_port *> _inports;
			private: std::vector<psml_port *> _outports;
			private: std::vector<psml_port *> _gports;
			//(key, value) = (ownerPort, proxy)
			private: std::map<psml_proxy_interface *, psml_proxy_interface *> _proxies;
			// A Singleton pattern to store the whole connector instances during a simulation instance.
			private: static std::vector<psml_component *> *__components;
			//private: std::map<psml_component *, psml_component *> __components;
			// The name of the component instance.
			private: String _objName;
			/*---------------------methods----------------*/
			// psml_component Class constructor.
			public: psml_component();
			// psml_component Class destructor.
			public: ~psml_component();
			// Stackful process methods are coroutines A generalized routine that in addition to
			// traditional subroutine operations such as invoke and return supports suspend and resume operations.
			//	Stackful Coroutine / Fiber / User-Mode Thread
			// A stackful coroutine state includes the full call stack associated with its execution
			// enabling suspension from nested stack frames. Stackful coroutines are equivalent to fibers
			// or user-mode threads.
			public: psml_process *register_process(ProcessMethod pm, bool stackful, const String &name = "");
			// This method creates a fast stackfull process that does not use fibers as the underlying context-siwtching mechanims.
			// The user is obligated to handle a much of the low-level tasks. This may cause to siginificant performance gain
			// in conrast to using stackfull fibered fibred pprocesses.
			public: psml_process *register_special_process(ProcessMethod pm, const psml_process_state *state, const String &name = "");
			// Registers a proxy associates with the component to connect to external components or sub-components in the structure hierarchy.
			// A proxy can act either port-less forwarder or port-bound forwarder. In the former, the proxy only
			// forward the access to the proxy to other proxies. The latter, in addition to forward the access, binds
			// a port.
			// Port-bound proxy.
			public: void register_proxy(psml_proxy_interface *proxy, psml_port *ownerPort);
			public: void register_proxy(psml_proxy_interface &proxy, psml_port &ownerPort);
			// Port-less proxy.
			public: void register_proxy(psml_proxy_interface *proxy);
			public: void register_proxy(psml_proxy_interface &proxy);
			// Registers a child port to the proxy. The child ports are in the same component that the
			// proxy resides. The proxy is used to connect the proxy to internal ports of the component.
			///private: void register_proxy_child(psml_proxy_interface *proxy, psml_port *child);
			///private: void register_proxy_child(psml_proxy_interface &proxy, psml_port &child);
			//public: void register_proxy(psml_proxy_interface *proxy, psml_port *port);
			//public: void register_proxy(psml_proxy_interface &proxy, psml_port &port);
			// Gets the name of the component instance.
			public: const String &GetName();
			public: void set_name(const String &name);
			// This method is invoked after elaboration phase.
			public: virtual void before_static_event_elaboration();
			public: virtual void before_end_of_elaboration();
		};
		//----------------------------------------------------
	}
};
//**************************************************************************************************************//

#endif


