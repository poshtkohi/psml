/**
#define meta ...
printf("%s\n", meta);
**/

/*
All rights reserved to Alireza Poshtkohi (c) 1999-2023.
Email: arp@poshtkohi.info
Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_pdes_heap_h__
#define __Parvicursor_psml_psml_pdes_heap_h__


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

#include "psml_sm_lock.h"
#include "psml_memory_pool.h"
#include "psml_pdes_event.h"

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
		class psml_pdes_memory_manager;
		class psml_pdes_event;
		class psml_sm_lock;
		//class psml_memory_pool;

		class psml_pdes_heap : public Object
		{
			friend class psml_pdes_memory_manager;
			/*---------------------fields-----------------*/
			private: psml_memory_pool *the_heap[256]; // 1024
			private: psml_sm_lock lock;
			private: UInt64 heap_allocations;
			private: UInt32 allocationSize;
			private: static const UInt32 maxAllocationSize = 255; //1023
			private: bool local;
			/*---------------------methods----------------*/
			// psml_pdes_heap Class constructor.
			public: psml_pdes_heap(bool local);
			// psml_pdes_heap Class destructor.
			public: ~psml_pdes_heap();
			private: void shrink();
			//----------------------------------------------------
			public: inline psml_pdes_event *Malloc(UInt32 dataSize)
			{
				psml_pdes_event *e;

				if(dataSize > maxAllocationSize)
					e  = (psml_pdes_event *)::malloc(sizeof(class psml_pdes_event) + dataSize);
				else
				{
					psml_memory_pool *pool = the_heap[dataSize];

					if(pool == null)
					{
						//UInt32 size = sizeof(class psml_pdes_event) + sizeof(Long) + dataSize;
						//pool = new psml_memory_pool(xParvicursor_getpagesize()/size, sizeof(class psml_pdes_event) + dataSize);

						//UInt32 size  = sizeof(class psml_pdes_event) + sizeof(Long) + dataSize;
						//psml_memory_pool *newPool = new psml_memory_pool((1 * xParvicursor_getpagesize())/size, sizeof(class psml_pdes_event) + dataSize);
						pool = new psml_memory_pool(allocationSize, sizeof(class psml_pdes_event) + dataSize);
						the_heap[dataSize] = pool;
					}

					e = (psml_pdes_event *)pool->Malloc();
				}

				e->SetEventType(NormalMessage);
				e->deletable = true;
				e->_data_size = dataSize;

				heap_allocations++;

				return e;
			}
			//----------------------------------------------------
			public: inline void Free(psml_pdes_event *e)
			{
                //std::cout << e->_data_size << std::endl;
				if(e->_data_size > maxAllocationSize)
					::free(e);
				else
					the_heap[e->_data_size]->Free(e);
			}
			//----------------------------------------------------
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
