/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_simulator_h__
#define __Parvicursor_psml_psml_simulator_h__


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
#include <System.Threading/ThreadStateException/ThreadStateException.h>
#include <System/Environment/Environment.h>

#include <Parvicursor/xSim2/SimulationManagerType.h>
#include <Parvicursor/xSim2/SequentialSimulationManager.h>
#include <Parvicursor/xSim2/DeadlockAvoidanceSimulationManager.h>
#include <Parvicursor/xSim2/DeadlockDetectionSimulationManager.h>
#include <Parvicursor/xSim2/Partitioner.h>
#include <Parvicursor/xSim2/RoundRobinPartitioner.h>
#include <Parvicursor/xSim2/DepthFirstSearchPartitioner.h>
#include <Parvicursor/xSim2/TopologicalSortPartitioner.h>
#include <Parvicursor/xSim2/StronglyConnectedComponentPartitioner.h>
//#include <Parvicursor/xSim2/MetisPartitioner.h>
#include <Parvicursor/xSim2/LevelizedPartitioner.h>
#include <Parvicursor/xSim2/Vector.h>
#include <Parvicursor/xSim2/RingBuffer.h>

#include "psml_component.h"
#include "psml_port.h"
#include "psml_process.h"
#include "psml_inport.h"
#include "psml_outport.h"
#include "psml_time.h"
#include "psml_interface.h"
#include "psml_event.h"
#include "psml_nonshared_interface.h"
#include "psml_shared_interface.h"
#include "psml_globals.h"

#include <limits.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>

using namespace System;
using namespace System::Threading;

using namespace Parvicursor::xSim2;
using namespace Parvicursor::xSim2::Collections;
using namespace Parvicursor::xSim2::psc;

using namespace std;

//**************************************************************************************************************/

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations.
		//class psml_nonshared_interface;
		class psml_wait_internal;
		class psml_pdes_memory_manager;
		template <class T> class psml_wire;

		class psml_simulator : public Object
		{
            friend class psml_wait_internal;
			friend class psml_pdes_memory_manager;
			friend class psml_nonshared_interface;
			template <class T> friend class psml_wire;
			/*---------------------fields-----------------*/
			//private: static Double time_resolution;	// in femto seconds
			//private: static psml_time_unit time_unit; // default in femto seconds
            private: static bool __parallelExecution;
			private: bool _started;
			private: static SimulationManagerType _simulationManagerType;
			private: PartitionerType _partitionerType;
			private: bool _parallelExecution;
			private: UInt32 _numOfLogicalProcessors;
			private: UInt64 _simulateUntil;
			private: UInt32 _channelCapacity;
			private: SimulationManagerInterface *_manager;
			private: Partitioner *_partitioner;
			// This is used to store  psml_nonshared_intefaces created by CreateInstance() at elaboration phase
		    // after partitioning. We must release them at the end of simulation by calling DestroyInstance().
			private: std::vector<psml_nonshared_interface *> _psml_nonshared_interfaces;
			//private: Double _v;
			//private: psml_time_unit _tu;
			/*---------------------methods----------------*/
			// psml_simulator Class constructor. Default time unit is PSML_FS.
			public: psml_simulator(UInt32 numOfLogicalProcessors = 1, Partitioner *partitioner = null);
			// psml_simulator Class constructor. tu represents the time unit during the entire simulation.
			//public: psml_simulator(UInt32 numOfLogicalProcessors = 1, Double v = 1, psml_time_unit tu = PSML_NS);
			// psml_simulator Class destructor.
			public: ~psml_simulator();
			public: void start();
			public: void start(Int32 duration, psml_time_unit unit);
			public: void start(Double duration, psml_time_unit unit);
			public: void start(const psml_time &duration);
			private: void Elaborate();
			public: static Double GetTimeResolution();
			public: static void SetTimeResolution(Double v, psml_time_unit tu);
			private: static void OnPartitioningCompletion(Object *state);
			private: static void PrintCopyright();
			private: void PrintDesignStatistics();
		    // This function is called before elaboration of static events.
			private: void BeforeStaticEventElaboration();
			private: static SimulationManagerType GetSimulationManagerType() { return _simulationManagerType; }
			public: inline static bool IsParallelExecution() { return __parallelExecution; }
			public: static void Stop(psml_process *issuer);
			public: template <typename T> static string to_string(const T &n)
			{
				std::ostringstream stm ;
				stm << n ;
				return stm.str() ;
			}
		};
		//----------------------------------------------------
		static inline void psml_set_time_resolution(Double v, const psml_time_unit &tu) { psml_simulator::SetTimeResolution(v, tu); }
		static inline void psml_stop(psml_process *issuer) { psml_simulator::Stop(issuer); }
		//----------------------------------------------------
	}
};
//**************************************************************************************************************//

#endif


