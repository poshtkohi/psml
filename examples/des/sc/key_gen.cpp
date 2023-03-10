/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "key_gen.h"

void key_gen::generate_key()
{
  bool prev0, prev1;


  sc_uint<56 > prev_key_var, non_perm_key_var;
  sc_uint<48 > new_key_var;
  sc_uint<28 > semi_key;

  prev_key_var = previous_key.read();
  new_key_var = 0;
  new_key.write(0);
  non_perm_key_var = 0;
  non_perm_key.write(0);

  if(!decrypt.read())
    {
      //Rotate left
      switch(iteration.read())
	{
	case 0:
	case 1:
	case 8:
	case 15:
	  semi_key = prev_key_var.range(55, 28);
	  prev0 = semi_key[27];
	  semi_key = semi_key << 1;
	  semi_key[0] = prev0;
	  non_perm_key_var.range(55, 28) = semi_key;
	  semi_key = prev_key_var.range(27, 0);
	  prev0 = semi_key[27];
	  semi_key = semi_key << 1;
	  semi_key[0] = prev0;
	  non_perm_key_var.range(27, 0) = semi_key;
	  break;
	default:
	  semi_key = prev_key_var.range(55, 28);
	  prev0 = semi_key[27];
	  prev1 = semi_key[26];
	  semi_key = semi_key << 2;
	  semi_key[1] = prev0;
	  semi_key[0] = prev1;
	  non_perm_key_var.range(55, 28) = semi_key;
	  semi_key = prev_key_var.range(27, 0);
	  prev0 = semi_key[27];
	  prev1 = semi_key[26];
	  semi_key = semi_key << 2;
	  semi_key[1] = prev0;
	  semi_key[0] = prev1;
	  non_perm_key_var.range(27, 0) = semi_key;
	}
    }
  else
    {
      //Rotate rigth
      switch(iteration.read())
	{
	case 0:
	  semi_key = prev_key_var.range(55, 28);
	  non_perm_key_var.range(55, 28) = semi_key;
	  semi_key = prev_key_var.range(27, 0);
	  non_perm_key_var.range(27, 0) = semi_key;
	  break;
	case 1:
	case 8:
	case 15:
	  semi_key = prev_key_var.range(55, 28);
	  prev0 = semi_key[0];
	  semi_key = semi_key >> 1;
	  semi_key[27] = prev0;
	  non_perm_key_var.range(55, 28) = semi_key;
	  semi_key = prev_key_var.range(27, 0);
	  prev0 = semi_key[0];
	  semi_key = semi_key >> 1;
	  semi_key[27] = prev0;
	  non_perm_key_var.range(27, 0) = semi_key;
	  break;
	default:
	  semi_key = prev_key_var.range(55, 28);
	  prev0 = semi_key[0];
	  prev1 = semi_key[1];
	  semi_key = semi_key >> 2;
	  semi_key[26] = prev0;
	  semi_key[27] = prev1;
	  non_perm_key_var.range(55, 28) = semi_key;
	  semi_key = prev_key_var.range(27, 0);
	  prev0 = semi_key[0];
	  prev1 = semi_key[1];
	  semi_key = semi_key >> 2;
	  semi_key[26] = prev0;
	  semi_key[27] = prev1;
	  non_perm_key_var.range(27, 0) = semi_key;
	}
    }

  non_perm_key.write(non_perm_key_var);

  //Apply PC_2 permutation
  //Expand the data
  new_key_var[47] = non_perm_key_var[42];
  new_key_var[46] = non_perm_key_var[39];
  new_key_var[45] = non_perm_key_var[45];
  new_key_var[44] = non_perm_key_var[32];
  new_key_var[43] = non_perm_key_var[55];
  new_key_var[42] = non_perm_key_var[51];
  new_key_var[41] = non_perm_key_var[53];
  new_key_var[40] = non_perm_key_var[28];

  new_key_var[39] = non_perm_key_var[41];
  new_key_var[38] = non_perm_key_var[50];
  new_key_var[37] = non_perm_key_var[35];
  new_key_var[36] = non_perm_key_var[46];
  new_key_var[35] = non_perm_key_var[33];
  new_key_var[34] = non_perm_key_var[37];
  new_key_var[33] = non_perm_key_var[44];
  new_key_var[32] = non_perm_key_var[52];

  new_key_var[31] = non_perm_key_var[30];
  new_key_var[30] = non_perm_key_var[48];
  new_key_var[29] = non_perm_key_var[40];
  new_key_var[28] = non_perm_key_var[49];
  new_key_var[27] = non_perm_key_var[29];
  new_key_var[26] = non_perm_key_var[36];
  new_key_var[25] = non_perm_key_var[43];
  new_key_var[24] = non_perm_key_var[54];

  new_key_var[23] = non_perm_key_var[15];
  new_key_var[22] = non_perm_key_var[4];
  new_key_var[21] = non_perm_key_var[25];
  new_key_var[20] = non_perm_key_var[19];
  new_key_var[19] = non_perm_key_var[9];
  new_key_var[18] = non_perm_key_var[1];
  new_key_var[17] = non_perm_key_var[26];
  new_key_var[16] = non_perm_key_var[16];

  new_key_var[15] = non_perm_key_var[5];
  new_key_var[14] = non_perm_key_var[11];
  new_key_var[13] = non_perm_key_var[23];
  new_key_var[12] = non_perm_key_var[8];
  new_key_var[11] = non_perm_key_var[12];
  new_key_var[10] = non_perm_key_var[7];
  new_key_var[9] = non_perm_key_var[17];
  new_key_var[8] = non_perm_key_var[0];

  new_key_var[7] = non_perm_key_var[22];
  new_key_var[6] = non_perm_key_var[3];
  new_key_var[5] = non_perm_key_var[10];
  new_key_var[4] = non_perm_key_var[14];
  new_key_var[3] = non_perm_key_var[6];
  new_key_var[2] = non_perm_key_var[20];
  new_key_var[1] = non_perm_key_var[27];
  new_key_var[0] = non_perm_key_var[24];

  new_key.write(new_key_var);
}
