/**
	#define meta ...
	printf("%s\n", meta);
**/

/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_pdes_memory_manager_h__
#define __Parvicursor_psml_psml_pdes_memory_manager_h__


#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/Environment/Environment.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>
#include <Parvicursor/xSim2/LogicalProcess.h>
#include <Parvicursor/xSim2/Vector.h>

#include "psml_sm_lock.h"
#include "psml_simulator.h"
#include "psml_pdes_heap.h"
#include "psml_stack_memory.h"
#include "psml_pdes_event.h"
#include "threadalloc.h"

#include <iostream>

using namespace System;
using namespace Parvicursor::xSim2;
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
		class psml_pdes_event;
		class psml_sm_lock;
		class psml_memory_pool;
		class psml_simulator;
		class psml_pdes_heap;
		class psml_pdes_lp;

		class psml_pdes_memory_manager : public Object
		{
			friend class psml_simulator;
			friend class psml_pdes_lp;
			/*---------------------fields-----------------*/
			//private: static psml_sm_lock __memory_pool_lock__;
			private: static Vector<psml_pdes_heap *> * __localEventHeaps__;
			private: static Vector<psml_pdes_heap *> *__globalEventHeaps__;
			/*---------------------methods----------------*/
			// psml_pdes_memory_manager Class constructor.
			//public: psml_pdes_memory_manager();
			// psml_pdes_memory_manager Class destructor.
			//public: ~psml_pdes_memory_manager();
			public: static psml_pdes_event *malloc(UInt32 senderClusterID, UInt32 receiverClusterID, UInt32 dataSize);
			/*{
                //psml_pdes_event *e  = (psml_pdes_event *)thread_alloc(sizeof(class psml_pdes_event) + dataSize);\
                e->SetEventType(NormalMessage);\
                e->deletable = true;\
                e->_data_size = dataSize;\
                return e;

                psml_pdes_event *e  = (psml_pdes_event *)::malloc(sizeof(class psml_pdes_event) + dataSize);\
                e->SetEventType(NormalMessage);\
                e->deletable = true;\
                e->_data_size = dataSize;\
                return e;
			}*/
			public: static void free(psml_pdes_event *e);
			/*{
                //thread_free(e);
                ::free(e);
			}*/
			private: static void shrink();
			private: static void before_end_of_elaboration(const psml_simulator *sim);
			private: static void end_of_simulation(const psml_simulator *sim);
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
