/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "pe_matmul_sc.h"

extern Int64 *__mem__;

//----------------------------------------------------
pe_matmul::pe_matmul(sc_module_name name_, int n_) : sc_module(name_), n(n_)
{
    SC_THREAD(pe_matmul_proc);
    sensitive << clock.pos();

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
void pe_matmul::pe_matmul_proc()
{
    sc_uint<64> base_address = 0;
    sc_int<32> work_item = -1;

    wait();

    while(true)
    {
        re_sched.write(true);
        cs_sched.write(true);
        wait();
        work_item = work_sched.read();
        cs_sched.write(false);

        //std::cout << "@time " << sc_time_stamp() << " " << name() << " work_item " << work_item << std::endl;

        if(work_item == -1)
            break;

        base_address = work_item * 3 * n * n;

        // Reads the matrix A from memory
        for(int i = 0 ; i < n * n ; i++, base_address++)
            A[i] = __mem__[base_address];

        // Reads matrix B from memory
        for(int i = 0 ; i < n * n ; i++, base_address++)
            B[i] = __mem__[base_address];

        // Multiplies two matrices A and B
        multiply();
        multiply();

        // Writes the matrix C back to the memory
        for(int i = 0 ; i < n * n ; i++, base_address++)
            __mem__[base_address] = C[i];

        //print_matrix("A", A, n);
        //print_matrix("B", B, n);
        //print_matrix("C", C, n);

        wait();
        //std::cout << "@time " << sc_time_stamp() << " pe_matmul_proc()" << " clock " << clock.read() << std::endl;
    }

    sc_stop();
}
//----------------------------------------------------
void pe_matmul::multiply()
{
    Int64 sum;

    for(int i = 0 ; i < n ; i++) //row of first matrix
    {
        for(int j = 0 ; j < n ; j++) //column of second matrix
        {
            sum = 0;

            for(int k = 0 ; k < n ; k++)
            {
                sum += A[n * i + k] * B[n * k + j];
            }

            C[n * i + j] = sum;
        }
    }
}
//----------------------------------------------------
void pe_matmul::print_matrix(const std::string &name, Int64 *x, int n)
{
    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < n ; j++)
        {
            std::cout << name << "[" << i << "][" << j << "]=" << x[n * i + j];
            std::cout << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;

    /*for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < n ; j++)
        {
            printf("%s[%d][%d]= %llu", name.c_str(), i , j, x[n * i + j]);
        }

        printf("\n");
    }

    printf("\n");*/
}
//----------------------------------------------------
