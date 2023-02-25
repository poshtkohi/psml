/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "source_comp.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            source_comp::source_comp(const String &name)
            {
				SetName(name);

				register_proxy(proxy_out, out);

				p0 = register_process(Wrapper_To_Call_P0, true);
				p0->register_port(out);
            }
            //----------------------------------------------------
            source_comp::~source_comp()
            {
            }
			//----------------------------------------------------
			void source_comp::P0(psml_process *ownerProcess)
			{
				//cout << "source_comp::P0()" << endl;
				Int32 counter = 0;
				psml_time delay(3, PSML_NS);
				psml_time signal_delay(1, PSML_NS);

				while(true)
				{
					//cout << "counter: " << counter << endl;

					//std::cout << "@time " << psml_time_stamp(ownerProcess) \
					<< " sleep in source_comp::P0() due to wait(" << delay  << ")" << endl;

					out->put(counter, signal_delay, ownerProcess);
					psml_wait(delay, ownerProcess);

					//std::cout << "@time " << psml_time_stamp(ownerProcess) \
					<< " wakeup in source_comp::P0() due to wait(" << delay  << ")" << endl;

					counter++;
				}
			}
			//----------------------------------------------------
			void source_comp::Wrapper_To_Call_P0(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to source_comp
				source_comp *mySelf = (source_comp *)owner;
				// call member
				mySelf->P0(ownerProcess);
			}
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
