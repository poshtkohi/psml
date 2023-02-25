/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "pe_work_stealing_scheduler_sc.h"

//----------------------------------------------------
pe_work_stealing_scheduler::pe_work_stealing_scheduler(sc_module_name name_, int numOfWorks_) : sc_module(name_), numOfWorks(numOfWorks_)
{
    SC_THREAD(pe_work_stealing_scheduler_proc);
    sensitive << clock.neg();
}
//----------------------------------------------------
pe_work_stealing_scheduler::~pe_work_stealing_scheduler()
{
}
//----------------------------------------------------
void pe_work_stealing_scheduler::pe_work_stealing_scheduler_proc()
{
    sc_int<32> current_work = 0;

    wait(); ///

    while(true)
    {
        for(int i = 0 ; i < NumOfCores ; i++)
        {
            if(cs_cores[i].read())
            {
                if(re_cores[i].read()) // Reads from scheduler
                {
                    if(current_work != numOfWorks)
                    {
                        work_cores[i].write(current_work);
                        //std::cout << "current_work " << current_work << std::endl;
                        current_work++;
#ifdef DebugMode
                        print_ports(i);
#endif
                    }
                    else
                    {
                        work_cores[i].write(-1); // break here for upon completion of all cores
#ifdef DebugMode
                        print_ports(i);
#endif
                    }
                }
                //std::cout << "@time " << sc_time_stamp() << " bus_controller_proc() cs[" << i << "]=" << cs_cores[i].read() << std::endl;
            }
        }

        wait();
        //std::cout << "@time " << sc_time_stamp() << " pe_work_stealing_scheduler_proc()" << " clock " << clock.read() << std::endl;
    }
}
//----------------------------------------------------
void pe_work_stealing_scheduler::print_ports(int i)
{
    std::cout << "@time " << sc_time_stamp() << " pe_work_stealing_scheduler_proc() Core " << i << " clock " << clock.read();
    std::cout << " ChipSelect " << cs_cores[i].read() << " ReadEnable " << re_cores[i].read();
    std::cout << " Work " << work_cores[i].read();
    std::cout << std::endl;
}
//----------------------------------------------------
