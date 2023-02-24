/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_sm_lock_h__
#define __Parvicursor_psml_psml_sm_lock_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/Guid/Guid.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>
#include <System.Threading/Thread/Thread.h>
#include <../Parvicursor/Atomic/atomic.h>

#include <System.Threading/Mutex/Mutex.h>

#include <iostream>

using namespace System::Threading;
using namespace System;

using namespace std;

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations
		
		// A fine-grained lock bases on Parvicursor's atomic compare-and-swap
		class psml_sm_lock : public Object
		{
			/*---------------------fields-----------------*/
			private: Mutex *mtx;
			private: Long volatile _lock;
			/*---------------------methods----------------*/
			// psml_sm_lock Class constructor.
			public: psml_sm_lock() : _lock(false)
			{
				mtx = new Mutex();
			}
			//----------------------------------------------------
			// psml_sm_lock Class destructor.
			public: ~psml_sm_lock()
			{
				delete mtx;
			}
			//----------------------------------------------------
			public: inline void lock()
			{
				mtx->Lock();
				//return;
				//__sync_val_compare_and_swap ( type *ptr, type oldval type newval, .
				// __sync_bool_compare_and_swap(lock, 0, 1)
				// These builtins perform an atomic compare and swap. That is, if the current value of *ptr is oldval, then write newval into *ptr.
				//The “bool” version returns true if the comparison is successful and newval was written. The “val” version returns the contents of *ptr before the operation. 
				/*Long volatile accum = false;
				//UInt32 i = 0;
				while(!xParvicursor_AtomicCompareAndSwap(&accum, &_lock, true))
				{
					accum = false;
					//i++;
					//if(i == 4)
					{
						//i = 0;
						sched_yield();
					}
				}*/
				// If the value of accum is equal to dest, the value of dest is changed to newval.
				// If this function fails due to the atomic operation or non-equality between accum and dest,
				// false is returned and accum is filled with dest.
			}
			//----------------------------------------------------
			public: inline void unlock()
			{
				mtx->Unlock();
				//return;
				//Long volatile accum = true;
				//xParvicursor_AtomicCompareAndSwap(&accum, &_lock, false);
				//_lock = false;
			}
			//----------------------------------------------------
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
