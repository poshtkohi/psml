/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_nonshared_interface_h__
#define __Parvicursor_psml_psml_nonshared_interface_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>
#include <System.Threading/Mutex/Mutex.h>

#include <Parvicursor/xSim2/Vector.h>

#include "psml_globals.h"
#include "psml_interface.h"

using namespace System;
using namespace System::Threading;
using namespace Parvicursor::xSim2::psc;

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations
		class psml_port;
		class psml_time;
		class psml_pdes_lp;
		class psml_process;
		class psml_simulator;
		class psml_pdes_event;
		class psml_clock;
		class psml_connector;
		template <class IF> class psml_inport;
		template <class IF> class psml_outport;

		// This interface type is used as a container interface to pass non-shared data between logical processes.
		// Marshalling methods are employed for distributed shared-memory (DSM) PSML execution.
		interface class psml_nonshared_interface : public psml_interface
		{
			friend class psml_port;
			friend class psml_pdes_lp;
			friend class psml_simulator;
			friend class psml_clock;
			friend class psml_process;
			friend class psml_connector;
			template <class IF> friend class psml_inport;
			template <class IF> friend class psml_outport;
			/*---------------------fields-----------------*/
			private: psml_process *_issuer;
			private: bool _isLevelHigh;
			private: bool _offloaded;
			//private: bool _commited;
			private: UInt16 _numOfSensitivePorts;
#if defined __cycles_enabled__
			private: static Vector<Vector<psml_nonshared_interface *>* > *__uncommitted_insensitive_nonshared_interfaces;
#endif
			/*---------------------methods----------------*/
			protected: psml_nonshared_interface();
			// Creates an actual copy of the psml_nonshared_interface instance.
			protected: virtual psml_nonshared_interface *CreateInstance() = 0;
			// Destroys an instance created by the CreateInstance() method.
			protected: virtual void DestroyInstance(psml_nonshared_interface *instance) = 0;
			// Indicates that if this instance has been created by calling the CreateInstance() method or not.
			protected: inline virtual bool GetIsInstance() = 0;
            // Set this channel as an instance created by CreateInstance() or not.
			protected: inline virtual void SetIsInstance(bool val) = 0;
//#if defined __zero_copy_enabled_for_psml_nonshared_interfaces__
			// If the channel is in zero-copy communication due to an intra-cluster communication, the writer
			// MUST not commit the value after updating it. Instead it should store the value in an uncommited
			// memory region. The following method allows the PSML runtime to use the uncommited field to
			// to update the interface properly. For multiple successive delayed writes within a process,
			// we can use a FIFO queue to guaranteer linearizability. Linearizability is a guarantee about
			// single operations on single objects. It provides a real-time (i.e., wall-clock) guarantee
			// on the behavior of a set of single operations (often reads and writes) on a single object
			// (e.g., distributed register or data item).
			protected: virtual void Commit() = 0;
			protected: virtual void Commit1() = 0;
//#endif
			// Clones a copy of the local data inside the container interface instance.
			// This method is used for shared-memory (multi-core) PSML execution.
		    // This method must return the reference to the value for intra-cluster communications.
			protected: virtual void *CloneValue(Out UInt16 &size) = 0;
			// Clones a copy of the local data inside the container interface instance.
			// This method is used for shared-memory (multi-core) PSML execution.
			// This method must return the reference to the value for intra-cluster communications.
			protected: inline virtual UInt16 ValueSize() = 0;
			protected: inline virtual void CloneValueToPointer(Out void *p) = 0;
			// Deletes a previously cloned value.
			// This method is used for shared-memory (multi-core) PSML execution.
			// This method must not delete the value for intra-cluster communications.
			protected: virtual void DeleteValue(In void *buffer) = 0;
			// Copies the buffer into local data inside the container interface instance.
			// If the local data is not updated, the value of false will be returned.
			// This method is used for shared-memory (multi-core) PSML execution.
			protected: inline virtual void CopyValue(In void *buffer) = 0;
			// Returns the reference to the value of the interface. This is used for
			// intra-cluster communications for zero-copy purposes.
			//protected: virtual void *GetValueReference() = 0;
			// Serializes the local data into buffer. If the buffer is enough to be filled, it can be resized.
			// This method is used for distributed shared-memory PSML execution.
			protected: virtual void Serialize(InOut char **buffer, In Int32 size, Out Int32 &newSize) = 0;
			// De-serializes the buffer into local data.
			// This method is used for distributed shared-memory PSML execution.
			protected: virtual void Deserialize(In char *buffer, In Int32 size) = 0;
			protected: inline pdsml_interface_type GetInterfaceType() { return __NonSharedInterface; }
		    // Flushes the value associated with this interface instance to the PDES layer.
			// ADD THIS SUPPORT: For the same cluster (intra-cluster communications) the value of psml_nonshared_interface instances
			// must not be cloned, rather the reference to them must be passed.
			//protected: void Flush(psml_process *issuer);
			protected: void Offload(/*const psml_time &delay, */psml_process *issuer);
			protected: static void CommitInsenistiveInterfaces(UInt32 ClusterID);
			// Flush clock wires.
			protected: void FlushClock(psml_process *issuer, Vector<psml_port *> *subscriber_inports);
			//protected: virtual bool HasMutexProtection() = 0;
			//protected: virtual void SetHasMutexProtection(bool val) = 0;
			//protected: virtual void SetMutex(Mutex *mmtx) = 0;
			protected: virtual void Print() = 0;
			protected: inline bool GetIsLevelHigh() { return _isLevelHigh; }
			protected: inline void SetIsLevelHigh(bool val) { _isLevelHigh = val; }
			//protected: virtual bool GetIsLevelHigh() = 0;
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
