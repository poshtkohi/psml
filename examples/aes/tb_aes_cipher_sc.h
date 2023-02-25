/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef TB_AES_CIPHER_H
#define TB_AES_CIPHER_H

#include "define_sc.h"

SC_MODULE (test_aes_cipher){

  // inputs
  sc_in<bool>          clk;
  sc_in<sc_uint<8> >  odata[SIZE];

  // outputs
  sc_out<sc_uint<8> >   idata[SIZE];
  sc_out<sc_uint<8> >   ikey[SIZE];

  sc_uint<8>  input_key[SIZE];



  /* C */
  void compare_results();

  /* R */
  void recv();

  /* S */
  void send();



  SC_CTOR ( test_aes_cipher ) {

    SC_THREAD(send);
    sensitive << clk.pos();

    SC_THREAD(recv);
    sensitive << clk.pos();
  }

  ~test_aes_cipher(){}

};


#endif // TB_AES_CIPHER_H
