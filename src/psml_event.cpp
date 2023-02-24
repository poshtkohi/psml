/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_event.h"
#include "psml_pdes_lp.h"
#include "psml_pdes_event.h"
#include "psml_pdes_memory_manager.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		std::vector<psml_event *> *psml_event::__static_events = null;
		LockFreeCounter psml_event::__event_counter = LockFreeCounter(0);
        //----------------------------------------------------
		psml_event::psml_event(const String &name, bool isStatic) : _isStatic(isStatic), _has_added_to_static_events(false)
        {
			if(name == "")
				_objName = Guid().NewGuid();
			else
				_objName = name;

            if(__static_events == null)
                __static_events = new std::vector<psml_event *>;

			_eventID = (UInt64)__event_counter.get_Count_And_Increment();
        }
        //----------------------------------------------------
        psml_event::~psml_event()
        {
			//std::cout << "psml_event::~psml_event() _initiators.size(): " << _initiators.size() << " _subscribers.size(): " << _subscribers.size() << std::endl;
        }
		//----------------------------------------------------
		void psml_event::register_initiator(psml_process *p)
		{
			if(p == null)
				throw ArgumentNullException("p", "p is null");

			for(std::vector<psml_process *>::iterator it = _initiators.begin() ; it != _initiators.end() ; ++it)
			{
				psml_process *_p = *it;
				if(p == _p)
					throw InvalidOperationException("The process p has already been registered as an initiator");
			}

			if(_isStatic && !_has_added_to_static_events)
			{
				psml_event::__static_events->push_back(this);
				_has_added_to_static_events = true;
			}

			_initiators.push_back(p);
		}
		//----------------------------------------------------
		void psml_event::register_initiator(psml_process &p)
		{
			register_initiator(&p);
		}
		//----------------------------------------------------
		void psml_event::register_subscriber(psml_process *p)
		{
			if(p == null)
				throw ArgumentNullException("p", "p is null");

			for(std::vector<psml_process *>::iterator it = _subscribers.begin() ; it != _subscribers.end() ; ++it)
			{
				psml_process *_p = *it;
				if(p == _p)
					throw InvalidOperationException("The process p has already been registered as a subscriber");
			}

			if(_isStatic && !_has_added_to_static_events)
			{
				psml_event::__static_events->push_back(this);
				_has_added_to_static_events = true;
			}

			//cout << "this " << this << " psml_event::register_subscriber() " << p->_lp->GetID() << std::endl;

			_subscribers.push_back(p);
		}
		//----------------------------------------------------
		void psml_event::register_subscriber(psml_process &p)
		{
			register_subscriber(&p);
		}
		//----------------------------------------------------
		void psml_event::notify(psml_process *issuer)
		{
			notify_internal(0, issuer);
		}
		//----------------------------------------------------
		void psml_event::notify(const psml_time &delay, psml_process *issuer)
		{
			notify_internal(delay._m_value, issuer);
		}
		//----------------------------------------------------
		void psml_event::notify(double delay, psml_time_unit tu, psml_process *issuer)
		{
			psml_time t(delay, tu);
			notify(t, issuer);
		}
		//----------------------------------------------------
		void psml_event::notify_internal(UInt64 delay, psml_process *issuer)
		{
			if(delay < 0)
				throw ArgumentOutOfRangeException("delay", "delay can not be negative");

			if(issuer == null)
				throw ArgumentNullException("issuer", "issuer is null");

			if(_initiators.size() == 0)
				throw InvalidOperationException("The event has not been registered with at least one initiator");

			if(_subscribers.size() == 0)
				throw InvalidOperationException("The event has not been registered with at least one subscriber");

			psml_pdes_lp *sender = issuer->_lp;
			UInt32 _sender = sender->GetID();
			UInt32 senderClusterID = sender->GetOwnerClusterID();

			UInt64 simTime = sender->GetSimulationTime();
			UInt64 sendTime = simTime;
			UInt64 recvTime = simTime + delay;

			//cout << "this " << this << " psml_event::notify_internal() _subscribers.size() " << _subscribers.size() << " _initiators.size() " << _initiators.size() << std::endl;

            // Change this to Vector.
			for(std::vector<psml_process *>::iterator it = _subscribers.begin() ; it != _subscribers.end() ; ++it)
			{
				psml_pdes_lp *receiver = (*it)->_lp;
				UInt32 _receiver = receiver->GetID();
				//cout << "this " << this << " psml_event::notify_internal() receiver " << receiver->GetID() << " delay " << delay << std::endl;
				//psml_pdes_event *e = new psml_pdes_event(sendTime, recvTime, sender, receiver);
				//psml_pdes_event *e = psml_pdes_memory_manager::malloc(sendTime, recvTime, _sender, _receiver, sizeof(UInt64));
				psml_pdes_event *e = psml_pdes_memory_manager::malloc(senderClusterID, receiver->GetOwnerClusterID(), sizeof(UInt64));
				e->SetSendTime(sendTime);
				e->SetReceiveTime(recvTime);
				e->SetSenderReference(_sender);
				e->SetReceiverReference(_receiver);

				e->_type = __PSML_EVENT;
				//e->_eventID = _eventID;
				UInt64 *__eventID = (UInt64 *)((unsigned char *)e + sizeof(class psml_pdes_event));
				*__eventID = _eventID;
				sender->SendEvent(e);
			}
		}
		//----------------------------------------------------
		const String &psml_event::GetName()
		{
			return _objName;
		}
        //----------------------------------------------------
	}
};
//**************************************************************************************************************//
