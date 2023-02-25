/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "tb_aes_cipher_sc.h"

extern std::vector<std::vector<sc_uint<8> > *> __inputs__;
extern sc_uint<8> __input_key__[SIZE];

/*
** Send data thread
*/
void test_aes_cipher::send()
{
    // Variables declaration
    int next_input = 0, i = 0;
	wait();
	//wait(1, SC_NS);

	while(true)
	{
        std::vector<sc_uint<8> > *input = __inputs__[next_input];

        //std::cout << "@time " << sc_time_stamp();\
        std::cout << " in send() ";
        for(i = 0 ; i < SIZE ; i++)
        {
            idata[i].write((*input)[i]);
            ikey[i].write(__input_key__[i]);
            //std::cout << (unsigned int)(*input)[i];
        }
        //std::cout << std::endl;

        next_input++;
        if(next_input == __inputs__.size())
            next_input = 0;

		wait();
	}
}



/*
** Receive data thread
*/
void test_aes_cipher::recv()
{
    // Variables declaration
    int i;
    sc_uint<8> out_write[SIZE];

	wait();

	while(true)
	{
        std::cout << "@time " << sc_time_stamp();\
        std::cout << " in recv() ";
        for(i=0 ; i < SIZE ; i++)
        {
            out_write[i] = odata[i].read();
            std::cout << (unsigned int)out_write[i];
        }
        std::cout << std::endl;

		wait();
	}
}
