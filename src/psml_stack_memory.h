/**
#define meta ...
printf("%s\n", meta);
**/

/*
All rights reserved to Alireza Poshtkohi (c) 1999-2023.
Email: arp@poshtkohi.info
Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_stack_memory_h__
#define __Parvicursor_psml_psml_stack_memory_h__


#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/Environment/Environment.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include <stdlib.h>

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations.
		class psml_stack_memory : public Object
		{
			private: struct node { void *data; };
			/*---------------------fields-----------------*/
			private: UInt32 _free;
			private: struct node *_stack;
			private: void *_data;
			private: UInt32 _slotSize;
			private: UInt32 _numOfSlots;
			private: Long _blockId;
			/*---------------------methods----------------*/
			// psml_stack_memory Class constructor.
			public: psml_stack_memory(Long blockId, UInt32 slotSize, UInt32 numOfSlots);
			// psml_stack_memory Class destructor.
			public: ~psml_stack_memory();
			public: inline void *malloc()
			{
				_free--;
				return _stack[_free].data;
			}
			public: inline void free(void *memslot)
			{
				_stack[_free].data = memslot;
				_free++;
			}
			public: inline UInt32 get_free() { return _free; }
			private: void shrink();
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
