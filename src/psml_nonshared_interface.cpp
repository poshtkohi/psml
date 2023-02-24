/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_nonshared_interface.h"
#include "psml_port.h"
#include "psml_connector.h"
#include "psml_process.h"
#include "psml_pdes_lp.h"
#include "psml_pdes_event.h"
#include "psml_time.h"
#include "psml_pdes_memory_manager.h"
#include "psml_simulator.h"

//Mutex *__mtx__ = new Mutex();
//extern UInt32 i_global;
//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        //----------------------------------------------------
#if defined __cycles_enabled__
        Vector<Vector<psml_nonshared_interface *>* > *psml_nonshared_interface::__uncommitted_insensitive_nonshared_interfaces = null;
#endif
		//----------------------------------------------------
		psml_nonshared_interface::psml_nonshared_interface(): _isLevelHigh(false), _offloaded(false), /*_commited(false),*/ _numOfSensitivePorts(0)
		{
		}
		//----------------------------------------------------
		void psml_nonshared_interface::Offload(/*const psml_time &delay, */psml_process *issuer)
		{
            bool is_intra_cluster_communication = ! GetIsInstance();

#if defined __cycles_enabled__

            if(_numOfSensitivePorts == 0 && is_intra_cluster_communication && issuer->get_delay()._m_value == 0)
            {
                //if(!_offloaded)
                {
                    (*__uncommitted_insensitive_nonshared_interfaces)[issuer->_lp->GetOwnerClusterID()]->PushBack(this);
                    //_offloaded = true;
                }
                return;
            }
#else
            if(_numOfSensitivePorts == 0 && is_intra_cluster_communication && issuer->get_delay()._m_value == 0)
            {
                //receiver->_uncommitted_insensitive_inports.PushBack(inport);
                Commit1();
                //_offloaded = false;
                //_commited = false;
                return;
            }
#endif

            if(!_offloaded /*|| issuer->get_delay()._m_value != 0*/)
            {
                issuer->_flush_list.PushBack(this);
                _offloaded = true;
                return;
                //_commited = false;
            }
            //std::cout << "@time " << issuer->_lp->GetSimulationTime() << " issuer " << issuer->GetName().get_BaseStream() << std::endl;
		}
		//----------------------------------------------------
		void psml_nonshared_interface::FlushClock(psml_process *issuer, Vector<psml_port *> *subscriber_inports)
		{
			//cout << "FlushClock" << std::endl;

			psml_pdes_lp *sender = issuer->_lp;
			UInt32 sender_ = sender->GetID();
			UInt32 senderClusterID = sender->GetOwnerClusterID();

			UInt64 simTime = sender->GetSimulationTime();
			UInt64 sendTime = simTime;
			UInt64 recvTime = sendTime + issuer->get_delay()._m_value;

            UInt32 allocation_size = sizeof(UInt16); //numOfMessages
            static const UInt16 numOfmessagesOne = 1;
            bool is_intra_cluster_communication = ! GetIsInstance();
            if(is_intra_cluster_communication)
                allocation_size += numOfmessagesOne * sizeof(UInt16); // numOfMessages*PortID
            else
                allocation_size += numOfmessagesOne * sizeof(UInt16) + ValueSize(); // numOfMessages*PortID+MessageSize

            ///std::cout << "allocation_size " << allocation_size << " is_intra_cluster_communication " << is_intra_cluster_communication << std::endl;

			//std::cout << "sendTime " << sendTime << " recvTime " << recvTime << std::endl;

			// Input ports with respect to the connector are destination logical processes.
            // If there are any structural casuality errors in the simulation, we have to
            // suspect this fast for loop. Then, replace it as you have seen such behaviour.
            //i_global = 0;
            for(register UInt32 i = 0 ; i < subscriber_inports->Size() ; i++)
			//for(register Int32 i = subscriber_inports->Size() - 1 ; i != -1 ; i--)
			{
				psml_port *inport = (*subscriber_inports)[i];
				psml_process *destination = inport->GetOwnerProcess();
				psml_pdes_lp *receiver = destination->_lp;
				UInt32 receiver_ = receiver->GetID();
                UInt32 receiverClusterID = receiver->GetOwnerClusterID();
                UInt16 portID = inport->GetPortID();

				psml_pdes_event *e;

                e = psml_pdes_memory_manager::malloc(senderClusterID, receiverClusterID, allocation_size);
                e->SetSendTime(sendTime);
                e->SetReceiveTime(recvTime);
                e->SetSenderReference(sender_);
                e->SetReceiverReference(receiver_);
                e->_type = __DATA_UPDATE_FOR_NONE_SHARED_INTERFACE_EVENT;
                //e->i = i_global;
#if defined __cycles_enabled__
                e->cycle = (*psml_process::__cycles)[senderClusterID] + 1;
#endif
                //std::cout << "e " << e << std::endl;

                unsigned char *p = (unsigned char *)e + sizeof(class psml_pdes_event);

                //::memcpy(p, &numOfmessagesOne, sizeof(UInt16)); // numOfMessages
                *p = numOfmessagesOne;
                p += sizeof(UInt16);

                //::memcpy(p, &portID, sizeof(UInt16)); // PortID
                *p = portID;
                p += sizeof(UInt16);

                if(!is_intra_cluster_communication)
                {
                    UInt16 messageSize = ValueSize();
                    CloneValueToPointer(p);
                    p += messageSize;
                }

				//std::cout << "FlushClock() "; e->print();
				///std::cout << "FlushClock() " << this << " ";  e->print();
				sender->SendEvent(e);
			}
		}
		//----------------------------------------------------
#if defined __cycles_enabled__
		void psml_nonshared_interface::CommitInsenistiveInterfaces(UInt32 ClusterID)
		{
            Vector<psml_nonshared_interface *> *myCluster = (*__uncommitted_insensitive_nonshared_interfaces)[ClusterID];
            if(myCluster->Size() != 0)
            {
                //for(register Int32 i = 0 ; i < myCluster->Size() ; i++)
                //for(register Int32 i = myCluster->Size() - 1 ; i != -1 ; i--)
                for(register UInt32 i = myCluster->Size() ; i != 0 ; i--)
                {
                    psml_nonshared_interface *nif = (*myCluster)[myCluster->Size() - i];
                    nif->Commit1();
                    //nif->_offloaded = false;
                }
                    //(*myCluster)[i]->Commit1();
                myCluster->Reset();
			}
		}
#endif
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
