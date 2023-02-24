/**
	#define meta ...
	printf("%s\n", meta);
**/

/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2016.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_pool_h__
#define __Parvicursor_psml_psml_pool_h__


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
		class psml_single_memory_pool
		{
			// Basic type define
			typedef int uint;
			typedef char uchar;
			/*---------------------fields----------------*/
			uint m_numOfBlocks; // Num of blocks
			uint m_sizeOfEachBlock; // Size of each block
			uint m_numFreeBlocks; // Num of remaining blocks
			uint m_numInitialized; // Num of initialized blocks
			uchar *m_memStart; // Beginning of memory pool
			uchar *m_next; // Num of next free block
			/*---------------------methods----------------*/
			public: psml_single_memory_pool(Long blockId, Int32 slotSize, Int32 numOfSlots)
			{
				m_numOfBlocks = 0;
				m_sizeOfEachBlock = 0;
				m_numFreeBlocks = 0;
				m_numInitialized = 0;
				m_memStart = NULL;
				m_next = 0;

				CreatePool(blockId, slotSize, numOfSlots);
			}
			//----------------------------------------------------
			public: ~psml_single_memory_pool()
			{
				DestroyPool();
			}
			//----------------------------------------------------
			private: void CreatePool(Long blockId, size_t sizeOfEachBlock, uint numOfBlocks)
			{
				m_numOfBlocks = numOfBlocks;
				m_sizeOfEachBlock = sizeOfEachBlock + sizeof(blockId);
				m_memStart = new uchar[ m_sizeOfEachBlock * m_numOfBlocks];
				m_numFreeBlocks = numOfBlocks;
				m_next = m_memStart;

				Long _pointer = -m_sizeOfEachBlock;

				for(register Int32 i = 0 ; i < numOfBlocks ; i++)
				{
					_pointer += m_sizeOfEachBlock;
					Long *slotBlockId = (Long *)(m_memStart + _pointer + sizeOfEachBlock);
					*slotBlockId = blockId;
					//std::cout << *slotBlockId << std::endl;
				}
			}
			//----------------------------------------------------
			private: void DestroyPool()
			{
				delete[] m_memStart;
				m_memStart = null;
			}
			//----------------------------------------------------
			private: inline uchar* AddrFromIndex(uint i) const
			{
				return m_memStart + ( i * m_sizeOfEachBlock );
			}
			//----------------------------------------------------
			private: inline uint IndexFromAddr(const uchar* p) const
			{
				return (((uint)(p - m_memStart)) / m_sizeOfEachBlock);
			}
			//----------------------------------------------------
			public: inline void *Allocate()
			{
				if (m_numInitialized < m_numOfBlocks )
				{
					uint* p = (uint*)AddrFromIndex( m_numInitialized );
					*p = m_numInitialized + 1;
					m_numInitialized++;
				}
				void *ret = null;
				if ( m_numFreeBlocks > 0 )
				{
					ret = (void*)m_next;
					--m_numFreeBlocks;
					if (m_numFreeBlocks != 0)
					{
						m_next = AddrFromIndex( *((uint*)m_next) );
					}
					else
					{
						m_next = null;
					}
				}
				return ret;
			}
			//----------------------------------------------------
			public: inline void DeAllocate(void *p)
			{
				if (m_next != null)
				{
					(*(uint*)p) = IndexFromAddr( m_next );
					m_next = (uchar*)p;
				}
				else
				{
					*((uint*)p) = m_numOfBlocks;
					m_next = (uchar*)p;
				}
					++m_numFreeBlocks;
			}
			//----------------------------------------------------
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
