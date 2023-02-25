/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "sink_comp.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            sink_comp::sink_comp(const String &name)
            {
				SetName(name);

				register_proxy(proxy_in1, in1);
				register_proxy(proxy_out2, out2);

				p1 = register_process(Wrapper_To_Call_P1, false, "p1");
				p1->register_port(in1);
				p1->register_port(out1);
				//p1->register_port(g1); ///
				p1->register_sensitivity(in1);
				out1.bind(c, c_w);
				//g1.bind(c_fifo, fifo);

				p2 = register_process(Wrapper_To_Call_P2, false, "p2");
				p2->register_port(in2);
				p2->register_port(out2);
				//p1->register_port(g2); ///
				p2->register_sensitivity(in2);
				in2.bind(c, c_w);
				//g2.bind(c_fifo, fifo);

				p4 = register_process(Wrapper_To_Call_P4, false, "p4");
				p4->register_port(in4);
				p4->register_sensitivity(in4);
				in4.bind(proxy_in1);

				aux = new aux_comp("aux");
				aux->proxy_in1.bind(proxy_in1); // or aux->proxy_in1(proxy_in1);
				aux->proxy_in2.bind(proxy_out2); // or aux->proxy_in2(proxy_out2);
            }
            //----------------------------------------------------
            sink_comp::~sink_comp()
            {
				delete aux;
            }
			//----------------------------------------------------
			void sink_comp::P1(psml_process *ownerProcess)
			{
				//cout << "sink_comp::P1()" << endl;

				Int32 _in1 = in1->fetch();

				//std::cout << "@time " << psml_time_stamp(ownerProcess) \
					<< " in sink_comp::P1 received in1: " << _in1 << endl;// " g1.get_interface() " << g1.get_interface() << endl;

				psml_time wire_delay(1, PSML_NS);

				out1->put(_in1, wire_delay, ownerProcess);
			}
			//----------------------------------------------------
			void sink_comp::P2(psml_process *ownerProcess)
			{
				//cout << "sink_comp::P2()" << endl;

				Int32 _in2 = in2->fetch();

				//std::cout << "@time " << psml_time_stamp(ownerProcess) \
					<< " in sink_comp::P2 received in2: " << _in2 << endl;

				psml_time wire_delay(1, PSML_NS);

				out2->put(_in2, wire_delay, ownerProcess);
			}
			//----------------------------------------------------
			void sink_comp::P4(psml_process *ownerProcess)
			{
				//cout << "sink_comp::P4()" << endl;

				Int32 _in4 = in4->fetch();

				//std::cout << "@time " << psml_time_stamp(ownerProcess) \
					<< " in sink_comp::P4 received in4: " << _in4 << endl;
			}
			//----------------------------------------------------
			void sink_comp::Wrapper_To_Call_P1(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to sink_comp
				sink_comp *mySelf = (sink_comp *)owner;
				// call member
				mySelf->P1(ownerProcess);
			}
			//----------------------------------------------------
			void sink_comp::Wrapper_To_Call_P2(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to sink_comp
				sink_comp *mySelf = (sink_comp *)owner;
				// call member
				mySelf->P2(ownerProcess);
			}
			//----------------------------------------------------
			void sink_comp::Wrapper_To_Call_P4(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to sink_comp
				sink_comp *mySelf = (sink_comp *)owner;
				// call member
				mySelf->P4(ownerProcess);
			}
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
