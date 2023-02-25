/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
All rights reserved to Alireza Poshtkohi (c) 1999-2023.
Email: arp@poshtkohi.info
Website: http://www.poshtkohi.info
*/

#include "tb_qsort_psml.h"

extern Vector<UInt32 > __inputs__;
//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		namespace Samples
		{
			//----------------------------------------------------
			test_qsort_comp::test_qsort_comp(const String &name)
			{
				set_name(name);

				register_proxy(clk, _clk_send);
                register_proxy(clk, _clk_revc);
				register_proxy(odata, _odata);
				register_proxy(indata, _indata);

				//p1 = register_process(Wrapper_To_Call_send, false, "send");
#ifdef __FiberLess_Enabled__
                p1 = register_process(send, false, "test_qsort_comp::send");
#else
                p1 = register_special_process(send, &_send_process_state, "test_qsort_comp::send");
#endif
				p1->register_port(_clk_send);
				p1->register_port(_indata);
				p1->register_sensitivity(_clk_send, PSML_POS_EDGE);
				p1->set_delay(psml_time(0, PSML_NS));
				//p1->dont_initialize(); ///

                //p2 = register_process(Wrapper_To_Call_recv, false, "recv");
#ifdef __FiberLess_Enabled__
                p2 = register_process(recv, false, "test_qsort_comp::recv");
#else
                p2 = register_special_process(recv, &_recv_process_state, "test_qsort_comp::recv");
#endif
				p2->register_port(_clk_revc);
				p2->register_port(_odata);
				p2->register_sensitivity(_clk_revc, PSML_POS_EDGE);
                //p2->dont_initialize(); ///

				//reader = new ifstream(INFILENAME, ios::in);

				//if(!reader->is_open())
				//	throw IOException("Could not open the file INFILENAME");
#ifdef __FiberLess_Enabled__
				next_input = 0;
#endif

			}
			//----------------------------------------------------
			test_qsort_comp::~test_qsort_comp()
			{
				//reader->close();
				//delete reader;
			}
            //----------------------------------------------------
			void test_qsort_comp::send(psml_process *ownerProcess)
			{
				test_qsort_comp *my = (test_qsort_comp *)ownerProcess->get_owner_component();

#ifdef __FiberLess_Enabled__
				Int32 in_read;

				//my->_indata->put(0, ownerProcess);

				in_read = __inputs__[my->next_input];
				my->_indata->put(in_read, ownerProcess);
				//std::cout << "@time " << psml_time_stamp(ownerProcess);\
                    std::cout << " in send() " << in_read << std::endl;

				my->next_input++;
				if(my->next_input == __inputs__.Size())
					my->next_input = 0;
#else
                test_qsort_comp::send_process_state *state = (test_qsort_comp::send_process_state *)ownerProcess->get_process_state();
                switch(state->get_label())
                {
                    case 0:
                        goto L0;
                        break;
                    case 1:
                        goto L1;
                        break;
                }

                psml_wait(ownerProcess); state->set_label(0); return; L0: {}

                while(true)
                {
                    state->in_read = __inputs__[state->next_input];
                    my->_indata->put(state->in_read, ownerProcess);

                    //std::cout << "@time " << psml_time_stamp(ownerProcess);\
                    std::cout << " in send() " << state->in_read << std::endl;

                    state->next_input++;
                    if(state->next_input == __inputs__.Size())
                        state->next_input = 0;

                    psml_wait(ownerProcess); state->set_label(1); return; L1: {}
                }
#endif
			}
			//----------------------------------------------------
			void test_qsort_comp::recv(psml_process *ownerProcess)
			{
                test_qsort_comp *my = (test_qsort_comp *)ownerProcess->get_owner_component();

#ifdef __FiberLess_Enabled__
                // Variables declaration
				Int32 out_write;
				out_write = my->_odata->fetch();
				//std::cout << "@time " << psml_time_stamp(ownerProcess);\
				std::cout << " in recv() " << out_write << std::endl;
#else
                test_qsort_comp::recv_process_state *state = (test_qsort_comp::recv_process_state *)ownerProcess->get_process_state();
                switch(state->get_label())
                {
                    case 0:
                        goto L0;
                        break;
                    case 1:
                        goto L1;
                        break;
                }

                psml_wait(ownerProcess); state->set_label(0); return; L0: {}

                while(true)
                {
                    state->out_write = my->_odata->fetch();

                    std::cout << "@time " << psml_time_stamp(ownerProcess);\
                    std::cout << " in recv() " << state->out_write << std::endl;

                    psml_wait(ownerProcess); state->set_label(1); return; L1: {}
                }
#endif
			}
			//----------------------------------------------------
			/*void test_qsort_comp::recv(psml_process *ownerProcess)
			{
				// Variables declaration
				Int32 out_write;
				out_write = _odata->fetch();
				//std::cout << "@time " << psml_time_stamp(ownerProcess);\
				std::cout << " in recv() " << out_write << endl;
			}
			//----------------------------------------------------
			void test_qsort_comp::send(psml_process *ownerProcess)
			{
				psml_time wire_delay(0, PSML_NS);

				Int32 in_read;

				_indata->put(0, wire_delay, ownerProcess);

				in_read = __inputs__[next_input];
				_indata->put(in_read, wire_delay, ownerProcess);

				next_input++;
				if(next_input == __inputs__.size())
					next_input = 0;

			}
			//----------------------------------------------------
			void test_qsort_comp::Wrapper_To_Call_recv(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a poInt32er to test_qsort_comp
				test_qsort_comp *mySelf = (test_qsort_comp *)owner;
				// call member
				mySelf->recv(ownerProcess);
			}
			//----------------------------------------------------
			void test_qsort_comp::Wrapper_To_Call_send(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a poInt32er to test_qsort_comp
				test_qsort_comp *mySelf = (test_qsort_comp *)owner;
				// call member
				mySelf->send(ownerProcess);
			}*/
			//----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
