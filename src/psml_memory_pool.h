/**
	#define meta ...
	printf("%s\n", meta);
**/

/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_memory_pool_h__
#define __Parvicursor_psml_psml_memory_pool_h__


#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include <Parvicursor/xSim2/Vector.h>

#include "psml_sm_lock.h"
#include "psml_stack_memory.h"
#include "psml_pdes_event.h"

#include <iostream>

using namespace System;
using namespace Parvicursor::xSim2::psc;

using namespace std;

// # is the alignment value. Valid entries are integer powers of two from 1 to 8192 (bytes), such as 2, 4, 8, 16, 32, or 64. declarator is the data that you are declaring as aligned.
//#define CACHE_LINE  64
//#define CACHE_ALIGN __declspec(align(CACHE_LINE))

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations.
		///struct Header;
		///struct Block;
		class psml_pdes_memory_manager;
		class psml_pdes_heap;

		/*struct Header // The header definition.
		{
			struct Block *owner;
			struct Header *next; // The successor node with respect to this node.
		};*/
		struct Block // The block definition.
		{
			///struct Header *firstNode; // The firstNode pointer.
			///struct Header *lastNode; // The lastNode pointer.
			struct Block *next; // A reference to the next node
			struct Block *prev; // A reference to the previous node
			///void *buffer; // The allocated memory for the memory block and free linked list.
			///psml_single_memory_pool *pool;
			psml_stack_memory *stack;
			UInt32 free;
			UInt32 capacity;
		};
		//----------------------------------------------------
		class psml_memory_pool : public Object
		{
			friend class psml_pdes_memory_manager;
			friend class psml_pdes_heap;
			/*---------------------fields-----------------*/
			private: struct Block *notFullList_firstNode;
			private: struct Block *notFullList_lastNode;
			private: struct Block *fullList_firstNode;
			private: struct Block *fullList_lastNode;
			private: UInt32 poolSlots; // The number of slots within each memory block.
			private: UInt32 slotSize; // The fixed size of the allocated memory associated with every slot.
			private: UInt32 defaultNumOfBlocks;
			/*---------------------methods----------------*/
			// psml_memory_pool Class constructor.
			public: psml_memory_pool(UInt32 poolSlots, UInt32 slotSize);
			// psml_memory_pool Class destructor.
			public: ~psml_memory_pool();
			public: inline void *Malloc()
			{
				void *memslot;
				Block *block;

				if(notFullList_firstNode != null)
				{
					//printf("Malloc 2\n"); fflush(stdout); ///
					block = notFullList_firstNode;
					memslot = block->stack->malloc();
					/*memslot = (void *)( (char *)block->firstNode + sizeof(struct Header) );
					block->firstNode = block->firstNode->next;*/
					///memslot = block->pool->Allocate();
				}
				else
				{
					//printf("Malloc 1\n"); fflush(stdout); ///
					block = AllocateNewBlock();
					memslot = block->stack->malloc();
					/*memslot = (void *)( (char *)block->firstNode + sizeof(struct Header) );
					block->firstNode = block->firstNode->next;*/
					///memslot = block->pool->Allocate();
				}

				block->free--;

				if(block->free == 0)
				{
					// Remove the block from notFullList and add to fullList

					if(block == notFullList_firstNode) {
						//change first to point to next link
						notFullList_firstNode = notFullList_firstNode->next;
					}else {
						//bypass the current link
						block->prev->next = block->next;
					}
					if(block == notFullList_lastNode){
						//change last to point to prev link
						notFullList_lastNode = block->prev;
					}else {
						block->next->prev = block->prev;
					}

					block->prev = null;
					block->next = null;

					if(fullList_firstNode == null){
						//make it the last link
						fullList_lastNode = block;
					}else {
						//update first prev link
						fullList_firstNode->prev = block;
					}
					//point it to old first link
					block->next = fullList_firstNode;
					//point first to new first link
					fullList_firstNode = block;
				}

				return memslot;
			}
			public: inline void Free(void *memslot)
			{
				//printf("Free 1\n"); fflush(stdout); ///

				/*Header *header = (struct Header *)( (char *)memslot - sizeof(struct Header) );
				Block *block = header->owner;*/
				Block *block = (struct Block *) *( (Long *)( (unsigned char *)memslot + slotSize ) );

				///Block *block = (struct Block *) *( (Long *)( (char *)memslot + slotSize ) );
				//std::cout << "psml_memory_pool::Free() memBlock: " << (Long)block << std::endl;
				///block->pool->DeAllocate(memslot);

				if(block->free == 0)
				{
					///block->firstNode = block->lastNode = header;

					// Remove from fullList and add into notFullList

					if(block == fullList_firstNode) {
						//change first to point to next link
						fullList_firstNode = fullList_firstNode->next;
					}else {
						//bypass the current link
						block->prev->next = block->next;
					}
					if(block == fullList_lastNode){
						//change last to point to prev link
						fullList_lastNode = block->prev;
					}else {
						block->next->prev = block->prev;
					}

					block->prev = null;
					block->next = null;

					if(notFullList_firstNode == null){
						//make it the last link
						notFullList_lastNode = block;
					}else {
						//update first prev link
						notFullList_firstNode->prev = block;
					}
					//point it to old first link
					block->next = notFullList_firstNode;
					//point first to new first link
					notFullList_firstNode = block;
				}
				/*else
				{
					// Insert into the end
					//block->lastNode->next = header;
					//block->lastNode = header;

					// Insert into the beginning. This is cache-friendly because it inserts the memslot
					// into beginning of the list where it is more possible to complete the next allocation request.
					header->next = block->firstNode;
					block->firstNode = header;
				}*/

				// lastFreeBlock
				block->free++;

				block->stack->free(memslot);
			}
			private: Block *AllocateNewBlock();
			//private: static void shrink();
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
