/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/


#ifndef __Parvicursor_psml_CorePartitioner_h__
#define __Parvicursor_psml_CorePartitioner_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/Random/Random.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>

#include <Parvicursor/xSim2/Partitioner.h>
#include <Parvicursor/xSim2/LogicalProcess.h>

#include <Parvicursor/psml/psml_process.h>

using namespace System;
using namespace Parvicursor::xSim2::psc;

#include <vector>
using namespace std;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations
		class CorePartitioner;

		class psml_core : public Object
		{
			friend class CorePartitioner;
			/*---------------------fields-----------------*/
			private: std::vector<psml_process *> _processes;
			/*---------------------methods----------------*/
			public: psml_core();
			public: ~psml_core();
			public: void add_process(psml_process *p);
		};
		//----------------------------------------------------
		class CorePartitioner : public Partitioner
		{
			/*---------------------fields-----------------*/
			private: std::vector<psml_core *> *_cores;
			/*---------------------methods----------------*/
			// CorePartitioner Class constructor.
			public: CorePartitioner(std::vector<psml_core *> *cores);
			// CorePartitioner Class destructor.
			public: ~CorePartitioner();

			/// Overridden methods
			public: PartitionInfo *Partition(Vector<LogicalProcess *> *objects, UInt32 &numberOfPartitions);
			/*--------------------------------------------*/

		};
		//----------------------------------------------------
	}
};
//**************************************************************************************************************//

#endif

