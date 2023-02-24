/**
	#define meta ...
	printf("%s\n", meta);
**/

/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_clock_h__
#define __Parvicursor_psml_psml_clock_h__


#define __STACKLESS__


#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>
#include <Parvicursor/xSim2/Vector.h>

#include "psml_component.h"
#include "psml_process.h"
#include "psml_connector.h"
#include "psml_wire.h"
#include "psml_outport.h"
#include "psml_proxy.h"
#include "psml_wait.h"
#include "psml_simulator.h"
#include "psml_time.h"
#include "psml_edge_trigger_type.h"

#include <iostream>

#include <vector>

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
		//class psml_process;
		//class psml_pdes_event;
        //class psml_component;
        /*class psml_process;
        class psml_connector;
        class psml_wait;
        class psml_simulator;
        class psml_time;
        class psml_port;
		template <class IF> class psml_outport;
		template <class IF> class psml_proxy;
		template <class T> class psml_wire;*/

		class psml_clock : public psml_component
		{
			/*---------------------fields-----------------*/
			public: psml_proxy<psml_wire<bool > > out;
			private: psml_outport<psml_wire<bool> > _out;
//#if defined __STACKLESS__
			/*private: psml_inport<psml_wire<UInt64> > _self;
			private: psml_connector _self_connector;
			private: psml_wire<UInt64> _self_wire;*/
//#endif
			private: bool first;
			private: UInt64 _m_counter;
			private: bool _m_clk;
			private: bool _m_posedge_first;   // true if first edge is positive
			private: Double _m_duty_cycle;	// the duty cycle (fraction of period)
			private: psml_time _m_start_time;	// the start time of the first edge
			private: psml_time _delay;
			private: psml_time _wire_delay;
			private: psml_time _m_period;		// the period of this clock
			private: Vector<psml_port *> _posedge_subscribers;
			private: Vector<psml_port *> _negedge_subscribers;
			private: Vector<psml_port *> _bothedges_subscribers;
			private: psml_process *p;
			/*---------------------methods----------------*/
			// psml_clock Class constructor.
			public: psml_clock(const String &name = "", const psml_time &period = psml_time(0), Double duty_cycle = 0.5, const psml_time &start_time = psml_time(0), bool posedge_first = true);
			// psml_clock Class destructor.
			public: ~psml_clock();
			//private: void process(psml_process *ownerProcess);
			//private: static void Wrapper_To_Call_process(psml_component *owner, psml_process *ownerProcess);
			private: static void process(psml_process *ownerProcess);
			private: void before_end_of_elaboration();
			private: void Initialize();
			public: inline psml_process *get_process() { return p; }
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
