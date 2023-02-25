/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "memory_sc.h"

//----------------------------------------------------
memory::memory(sc_module_name name_, int size_) : sc_module(name_), size(size_)
{
    SC_THREAD(memory_proc);
    sensitive << clock.pos();

    array = new Int64[size];

    for(int i = 0 ; i < size ; i++)
        array[i] = i;

    //print_memory();
}
//----------------------------------------------------
memory::~memory()
{
    delete array;
}
//----------------------------------------------------
void memory::memory_proc()
{
    wait();

    while(true)
    {
        if(cs.read())
        {
            if(re.read() && !we.read()) // Reads from memory
            {
                data_out.write(array[address.read()]);
#ifdef DebugMode
                print_ports();
#endif
            }
            else if(!re.read() && we.read())   // Writes to memory
            {
                array[address.read()] = data_in.read();
#ifdef DebugMode
                print_ports();
#endif
            }
        }

        wait();
        //std::cout << "@time " << sc_time_stamp() << " memory_proc()" << " clock " << clock.read() << std::endl;
    }
}
//----------------------------------------------------
void memory::print_memory()
{
    for(int i = 0 ; i < size ; i++)
        std::cout << "mem[" << i << "]=" << array[i] << std::endl;
}
//----------------------------------------------------
void memory::print_ports()
{
    std::cout << "@time " << sc_time_stamp() << " memory_proc()" << " clock " << clock.read();
    std::cout << " ChipSelect " << cs.read() << " ReadEnable " << re.read() << " WriteEnable " << we.read();
    std::cout << " address " << address.read() << " data_in " << data_in.read();
    std::cout << " data_out " << array[address.read()] << std::endl;
}
//----------------------------------------------------
