/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "memory_psml.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		namespace Samples
		{
            //----------------------------------------------------
            memory::memory(const String &name_, Int32 size_) : size(size_)
            {
                set_name(name_);

                register_proxy(clock, _clock);
                register_proxy(re, _re);
                register_proxy(we, _we);
                register_proxy(cs, _cs);
                register_proxy(address, _address);
                register_proxy(data_in, _data_in);
                register_proxy(data_out, _data_out);

                p = register_special_process(memory_proc, &_memory_proc_process_state, "memory::memory_proc");
				p->register_port(_clock);
				p->register_port(_re);
				p->register_port(_we);
				p->register_port(_cs);
				p->register_port(_address);
				p->register_port(_data_in);
				p->register_port(_data_out);
				p->register_sensitivity_clock(_clock, PSML_POS_EDGE);
                p->set_delay(psml_time(0, PSML_NS));

                size = 1024 * 1024;

                array = new Int64[size];

                for(register Int32 i = 0 ; i < size ; i++)
                    array[i] = i;

                //print_memory();
            }
            //----------------------------------------------------
            memory::~memory()
            {
                delete array;
            }
            //----------------------------------------------------
            void memory::memory_proc(psml_process *ownerProcess)
            {
                memory *my = (memory *)ownerProcess->get_owner_component();

                memory::memory_proc_process_state *state = (memory::memory_proc_process_state *)ownerProcess->get_process_state();
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
                    if(my->_cs->fetch())
                    {
                        if(my->_re->fetch() && !my->_we->fetch()) // Reads from memory
                        {
                            my->_data_out->put(my->array[my->_address->fetch()], ownerProcess);
#ifdef DebugMode
                            my->print_ports(ownerProcess);
#endif
                        }
                        else if(!my->_re->fetch() && my->_we->fetch())   // Writes to memory
                        {
                            my->array[my->_address->fetch()] = my->_data_in->fetch();
#ifdef DebugMode
                            my->print_ports(ownerProcess);
#endif
                        }
                    }

                    psml_wait(ownerProcess); state->set_label(1); return; L1: {}
                    //std::cout << "@time " << psml_time_stamp(ownerProcess) << " memory_proc()" << " clock " << my->_clock->fetch() << std::endl;
                    ///std::cout << "@time " << sc_time_stamp() << " memory_proc()" << std::endl;
                }
            }
            //----------------------------------------------------
            void memory::print_memory()
            {
                for(register Int32 i = 0 ; i < 100 ; i++)
                    std::cout << "mem[" << i << "]=" << array[i] << std::endl;
            }
            //----------------------------------------------------
            void memory::print_ports(psml_process *ownerProcess)
            {
                std::cout << "@time " << psml_time_stamp(ownerProcess) << " memory_proc()" << " clock " << _clock->fetch();
                std::cout << " ChipSelect " << _cs->fetch() << " ReadEnable " << _re->fetch() << " WriteEnable " << _we->fetch();
                std::cout << " address " << _address->fetch() << " data_in " << _data_in->fetch();
                std::cout << " data_out " << array[_address->fetch()] << std::endl;
            }
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
