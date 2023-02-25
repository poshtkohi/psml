/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef AES_CIPHER_H
#define AES_CIPHER_H

#include "define_sc.h"



SC_MODULE (aes_cipher) {
  // input
  sc_in<bool>          clk;

  sc_in<sc_uint<8> >   idata[SIZE];
  sc_in<sc_uint<8> >   ikey[SIZE];

  sc_out<sc_uint<8> >  odata[SIZE];


  int w[60];
  int data[NB];

  sc_uint<8> keys[SIZE];
  sc_uint<8> init[SIZE];


  /* A */
  void AddRoundKey(int *,int);

  /* C */
  int Cipher(int *, const int*);

  /* D */
  int dataget(int *,int);

  /* K */
  void KeyExpansion(sc_uint<8> *, const int*);

  /* M */
  void MixColumns(int *);
  int mul(int,int);

  /* S */
  void ShiftRows(int *);
  void SubBytes(int *, const int*);
  int SubWord(int, const int*);


  /* R */
  int RotWord(int);
  void run();

  SC_CTOR (aes_cipher)
  {
     SC_THREAD(run);
     sensitive << clk.pos();
  }

};

#endif // AES_CIPHER_H
