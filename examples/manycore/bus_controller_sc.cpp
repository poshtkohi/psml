/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "bus_controller_sc.h"

//----------------------------------------------------
bus_controller::bus_controller(sc_module_name name_) : sc_module(name_)
{
    SC_THREAD(bus_controller_proc);
    sensitive << clock.neg();
}
//----------------------------------------------------
bus_controller::~bus_controller()
{
}
//----------------------------------------------------
void bus_controller::bus_controller_proc()
{
    wait();

    while(true)
    {
        for(int i = 0 ; i < NumOfCores ; i++)
        {
            if(cs_cores[i].read())
            {
                if(re_cores[i].read() && !we_cores[i].read()) // Reads from memory
                {
                    //wait();
                    address_mem.write(address_cores[i].read());
                    re_mem.write(true); // lw request
                    we_mem.write(false);
                    cs_mem.write(true);
                    //wait(2); // For data to appear for cores=2
                    wait(); // For data to appear
                    // Recives the data from memory
                    data_out_cores[i].write(data_in_mem.read());
                    cs_mem.write(false);
                    wait();
#ifdef DebugMode
                    print_ports(i);
#endif
                }
                else if(!re_cores[i].read() && we_cores[i].read())   // Writes to memory
                {
                    //wait();
                    address_mem.write(address_cores[i].read());
                    re_mem.write(false); // sw request
                    we_mem.write(true);
                    cs_mem.write(true);
                    //wait(2); // For data to appear for cores=2
                    wait(); // For data to appear
                    // Recives the data from memory
                    data_out_cores[i].write(data_in_mem.read());
                    cs_mem.write(false);
                    wait();
#ifdef DebugMode
                    print_ports(i);
#endif
                }
                //std::cout << "@time " << sc_time_stamp() << " bus_controller_proc() cs[" << i << "]=" << cs_cores[i].read() << std::endl;
            }
        }

        wait();
        //std::cout << "@time " << sc_time_stamp() << " bus_controller_proc()" << " clock " << clock.read() << std::endl;
        //std::cout << "@time " << sc_time_stamp() << " bus_controller_proc()" << std::endl;
    }
}
//----------------------------------------------------
void bus_controller::print_ports(int i)
{
    std::cout << "@time " << sc_time_stamp() << " bus_controller_proc() Core " << i << " clock " << clock.read();
    std::cout << " ChipSelect " << cs_cores[i].read() << " ReadEnable " << re_cores[i].read() << " WriteEnable " << we_cores[i].read();
    std::cout << " address " << address_cores[i].read() << " data_in " << data_in_cores[i].read();
    std::cout << " data_out " << data_out_cores[i].read() << std::endl;
}
//----------------------------------------------------
