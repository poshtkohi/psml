/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "monitor_comp.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            monitor_comp::monitor_comp(const String &name)
            {
				SetName(name);

				register_proxy(COUT_s, _COUT_s);
				register_proxy(S0, _S0);
				register_proxy(S1, _S1);
				register_proxy(S2, _S2);
				register_proxy(S3, _S3);

				p = register_process(Wrapper_To_Call_process, false, "process");
				p->register_port(_COUT_s);
				p->register_port(_S0);
				p->register_port(_S1);
				p->register_port(_S2);
				p->register_port(_S3);
				p->register_sensitivity(_COUT_s);
				p->register_sensitivity(_S0);
				p->register_sensitivity(_S1);
				p->register_sensitivity(_S2);
				p->register_sensitivity(_S3);
            }
            //----------------------------------------------------
            monitor_comp::~monitor_comp()
            {
            }
			//----------------------------------------------------
			inline bool toBool(char c)
			{
				if(c == '1')
					return true;
				else
					return false;
			}
			//----------------------------------------------------
			void monitor_comp::process(psml_process *ownerProcess)
			{
				std::cout << "@time " << psml_time_stamp(ownerProcess);\
				std::cout << " S3 S2 S1 S0 COUT_s "<< _S3->fetch() << _S2->fetch() << _S1->fetch() << _S0->fetch() << _COUT_s->fetch() << endl;
			}
			//----------------------------------------------------
			void monitor_comp::Wrapper_To_Call_process(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to monitor_comp
				monitor_comp *mySelf = (monitor_comp *)owner;
				// call member
				mySelf->process(ownerProcess);
			}
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
