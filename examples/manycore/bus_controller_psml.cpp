/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "bus_controller_psml.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		namespace Samples
		{
            //----------------------------------------------------
            bus_controller::bus_controller(const String &name_)
            {
                set_name(name_);

                register_proxy(clock, _clock);
                for(register Int32 i = 0 ; i < NumOfCores ; i++)
                {
                    register_proxy(re_cores[i], _re_cores[i]);
                    register_proxy(we_cores[i], _we_cores[i]);
                    register_proxy(cs_cores[i], _cs_cores[i]);
                    register_proxy(address_cores[i], _address_cores[i]);
                    register_proxy(data_in_cores[i], _data_in_cores[i]);
                    register_proxy(data_out_cores[i], _data_out_cores[i]);
                }
                register_proxy(data_in_mem, _data_in_mem);
                register_proxy(re_mem, _re_mem);
                register_proxy(we_mem, _we_mem);
                register_proxy(cs_mem, _cs_mem);
                register_proxy(address_mem, _address_mem);
                register_proxy(data_out_mem, _data_out_mem);

                p = register_special_process(bus_controller_proc, &_bus_controller_proc_process_state, "bus_controller::bus_controller_proc");
				p->register_port(_clock);
                for(register Int32 i = 0 ; i < NumOfCores ; i++)
                {
                    p->register_port(_re_cores[i]);
                    p->register_port(_we_cores[i]);
                    p->register_port(_cs_cores[i]);
                    p->register_port(_address_cores[i]);
                    p->register_port(_data_in_cores[i]);
                    p->register_port(_data_out_cores[i]);
                }
                p->register_port(_data_in_mem);
                p->register_port(_re_mem);
                p->register_port(_we_mem);
                p->register_port(_cs_mem);
                p->register_port(_address_mem);
                p->register_port(_data_out_mem);
				p->register_sensitivity_clock(_clock, PSML_NEG_EDGE);
                p->set_delay(psml_time(0, PSML_NS));
            }
            //----------------------------------------------------
            bus_controller::~bus_controller()
            {
            }
            //----------------------------------------------------
            void bus_controller::bus_controller_proc(psml_process *ownerProcess)
            {
                bus_controller *my = (bus_controller *)ownerProcess->get_owner_component();

                bus_controller::bus_controller_proc_process_state *state = (bus_controller::bus_controller_proc_process_state *)ownerProcess->get_process_state();
                switch(state->get_label())
                {
                    case 0:
                        goto L0;
                        break;
                    case 1:
                        goto L1;
                        break;
                    case 2:
                        goto L2;
                        break;
                    case 3:
                        goto L3;
                        break;
                    case 4:
                        goto L4;
                        break;
                    case 5:
                        goto L5;
                        break;
                }

                psml_wait(ownerProcess); state->set_label(0); return; L0: {}

                while(true)
                {
                    for(state->i = 0 ; state->i < NumOfCores ; state->i++)
                    {
                        if(my->_cs_cores[state->i]->fetch())
                        {
                            if(my->_re_cores[state->i]->fetch() && !my->_we_cores[state->i]->fetch()) // Reads from memory
                            {
                                //wait();
                                my->_address_mem->put(my->_address_cores[state->i]->fetch(), ownerProcess);
                                my->_re_mem->put(true, ownerProcess); // lw request
                                my->_we_mem->put(false, ownerProcess);
                                my->_cs_mem->put(true, ownerProcess);
                                //wait(2); // For data to appear for cores=2
                                psml_wait(ownerProcess); state->set_label(1); return; L1: {} // For data to appear
                                // Recives the data from memory
                                my->_data_out_cores[state->i]->put(my->_data_in_mem->fetch(), ownerProcess);
                                my->_cs_mem->put(false, ownerProcess);
                                psml_wait(ownerProcess); state->set_label(2); return; L2: {}
#ifdef DebugMode
                                my->print_ports(state->i, ownerProcess);
#endif
                            }
                            else if(!my->_re_cores[state->i]->fetch() && my->_we_cores[state->i]->fetch())   // Writes to memory
                            {
                                //wait();
                                my->_address_mem->put(my->_address_cores[state->i]->fetch(), ownerProcess);
                                my->_re_mem->put(false, ownerProcess); // sw request
                                my->_we_mem->put(true, ownerProcess);
                                my->_cs_mem->put(true, ownerProcess);
                                //wait(2); // For data to appear for cores=2
                                psml_wait(ownerProcess); state->set_label(3); return; L3: {} // For data to appear
                                // Recives the data from memory
                                my->_data_out_cores[state->i]->put(my->_data_in_mem->fetch(), ownerProcess);
                                my->_cs_mem->put(false, ownerProcess);
                                psml_wait(ownerProcess); state->set_label(4); return; L4: {}
#ifdef DebugMode
                                my->print_ports(state->i, ownerProcess);
#endif
                            }
                            //std::cout << "@time " << sc_time_stamp() << " bus_controller_proc() cs[" << i << "]=" << cs_cores[i]->fetch() << std::endl;
                        }
                    }

                    psml_wait(ownerProcess); state->set_label(5); return; L5: {}

                    //psml_wait(ownerProcess); state->set_label(1); return; L1: {}
                    //std::cout << "@time " << psml_time_stamp(ownerProcess) << " bus_controller_proc()" << " clock " << my->_clock->fetch() << std::endl;
                }
            }
            //----------------------------------------------------
            void bus_controller::print_ports(Int32 i, psml_process *ownerProcess)
            {
                std::cout << "@time " << psml_time_stamp(ownerProcess) << " bus_controller_proc() Core " << i << " clock " << _clock->fetch();
                std::cout << " ChipSelect " << _cs_cores[i]->fetch() << " ReadEnable " << _re_cores[i]->fetch() << " WriteEnable " << _we_cores[i]->fetch();
                std::cout << " address " << _address_cores[i]->fetch() << " data_in " << _data_in_cores[i]->fetch();
                std::cout << " data_out " << _data_out_cores[i]->fetch() << std::endl;
            }
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
