/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "tb.h"

//---------------------------------------
extern std::vector<sc_uint<64> > __patterns__;
extern sc_uint<64> __input_key__;
//---------------------------------------
// This module is used to drive the model for encryption.
void tb::send()
{
    // Variables declaration
    int next_input = 0;
	sc_uint<64> input;
	sc_uint<64> result;

    rt_reset.write(0);
    //wait(clk.posedge_event());
    wait();
    rt_reset.write(1);
    //cout << "Reseted" << endl;

	while(true)
	{
        //wait(clk.posedge_event());
        wait();

        input = __patterns__[next_input];

        //std::cout << "@time " <<  sc_time_stamp() << " in send() input " << input << std::endl;// << " key " << __input_key__<< std::endl;

        rt_load_o.write(1);
        rt_des_data_o.write(input);
        rt_des_key_o.write(__input_key__);
        rt_decrypt_o.write(0);

        // We must implement such this wait calls for PSML in future.
        //wait(clk.posedge_event());
        wait();
        rt_load_o.write(0);

        //wait(rt_des_ready_i.posedge_event());
        wait(16); // 16 + 1

        //result = rt_des_data_i.read();

        //std::cout << "@time " <<  sc_time_stamp() << " in recv() " << result << std::endl;

        next_input++;
        if(next_input == __patterns__.size())
            next_input = 0;
	}
}
//---------------------------------------
void tb::recv()
{
    // Variables declaration
    sc_uint<64> result;

	while(true)
	{
        //wait(rt_des_ready_i.posedge_event());
        wait();

        result = rt_des_data_i.read();

        std::cout << "@time " <<  sc_time_stamp() << " in recv() " << result << std::endl;
	}
}
//---------------------------------------
