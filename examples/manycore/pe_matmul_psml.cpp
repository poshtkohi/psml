/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "pe_matmul_psml.h"

extern Int64 *__mem__;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		namespace Samples
		{
            //----------------------------------------------------
            pe_matmul::pe_matmul(const String &name_, Int32 n_) : n(n_)
            {
                set_name(name_);

                register_proxy(clock, _clock);
                //register_proxy(data_in, _data_in);
                register_proxy(work_sched, _work_sched);
                /*register_proxy(re_bus_ctrl, _re_bus_ctrl);
                register_proxy(we_bus_ctrl, _we_bus_ctrl);
                register_proxy(cs_bus_ctrl, _cs_bus_ctrl);*/
                register_proxy(re_sched, _re_sched);
                register_proxy(cs_sched, _cs_sched);
                /*register_proxy(address, _address);
                register_proxy(data_out, _data_out);*/

                p = register_special_process(pe_matmul_proc, &_pe_matmul_proc_process_state, "pe_matmul::pe_matmul_proc");
				p->register_port(_clock);
                //p->register_port(_data_in);
                p->register_port(_work_sched);
                /*p->register_port(_re_bus_ctrl);
                p->register_port(_we_bus_ctrl);
                p->register_port( _cs_bus_ctrl);*/
                p->register_port(_re_sched);
                p->register_port(_cs_sched);
                /*p->register_port(_address);
                p->register_port(_data_out);*/
				p->register_sensitivity(_clock, PSML_POS_EDGE);
                p->set_delay(psml_time(0, PSML_NS));

                local_memory = new Int64[3 * n * n]; // The memory to store three matrices A, B and C
                memset(local_memory, 0, sizeof(Int64) * 3 * n * n);

                A = local_memory;
                B = A + n * n;
                C = B + n * n;
            }
            //----------------------------------------------------
            pe_matmul::~pe_matmul()
            {
                delete local_memory;
            }
            //----------------------------------------------------
            void pe_matmul::pe_matmul_proc(psml_process *ownerProcess)
            {
                pe_matmul *my = (pe_matmul *)ownerProcess->get_owner_component();

                pe_matmul::pe_matmul_proc_process_state *state = (pe_matmul::pe_matmul_proc_process_state *)ownerProcess->get_process_state();
                switch(state->get_label())
                {
                    case 0:
                        goto L0;
                        break;
                    /*case 1:
                        goto L1;
                        break;*/
                    case 2:
                        goto L2;
                        break;
                    /*case 3:
                        goto L3;
                        break;
                    case 4:
                        goto L4;
                        break;
                    case 5:
                        goto L5;
                        break;
                    case 6:
                        goto L6;
                        break;
                    case 7:
                        goto L7;
                        break;
                    case 8:
                        goto L8;
                        break;
                    case 9:
                        goto L9;
                        break;
                    case 10:
                        goto L10;
                        break;
                    case 11:
                        goto L11;
                        break;
                    case 12:
                        goto L12;
                        break;*/
                    case 13:
                        goto L13;
                        break;
                }

                psml_wait(ownerProcess); state->set_label(0); return; L0: {}

                while(true)
                {
                    // Select Work Item from the Scheduler
                    //L1://psml_wait(ownerProcess); state->set_label(1); return; L1: {}
                    ///std::cout << "@time " << sc_time_stamp() << " pe_matmul_proc() - work_request_phase" << std::endl;
                    my->_re_sched->put(true, ownerProcess);
                    my->_cs_sched->put(true, ownerProcess);
                    //psml_wait(NumOfCores, ownerProcess); state->set_label(2); return; L2: {} // For data to appear
                    psml_wait(ownerProcess); state->set_label(2); return; L2: {} // For data to appear
                    state->work_item = my->_work_sched->fetch();
                    my->_cs_sched->put(false, ownerProcess);

                    /*for(Int32 i = 0 ; i < 10000  ; i++)
                    {
                      j += i;
                      int x = i & j;
                    }*/
                    //std::cout << "1 @time " << psml_time_stamp(ownerProcess)<< std::endl;// << " work_item " << state->work_item << std::endl;
                    //L3://psml_wait(ownerProcess); state->set_label(3); return; L3: {}
                    //std::cout << "@time " << psml_time_stamp(ownerProcess) << " " << my->GetName().get_BaseStream() << " work_item " << state->work_item << std::endl;
                    if(state->work_item == -1)
                        break;

                    state->base_address = state->work_item * 3 * my->n * my->n;

                    // Reads the matrix A from memory
                    for(state->i = 0 ; state->i < my->n * my->n ; state->i++, state->base_address++)
                        my->A[state->i] = __mem__[state->base_address];

                    // Reads matrix B from memory
                    for(state->i = 0 ; state->i < my->n * my->n ; state->i++, state->base_address++)
                        my->B[state->i] = __mem__[state->base_address];

                    // Multiplies two matrices A and B
                    my->multiply();
                    my->multiply();

                    // Writes the matrix C back to the memory
                    for(state->i = 0 ; state->i < my->n * my->n ; state->i++, state->base_address++)
                        __mem__[state->base_address] = my->C[state->i];

                    //my->print_matrix("A", my->A, my->n);
                    //my->print_matrix("B", my->B, my->n);
                    //my->print_matrix("C", my->C, my->n);

                    //psml_wait(psml_time(2, PSML_NS), ownerProcess); state->set_label(13); return; L13: {}
                    psml_wait(ownerProcess); state->set_label(13); return; L13: {}
                    //std::cout << "@time " << psml_time_stamp(ownerProcess) << " pe_matmul_proc()" << " clock " << my->_clock->fetch() << std::endl;
                }

                psml_stop(ownerProcess);
            }
            //----------------------------------------------------
            void pe_matmul::multiply()
            {
                Int64 sum;

                for(register Int32 i = 0 ; i < n ; i++) //row of first matrix
                {
                    for(register Int32 j = 0 ; j < n ; j++) //column of second matrix
                    {
                        sum = 0;

                        for(register Int32 k = 0 ; k < n ; k++)
                        {
                            sum += A[n * i + k] * B[n * k + j];
                        }

                        C[n * i + j] = sum;
                    }
                }
            }
            //----------------------------------------------------
            void pe_matmul::print_matrix(const std::string &name, Int64 *x, Int32 n)
            {
                for(register Int32 i = 0 ; i < n ; i++)
                {
                    for(register Int32 j = 0 ; j < n ; j++)
                    {
                        std::cout << name << "[" << i << "][" << j << "]=" << x[n * i + j];
                        std::cout << " ";
                    }

                    std::cout << std::endl;
                }

                std::cout << std::endl;

                /*for(Int32 i = 0 ; i < n ; i++)
                {
                    for(Int32 j = 0 ; j < n ; j++)
                    {
                        printf("%s[%d][%d]= %llu", name.c_str(), i , j, x[n * i + j]);
                    }

                    printf("\n");
                }

                printf("\n");*/
            }
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
