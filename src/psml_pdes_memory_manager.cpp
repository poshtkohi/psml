/**
	#define meta ...
	printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_pdes_memory_manager.h"
#include "psml_pdes_event.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		//psml_sm_lock psml_pdes_memory_manager::__memory_pool_lock__ = psml_sm_lock();
		Vector<psml_pdes_heap *> *psml_pdes_memory_manager::__localEventHeaps__ = null;
		Vector<psml_pdes_heap *> *psml_pdes_memory_manager::__globalEventHeaps__ = null;
		//----------------------------------------------------
		psml_pdes_event *psml_pdes_memory_manager::malloc(UInt32 senderClusterID, UInt32 receiverClusterID, UInt32 dataSize)
		{
			//psml_pdes_event *e  = (psml_pdes_event *)thread_alloc(sizeof(class psml_pdes_event) + dataSize);\
			e->SetEventType(NormalMessage);\
			e->deletable = true;\
			e->_data_size = dataSize;\
			return e;

			//psml_pdes_event *e  = (psml_pdes_event *)::malloc(sizeof(class psml_pdes_event) + dataSize);\
			e->SetEventType(NormalMessage);\
			e->deletable = true;\
			e->_data_size = dataSize;\
			return e;

			//if(psml_simulator::IsParallelExecution())
			{
				if(senderClusterID == receiverClusterID)
				{
					return (psml_pdes_event *)(*__localEventHeaps__)[senderClusterID]->Malloc(dataSize);
					//std::cout << "psml_pdes_memory_manager::malloc() " << e << std::endl;
				}
				else
				{
					//psml_pdes_heap *heap = (*__globalEventHeaps__)[senderClusterID];\
					heap->lock.lock();\
					e = heap->Malloc(dataSize);\
					heap->lock.unlock();

					psml_pdes_event *e = (psml_pdes_event *)::malloc(sizeof(class psml_pdes_event) + dataSize);\
                    e->SetEventType(NormalMessage);\
                    e->deletable = true;\
                    e->_data_size = dataSize;

					//e  = (psml_pdes_event *)thread_alloc(sizeof(class psml_pdes_event) + dataSize);\
					e->SetEventType(NormalMessage);\
					e->deletable = true;\
					e->_data_size = dataSize;
                    return e;
				}
			}
			//else
			{
				//psml_pdes_event *e = (psml_pdes_event *)::malloc(sizeof(class psml_pdes_event) + dataSize);\
				e->SetEventType(NormalMessage);\
				e->deletable = true;\
				e->_data_size = dataSize;\
				return e;
                //std::cout << "psml_pdes_memory_manager::malloc()" << std::endl;
				//return (psml_pdes_event *)(*__localEventHeaps__)[0]->Malloc(dataSize);
			}
		}
		//----------------------------------------------------
		void psml_pdes_memory_manager::free(psml_pdes_event *e)
		{
			//thread_free(e);\
			return;
			//::free(e);\
			return;

			//pool.DeAllocate(e);
			//stack.free(e);

			//if(psml_simulator::IsParallelExecution())
			{
				UInt32 senderClusterID = LogicalProcess::GetLogicalProcessById(e->GetSenderReference())->GetOwnerClusterID();
				UInt32 receiverClusterID = LogicalProcess::GetLogicalProcessById(e->GetReceiverReference())->GetOwnerClusterID();

				if(senderClusterID == receiverClusterID)
				{
					(*__localEventHeaps__)[senderClusterID]->Free(e);
				}
				else
				{
					//psml_pdes_heap *heap = (*__globalEventHeaps__)[senderClusterID];\
					heap->lock.lock();\
					heap->Free(e);\
					heap->lock.unlock();

					::free(e);

					//thread_free(e);
				}
			}
			//else
			{
				//(*__localEventHeaps__)[0]->Free(e);
				//::free(e);
			}
		}
		//----------------------------------------------------
		void psml_pdes_memory_manager::before_end_of_elaboration(const psml_simulator *sim)
		{
            //return;
			//std::cout << "psml_pdes_memory_manager::before_end_of_elaboration()" << std::endl;

			UInt32 numOfClusters = sim->_manager->GetClusterCount();

			//std::cout << "numOfClusters " << numOfClusters << std::endl;

			if(numOfClusters > 0)
			{
				__localEventHeaps__ = new Vector<psml_pdes_heap *>(numOfClusters);

				for(register UInt32 i = 0 ; i < numOfClusters ; i++)
					__localEventHeaps__->PushBack(null);
					//__localEventHeaps__->PushBack(new psml_pdes_heap());
			}

			/*if(numOfClusters > 1)
			{
				__globalEventHeaps__ = new Vector<psml_pdes_heap *>(numOfClusters);

				for(register UInt32 i = 0 ; i < numOfClusters ; i++)
					__globalEventHeaps__->PushBack(null);
					//__globalEventHeaps__->PushBack(new psml_pdes_heap());
			}*/
		}
		//----------------------------------------------------
		void psml_pdes_memory_manager::end_of_simulation(const psml_simulator *sim)
		{
            //return;
			//std::cout << "psml_pdes_memory_manager::end_of_simulation()" << std::endl;

			/*if(__localEventHeaps__ != null && __globalEventHeaps__ != null)
			{
				for(register UInt32 i = 0 ; i < __localEventHeaps__->Size() ; i++)
					std::cout << "(local,global) Cluster " << i <<  " (" << (*__localEventHeaps__)[i]->heap_allocations << "," << (*__globalEventHeaps__)[i]->heap_allocations << ")" << std::endl;
			}*/

			if(__localEventHeaps__ != null)
			{
				//std::cout << "__localEventHeaps__->Size() " << __localEventHeaps__->Size() << std::endl;
				for(register UInt32 i = 0 ; i < __localEventHeaps__->Size() ; i++)
					if((*__localEventHeaps__)[i] != null)
						delete (*__localEventHeaps__)[i];

				delete __localEventHeaps__;
			}

			/*if(__globalEventHeaps__ != null)
			{
				for(register UInt32 i = 0 ; i < __globalEventHeaps__->Size() ; i++)
					if((*__globalEventHeaps__)[i] != null)
						delete (*__globalEventHeaps__)[i];

				delete __globalEventHeaps__;
			}*/
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
