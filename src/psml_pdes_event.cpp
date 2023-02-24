/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/


#include "psml_pdes_event.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        //----------------------------------------------------
        psml_pdes_event::psml_pdes_event(UInt64 sendTime, UInt64 receiveTime, UInt32 sender, UInt32 receiver) : _data_size(0)
        {
			SetSendTime(sendTime);
			SetReceiveTime(receiveTime);
			SetSenderReference(sender);
			SetReceiverReference(receiver);

            //disposed = false;
        }
        //----------------------------------------------------
        psml_pdes_event::~psml_pdes_event()
        {
            //if(!disposed)
            //{
            //    disposed = true;
            //}
        }
        //----------------------------------------------------
        void psml_pdes_event::print()
        {
            std::cout << "sender(" << GetSenderReference() << "," << LogicalProcess::GetLogicalProcessById(GetSenderReference())->GetName().get_BaseStream() << ")";
            std::cout << " receiver(" << GetReceiverReference() << "," << LogicalProcess::GetLogicalProcessById(GetReceiverReference())->GetName().get_BaseStream() << ")";
            std::cout << " sendTime " << GetSendTime();
            std::cout << " receiveTime " << GetReceiveTime();
            std::cout << " psml_event_type " << _type << std::endl;
        }
        //----------------------------------------------------
	}
};
//**************************************************************************************************************//
