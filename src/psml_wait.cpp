/**
	#define meta ...
	printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/


#include "psml_wait.h"
#include "psml_simulator.h"
#include "psml_pdes_event.h"
#include "psml_pdes_memory_manager.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		void psml_wait_internal::wait(psml_process *issuer)
		{
			if(issuer == null)
				throw ArgumentNullException("issuer", "issuer is null");

			if(issuer->_process_type == __STACKLESS_PROCESS__)
				throw InvalidOperationException("psml_wait() can only be called within a stackful or special process methods");

			//std::cout << "psml_wait()" << std::endl;
			issuer->_wait_type = __WAIT;
			issuer->_has_wait_registered = true;
            //issuer->_n_registered = 0;
			//issuer->_n_counter = 0;

            if(psml_simulator::GetSimulationManagerType() == DeadlockDetection)
                broadcast_null_messages_for_ddr_execution(1, issuer);
			//std::cout << "psml_wait(t, owner)" << std::endl;

			//suspend_me(issuer);
			if(issuer->_process_type == __STACKFULL_PROCESS__)
                Fiber::SwitchToFiber(issuer->_fib_process, psml_process::__fiber_clusters[issuer->_lp->GetOwnerClusterID()]);
            else
                return;
		}
        //----------------------------------------------------
		void psml_wait_internal::wait(UInt32 n, psml_process *issuer)
		{
            //std::cout << "wait(n)" << std::endl;
            if(n == 0)
                throw ArgumentOutOfRangeException("n", "index is equal to zero");
				//throw ArgumentOutOfRangeException("n", "index is equal to or less than zero");

			if(issuer == null)
				throw ArgumentNullException("issuer", "issuer is null");

			if(issuer->_process_type == __STACKLESS_PROCESS__)
				throw InvalidOperationException("psml_wait() can only be called within a stackful or special process methods");

			//std::cout << "psml_wait()" << std::endl;
			issuer->_wait_type = __WAIT_N;
			issuer->_has_wait_registered = true;
			issuer->_n_registered = n;
			issuer->_n_counter = 0;

            if(psml_simulator::GetSimulationManagerType() == DeadlockDetection)
                broadcast_null_messages_for_ddr_execution(n, issuer);

			//suspend_me(issuer);
			if(issuer->_process_type == __STACKFULL_PROCESS__)
                Fiber::SwitchToFiber(issuer->_fib_process, psml_process::__fiber_clusters[issuer->_lp->GetOwnerClusterID()]);
            else
                return;
		}
		//----------------------------------------------------
		void psml_wait_internal::wait(psml_event *e, psml_process *issuer)
		{
			if(issuer == null)
				throw ArgumentNullException("issuer", "issuer is null");

			if(e == null)
				throw ArgumentNullException("e", "e is null");

			if(issuer->_process_type == __STACKLESS_PROCESS__)
				throw InvalidOperationException("psml_wait() can only be called within a stackful or special process methods");

			//std::cout << "psml_wait(e, issuer)" << std::endl;
			issuer->_wait_e = e;
			issuer->_wait_type = __WAIT_E;
			issuer->_has_wait_registered = true;

			//suspend_me(issuer);
			if(issuer->_process_type == __STACKFULL_PROCESS__)
                Fiber::SwitchToFiber(issuer->_fib_process, psml_process::__fiber_clusters[issuer->_lp->GetOwnerClusterID()]);
            else
                return;
		}
		//----------------------------------------------------
		void psml_wait_internal::wait(psml_event &e, psml_process *issuer)
		{
			psml_wait_internal::wait(&e, issuer);
		}
		//----------------------------------------------------
		void psml_wait_internal::wait(const psml_time &t, psml_process *issuer)
		{
			if(issuer == null)
				throw ArgumentNullException("issuer", "issuer is null");

			if(issuer->_process_type == __STACKLESS_PROCESS__)
				throw InvalidOperationException("psml_wait() can only be called within a stackful or special process methods");

			//std::cout << "psml_wait(t, owner)" << std::endl;
			psml_pdes_lp *self = issuer->_lp;
			UInt32 _self = self->GetID();
			UInt32 selfClusterID = self->GetOwnerClusterID();

			UInt64 simTime = self->GetSimulationTime();
			UInt64 sendTime = simTime;
			UInt64 recvTime = simTime + t._m_value;

			//cout << "sendTime " << sendTime << " recvTime " << recvTime  << std::endl;

			// Optimize here based on static events.
			//psml_pdes_event *e = psml_pdes_memory_manager::malloc(sendTime, recvTime, _self, _self, 0);
			psml_pdes_event *e = psml_pdes_memory_manager::malloc(selfClusterID, selfClusterID, 0);
			e->SetSendTime(sendTime);
			e->SetReceiveTime(recvTime);
			e->SetSenderReference(_self);
			e->SetReceiverReference(_self);
			e->_type = __WAIT_SELF_EVENT;

			self->SendEvent(e);

			issuer->_wait_type = __WAIT_T;
			issuer->_has_wait_registered = true;

            //if(psml_simulator::GetSimulationManagerType() == DeadlockDetection)
            //    broadcast_null_messages_for_ddr_execution(1, issuer);

			//suspend_me(issuer);
			if(issuer->_process_type == __STACKFULL_PROCESS__)
                Fiber::SwitchToFiber(issuer->_fib_process, psml_process::__fiber_clusters[self->GetOwnerClusterID()]);
            else
                return;
		}
		//----------------------------------------------------
		void psml_wait_internal::broadcast_null_messages_for_ddr_execution(UInt32 n, psml_process *issuer)
		{
            // The following must only be activated in DDR simuluation, neither sequential nor
            // Null-Message CMB nor optimistic.
            return;

			psml_pdes_lp *target = issuer->_lp;
			UInt32 _target = target->GetID();
			//if(_target < 4) return;

            UInt32 targetClusterID = target->GetOwnerClusterID();

            const UInt32 lookahead = 2;
            UInt64 simTime = target->GetSimulationTime();
            UInt64 sendTime = simTime;
            UInt64 recvTime = simTime + n * lookahead;

            //cout << "sendTime " << sendTime << " recvTime " << recvTime  << std::endl;


            //Int32 i = 0;
            for(register Int32 i = 0 ; i < target->GetOutputLpCount() ; i++)
            {
                LogicalProcess *dest = LogicalProcess::GetLogicalProcessById(target->GetOutputLpId(i));
                UInt32 destClusterID = dest->GetOwnerClusterID();
                if(targetClusterID != destClusterID)
                {
                    psml_pdes_event *e = psml_pdes_memory_manager::malloc(targetClusterID, destClusterID, 0);
                    e->SetSendTime(sendTime);
                    e->SetReceiveTime(recvTime);
                    e->SetSenderReference(_target);
                    e->SetReceiverReference(dest->GetID());
                    e->_type = __PSML_NULL_MESSAGE;

                    target->SendEvent(e);
                    break;
                }
            }
		}
		//----------------------------------------------------
		/*Int32 counter = 0;
		void psml_wait_internal::suspend_me(psml_process *me)
		{
			Fiber::SwitchToFiber(me->_fib_process, psml_process::__fiber_clusters[me->_lp->GetOwnerClusterID()]);
			return;

			if(!psml_simulator::IsParallelExecution()) // Sequential Execution
			{
				psml_pdes_event *nextEvent = (psml_pdes_event *)me->_lp->PeekNextEvent();

				if(nextEvent == null)
				{
					counter = 0;
					Fiber::SwitchToFiber(me->_fib_process, psml_process::__fiber_clusters[me->_lp->GetOwnerClusterID()]);
				}
				else if(me->_lp->GetSimulationTime() == nextEvent->GetReceiveTime())
				{
					psml_pdes_lp *receiver =(psml_pdes_lp *) me->_lp->GetLogicalProcessById(nextEvent->GetReceiverReference());

					if(receiver->_p == me)
					{
						Fiber::SwitchToFiber(me->_fib_process, psml_process::__fiber_clusters[me->_lp->GetOwnerClusterID()]);
					}
					else if(receiver->_p->_stackful)
					{
						receiver->SetSimulationTime(nextEvent->GetReceiveTime());
						receiver->ExecuteProcess();
						if(nextEvent->deletable)
							receiver->DeallocateEvent(nextEvent);
					}
					else
					{
						Fiber::SwitchToFiber(me->_fib_process, psml_process::__fiber_clusters[me->_lp->GetOwnerClusterID()]);
					}
				}
				else
				{
					Fiber::SwitchToFiber(me->_fib_process, psml_process::__fiber_clusters[me->_lp->GetOwnerClusterID()]);
				}
			}
			else
				Fiber::SwitchToFiber(me->_fib_process, psml_process::__fiber_clusters[me->_lp->GetOwnerClusterID()]);
		}*/
		//----------------------------------------------------
	}
};
//**************************************************************************************************************//

/**

inline void sc_thread_process::suspend_me()
{
	// remember, if we're currently unwinding

	bool unwinding_preempted = m_unwinding;

	sc_simcontext* simc_p = simcontext();
	sc_cor*         cor_p = simc_p->next_cor();

	// do not switch, if we're about to execute next (e.g. suicide)

	if( m_cor_p != cor_p )
	{
		IncrementActivatedProcesss();
		DEBUG_MSG( DEBUG_NAME , this, "suspending thread");
		simc_p->cor_pkg()->yield( cor_p );
		DEBUG_MSG( DEBUG_NAME , this, "resuming thread");
	}
}

**/
