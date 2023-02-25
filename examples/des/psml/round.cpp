

#include "round.h"


void desround::registers(psml_process *owner)
{
	desround *my = (desround *)owner->get_owner_component();


    if(!my->reset_registers->fetch())
    {
        //std::cout << "@time " <<  psml_time_stamp(owner) << " desround::registers 1" << std::endl;

        my->L_o_registers->put(0, owner);
        my->R_o_registers->put(0, owner);
        my->Key_o_registers->put(0, owner);
    }
    else
    {
        //std::cout << "@time " <<  psml_time_stamp(owner) << " desround::registers"\
                  << " R_i " << my->R_i_registers->fetch()\
                  << " next_R " << my->next_R_registers->fetch()\
                  << std::endl;

        my->L_o_registers->put(my->R_i_registers->fetch(), owner);
        my->R_o_registers->put(my->next_R_registers->fetch(), owner);
        my->Key_o_registers->put(my->non_perm_key_registers->fetch(), owner);
    }
}



void desround::round_proc(psml_process *owner)
{
	desround *my = (desround *)owner->get_owner_component();


  psml_uint<48 > expandedR;
  psml_uint<48 > round_key;
  psml_uint<48 > KER;
  psml_uint<32 > R_i_var;

  psml_uint<32 > Soutput;
  psml_uint<32 > f;

  R_i_var = my->R_i_round_proc->fetch();


  expandedR[47] = R_i_var[0];
  expandedR[46] = R_i_var[31];
  expandedR[45] = R_i_var[30];
  expandedR[44] = R_i_var[29];
  expandedR[43] = R_i_var[28];
  expandedR[42] = R_i_var[27];
  expandedR[41] = R_i_var[28];
  expandedR[40] = R_i_var[27];

  expandedR[39] = R_i_var[26];
  expandedR[38] = R_i_var[25];
  expandedR[37] = R_i_var[24];
  expandedR[36] = R_i_var[23];
  expandedR[35] = R_i_var[24];
  expandedR[34] = R_i_var[23];
  expandedR[33] = R_i_var[22];
  expandedR[32] = R_i_var[21];

  expandedR[31] = R_i_var[20];
  expandedR[30] = R_i_var[19];
  expandedR[29] = R_i_var[20];
  expandedR[28] = R_i_var[19];
  expandedR[27] = R_i_var[18];
  expandedR[26] = R_i_var[17];
  expandedR[25] = R_i_var[16];
  expandedR[24] = R_i_var[15];

  expandedR[23] = R_i_var[16];
  expandedR[22] = R_i_var[15];
  expandedR[21] = R_i_var[14];
  expandedR[20] = R_i_var[13];
  expandedR[19] = R_i_var[12];
  expandedR[18] = R_i_var[11];
  expandedR[17] = R_i_var[12];
  expandedR[16] = R_i_var[11];

  expandedR[15] = R_i_var[10];
  expandedR[14] = R_i_var[9];
  expandedR[13] = R_i_var[8];
  expandedR[12] = R_i_var[7];
  expandedR[11] = R_i_var[8];
  expandedR[10] = R_i_var[7];
  expandedR[9] = R_i_var[6];
  expandedR[8] = R_i_var[5];

  expandedR[7] = R_i_var[4];
  expandedR[6] = R_i_var[3];
  expandedR[5] = R_i_var[4];
  expandedR[4] = R_i_var[3];
  expandedR[3] = R_i_var[2];
  expandedR[2] = R_i_var[1];
  expandedR[1] = R_i_var[0];
  expandedR[0] = R_i_var[31];


  my->previous_key_round_proc->put(my->Key_i_round_proc->fetch(), owner);
  my->iteration_round_proc->put(my->iteration_i_round_proc->fetch(), owner);
  my->decrypt_round_proc->put(my->decrypt_i_round_proc->fetch(), owner);

  round_key = my->new_key_round_proc->fetch();


  KER = expandedR ^ round_key;


  my->s1_o_round_proc->put(KER.range(47, 42), owner);
  my->s2_o_round_proc->put(KER.range(41, 36), owner);
  my->s3_o_round_proc->put(KER.range(35, 30), owner);
  my->s4_o_round_proc->put(KER.range(29, 24), owner);
  my->s5_o_round_proc->put(KER.range(23, 18), owner);
  my->s6_o_round_proc->put(KER.range(17, 12), owner);
  my->s7_o_round_proc->put(KER.range(11, 6), owner);
  my->s8_o_round_proc->put(KER.range(5, 0), owner);

  Soutput.range(31, 28) = my->s1_i_round_proc->fetch();
  Soutput.range(27, 24) = my->s2_i_round_proc->fetch();
  Soutput.range(23, 20) = my->s3_i_round_proc->fetch();
  Soutput.range(19, 16) = my->s4_i_round_proc->fetch();
  Soutput.range(15, 12) = my->s5_i_round_proc->fetch();
  Soutput.range(11, 8) = my->s6_i_round_proc->fetch();
  Soutput.range(7, 4) = my->s7_i_round_proc->fetch();
  Soutput.range(3, 0) = my->s8_i_round_proc->fetch();



  f[31] = Soutput[16];
  f[30] = Soutput[25];
  f[29] = Soutput[12];
  f[28] = Soutput[11];
  f[27] = Soutput[3];
  f[26] = Soutput[20];
  f[25] = Soutput[4];
  f[24] = Soutput[15];

  f[23] = Soutput[31];
  f[22] = Soutput[17];
  f[21] = Soutput[9];
  f[20] = Soutput[6];
  f[19] = Soutput[27];
  f[18] = Soutput[14];
  f[17] = Soutput[1];
  f[16] = Soutput[22];

  f[15] = Soutput[30];
  f[14] = Soutput[24];
  f[13] = Soutput[8];
  f[12] = Soutput[18];
  f[11] = Soutput[0];
  f[10] = Soutput[5];
  f[9] = Soutput[29];
  f[8] = Soutput[23];

  f[7] = Soutput[13];
  f[6] = Soutput[19];
  f[5] = Soutput[2];
  f[4] = Soutput[26];
  f[3] = Soutput[10];
  f[2] = Soutput[21];
  f[1] = Soutput[28];
  f[0] = Soutput[7];

  my->next_R_round_proc->put(my->L_i_round_proc->fetch() ^ f, owner);

  //my->expanRSig_round_proc->put(my->L_i_round_proc->fetch() ^ f, owner);

}
