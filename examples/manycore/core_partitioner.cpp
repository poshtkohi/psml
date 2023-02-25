/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "core_partitioner.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            psml_core::psml_core()
            {
            }
            //----------------------------------------------------
            psml_core::~psml_core()
            {
                _processes.clear();
            }
            //----------------------------------------------------
            void psml_core::add_process(psml_process *p)
            {
                if(p == null)
                    throw ArgumentNullException("p", "p is null");

                _processes.push_back(p);
            }
            //----------------------------------------------------
            core_partitioner::core_partitioner(std::vector<psml_core *> *cores)
            {
                if(cores == null)
                    throw ArgumentNullException("cores", "cores is null");

                _cores = cores;
            }
            //----------------------------------------------------
            core_partitioner::~core_partitioner()
            {
            }
            //----------------------------------------------------
            PartitionInfo *core_partitioner::Partition(Vector<LogicalProcess *> *objects, UInt32 &numberOfPartitions)
            {
                if(objects == null)
                    throw ArgumentNullException("objects", "objects is null");

                if(numberOfPartitions <= 0)
                    throw ArgumentOutOfRangeException("numberOfPartitions", "numberOfPartitions is less than or equal to zero");

                PartitionInfo *info = new PartitionInfo();
                Vector< Vector<LogicalProcess*>* > partitions(numberOfPartitions);

                for(register UInt32 i = 0 ; i < numberOfPartitions ; i++)
                    partitions[i] = new Vector<LogicalProcess *>;

                UInt32 perCore = _cores->size() / numberOfPartitions;
                UInt32 remainder = _cores->size() % numberOfPartitions;

                // Distributes objects Int32o cores.
                for(register UInt32 partitionIndex = 0 ; partitionIndex < numberOfPartitions ; partitionIndex++)
                {
                    for(register UInt32 index = partitionIndex * perCore ; index < partitionIndex * perCore + perCore ; index++)
                    {
                        psml_core *core = (*_cores)[index];
                        for(register UInt32 lpIndex = 0 ; lpIndex < core->_processes.size() ; lpIndex++)
                        {
                            LogicalProcess *lp = core->_processes[lpIndex]->get_logical_process_reference();
                            lp->SetOwnerClusterID(partitionIndex);
                            partitions[partitionIndex]->PushBack(lp);
                        }
                    }
                }

                // Distributes the remainder objects Int32o the last core.
                if(remainder > 0)
                {
                    for(register UInt32 index = numberOfPartitions * perCore ; index < _cores->size() ; index++)
                    {
                        psml_core *core = (*_cores)[index];
                        for(register UInt32 lpIndex = 0 ; lpIndex < core->_processes.size() ; lpIndex++)
                        {
                            LogicalProcess *lp = core->_processes[lpIndex]->get_logical_process_reference();
                            lp->SetOwnerClusterID(numberOfPartitions - 1);
                            partitions[numberOfPartitions - 1]->PushBack(lp);
                        }
                    }
                }

                for(register UInt32 i = 0 ; i < numberOfPartitions ; i++)
                    info->AddPartition(partitions[i]);

                //info->PrInt32Information();

                return info;
            }
            //----------------------------------------------------
        }
	}
};
//**************************************************************************************************************//
