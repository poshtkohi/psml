/**
	#define meta ...
	printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_pdes_lp.h"
#include "psml_process.h"
#include "psml_pdes_event.h"
#include "psml_wait.h"
#include "psml_component.h"
#include "psml_connector.h"
#include "psml_interface.h"
#include "psml_nonshared_interface.h"
#include "psml_pdes_memory_manager.h"
#include "psml_wait_types.h"
#include "psml_port.h"
#include "psml_simulator.h"
#include "psml_pdes_heap.h"

Int64 numberOfActivatedProcesss = 0;
//UInt32 i_global = 0;
//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		psml_pdes_lp::psml_pdes_lp(psml_process *p)
		{
			if(p == null)
				throw ArgumentNullException("p", "p is null");

			_p = p;
			_first_execution = true;
		}
		//----------------------------------------------------
		psml_pdes_lp::~psml_pdes_lp()
		{
		}
		//----------------------------------------------------
		void psml_pdes_lp::Elaborate()
		{
			UInt32 clusterID = GetOwnerClusterID();

			//cout << "psml_pdes_lp::Elaborate() clusterID: " << clusterID << " __fiber_clusters.size(): " << psml_process::__fiber_clusters.size() << std::endl;

			if(psml_process::__fiber_clusters[clusterID] == null)
				psml_process::__fiber_clusters[clusterID] = Fiber::ConvertThreadToFiber();

			if((*(psml_pdes_memory_manager::__localEventHeaps__))[clusterID] == null)
				(*(psml_pdes_memory_manager::__localEventHeaps__))[clusterID] = new psml_pdes_heap(true);

			//if(psml_simulator::IsParallelExecution())\
				if((*(psml_pdes_memory_manager::__globalEventHeaps__))[clusterID] == null)\
					(*(psml_pdes_memory_manager::__globalEventHeaps__))[clusterID] = new psml_pdes_heap(false);

			/**------------------ Elaborating the psml_nonshared_interface Instances -----------------**/
			// Elaborates non-shared interfaces for intra and inter cluster communications.
			/*for(std::map<psml_port *, psml_port *>::iterator it1 = _p->_inports.begin() ; it1 != _p->_inports.end() ; ++it1)
			{
			psml_port *inport_dest = it1->second;
			psml_process *destination = inport_dest->GetOwnerProcess();
			psml_connector *connector = inport_dest->GetBoundConnector();
			std::cout << "inport_dest name: " << inport_dest->GetName().get_BaseStream() << " owner component: " << inport_dest->GetOwnerComponent()->GetName().get_BaseStream() << std::endl;
			// Output ports with respect to the connector are target logical processes.
			// In PMSL language, a psml_outport is only connected to a connector. A single connector may be
			// connected to several psml_port instances.
			for(std::vector<psml_port *>::iterator it2 = connector->_outports.begin() ; it2 != connector->_outports.end() ; ++it2)
			{
			psml_port *inport_target = *it2;
			psml_process *target = inport_target->GetOwnerProcess();
			cout << "target ID: " << target->_lp->GetID() << std::endl;
			// We have found an inter-cluster link between target and destination.
			if( target->_lp->GetOwnerClusterID() != destination->_lp->GetOwnerClusterID() )
			{
			//cout << "found" << std::endl;
			psml_interface *_if = inport_dest->GetBoundInterface();
			if(_if->GetInterfaceType() == __NonSharedInterface)
			{

			psml_nonshared_interface *_nif = (psml_nonshared_interface *)_if;
			psml_nonshared_interface *_newif = _nif->CreateInstance();
			_newif->SetIsInstance(true);
			//
			break;
			}
			}
			}
			}*/
			/**------------------ End of Elaboration--------------------------------------------------**/
		}
		//----------------------------------------------------
		void psml_pdes_lp::Initialize()
		{
            _inports = &(_p->_ownerComponent->_inports);

            /*if(!GetIsClockLogicalProcess() && _p->_inports.size() != 0)
            {
                _p->_dont_initialize = true;
                return;
            }*/

            /*for(Int32 i = 0 ; i < _inports->Size() ; i++)
            {
                psml_port *inport = (*_inports)[i];
                psml_nonshared_interface *_nif = (psml_nonshared_interface *)inport->GetBoundInterface();
                _nif->Commit();
            }*/

            if(_p->_dont_initialize)
            {
                ///std::cout << "Initialize() " << GetID() << " comp " << _p->_ownerComponent->GetName().get_BaseStream() << std::endl;
                return;
            }

			if(_p->_process_type == __STACKFULL_PROCESS__)
			{
				//_p->_fib = Fiber::ConvertThreadToFiber();
				//Fiber::SwitchToFiber(_p->_fib, _p->_fib_process);
				Fiber::SwitchToFiber(psml_process::__fiber_clusters[GetOwnerClusterID()], _p->_fib_process);
			}
			else
				_p->_pm(_p);

			//_p->_cur_fiber = psml_process::__fiber_clusters[GetOwnerClusterID()];
			//numEventProcessed = 0;
			//myState = (ServerState *)AllocateState();
		}
		//----------------------------------------------------
		void psml_pdes_lp::Finalize()
		{
			UInt32 clusterID = GetOwnerClusterID();

			if(psml_process::__fiber_clusters[clusterID] != null)
			{
				Fiber::ConvertFiberToThread();
				delete psml_process::__fiber_clusters[clusterID];
				psml_process::__fiber_clusters[clusterID] = null;
			}

			//DeallocateState(myState);
		}
		//----------------------------------------------------
		void psml_pdes_lp::ExecuteProcess()
		{
            //std::cout << "ExecuteProcess() " << GetID() << " comp " << _p->_ownerComponent->GetName().get_BaseStream() << std::endl;

			psml_pdes_event *inputEvent = (psml_pdes_event *)GetNextEvent();

#if defined __cycles_enabled__
			if(inputEvent->GetSendTime() != inputEvent->GetReceiveTime())
			{
                (*psml_process::__cycles)[GetOwnerClusterID()] = 0;
                psml_nonshared_interface::CommitInsenistiveInterfaces(GetOwnerClusterID());
                //i_global = 0;
            }
            else
            {
                if(inputEvent->cycle > (*psml_process::__cycles)[GetOwnerClusterID()])
                {
                    psml_nonshared_interface::CommitInsenistiveInterfaces(GetOwnerClusterID());
                    (*psml_process::__cycles)[GetOwnerClusterID()] = inputEvent->cycle;
                }

                //i_global = inputEvent->i;
            }
#endif

			//if(inputEvent != null)
			{
                //CommitInsenistivePorts();

                /*if(_p->_stop_requested || inputEvent->GetEventType() == TerminationHasRequestedMessageByUser)
                    return;
                else if(inputEvent->_type == __PSML_NULL_MESSAGE)
                {
                    //inputEvent->print();
                    return;
                }
				else */
				if(inputEvent->_type == __DATA_UPDATE_FOR_NONE_SHARED_INTERFACE_EVENT)
				{
                    bool isTriggered = ProcessAggregatedNsiEvent(inputEvent);

                    //if(!psml_simulator::IsParallelExecution())
                        if(ProcessPeekedNextEvents())
                            isTriggered = true;

					if(isTriggered)
					{
						if(_p->_process_type == __STACKLESS_PROCESS__)
						{
#if defined __profile_mode_enabled_for_psml_pdes_lp__
							numberOfActivatedProcesss++;
#endif
							_p->_pm(_p);
						}
						else
						{
							if(_p->_has_wait_registered)
							{
								if(_p->_wait_type == __WAIT)
								{
#if defined __profile_mode_enabled_for_psml_pdes_lp__
                                    numberOfActivatedProcesss++;
#endif
									_p->_has_wait_registered = false;
									if(_p->_process_type == __STACKFULL_PROCESS__)
                                        Fiber::SwitchToFiber(psml_process::__fiber_clusters[GetOwnerClusterID()], _p->_fib_process);
                                    else if(_p->_process_type == __SPECIAL_PROCESS__)
                                        _p->_pm(_p);
                                    else
                                        throw InvalidOperationException("Unsupported psml_process_type");
									//Fiber::SwitchToFiber(_p->_cur_fiber, _p->_fib_process);
								}
                                else if(_p->_wait_type == __WAIT_N)
                                {
                                    //if(_p->_num_of_sensitivity_list == 0)
                                    //    throw InvalidOperationException("psml_wait(n) was called but the psml_process had not senstive to at least a psml_inport or an event");

                                    ///_p->_n_counter++;

                                    if(_p->_n_registered == _p->_n_counter)
                                    {
#if defined __profile_mode_enabled_for_psml_pdes_lp__
                                        numberOfActivatedProcesss++;
#endif
                                        _p->_n_registered  = 0;
                                        _p->_n_counter = 0;
                                        _p->_has_wait_registered = false;
                                        if(_p->_process_type == __STACKFULL_PROCESS__)
                                            Fiber::SwitchToFiber(psml_process::__fiber_clusters[GetOwnerClusterID()], _p->_fib_process);
                                        else if(_p->_process_type == __SPECIAL_PROCESS__)
                                            _p->_pm(_p);
                                        else
                                            throw InvalidOperationException("Unsupported psml_process_type");
                                    }

                                    goto End;
                                    //return;
                                }
								else if(_p->_wait_type == __WAIT_T || _p->_wait_type == __WAIT_E) // do nothing.
                                    goto End;
									//return;
								else
									throw InvalidOperationException("1An update was made on a sensitive port but a wrong wait(expr) issued rather than wait()");
							}
							else
							{
                                if(_first_execution && _p->_dont_initialize)
                                {
                                    _first_execution = false;
                                    //std::cout << "_first_execution" << std::endl;
                                    //inputEvent->print();
                                    if(_p->_process_type == __STACKFULL_PROCESS__)
                                        Fiber::SwitchToFiber(psml_process::__fiber_clusters[GetOwnerClusterID()], _p->_fib_process);
                                    else if(_p->_process_type == __SPECIAL_PROCESS__)
                                        _p->_pm(_p);

                                    goto End;
                                }

                                //inputEvent->print();
								throw InvalidOperationException("2An update was made on a sensitive port but no wait() issued");
                            }
						}
					}
					else
					{
                        //inputEvent->print();
                        //std::cout << "@time " << GetSimulationTime() << " port " << inputEvent->_destinationPort << " val ";\
                        _nif->Print();
					}
				}
				/*------------------------  __WAIT_SELF_EVENT ----------------------- */
				else if(inputEvent->_type == __WAIT_SELF_EVENT)
				{
					if(_p->_has_wait_registered)
					{
#if defined __profile_mode_enabled_for_psml_pdes_lp__
						numberOfActivatedProcesss++;
#endif
						_p->_has_wait_registered = false;
                        if(_p->_process_type == __STACKFULL_PROCESS__)
                            Fiber::SwitchToFiber(psml_process::__fiber_clusters[GetOwnerClusterID()], _p->_fib_process);
                        else if(_p->_process_type == __SPECIAL_PROCESS__)
                            _p->_pm(_p);
                        else
                            throw InvalidOperationException("Unsupported psml_process_type");
						//Fiber::SwitchToFiber(_p->_cur_fiber, _p->_fib_process);
						//Fiber::SwitchToFiber(_p->_fib, _p->_fib_process);
					}
					else
						throw InvalidOperationException("A psml_event was received but no wait(expr) issued");
				}
				/*------------------------  __CLOCK_SELF_EVENT ----------------------- */
				else if(inputEvent->_type == __CLOCK_SELF_EVENT)
				{
#if defined __profile_mode_enabled_for_psml_pdes_lp__
                    numberOfActivatedProcesss++;
#endif
					if(_p->_process_type == __STACKLESS_PROCESS__)
						_p->_pm(_p);
                    else
                        throw InvalidOperationException("Clock processes can not be stackful");
				}
				/*------------------------  __PSML_EVENT --------------------------- */
				else if(inputEvent->_type == __PSML_EVENT)
				{
#if defined __profile_mode_enabled_for_psml_pdes_lp__
					numberOfActivatedProcesss++;
#endif
                    if(_p->_process_type == __STACKLESS_PROCESS__)
						_p->_pm(_p);
					else
					{
						if(_p->_has_wait_registered)
						{
							/*-----------  wait(e) ----------------- */
							// Consider here for wait(n)
							if(_p->_wait_type == __WAIT_E)
							{
								UInt64 eventID = *( (UInt64 *)((unsigned char *)inputEvent + sizeof(UInt64)) );
								//if(_p->_wait_e->_eventID == inputEvent->_eventID)
								if(_p->_wait_e->_eventID == eventID)
								{
									_p->_has_wait_registered = false;
                                    if(_p->_process_type == __STACKFULL_PROCESS__)
                                        Fiber::SwitchToFiber(psml_process::__fiber_clusters[GetOwnerClusterID()], _p->_fib_process);
                                    else if(_p->_process_type == __SPECIAL_PROCESS__)
                                        _p->_pm(_p);
                                    else
                                        throw InvalidOperationException("Unsupported psml_process_type");
									//Fiber::SwitchToFiber(_p->_fib, _p->_fib_process);
								}
								else
									throw InvalidOperationException("A wait(e) was issued but an incorrect psml_event has received");
							}
							/*-----------  wait(andlist) ----------- */
							/*-----------  wait(orlist) ------------ */
							/*-----------  Otherwise --------------- */
							else
							{
								//std::cout << "throw InvalidOperationException(Unsupported wait(expr) type GetID(): " << GetID() << " _p->_wait_type: " << _p->_wait_type << " inputEvent->_type: " << inputEvent->_type << " _eventID: " << inputEvent->_eventID << std::endl;
								throw InvalidOperationException("Unsupported wait(expr) type");
							}
							//throw InvalidOperationException("Unsupported wait(expr) type GetID(): " + Convert::ToString(GetID()) + " _p->_wait_type: " + Convert::ToString(_p->_wait_type) + " inputEvent->_type: " + Convert::ToString(inputEvent->_type) + " _eventID: " + Convert::ToString(inputEvent->_eventID));
						}
						else
							throw InvalidOperationException("A psml_event was received but no wait(expr) issued");
					}
				}
				/*------------------------  Otherwise ------------------------------ */
				else
					throw InvalidOperationException("Unsupported psml_pdes_event was received");
			}

        End:
            if(/*!GetIsClockLogicalProcess() && _p->_outports.size() != 0 &&*/ _p->_flush_list.Size() != 0)
                _p->Flush(); // We must extend this function ti support psml_event.
		}
        //----------------------------------------------------
        bool psml_pdes_lp::ProcessPeekedNextEvents()
        {
            UInt16 triggers = 0;
            psml_pdes_event *nextEvent;

            //while(HaveMoreEvents())
            while( (nextEvent = (psml_pdes_event *)PeekNextEvent()) != null)
            {
                //psml_pdes_event *nextEvent = (psml_pdes_event *)PeekNextEvent();

                //if(nextEvent->_type == __DATA_UPDATE_FOR_NONE_SHARED_INTERFACE_EVENT && nextEvent->i == i_global)
#if defined __cycles_enabled__
                if(nextEvent->_type == __DATA_UPDATE_FOR_NONE_SHARED_INTERFACE_EVENT && nextEvent->cycle == (*psml_process::__cycles)[GetOwnerClusterID()])
#else
                if(nextEvent->_type == __DATA_UPDATE_FOR_NONE_SHARED_INTERFACE_EVENT)
#endif
                {
                    nextEvent = (psml_pdes_event *)GetNextEvent();
                    if(ProcessAggregatedNsiEvent(nextEvent))
                        triggers++;

                    //if(isTriggered && _p->_wait_type == __WAIT_N)\
                    {\
                        if(_p->_n_counter == _p->_n_registered)\
                        {\
                            std::cout << "_p->_n_counter " << _p->_n_counter << std::endl;\
                            exit(0);\
                            break;\
                        }\
                    }
                }
                else
                    break;
            }

            return triggers;
        }
        //----------------------------------------------------
        bool psml_pdes_lp::ProcessAggregatedNsiEvent(psml_pdes_event *e)
        {
            //std::cout << "psml_pdes_lp::ProcessAggregatedNsiEvent() " << GetName().get_BaseStream() << std::endl;\
            e->print();

            unsigned char *p = (unsigned char *)e + sizeof(class psml_pdes_event);

            psml_port *inport;
            psml_nonshared_interface *nif;
            bool is_intra_cluster_communication;

            UInt16 portID;
            UInt16 numOfSensitivePorts = 0;
            UInt16 numOfMessages;
            UInt16 messageSize;
            //::memcpy(&numOfMessages, p, sizeof(UInt16)); // numOfMessages
            numOfMessages = *p;
            p += sizeof(UInt16);

            //mtx->Lock();

            //std::cout << "@time " << GetSimulationTime() << " ClusterID " << GetOwnerClusterID() << " sender " << e->GetSenderReference() << " receiver " << GetID() << " numOfMessages " << numOfMessages << " ";\
            //std::cout << "portIDs ";

            //for(register UInt16 i = 0 ; i < numOfMessages ; i++)
            //for(register Int16 i = numOfMessages - 1 ; i != -1 ; i--)
            for(register UInt16 i = numOfMessages ; i != 0 ; i--)
            {
                //::memcpy(&portID, p, sizeof(UInt16)); // PortID
                portID = *p;
                p += sizeof(UInt16);

                //std::cout << " " << portID; std::cout.flush();

                inport = (*_inports)[portID];
                nif = (psml_nonshared_interface *)inport->GetBoundInterface();
                psml_edge_trigger_type edgeType = inport->GetEdgeTriggerType();
                is_intra_cluster_communication = ! nif->GetIsInstance();

                ///nif->Print();

                if(is_intra_cluster_communication)
                    nif->Commit();
                else
                {
                    messageSize = nif->ValueSize();
                    nif->CopyValue(p);
                    p += messageSize;
                }

                if(edgeType == PSML_UNKNOW_EDGE)
                    continue;
                else if(edgeType == PSML_BOTH_EDGES)
                {
                    numOfSensitivePorts++;
                }
                else if(edgeType == PSML_POS_EDGE && nif->GetIsLevelHigh())
                {
                    numOfSensitivePorts++;
                }
                else if(edgeType == PSML_NEG_EDGE && !nif->GetIsLevelHigh())
                {
                    numOfSensitivePorts++;
                }

                //std::cout << "portID " << portID << " port_is_sensitive " << port_is_sensitive\
                          << " is_intra_cluster_communication " << is_intra_cluster_communication\
                          << " isTriggered_ " << isTriggered_ << std::endl;\
                nif->Print();\
                std::cout << "ProcessAggregatedNsiEvent() " << nif << std::endl;
            }

            //std::cout << std::endl;

            //mtx->Unlock();

            //exit(0);


            if(_p->_wait_type == __WAIT_N)
            {
                _p->_n_counter += numOfSensitivePorts;
                        //std::cout << "_p->_n_counter " << _p->_n_counter << std::endl;
            }

            return numOfSensitivePorts;
        }
        //----------------------------------------------------
        /*void psml_pdes_lp::CommitInsenistivePorts()
		{
            //return;
            if(_uncommitted_insensitive_inports.Size() != 0)
            {
                //std::cout << "_uncommitted_insensitive_inports " << _uncommitted_insensitive_inports.Size() << std::endl;
                for(register Int32 i = 0 ; i < _uncommitted_insensitive_inports.Size() ; i++)
                //for(register Int32 i = _uncommitted_insensitive_inports.Size() - 1 ; i != -1 ; i--)
                {
                    psml_port *inport = _uncommitted_insensitive_inports[i];
                    psml_nonshared_interface *_nif = (psml_nonshared_interface *)inport->GetBoundInterface();
                    _nif->Commit();
                }
                _uncommitted_insensitive_inports.Reset();
			}
		}*/
		//----------------------------------------------------
		const String &psml_pdes_lp::GetName()
		{
			return _p->GetName();
			//return objName;
		}
		//----------------------------------------------------
		State *psml_pdes_lp::GetState()
		{
			return null;
			//return myState;
		}
		//----------------------------------------------------
		State *psml_pdes_lp::AllocateState()
		{
			return null;
			//return (State *)new ServerState();
		}
		//----------------------------------------------------
		void psml_pdes_lp::DeallocateState(const State *state)
		{
			//delete (ServerState *)state;
		}
		//----------------------------------------------------
		void psml_pdes_lp::DeallocateEvent(const EventInterface *e)
		{
			psml_pdes_memory_manager::free((psml_pdes_event *)e);
			//delete (psml_pdes_event *)e;
		}
		//----------------------------------------------------
		void psml_pdes_lp::PrintInformation()
		{
			cout << "------------------------------------------------\n";
			cout << "The logical process '" << GetName().get_BaseStream() << "' was successfully instantiated.\n";
			cout << "Information of Logical Process '" << GetName().get_BaseStream() << "':\n";
			cout << "GetID: " << GetID() << "\n";
			cout << "GetOutputLpCount(): " << GetOutputLpCount() << "\n";
			cout << std::endl;
		}
		//----------------------------------------------------
	}
};
//**************************************************************************************************************//
