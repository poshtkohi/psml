/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_clock.h"
#include "psml_pdes_event.h"
#include "psml_event.h"
#include "psml_pdes_memory_manager.h"
#include "psml_simulator.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		psml_clock::psml_clock(const String &name, const psml_time &period, Double duty_cycle, const psml_time &start_time, bool posedge_first)
				: _m_period(period), _m_duty_cycle(duty_cycle), _m_start_time(start_time), _m_posedge_first(posedge_first)
		{
			set_name(name);

			register_proxy(out, _out);

/*			bool stackful = true;
#if defined __STACKLESS__
			stackful = false;
#endif*/
			p = register_process(process, false, "psml_clock_process");
			//p = register_process(process, true, "psml_clock::process");
			p->_lp->SetIsClockLogicalProcess();
			p->register_port(_out);

//#if defined __STACKLESS__
			/*p->register_port(_self);
			p->register_sensitivity(_self);
			_self.bind(_self_connector, _self_wire);*/
//#endif

			first = true;
		}
		//----------------------------------------------------
		psml_clock::~psml_clock()
		{
			//p->_lp->DeallocateEvent(_e_self);
			//delete _e_self;
			//_posedge_subscribers.clear();
			//_negedge_subscribers.clear();
			//_bothedges_subscribers.clear();
		}
		//----------------------------------------------------
		extern bool __simulation_terminated;
		void psml_clock::process(psml_process *ownerProcess)
		{
			//std::cout << "psml_wait(t, owner)" << std::endl
			psml_clock *my = (psml_clock *)ownerProcess->get_owner_component();
			if(!psml_simulator::IsParallelExecution())
                if(__simulation_terminated)
                    return;

			psml_pdes_event *e;

			if(my->first)
			{
				if(my->_m_start_time > 0)
				{
					psml_pdes_lp *self = ownerProcess->_lp;
					UInt32 _self = self->GetID();
					UInt32 selfClusterID = self->GetOwnerClusterID();

					UInt64 simTime = self->GetSimulationTime();
					UInt64 sendTime = simTime;
					UInt64 recvTime = simTime + my->_m_start_time._m_value;

					//psml_pdes_event *e = new psml_pdes_event(sendTime, recvTime, self, self);
					//e = psml_pdes_memory_manager::malloc(sendTime, recvTime, _self, _self, 0);
					e = psml_pdes_memory_manager::malloc(selfClusterID, selfClusterID, 0);
					e->SetSendTime(sendTime);
					e->SetReceiveTime(recvTime);
					e->SetSenderReference(_self);
					e->SetReceiverReference(_self);
					e->_type = __CLOCK_SELF_EVENT;

					self->SendEvent(e);

					my->first = false;
					return;
				}
				///my->first  = false;
			}

			if(my->_m_clk) // PosEdge
			{
				if(my->_posedge_subscribers.Size() != 0)
				{
					my->_out->put_clock(my->_m_clk, ownerProcess, &my->_posedge_subscribers);
				}
			}
			else // NegEdge
			{
				if(my->_negedge_subscribers.Size() != 0)
				{
					my->_out->put_clock(my->_m_clk, ownerProcess, &my->_negedge_subscribers);
				}
			}

			if(my->_bothedges_subscribers.Size() != 0) // BothEdges
				my->_out->put_clock(my->_m_clk, ownerProcess, &my->_bothedges_subscribers);

			//_out->put(clk, wire_delay, ownerProcess);

			psml_pdes_lp *self = ownerProcess->_lp;
			UInt32 _self = self->GetID();
			UInt32 selfClusterID = self->GetOwnerClusterID();

			UInt64 simTime = self->GetSimulationTime();
			UInt64 sendTime = simTime;
			UInt64 recvTime = simTime + my->_delay._m_value;

			//psml_pdes_event *e = new psml_pdes_event(sendTime, recvTime, self, self);
			//e = psml_pdes_memory_manager::malloc(sendTime, recvTime, _self, _self, 0);
			e = psml_pdes_memory_manager::malloc(selfClusterID, selfClusterID, 0);
			e->SetSendTime(sendTime);
			e->SetReceiveTime(recvTime);
			e->SetSenderReference(_self);
			e->SetReceiverReference(_self);
			e->_type = __CLOCK_SELF_EVENT;
            //e->print();
            //cout << my->_delay._m_value << std::endl;

			self->SendEvent(e);

			//psml_wait(_delay, ownerProcess);
			//std::cout << "@time " << simTime << " in clock clk " << my->_m_clk << std::endl;
			my->_m_clk = !my->_m_clk;
			//std::cout << "counter: " << counter << " clk: " << clk << " delay: " << delay << std::endl;
			my->_m_counter++;

/*			while(true)
			{
                //my->_m_clk = !my->_m_clk;
                //psml_wait(my->_delay, ownerProcess);
				//std::cout << "@time " << psml_time_stamp(ownerProcess) << " psml_clock::process()" << std::endl;
				//std::cout << _posedge_subscribers.size() << " " << clk << std::endl;
				if(my->_m_clk) // PosEdge
				{
					if(my->_posedge_subscribers.Size() != 0)
					{
						my->_out->put_clock(my->_m_clk, my->_wire_delay, ownerProcess, &my->_posedge_subscribers);
					}
				}
				else // NegEdge
				{
					if(my->_negedge_subscribers.Size() != 0)
					{
						my->_out->put_clock(my->_m_clk, my->_wire_delay, ownerProcess, &my->_negedge_subscribers);
					}
				}

				if(my->_bothedges_subscribers.Size() != 0) // BothEdges
					my->_out->put_clock(my->_m_clk, my->_wire_delay, ownerProcess, &my->_bothedges_subscribers);

                //if(my->_m_clk)\
                    my->_out->put(my->_m_clk, ownerProcess);

				psml_wait(my->_delay, ownerProcess);
				my->_m_clk = !my->_m_clk;
				//std::cout << "counter: " << counter << " clk: " << clk << " delay: " << delay << std::endl;
				my->_m_counter++;

				//std::cout << "__STACKFUL__" << std::endl;
			}
*/
//#if defined __STACKLESS__
			/*if(_self->fetch() == 0 && _m_start_time > 0)
			{
				_self->put(1, _m_start_time, ownerProcess);
				return ;
			}

			if(_m_clk) // PosEdge
			{
				if(_posedge_subscribers.Size() != 0)
				{
					_out->put_clock(_m_clk, _wire_delay, ownerProcess, &_posedge_subscribers);
				}
			}
			else // NegEdge
			{
				if(_negedge_subscribers.Size() != 0)
				{
					_out->put_clock(_m_clk, _wire_delay, ownerProcess, &_negedge_subscribers);
				}
			}

			if(_bothedges_subscribers.Size() != 0) // BothEdges
				_out->put_clock(_m_clk, _wire_delay, ownerProcess, &_bothedges_subscribers);

			_self->put(_self->fetch() + 1 , _delay, ownerProcess);
			_m_clk = !_m_clk;
			_m_counter++;*/
			//std::cout << "__STACKLESS__" << std::endl;
			//std::cout << "_m_counter: " << _m_counter << " _m_clk: " << _m_clk << " _delay: " << _delay << std::endl;
/*#else
			if(_m_start_time > 0)
				psml_wait(_m_start_time, ownerProcess);

			while(true)
			{
				//std::cout << "@time " << psml_time_stamp(ownerProcess) << " psml_clock::process()" << std::endl;
				//std::cout << _posedge_subscribers.size() << " " << clk << std::endl;
				if(_m_clk) // PosEdge
				{
					if(_posedge_subscribers.size() != 0)
					{
						_out->put_clock(_m_clk, _wire_delay, ownerProcess, &_posedge_subscribers);
					}
				}
				else // NegEdge
				{
					if(_negedge_subscribers.size() != 0)
					{
						_out->put_clock(_m_clk, _wire_delay, ownerProcess, &_negedge_subscribers);
					}
				}

				if(_bothedges_subscribers.size() != 0) // BothEdges
					_out->put_clock(_m_clk, _wire_delay, ownerProcess, &_bothedges_subscribers);

				//_out->put(clk, wire_delay, ownerProcess);

				psml_wait(_delay, ownerProcess);
				_m_clk = !_m_clk;
				//std::cout << "counter: " << counter << " clk: " << clk << " delay: " << delay << std::endl;
				_m_counter++;

				//std::cout << "__STACKFUL__" << std::endl;
			}
#endif*/
		}
		//----------------------------------------------------
		/*void psml_clock::Wrapper_To_Call_process(psml_component *owner, psml_process *ownerProcess)
		{
			// explicitly cast to a pointer to psml_clock
			psml_clock *mySelf = (psml_clock *)owner;
			// call member
			mySelf->process(ownerProcess);
		}*/
		//----------------------------------------------------
		void psml_clock::before_end_of_elaboration()
		{
			psml_connector *connector = _out.GetBoundConnector();
			//std::cout << connector->_inports.size() << std::endl;
			//for(std::vector<psml_port *>::iterator it = connector->_inports.begin() ; it != connector->_inports.end() ; ++it)
			for(register UInt32 it = 0 ; it < connector->_inports->Size() ; ++it)
			//for(register UInt32 it = connector->_inports->Size() - 1 ; it != -1  ; --it)
			{
				psml_port *inport = (*connector->_inports)[it];//*it;
				//psml_process *destination = inport->GetOwnerProcess();
				psml_edge_trigger_type type = inport->GetEdgeTriggerType();
				//std::cout << "inport " <<  inport << " ownerComponent " << inport->GetOwnerComponent()->GetName().get_BaseStream() << " ownerProcess " << inport->GetOwnerProcess()->GetName().get_BaseStream() << std::endl;
				if(type == PSML_POS_EDGE)
					_posedge_subscribers.PushBack(inport);
				else if(type == PSML_NEG_EDGE)
					_negedge_subscribers.PushBack(inport);
				else if(type == PSML_BOTH_EDGES)
					_bothedges_subscribers.PushBack(inport);
				else
					throw InvalidOperationException("Clock was bound to the psml_inport " + String(psml_simulator::to_string(inport).c_str()) + " with an unknown clock edge type");

				//std::cout << "dest: " << inport->GetOwnerProcess()->GetName().get_BaseStream() << std::endl;
			}

			Initialize();
		}
		//----------------------------------------------------
		void psml_clock::Initialize()
		{
			if(_m_period._m_value == 0)
				throw InvalidOperationException("Increase the clock period");

			if( _m_duty_cycle <= 0.0 || _m_duty_cycle >= 1.0)
				_m_duty_cycle = 0.5;


			// To create delay and wire_delay don't work in the class constructor
			// because psml_simulator is often instantiated after component instantiation.
			// We must think of another solution due to unnecessary user-level context switch overheads.
			//_delay = psml_time((_m_period * _m_duty_cycle)._m_value, psml_time::global_time_unit);
			//psml_time period = _m_period * _m_duty_cycle;
			//std::cout << "_m_period " << _m_period << std::endl;
			//std::cout << "psml_time::global_time_resolution " << psml_time::global_time_resolution << std::endl;

			_delay = psml_time(_m_period * _m_duty_cycle);
			_wire_delay = psml_time(0, psml_time::global_time_unit);
			p->set_delay(_wire_delay);

			//std::cout << "_delay " << _delay << std::endl;

			//exit(0);
			_m_counter = 0;

			if(_m_posedge_first)
				_m_clk = true;
			else
				_m_clk = false;
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
