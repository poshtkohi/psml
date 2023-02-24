/**
	#define meta ...
	printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_pdes_heap.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		psml_pdes_heap::psml_pdes_heap(bool local) : heap_allocations(0), local(local)
		{
			//for(register UInt32 i = 0 ; i < maxAllocationSize + 1 ; i++)
			//	the_heap[i] = null;

			// Select the heap size based on PAGE_SIZE/(sizof(psml_pdes_event) + dataSize + sizeof(long))
			if(local)
				allocationSize = 128;//128;
			else
				allocationSize = 128;//128;

			for(register UInt32 i = 0 ; i < maxAllocationSize + 1 ; i++)
			{
				// Pre-allocation for common data types
				/*std::cout << "sizeof(psml_pdes_event) " << sizeof(psml_pdes_event) << std::endl;
				std::cout << "sizeof(bool) " << sizeof(bool) << std::endl;
				std::cout << "sizeof(char) " << sizeof(char) << std::endl;
				std::cout << "sizeof(Int16) " << sizeof(Int16) << std::endl;
				std::cout << "sizeof(Int32) " << sizeof(Int32) << std::endl;
				std::cout << "sizeof(UInt64) " << sizeof(UInt64) << std::endl;
				std::cout << "sizeof(Int128) " << sizeof(Int128) << std::endl;
				std::cout << "sizeof(Float) " << sizeof(Float) << std::endl;
				std::cout << "sizeof(Double) " << sizeof(Double) << std::endl;*/

				// common data types: bool, Int16 and Int32
				if(i == 0 || i == sizeof(bool)/* || i == sizeof(Int16) || i == sizeof(Int32)*/)
				{
					//UInt32 size = sizeof(class psml_pdes_event) + sizeof(Long) + i;
					//cout << nearestPower(sizeof(class psml_pdes_event) + i) << std::endl;
					//psml_memory_pool *newPool = new psml_memory_pool((1 * xParvicursor_getpagesize())/size, sizeof(class psml_pdes_event) + i);
					psml_memory_pool *newPool = new psml_memory_pool(allocationSize, sizeof(class psml_pdes_event) + i);
					the_heap[i] = newPool;
				}

				else
					the_heap[i] = null;
			}
		}
		//----------------------------------------------------
		psml_pdes_heap::~psml_pdes_heap()
		{
			for(register UInt32 i = 0 ; i < maxAllocationSize + 1 ; i++)
				if(the_heap[i] != null)
					delete the_heap[i];
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
