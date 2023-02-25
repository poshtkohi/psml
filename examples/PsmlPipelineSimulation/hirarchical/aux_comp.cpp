/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "aux_comp.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            aux_comp::aux_comp(const String &name)
            {
				SetName(name);

				register_proxy(proxy_in1, in1);
				register_proxy(proxy_in2, in2);

				p3 = register_process(Wrapper_To_Call_P3, false);
				p3->register_port(in1);
				p3->register_port(in2);
				p3->register_sensitivity(in1);
				p3->register_sensitivity(in2);
            }
            //----------------------------------------------------
            aux_comp::~aux_comp()
            {
            }
			//----------------------------------------------------
			void aux_comp::P3(psml_process *ownerProcess)
			{
				//cout << "aux_comp::P3()" << endl;

				Int32 _in1 = in1->fetch();
				Int32 _in2 = in2->fetch();

				std::cout << "@time " << psml_time_stamp(ownerProcess) << " in1: " << _in1 << " in2: " << _in2<< endl;

				//std::cout << "@time " << psml_time_stamp(ownerProcess) \
					<< " in aux_comp::P3 received in1: " << _in1 << " in2: " << _in2<< endl;
			}
			//----------------------------------------------------
			void aux_comp::Wrapper_To_Call_P3(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to aux_comp
				aux_comp *mySelf = (aux_comp *)owner;
				// call member
				mySelf->P3(ownerProcess);
			}
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
