/**
	#define meta ...
	printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_stack_memory.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		psml_stack_memory::psml_stack_memory(Long blockId, UInt32 slotSize, UInt32 numOfSlots) : _blockId(blockId), _slotSize(slotSize), _numOfSlots(numOfSlots)
		{
			// UInt32 __slotSize = ((slotSize + sizeof(blockId) + 8-1) & ~(8-1));
			//std::cout << "psml_stack_memory::psml_stack_memory() block " << blockId << std::endl;
			//exit(0);
			_stack = (struct node *)::malloc(_numOfSlots * sizeof(struct node));
			//void *__stack = null;
			//posix_memalign(&__stack , 64, _numOfSlots * sizeof(struct node));
			 //_stack = (struct node *)__stack;
			_data = (void *)::malloc((slotSize + sizeof(blockId)) * _numOfSlots);
			//_data = (void *)::malloc(1 * xParvicursor_getpagesize());


			//posix_memalign(&_data, 64, (slotSize + sizeof(blockId)) * _numOfSlots);


			if(_stack == null)
			{
				printf("malloc() failed in psml_stack_memory::psml_stack_memory()\n");//
				exit(-1);
			}

			_free = numOfSlots;
			Long _pointer = -(_slotSize + sizeof(blockId));
			//Long _pointer = -_slotSize;

			for(register UInt32 i = 0 ; i < _numOfSlots ; i++)
			{
				//_pointer += _slotSize;
				//void *slot = (void *)((char *)_data + _pointer);
				_pointer += (_slotSize + sizeof(blockId));
				void *slot = (void *)((unsigned char *)_data + _pointer);
				//Long *slotBlockId = (Long *)((char *)slot + _slotSize );
				//*slotBlockId = blockId;
				//std::cout << *slotBlockId << std::endl;
				//void *slot = (void *)::malloc(_slotSize + sizeof(blockId) ) ;
				//void *slot = null;
				//posix_memalign(&slot, 64, _slotSize + sizeof(blockId));
				Long *slotBlockId = (Long *)((unsigned char *)slot + _slotSize );
				*slotBlockId = blockId;
				_stack[i].data = slot;
			}

		}
		//----------------------------------------------------
		psml_stack_memory::~psml_stack_memory()
		{
			if(_stack != null)
				::free(_stack);

			if(_data != null)
				::free(_data);

			//std::cout << "~psml_stack_memory()" << std::endl;
		}
		//----------------------------------------------------
		/*void *psml_stack_memory::malloc()
		{
			if(_free == 0)
				return null;

			_free--;
			//cout << "_free " << _free << std::endl;
			//cout << "psml_stack_memory::malloc() _free " << _free << std::endl;
			return _stack[_free].data;
		}
		//----------------------------------------------------
		void psml_stack_memory::free(void *memslot)
		{
			//cout << "psml_stack_memory::free() _free " << _free << std::endl;
			_stack[_free].data = memslot;
			_free++;
		}*/
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
