/**
    #define meta ...
    prInt32f("%s\n", meta);
**/
/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "tb_aes_psml.h"

extern Vector<Vector<UInt8> *> __inputs__;
extern UInt8 __input_key__[SIZE];
//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		namespace Samples
		{
			//----------------------------------------------------
			test_aes_comp::test_aes_comp(const String &name)
			{
				set_name(name);

				// _clk_recv is bound as the parent port to clk proxy.
                register_proxy(clk, _clk_send);
                register_proxy(clk, _clk_revc);

				for(register Int32 i = 0 ; i < SIZE ; i++)
				{
					register_proxy(idata[i], _idata[i]);
					register_proxy(ikey[i], _ikey[i]);
					register_proxy(odata[i], _odata[i]);
				}

#ifdef __FiberLess_Enabled__
                p1 = register_process(send, false, "test_aes_comp::send");
#else
                p1 = register_special_process(send, &_send_process_state, "test_aes_comp::send");
#endif
				p1->register_port(_clk_send);
				for(register Int32 i = 0 ; i < SIZE ; i++)
				{
					p1->register_port(_idata[i]);
					p1->register_port(_ikey[i]);
				}
				p1->register_sensitivity(_clk_send, PSML_POS_EDGE);
                p1->set_delay(psml_time(0, PSML_NS));
				///p1->dont_initialize();///

#ifdef __FiberLess_Enabled__
                p2 = register_process(recv, false, "test_aes_comp::recv");
#else
                p2 = register_special_process(recv, &_recv_process_state, "test_aes_comp::recv");
#endif
				p2->register_port(_clk_revc);
				for(register Int32 i = 0 ; i < SIZE ; i++)
					p2->register_port(_odata[i]);
				p2->register_sensitivity(_clk_revc, PSML_POS_EDGE);
                p2->set_delay(psml_time(0, PSML_NS));
				///p2->dont_initialize();///

#ifdef __FiberLess_Enabled__
				next_input = 0;
#endif
			}
			//----------------------------------------------------
			test_aes_comp::~test_aes_comp()
			{
				//reader->close();
				//delete reader;
			}
            //----------------------------------------------------
			void test_aes_comp::send(psml_process *ownerProcess)
			{
                test_aes_comp *my = (test_aes_comp *)ownerProcess->get_owner_component();

#ifdef __FiberLess_Enabled__
				Vector<UInt8> *input = __inputs__[my->next_input];

				for(register Int32 i = 0 ; i < SIZE ; i++)
				{
					my->_idata[i]->put((*input)[i], ownerProcess);
					my->_ikey[i]->put(__input_key__[i], ownerProcess);
				}

				my->next_input++;
				if(my->next_input == __inputs__.Size())
					my->next_input = 0;
#else
                test_aes_comp::send_process_state *state = (test_aes_comp::send_process_state *)ownerProcess->get_process_state();
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
                    state->input = __inputs__[state->next_input];

                    //std::cout << "@time " << psml_time_stamp(ownerProcess);\
                    std::cout << " in send() ";
                    for(state->i = 0 ; state->i < SIZE ; state->i++)
                    {
                        my->_idata[state->i]->put((*state->input)[state->i], ownerProcess);
                        my->_ikey[state->i]->put(__input_key__[state->i], ownerProcess);
                        //std::cout << (UInt32)(*state->input)[state->i];
                    }
                    //std::cout << std::endl;

                    state->next_input++;
                    if(state->next_input == __inputs__.Size())
                        state->next_input = 0;

                    psml_wait(ownerProcess); state->set_label(1); return; L1: {}
                }
#endif

			}
			//----------------------------------------------------
			void test_aes_comp::recv(psml_process *ownerProcess)
			{
                test_aes_comp *my = (test_aes_comp *)ownerProcess->get_owner_component();

#ifdef __FiberLess_Enabled__
				// Variables declaration
				UInt8 out_write[SIZE];
				Int32 i;

				std::cout << "@time " << psml_time_stamp(ownerProcess);\
				std::cout << " in recv() ";
				for(i = 0 ; i < SIZE ; i++)
				{
					out_write[i] = my->_odata[i]->fetch();
					std::cout << (UInt32)out_write[i];
				}
				std::cout << std::endl;
#else
                test_aes_comp::recv_process_state *state = (test_aes_comp::recv_process_state *)ownerProcess->get_process_state();
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
                	//std::cout << "@time " << psml_time_stamp(ownerProcess);\
                    std::cout << " in recv() ";
                    for(state->i = 0 ; state->i < SIZE ; state->i++)
                    {
                        state->out_write[state->i] = my->_odata[state->i]->fetch();
                        //std::cout << (UInt32)state->out_write[state->i];
                    }
                    //std::cout << std::endl;

                    psml_wait(ownerProcess); state->set_label(1); return; L1: {}
                }
#endif
			}
			//----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
