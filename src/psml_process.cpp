/**
	#define meta ...
	printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_process.h"
#include "psml_interface.h"
#include "psml_proxy_interface.h"
#include "psml_component.h"
#include "psml_connector.h"
#include "psml_nonshared_interface.h"
#include "psml_pdes_memory_manager.h"

//#include <unordered_map>
extern Mutex *__mtx__;
//extern UInt32 i_global;
//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		std::vector<psml_process *> *psml_process::__processes = null;
		Vector<Fiber *> psml_process::__fiber_clusters;
#if defined __cycles_enabled__
		Vector<UInt32> *psml_process::__cycles = null;
#endif
		//----------------------------------------------------
		psml_process::psml_process() : _dont_initialize(false)
		{
            if(__processes == null)
                __processes = new std::vector<psml_process *>;

			_lp = new psml_pdes_lp(this);
			_lp->SetData(this);
			//__processes.push_back(this);
			_fib_process = null;
			//_cur_fiber = null;
			_process_state = null;
			//_from_chained_execution = false;
			//_fib = null;
			_has_wait_registered = false;
			_num_of_sensitivity_list = 0;
			_n_registered = 0;
			_n_counter = 0;
			_flush_ht = null;
			_order = null;
			_stop_requested = false;
			//_scheduled = false;
		}
		//----------------------------------------------------
		psml_process::~psml_process()
		{
			if(_process_type == __STACKFULL_PROCESS__)
			{
				//Fiber::ConvertFiberToThread();
                ///std::cout << _fib_process << " process " << _lp->GetID() << std::endl;
				delete _fib_process;
				//delete _fib;
				//delete psml_process::__fiber_clusters[_lp->GetOwnerClusterID()];
			}

			if(_flush_ht != null)
			{
                for(register Int32 i = 0 ; i < _flush_ht->capacity() ; i++)
                {
                    hashTableType val;
                    if(_flush_ht->findAtIndex(i, val))
                    {
                        UInt32 *values_size = val.first;
                        vectorType *v = val.second;
                        delete values_size;
                        delete v;
                    }
                }

                delete _flush_ht;
			}

			if(_order != null)
                delete _order;
		}
		//----------------------------------------------------
		void psml_process::register_port(psml_port *port)
		{
			if(port == null)
				throw ArgumentNullException("port", "port is null");

			if(port->GetPortType() == __InPort)
			{
				if(_inports.count(port) == 0)
				{
                    port->SetEdgeTriggerType(PSML_UNKNOW_EDGE);
					port->SetOwnerProcess(this);
					port->SetOwnerComponent(_ownerComponent);
					port->SetPortID(_ownerComponent->_inports.Size());
					_ownerComponent->_inports.PushBack(port);
					_inports[port] = port;
				}
				else
					throw InvalidOperationException("port has already been registered");
			}
			else if(port->GetPortType() == __OutPort)
			{
				if(_outports.count(port) == 0)
				{
					port->SetOwnerProcess(this);
					port->SetOwnerComponent(_ownerComponent);
					port->SetPortID(_ownerComponent->_outports.size());
					_ownerComponent->_outports.push_back(port);
					_outports[port] = port;
				}
				else
					throw InvalidOperationException("port has already been registered");
			}
			else if(port->GetPortType() == __GeneralPort)
			{
				if(_gports.count(port) == 0)
				{
					port->SetOwnerProcess(this);
					port->SetOwnerComponent(_ownerComponent);
					port->SetPortID(_ownerComponent->_gports.size());
					_ownerComponent->_gports.push_back(port);
					_gports[port] = port;
				}
				else
					throw InvalidOperationException("port has already been registered");
			}
			else
				throw InvalidOperationException("port type is unknown");
		}
		//----------------------------------------------------
		void psml_process::register_port(psml_port &port)
		{
			register_port(&port);
		}
		//----------------------------------------------------
		void psml_process::register_sensitivity(psml_port *port)
		{
			if(port == null)
				throw ArgumentNullException("port", "port is null");

			if(port->GetPortType() != __InPort)
				throw InvalidOperationException("Only input ports (psm_inport) can be registered as a sensitive port");

			if(port->GetSensitive())
				throw InvalidOperationException("This port has already been registered as a sensitive port");

			//psml_interface *_if = port->GetBoundInterface();
			//cout << "_if" << _if << std::endl;
			//if(_if->GetInterfaceType() != __NonSharedInterface)
			//	throw InvalidOperationException("Only ports bound to psml_nonshared_interface instances can be registered as a sensitive port");

			port->SetSensitive(true);
			port->SetEdgeTriggerType(PSML_BOTH_EDGES);
			_num_of_sensitivity_list++;
		}
		//----------------------------------------------------
		void psml_process::register_sensitivity(psml_port &port)
		{
			register_sensitivity(&port);
		}
		//----------------------------------------------------
		void psml_process::register_sensitivity(psml_port *port, psml_edge_trigger_type type)
		{
			if(port == null)
				throw ArgumentNullException("port", "port is null");

			if(port->GetPortType() != __InPort)
				throw InvalidOperationException("Only input ports (psm_inport) can be registered as a sensitive port");

			//if(port->GetSensitive())
			//	throw InvalidOperationException("This port has already been registered as a sensitive port");

			psml_edge_trigger_type t = port->GetEdgeTriggerType();

			if(t == PSML_UNKNOW_EDGE)
			{
				port->SetEdgeTriggerType(type);
				port->SetSensitive(true);
				_num_of_sensitivity_list++;
			}
			else if(t == PSML_POS_EDGE && type == PSML_NEG_EDGE)
				port->SetEdgeTriggerType(PSML_BOTH_EDGES);
			else if(t == PSML_NEG_EDGE && type == PSML_POS_EDGE)
				port->SetEdgeTriggerType(PSML_BOTH_EDGES);
			else
				throw InvalidOperationException("This port has already been registered as a sensitive port");
		}
		//----------------------------------------------------
		void psml_process::register_sensitivity(psml_port &port, psml_edge_trigger_type type)
		{
			register_sensitivity(&port, type);
		}
		//----------------------------------------------------
		void *psml_process::Wrapper_To_Call_Fiber(void *pt2Object)
		{
			// explicitly cast to a pointer to psml_process
			psml_process *mySelf = (psml_process *)pt2Object;

			// call member
			mySelf->_pm(/*mySelf->_ownerComponent,*/ mySelf);

			Fiber::SwitchToFiber(mySelf->_fib_process, psml_process::__fiber_clusters[mySelf->_lp->GetOwnerClusterID()]);
			//Fiber::SwitchToFiber(mySelf->_fib_process, mySelf->_fib);
			//Fiber::ConvertFiberToThread();

			return pt2Object;
		}
		//----------------------------------------------------
		const String &psml_process::GetName()
		{
			return _objName;
		}
        //----------------------------------------------------
        void psml_process::dont_initialize()
		{
            _dont_initialize = true;
		}
        //----------------------------------------------------
        void psml_process::Flush()
		{
            //if(_flush_list.Size() == 0)
            //    return;

            UInt32 sender_ = _lp->GetID();
            UInt32 senderClusterID = _lp->GetOwnerClusterID();
            UInt64 simTime = _lp->GetSimulationTime();
			UInt64 sendTime = simTime;
			UInt64 recvTime = sendTime + get_delay()._m_value;
            //std::cout << "psml_process::Flush() _flush_list " << _flush_list.Size() << " processType " << _process_type << std::endl;

            //for(register Int32 i = _flush_list.Size() - 1 ; i != -1 ; i--)
            //std::unordered_map<UInt32, UInt32> order;
            //psml_hash_table<UInt32> order = psml_hash_table<UInt32>(_flush_ht->capacity()); // For performance !
            //Vector<UInt32> orders;

            /*__mtx__->Lock();
            std::cout << "-------------------------------------" << std::endl;
            std::cout << "in " << GetName().get_BaseStream() << std::endl;
            std::cout << "ht " << _flush_ht->capacity() << " order " << order.capacity() << std::endl;
            std::cout << "Flush order " << _flush_list.Size() << std::endl;*/

            //for(register UInt32 i = 0 ; i < _flush_list.Size() ; i++)
            for(register UInt32 i = _flush_list.Size() ; i != 0 ; i--)
            {
                psml_nonshared_interface *nif = _flush_list[_flush_list.Size() - i];
                //psml_nonshared_interface *nif = _flush_list[i];
                //std::cout << "psml_process::Flush() " << i << std::endl;
                psml_connector *connector = nif->GetOwnerConnector();
                Vector<psml_port *> *inports = connector->_inports;
                bool is_intra_cluster_communication = ! nif->GetIsInstance();
                /*if(inports->Size() == 0)
                {
                    std::cout << "processName " << GetName().get_BaseStream()
                              << " connector " << connector
                              << " nif " << nif
                              << " outports " << connector->_outports.size()
                              << " portID " << connector->_outports[0]->GetPortID()
                              << " portName " << connector->_outports[0]->GetName().get_BaseStream()
                              << " owner " << connector->_outports[0]->GetOwnerProcess()->GetName().get_BaseStream() << std::endl;
                    nif->Print();
                    exit(0);
                }*/
                //for(register Int32 j = inports->Size() - 1 ; j != -1 ; j--)
                //bool senstive_to_at_least_one_port = false;//
                //for(register UInt32 j = 0 ; j < inports->Size() ; j++)
                for(register UInt32 j = inports->Size() ; j != 0 ; j--)
                {
                    //std::cout << "inports->Size() " << inports->Size() << std::endl;
                    psml_port *inport = (*inports)[inports->Size() - j];
                    //psml_port *inport = (*inports)[j];
                    //if(inport->GetSensitive() ) /// Attention: For DSM execution //
                    //    senstive_to_at_least_one_port = true;
                    psml_process *destination = inport->GetOwnerProcess();
                    psml_pdes_lp *receiver = destination->_lp;
                    UInt32 _receiver = receiver->GetID();
                    /*if(!inport->GetSensitive() && is_intra_cluster_communication && sendTime == recvTime) /// Attention: For DSM execution //
                    {
                        //std::cout << "hello" << std::endl;
                        //std::cout << "j " << j << std::endl;
                        //nif->Commit();
                        //std::cout << "receiver " << receiver->GetName().get_BaseStream() << std::endl;
                        receiver->_uncommitted_insensitive_inports.PushBack(inport);
                        nif->_offloaded = false;
                        continue;
                    }*/

                    //std::cout << "\treceiver " << _receiver << std::endl;

                    hashTableType val;
                    if(_flush_ht->search(_receiver, val))
                    {
                        UInt32 *values_size = val.first;
                        vectorType *v = val.second;
                        *values_size += nif->ValueSize();
                        v->PushBack(portType(inport->GetPortID(), nif));

                        //if(order.count(_receiver) == 0)
                        //    order.insert(std::pair<UInt32, UInt32>(_receiver, _receiver));
                        //UInt32 dummy;
                        //if(!order.search(_receiver, dummy))
                        if(!_order->contains(_receiver))
                        {
                            _order->insert(_receiver, _receiver);
                            _orders.PushBack(_receiver);
                        }
                        //std::cout << "psml_process::before_end_of_elaboration() 1 " << nif->ValueSize() << std::endl;
                    }
                    else
                        throw InvalidOperationException("The process ID could not be found in the hashtable");
                }

                /*if(!senstive_to_at_least_one_port && !nif->GetIsInstance() && sendTime == recvTime)//
                {
                    nif->Commit();
                    nif->_offloaded = false;
                    //goto End;
                }*/
            }

            //std::cout << "Send order " << std::endl;
            //mtx->Lock();
            //for(Int32 i = _flush_ht->capacity() - 1 ; i != -1 ; i--)
            //for(register UInt32 i = 0 ; i < _flush_ht->capacity() ; i++)
            //for(register UInt32 i = 0 ; i < _orders.Size() ; i++)
            for(register UInt32 i = _orders.Size() ; i != 0 ; i--)
            {
                hashTableType val;
                //UInt32 key = _orders[i];
                UInt32 key = _orders[_orders.Size() - i];
                //UInt32 key;
                //if(_flush_ht->findAtIndex(i, key, val))
                //if(_flush_ht->findAtIndex(orders[i], key, val))
                if(_flush_ht->search(key, val))
                {
                    //std::cout << "psml_process::Flush() v " << i << std::endl;
                    UInt32 *values_size = val.first;
                    vectorType *v = val.second;
                    UInt16 numOfMessages = v->Size();
                    if(numOfMessages > 0)
                    {
                        UInt32 receiver_ = key;
                        //std::cout << "\t@time "  << _lp->GetSimulationTime() << " ClusterID " << _lp->GetOwnerClusterID() << " sender " <<  _lp->GetID() << " " << GetName().get_BaseStream() << " receiver " << receiver_ << " numOfMessages " << numOfMessages << " portIDs ";
                        LogicalProcess *receiver = LogicalProcess::GetLogicalProcessById(receiver_);
                        UInt32 receiverClusterID = receiver->GetOwnerClusterID();
                        bool is_intra_cluster_communication = ! (*v)[0].second->GetIsInstance();
                        psml_pdes_event *e;
                        // A message: {PortID,MessageBody}
                        // Aggregated Messages: numOfMessages {message}
                        UInt32 allocation_size = sizeof(UInt16); //numOfMessages
                        if(is_intra_cluster_communication)
                        {
                            //std::cout << "numOfMessages " << numOfMessages << std::endl;
                            allocation_size += numOfMessages * sizeof(UInt16); // numOfMessages*PortID
                            //std::cout << "is_intra_cluster_communication " << is_intra_cluster_communication << std::endl;
                        }
                        else
                        {
                            allocation_size += numOfMessages * sizeof(UInt16) + *values_size; // numOfMessages*PortID+MessageSize)
                            ///std::cout << "receiver " << receiver_ << " allocation_size " << allocation_size << std::endl;
                        }

                        e = psml_pdes_memory_manager::malloc(senderClusterID, receiverClusterID, allocation_size);
                        e->SetSendTime(sendTime);
                        e->SetReceiveTime(recvTime);
                        e->SetSenderReference(sender_);
                        e->SetReceiverReference(receiver_);
                        e->_type = __DATA_UPDATE_FOR_NONE_SHARED_INTERFACE_EVENT;
                        //e->i = i_global + 1;
#if defined __cycles_enabled__
                        e->cycle = (*psml_process::__cycles)[_lp->GetOwnerClusterID()] + 1;
#endif
                        //e->_data_size = allocation_size;
                        ///std::cout << "e " << e << std::endl;

                        unsigned char *p = (unsigned char *)e + sizeof(class psml_pdes_event);
                        // void *memcpy(void * destination, const void * source, size_t num)
                        // Copy block of memory
                        // Copies the values of num bytes from the location pointed to by source directly to the memory block pointed to by destination.
                        //::memcpy(p, &numOfMessages, sizeof(UInt16)); // numOfMessages
                        *p = numOfMessages;
                        p += sizeof(UInt16);

                        //for(register UInt16 j = 0 ; j < numOfMessages ; j++)
                        //for(register Int16 j = numOfMessages - 1 ; j != -1 ; j--)
                        for(register UInt16 j = numOfMessages ; j != 0 ; j--)
                        {
                            portType port = (*v)[numOfMessages - j];
                            //portType port = (*v)[j];
                            UInt16 portID = port.first;
                            psml_nonshared_interface *nif = port.second;
                            nif->_offloaded = false;

                            //std::cout << " " << portID ; std::cout.flush();

                            //::memcpy(p, &portID, sizeof(UInt16)); // PortID
                            *p = portID;
                            p += sizeof(UInt16);

                            /*if(is_intra_cluster_communication) // enable it for serial execution ?
                            {
                                if(sendTime == recvTime)//
                                {
                                    //std::cout << "is_intra_cluster_communication" << std::endl;
                                    nif->Commit();
                                }
                            }*/
                            if(!is_intra_cluster_communication)
                            //else
                            {
                                UInt16 messageSize = nif->ValueSize();
                                nif->CloneValueToPointer(p);
                                p += messageSize;
                            }
                        }

                        //std::cout << std::endl;
                        ///e->print();

                        _lp->SendEvent(e);

                        *values_size = 0;
                        v->Reset();
                    }
                }
            }

            /*std::cout << "Ordered" << std::endl; // for zero-copy ?
            //for(std::unordered_map<UInt32, UInt32>::iterator it = order.end() ; it != order.begin() ; it--)
            for(Int32 i = 0 ; i < orders.size() ; i++)
            {
                UInt32 receiver = orders[i];
                std::cout << "\treceiver " << receiver << std::endl;
            }*/

            /*for(register Int32 i = 0 ; i < _flush_ht->capacity() ; i++)
            {
                hashTableType val;
                if(_flush_ht->findAtIndex(i, val))
                {
                    UInt32 *values_size = val.first;
                    vectorType *v = val.second;
                    //std::cout << "v " << v->Size() << " values_size " << *values_size<< std::endl;
                    v->Reset();
                }
            }*/

            //mtx->Unlock();

            //__mtx__->Unlock();
        //End:
            _order->reset();
            _orders.Reset();
            _flush_list.Reset();
		}
		//----------------------------------------------------
		void psml_process::before_end_of_elaboration()
		{
            //if(_lp->GetIsClockLogicalProcess() || _flush_ht != null || _outports.size() == 0)
            //    return;

            //std::cout << "psml_process::before_end_of_elaboration() " << GetName().get_BaseStream() << std::endl;

            std::map<UInt32, UInt32> destProcesses;

			for(std::map<psml_port *, psml_port *>::iterator it = _outports.begin() ; it != _outports.end() ; ++it)
            {
                psml_port *outport = it->first;
                psml_connector *connector = outport->GetBoundConnector();
                Vector<psml_port *> *inports = connector->_inports;
                UInt32 _inports_size = inports->Size();

                for(register Int32 i = _inports_size - 1 ; i != -1 ; i--)
                {
                    psml_port *inport = (*inports)[i];
                    UInt32 destLpID = inport->GetOwnerProcess()->_lp->GetID();
                    if(destProcesses.count(destLpID) == 0)
                        destProcesses.insert(std::pair<UInt32, UInt32>(destLpID, destLpID));
                    //std::cout << "destLpID " << destLpID << std::endl;
                }
            }

            if(destProcesses.size() > 0)
            {
                _flush_ht = new psml_hash_table<hashTableType>(destProcesses.size());
                _order = new psml_hash_table<UInt32>(_flush_ht->capacity());

                for(std::map<UInt32, UInt32>::iterator it = destProcesses.begin() ; it != destProcesses.end() ; ++it)
                {
                    UInt32 destLpID = it->first;
                    vectorType *vt = new vectorType;
                    UInt32 *values_size = new UInt32(0);
                    hashTableType htt = hashTableType(values_size, vt);
                    _flush_ht->insert(destLpID, htt);
                }

                //std::cout << "psml_process::before_end_of_elaboration()1 " << _flush_ht->capacity() << std::endl;

                /*for(register Int32 i = 0 ; i < _flush_ht->capacity() ; i++)
                {
                    Vector<std::pair<UInt16, psml_nonshared_interface *> > *v;
                    if(_flush_ht->findAtIndex(i, v))
                    {
                        std::cout << "psml_process::before_end_of_elaboration()2 " << v << std::endl;
                    }
                }*/
                destProcesses.clear();
            }
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
