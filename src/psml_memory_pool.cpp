/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_memory_pool.h"

//#include <sys/mman.h>
//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		psml_memory_pool::psml_memory_pool(UInt32 poolSlots, UInt32 slotSize)
		{
			//if(slotSize == 0)
			//	slotSize = sizeof(UInt64);

			if(slotSize == 0)
				throw ArgumentOutOfRangeException("slotSize", "slotSize can not be zero");

			defaultNumOfBlocks = 1;
			this->poolSlots = poolSlots;

			this->slotSize = slotSize;

			notFullList_firstNode = null;
			notFullList_lastNode = null;
			fullList_firstNode = null;
			fullList_lastNode = null;

			for(register UInt32 i = 0 ; i < defaultNumOfBlocks ; i++)
			{
				AllocateNewBlock();
			}
		}
		//----------------------------------------------------
		psml_memory_pool::~psml_memory_pool()
		{
			///std::cout << "psml_memory_pool::~psml_memory_pool()" << std::endl;

			Block *node = notFullList_firstNode;
			Block *victim = null;
			//navigate till the end of the list
			while(node != null)
			{
				victim = node;
				//cout << "Block1 " << victim << " free " << victim->free << std::endl;
				node = node->next;
				//std::cout << "Block " << victim << " in notFullList have " << (victim->capacity - victim->free) << " allocated slots and " << victim->free << " free slots."<< endl;
				///::free(victim->buffer);
				delete victim->stack;
				///delete victim->pool;
				::free(victim);
			}

			node = fullList_firstNode;
			victim = null;
			while(node != null)
			{
				victim = node;
				//cout << "Block2 " << victim << " free " << victim->free << std::endl;
				node = node->next;
				//std::cout << "Block " << victim << " in fullList have " << (victim->capacity - victim->free) << " allocated slots and " << victim->free << " free slots."<< endl;
				///::free(victim->buffer);
				delete victim->stack;
				///delete victim->pool;
				::free(victim);
			}
		}
		//----------------------------------------------------
		Block *psml_memory_pool::AllocateNewBlock()
		{
			/*void *buffer = (void *)malloc(poolSlots * (slotSize + sizeof(struct Header)));
			//p = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
			//void *buffer = mmap(null, poolSlots * (slotSize + sizeof(struct Header)), PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

			if(buffer == null)
			{
				printf("malloc failed\n");//
				exit(-1);
			}*/

			struct Block *memBlock = (struct Block *)malloc(sizeof(struct Block));
			memBlock->prev = null;
			memBlock->next = null;
			/*struct Header *node = null;
			struct Header *temp = null;
			struct Header *head = null;
			struct Header *firstNode = null;
			struct Header *lastNode = null;

			for(register UInt32 i = 0 ; i < poolSlots ; i++)
			{
				struct Header *node = (struct Header *)( (char *)buffer + i*(slotSize + sizeof(struct Header)) );
				node->owner = memBlock;
				//node->index = i;
				head = node;

				if(temp != null)
					temp->next = node;

				temp = node;

				if(i == 0)
					firstNode = (Header *)head;

				//printf("i: %d\n", i);//
			}

			head->next = null;
			lastNode = (Header *)head;
			head = firstNode;

			//std::cout << "psml_memory_pool::AllocateNewBlock() memBlock: " << (Long)memBlock << std::endl;
			//exit(0);
			memBlock->buffer = buffer;*/
			///memBlock->pool = new psml_single_memory_pool((Long)memBlock, slotSize, poolSlots);
			memBlock->stack = new psml_stack_memory((Long)memBlock, slotSize, poolSlots);
			//std::cout << "psml_memory_pool::AllocateNewBlock() memBlock: " << (Long)memBlock << std::endl;
			memBlock->free = poolSlots;
			memBlock->capacity = poolSlots;
			///memBlock->firstNode = firstNode;
			///memBlock->lastNode = lastNode;

			//printf("New Block %p Free %d Capacity %d\n", memBlock, memBlock->free, memBlock->capacity); fflush(stdout);

			/*if(notFullList_firstNode == null)
			{
				printf("AllocateNewBlock 1\n"); fflush(stdout); ///
				notFullList_firstNode = memBlock;
				notFullList_lastNode = memBlock;
				memBlock->prev = null;
				memBlock->next = null;
			}
			else
			{
				printf("AllocateNewBlock 2\n"); fflush(stdout); ///
				// Insert into the beginning of the notFullList.
				memBlock->prev = notFullList_firstNode->prev;
				memBlock->next = notFullList_firstNode;
				if(notFullList_firstNode->prev == null)
					notFullList_firstNode = memBlock;
				else
					notFullList_firstNode->prev->next = memBlock;
				notFullList_firstNode->prev = memBlock;
			}*/

			if(notFullList_firstNode == null){
				//make it the last link
				notFullList_lastNode = memBlock;
			}else {
				//update first prev link
				notFullList_firstNode->prev = memBlock;
			}
			//point it to old first link
			memBlock->next = notFullList_firstNode;
			//point first to new first link
			notFullList_firstNode = memBlock;

			poolSlots *= 2;
			return memBlock;
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
