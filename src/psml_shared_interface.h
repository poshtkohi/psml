/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_shared_interface_h__
#define __Parvicursor_psml_psml_shared_interface_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>

#include "psml_interface.h"
#include "psml_remote_param_interface.h"

using namespace System;

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations
		class psml_simulator;
		template <class IF> class psml_gport;

		// This interface type is used for shared interfaces.
		// Remote Invocation Method (RMI) techniques are used to implement remote interface function calls.
		// Marshalling methods are employed for distributed shared-memory (DSM) PSML execution in RMI calls.
		// Critical sections are used for shared-memory (multi-core) PSML execution.
		interface class psml_shared_interface : public psml_interface
		{
			friend class psml_simulator;
			template <class IF> friend class psml_gport;
			/*---------------------fields-----------------*/
			private: bool _mustProtect;
			/*---------------------methods----------------*/
			public: psml_shared_interface();
			public: ~psml_shared_interface();
			// This is the method the user must implement for interface calls.
			protected: virtual void Execute(InOut psml_remote_param_interface *param) = 0;
			// This can be called by the user and is implemented by the PSML framework.
			public: void CallExecute(InOut psml_remote_param_interface *param);
			// Waits for ownership of the specified critical section.
			// The function returns when the calling thread is granted ownership.
			// This method is used for shared-memory (multi-core) PSML execution.
			// It is called before calling the method Execute().
			protected: virtual void EnterCriticalSection() = 0;
			// Releases ownership of the specified critical section.
			// This method is used for shared-memory (multi-core) PSML execution.
			// It is called after calling the method Execute().
			protected: virtual void LeaveCriticalSection() = 0;
			// Serializes the param into buffer. If the buffer is enough to be filled, it can be resized.
			// This method is used for distributed shared-memory PSML execution.
			protected: virtual void Serialize(In const psml_remote_param_interface *param, InOut char **buffer, In Int32 size, Out Int32 &newSize) = 0;
			// De-serializes the buffer into local data.
			// This method is used for distributed shared-memory PSML execution.
			protected: virtual void Deserialize(In psml_remote_param_interface *param, In char *buffer, In Int32 size) = 0;
			protected: inline pdsml_interface_type GetInterfaceType() { return __SharedInterface; }
			public: inline bool MustProtect() { return _mustProtect; }
			private: void SetMustProtect(bool val);
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
