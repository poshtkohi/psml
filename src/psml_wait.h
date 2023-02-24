/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_wait_h__
#define __Parvicursor_psml_psml_wait_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System.Threading/Fiber/Fiber.h>

#include "psml_process.h"
#include "psml_time.h"
#include "psml_event.h"
#include "psml_wait_types.h"

#include <iostream>
#include <vector>
#include <map>

using namespace System;
using namespace System::Threading;

using namespace std;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations.

		class psml_wait_internal : public Object
		{
			/*---------------------fields-----------------*/
			/*---------------------methods----------------*/
			// Dynamic sensitivity for processes
			// Waits for the sensitive list event to occur.
			public: static void wait(psml_process *issuer);
			// Dynamic sensitivity for processes
			// Waits for the event mentioned as parameter to occur.
			public: static void wait(psml_event *e, psml_process *issuer);
			public: static void wait(psml_event &e, psml_process *issuer);
			//public: static void wait(const psml_event_or_list &el);
			//public: static void wait(const psml_event_and_list &el);
			// Waits for specified time.
			public: static void wait(const psml_time &t, psml_process *issuer);
			// Waits for n events to occur, events are the one in sensitive list.
			// It shall be an error to pass an argument value less than or equal to zero.
			public: static void wait(UInt32 n, psml_process *issuer);
			private: inline static void suspend_me(psml_process *me);
			// The following method is called to broadcast null messages to the neighboring clusters
			// in order to avoid unneccessary deadlocks. When we call wait(exp), we have an exact
			// knowledge when this process will be resumed; therefore, we can inform destionatopns LPs
			// of this time.
			private: static void broadcast_null_messages_for_ddr_execution(UInt32 n, psml_process *issuer);
			//public: static void wait(double v, sc_time_unit tu);
			//public: static void wait(const psml_time &t, const psml_event &e);
			//public: static void wait(double v, sc_time_unit tu, const psml_event &e e)
			//public: static void wait(const psml_time &t, const psml_event_or_list &el);
			//public: static void wait(double v, sc_time_unit tu, const psml_event_or_list &el);
			//public: static void wait(const psml_time &t const psml_event_and_list &el);
			//public: static void wait(double v, sc_time_unit tu, const psml_event_and_list &el);

			// Dynamic sensitivity for processes
			//public: static void next_trigger(const psml_process *issuer);
			// Dynamic sensitivity for processes
			//public: static void next_trigger(const psml_event *e, const psml_process *issuer);
			//public: inline static void next_trigger(const psml_event &e, const psml_process *issuer);
			//public: static void next_trigger(const psml_event_or_list &el);
			//public: static void next_trigger(const psml_event_and_list &el);
			//public: static void next_trigger(const psml_time &t, const psml_process *issuer);
			//public: static void next_trigger(double v, sc_time_unit tu);
			//public: static void next_trigger(const psml_time &t, const psml_event &e);
			//public: static void next_trigger(double v, sc_time_unit tu, const psml_event &e e)
			//public: static void next_trigger(const psml_time &t, const psml_event_or_list &el);
			//public: static void next_trigger(double v, sc_time_unit tu, const psml_event_or_list &el);
			//public: static void next_trigger(const psml_time &t const psml_event_and_list &el);
			//public: static void next_trigger(double v, sc_time_unit tu, const psml_event_and_list &el);

			/*--------------------------------------------*/

		};
		//----------------------------------------------------
		inline static void psml_wait(psml_process *issuer) { psml_wait_internal::wait(issuer); }
		inline static void psml_wait(psml_event *e, psml_process *issuer) { psml_wait_internal::wait(e, issuer); }
		inline static void psml_wait(psml_event &e, psml_process *issuer) { psml_wait_internal::wait(e, issuer); }
		inline static void psml_wait(const psml_time &t, psml_process *issuer) { psml_wait_internal::wait(t, issuer); }
		inline static void psml_wait(UInt32 n, psml_process *issuer) { psml_wait_internal::wait(n, issuer); }
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif

