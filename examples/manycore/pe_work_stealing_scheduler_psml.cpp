/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "pe_work_stealing_scheduler_psml.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		namespace Samples
		{
            //----------------------------------------------------
            pe_work_stealing_scheduler::pe_work_stealing_scheduler(const String &name_, Int32 numOfWorks_) : numOfWorks(numOfWorks_)
            {
                set_name(name_);

                register_proxy(clock, _clock);
                for(register Int32 i = 0 ; i < NumOfCores ; i++)
                {
                    register_proxy(re_cores[i], _re_cores[i]);
                    register_proxy(cs_cores[i], _cs_cores[i]);
                    register_proxy(work_cores[i], _work_cores[i]);
                }

                p = register_special_process(pe_work_stealing_scheduler_proc, &_pe_work_stealing_scheduler_proc_process_state, "pe_work_stealing_scheduler::pe_work_stealing_scheduler_proc");
				p->register_port(_clock);
                for(register Int32 i = 0 ; i < NumOfCores ; i++)
                {
                    p->register_port(_re_cores[i]);
                    p->register_port(_cs_cores[i]);
                    p->register_port(_work_cores[i]);
                }
				p->register_sensitivity(_clock, PSML_NEG_EDGE);
                p->set_delay(psml_time(0, PSML_NS));
            }
            //----------------------------------------------------
            pe_work_stealing_scheduler::~pe_work_stealing_scheduler()
            {
            }
            //----------------------------------------------------
            void pe_work_stealing_scheduler::pe_work_stealing_scheduler_proc(psml_process *ownerProcess)
            {
                pe_work_stealing_scheduler *my = (pe_work_stealing_scheduler *)ownerProcess->get_owner_component();

                pe_work_stealing_scheduler::pe_work_stealing_scheduler_proc_process_state *state = (pe_work_stealing_scheduler::pe_work_stealing_scheduler_proc_process_state *)ownerProcess->get_process_state();
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
                    for(state->i = 0 ; state->i < NumOfCores ; state->i++)
                    {
                        if(my->_cs_cores[state->i]->fetch())
                        {
                            if(my->_re_cores[state->i]->fetch()) // Reads from scheduler
                            {
                                if(state->current_work != my->numOfWorks)
                                {
                                    my->_work_cores[state->i]->put(state->current_work, ownerProcess);
                                    //std::cout << "current_work " << current_work << std::endl;
                                    state->current_work++;
#ifdef DebugMode
                                    my->print_ports(state->i, ownerProcess);
#endif // DebugMode
                                }
                                else
                                {
                                    my->_work_cores[state->i]->put(-1, ownerProcess); // break here for upon completion of all cores
#ifdef DebugMode
                                    my->print_ports(state->i, ownerProcess);
#endif
                                }
                            }
                            //std::cout << "@time " << sc_time_stamp() << " bus_controller_proc() cs[" << i << "]=" << cs_cores[i].read() << std::endl;
                        }
                    }

                    psml_wait(ownerProcess); state->set_label(1); return; L1: {}

                    //psml_wait(ownerProcess); state->set_label(1); return; L1: {}
                    //std::cout << "@time " << psml_time_stamp(ownerProcess) << " pe_work_stealing_scheduler_proc()" << " clock " << my->_clock->fetch() << std::endl;
                }
            }
            //----------------------------------------------------
            void pe_work_stealing_scheduler::print_ports(Int32 i, psml_process *ownerProcess)
            {
                std::cout << "@time " << psml_time_stamp(ownerProcess) << " pe_work_stealing_scheduler_proc() Core " << i << " clock " << _clock->fetch();
                std::cout << " ChipSelect " << _cs_cores[i]->fetch() << " ReadEnable " << _re_cores[i]->fetch();
                std::cout << " Work " << _work_cores[i]->fetch();
                std::cout << std::endl;
            }
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
