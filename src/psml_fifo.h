/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_fifo_h__
#define __Parvicursor_psml_psml_fifo_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/Guid/Guid.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include <Parvicursor/xSim2/RingBuffer.h>

#include "psml_shared_interface.h"
#include "psml_port.h"
#include "psml_time.h"
#include "psml_event.h"
#include "psml_sm_lock.h"
//#include "psml_simulator.h"
#include "psml_process.h"

#include <iostream>

using namespace System;


using namespace std;

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations
		class psml_connector;
		class psml_process;
		//class psml_time;
		class psml_proxy_interface;
		template <class IF> class psml_proxy;

		// An unbounded FIFO queue.
		template <class T>
		class psml_fifo : public psml_shared_interface
		{
			friend class psml_connector;
			friend class psml_proxy_interface;
			template <class IF> friend class psml_proxy;
			/*---------------------fields-----------------*/
			//private: T _buff[__size];
		    private: RingBuffer<T> rb;
			//private: psml_connector *_owner;
			private: psml_sm_lock _lock;
			private: psml_event _e;
			/*---------------------methods----------------*/
			// psml_fifo Class constructor.
			public: psml_fifo()/* : _owner(null)*/
			{

			}
			//----------------------------------------------------
			// psml_fifo Class destructor.
			public: ~psml_fifo()
			{
				//cout << "~psml_fifo _interface: " << this << " connector: " << GetOwnerConnector() << std::endl;
			}
			//----------------------------------------------------
			private: void Execute(InOut psml_remote_param_interface *param)
			{

			}
			//----------------------------------------------------
			public: void CallExecute(InOut psml_remote_param_interface *param)
			{

			}
			//----------------------------------------------------
			private: inline void EnterCriticalSection()
			{
				_lock.lock();
			}
			//----------------------------------------------------
			private: inline void LeaveCriticalSection()
			{
				_lock.unlock();
			}
			//----------------------------------------------------
			private: void Serialize(In const psml_remote_param_interface *param, InOut char **buffer, In Int32 size, Out UInt32 &newSize)
			{
			}
			//----------------------------------------------------
			private: void Deserialize(In psml_remote_param_interface *param, In char *buffer, In UInt32 size)
			{
			}
			//----------------------------------------------------
			/*public: inline psml_connector *GetOwnerConnector()
			{
				return _owner;
			}
			//----------------------------------------------------
			public: inline void RegisterOwnerConnector(psml_connector *conn)
			{
				_owner = conn;
			}*/
			//----------------------------------------------------
			private: void before_static_event_elaboration()
			{
				//std::cout << "psml_fifo::before_static_event_elaboration()" <<endl;
			}
			//----------------------------------------------------
			// non-blocking write
			public: void nb_write(const T & val, const psml_time &delay, psml_process *issuer)
			{
				// Modify here only to lock on an inter-cluster interface.
				// This is can be determined based on the connector bound to this interface.
				if(MustProtect())
					_lock.lock();

				rb.Enqueue(val);

				if(MustProtect())
					_lock.unlock();

				_e.notify(delay, issuer);
				//std::cout << "psml_fifo::write() val " << val << std::endl;
			}
			//----------------------------------------------------
			// blocking read
			public: void read(T &val, psml_process *issuer)
			{
				if(MustProtect())
					_lock.lock();

				val = rb.Dequeue();

				if(MustProtect())
					_lock.unlock();

				psml_wait(_e, issuer);
				// lock.unlock
				//std::cout << "psml_fifo::read() val " << val << std::endl;
			}
			//----------------------------------------------------
			// non-blocking read
			public: void nb_read(T &val)
			{
				if(MustProtect())
					_lock.lock();

				val = rb.Dequeue();

				if(MustProtect())
					_lock.unlock();
			}
			//----------------------------------------------------
			public: inline psml_event *get_event()
			{
				return &_e;
			}
			//----------------------------------------------------
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
