/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_process_h__
#define __Parvicursor_psml_psml_process_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>
#include <System.Threading/Fiber/Fiber.h>

#include <Parvicursor/xSim2/Vector.h>

//#include "psml_port.h"
#include "psml_pdes_lp.h"
#include "psml_edge_trigger_type.h"
#include "psml_wait_types.h"
#include "psml_time.h"
#include "psml_hash_table.h"
#include "psml_globals.h"

#include <iostream>
#include <vector>
#include <map>

using namespace System;
using namespace System::Threading;
using namespace Parvicursor::xSim2::psc;

using namespace std;

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
        //----------------------------------------------------
        enum psml_process_type
        {
            __STACKLESS_PROCESS__ ,
            __STACKFULL_PROCESS__ ,
            __SPECIAL_PROCESS__
        };
        //----------------------------------------------------
        class psml_process_state : public Object
        {
            private: Int32 _label;
            public: psml_process_state() : _label(-1) {}
            public: ~psml_process_state() { _label = -1; }
            public: inline Int32 get_label() { return _label; }
            public: inline void set_label(Int32 newLabel) { _label = newLabel; }
            //public: virtual void serialize() = 0;
            //public: virtual void deserialize() = 0;
        };
		//----------------------------------------------------
		#define __PMSL_DEFAULT_STACK_SIZE 16384//4096 // 32KB

/*#if defined WIN32 || WIN64
#	if !defined _M_X64
	#define __PMSL_DEFAULT_STACK_SIZE 0x20000
#	else if (defined _M_X64)
	#define __PMSL_DEFAULT_STACK_SIZE 0x40000
#	endif

#else

#	if !defined __x86_64__
    #define __PMSL_DEFAULT_STACK_SIZE 0x20000
#	else if (defined __x86_64__)
    #define __PMSL_DEFAULT_STACK_SIZE 0x40000
#	endif
#endif*/
		// Forward declarations.
		class psml_component;
		class psml_process;
		class psml_simulator;
		class psml_event;
		class psml_wait_internal;
		class psml_time;
		class psml_nonshared_interface;
		class psml_port;
		template <class T> class psml_wire;
		class psml_proxy_interface;
		enum psml_edge_trigger_type;
		class psml_clock;

		typedef void (*ProcessMethod)(/*psml_component *, */psml_process *);

		class psml_process : public Object
		{
			friend class psml_simulator;
			friend class psml_component;
			friend class psml_pdes_lp;
			friend class psml_event;
			friend class psml_wait_internal;
			friend class psml_time;
			friend class psml_nonshared_interface;
			template <class T> friend class psml_wire;
			friend class psml_clock;
			/*---------------------fields-----------------*/
			//private: bool _scheduled;
			// All internal data such as the following (_wait_type, event IDs, counters, _process_state and etc) must be
			// checkpointed for optimistic protocols with PS2.
			private: psml_wait_types _wait_type;
			private: psml_process_type _process_type;
			private: ProcessMethod _pm;
			private: psml_component *_ownerComponent;
			// Input ports of the process.
			//private: vector<psml_port *> inports;
			private: std::map<psml_port *, psml_port *> _inports; // map: (key, value)
			// Output ports of the process
			//private: vector<psml_port *> outports;
			private: std::map<psml_port *, psml_port *> _outports;
			private: std::map<psml_port *, psml_port *> _gports;
            private: Vector<psml_nonshared_interface *> _flush_list;
            // uInt32 is size of the total message. UInt16 is port number.
            typedef std::pair<UInt16, psml_nonshared_interface *> portType;
            typedef Vector<portType> vectorType;
            typedef std::pair<UInt32 *, vectorType *> hashTableType;
            private: psml_hash_table<hashTableType> *_flush_ht;
			private: psml_pdes_lp *_lp; //
			//private: Fiber *_fib;
			private: Fiber *_fib_process;
			//private: Fiber *_cur_fiber;
			private: const psml_process_state *_process_state;
			// There are used to implement psml_wait(n)
			private: UInt32 _n_registered;
			private: UInt32 _n_counter;
			// We should store the refernce to senstive ports or psml_event in the sensitivity list
			private: UInt32 _num_of_sensitivity_list;
			// The following variables must be added to the state for optimistic PDES execution.
			private: bool _has_wait_registered;
			private: psml_event *_wait_e;
			// A Singleton pattern to store the whole process methods during a simulation instance.
			private: static vector<psml_process *> *__processes;
			private: static Vector<Fiber *> __fiber_clusters;
			private: static Vector<UInt32> *__cycles;
			private: String _objName;
			//private: bool _from_chained_execution;
            private: psml_time _delay;
            private: bool _dont_initialize;
            private: bool _stop_requested;
            private: psml_hash_table<UInt32> *_order;
            private: Vector<UInt32> _orders;
			/*---------------------methods----------------*/
			// psml_process Class constructor.
			public: psml_process();
			// psml_process Class destructor.
			public: ~psml_process();
			// psml_process Class destructor.
			// Registers a port with the process.
			public: void register_port(psml_port *port);
			public: void register_port(psml_port &port);
			// Only input ports which contain the psml_nonshared_interface instance can be registered
			// as sensitivity list. This eases the development of RTL-style codes atop PSML.
			// Other semantics similar to what are present in SystemC can be archived with integrating
			// the two classes of psml_shared_interface and psml_event for mixed or system-level
			// model development.
			public: void register_sensitivity(psml_port *port);
			public: void register_sensitivity(psml_port &port);
			// The following methods are used to declare sensitivity on edge  triggers.
			public: void register_sensitivity(psml_port *port, psml_edge_trigger_type type);
			public: void register_sensitivity(psml_port &port, psml_edge_trigger_type type);
			//public: void register_sensitivity(psml_event *e);
			//public: void register_sensitivity(psml_event &e);
			private: static void *Wrapper_To_Call_Fiber(void *pt2Object);
			public: const String &GetName();
			public: inline LogicalProcess *get_logical_process_reference() { return _lp; }
			// Gets the processes type registered with register_process() or register_special_process() method.
			public: inline psml_process_type get_process_type() { return _process_type; }
			// Gets the state associated with this process instance through register_special_process() method.
			public: inline const psml_process_state *get_process_state() { return _process_state; }
			public: inline psml_component *get_owner_component() { return _ownerComponent; }
            // Gets the current delay assocciated with this process instance.
			public: const psml_time &get_delay() const { return _delay; }
			// Sets the delay assocciated with this process instance.
			public: void set_delay(const psml_time &d) { _delay = d; }
            // Prevents the processe to get extecuted at the begining of the simulation phase.
            public: void dont_initialize();
            // Flushes the flush list to the PDES layer.
			protected: void Flush();
			protected: void before_end_of_elaboration();
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
