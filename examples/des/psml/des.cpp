

#include "des.h"


void des::reg_signal(psml_process *owner)
{
	des *my = (des *)owner->get_owner_component();

	//std::cout << "@time " <<  psml_time_stamp(owner) << " in des::reg_signal() " << my->reset_reg_signal->fetch() << std::endl;

    if(!my->reset_reg_signal->fetch())
    {
        my->ready_o_reg_signal->put(0, owner);
        my->data_o_reg_signal->put(0, owner);
        my->stage1_iter_reg_signal->put(0, owner);
        my->data_ready_reg_signal->put(1, owner);
    }
    else
    {
        my->data_o_reg_signal->put(my->next_data_o_reg_signal->fetch(), owner);//
        my->ready_o_reg_signal->put(my->next_ready_o_reg_signal->fetch(), owner);
        //std::cout << "@time " <<  psml_time_stamp(owner) << " in des::reg_signal() " << my->next_data_o_reg_signal->fetch() << std::endl;

        my->stage1_iter_reg_signal->put(my->next_stage1_iter_reg_signal->fetch(), owner);
        my->data_ready_reg_signal->put(my->next_data_ready_reg_signal->fetch(), owner);
    }
}

void des::des_proc(psml_process *owner)
{
	des *my = (des *)owner->get_owner_component();

    //std::cout << "@time " <<  psml_time_stamp(owner) << " in des::des_proc()"\
              << " load_i " << my->load_i_des_proc->fetch()\
              << " data_i " << my->data_i_des_proc->fetch()\
              << " key_i " << my->key_i_des_proc->fetch()\
              << " stage1_R_o " <<  my->stage1_R_o_des_proc->fetch()\
              << " stage1_L_o " << my->stage1_L_o_des_proc->fetch()\
              << std::endl;

    psml_uint<32 > L_i_var, R_i_var;
    psml_uint<64 > data_i_var, data_o_var, data_o_var_t, key_i_var;
    psml_uint<56 > key_var_perm;

    L_i_var = 0;
    R_i_var = 0;
    data_i_var = 0;

    my->next_ready_o_des_proc->put(0, owner);
    my->next_data_ready_des_proc->put(my->data_ready_des_proc->fetch(), owner);
    my->next_stage1_iter_des_proc->put(my->stage1_iter_des_proc->fetch(), owner);

    my->stage1_L_i_des_proc->put(0, owner);
    my->stage1_R_i_des_proc->put(0, owner);
    my->stage1_round_key_i_des_proc->put(0, owner);


    key_i_var = my->key_i_des_proc->fetch();

    //std::cout << "@time " <<  psml_time_stamp(owner) << " in des::des_proc()"\
              << " key_i " << my->key_i_des_proc->fetch()\
              << " key_i_var " << key_i_var\
              << std::endl;

    key_var_perm[55] = key_i_var[7];
    key_var_perm[54] = key_i_var[15];
    key_var_perm[53] = key_i_var[23];
    key_var_perm[52] = key_i_var[31];
    key_var_perm[51] = key_i_var[39];
    key_var_perm[50] = key_i_var[47];
    key_var_perm[49] = key_i_var[55];
    key_var_perm[48] = key_i_var[63];

    key_var_perm[47] = key_i_var[6];
    key_var_perm[46] = key_i_var[14];
    key_var_perm[45] = key_i_var[22];
    key_var_perm[44] = key_i_var[30];
    key_var_perm[43] = key_i_var[38];
    key_var_perm[42] = key_i_var[46];
    key_var_perm[41] = key_i_var[54];
    key_var_perm[40] = key_i_var[62];

    key_var_perm[39] = key_i_var[5];
    key_var_perm[38] = key_i_var[13];
    key_var_perm[37] = key_i_var[21];
    key_var_perm[36] = key_i_var[29];
    key_var_perm[35] = key_i_var[37];
    key_var_perm[34] = key_i_var[45];
    key_var_perm[33] = key_i_var[53];
    key_var_perm[32] = key_i_var[61];

    key_var_perm[31] = key_i_var[4];
    key_var_perm[30] = key_i_var[12];
    key_var_perm[29] = key_i_var[20];
    key_var_perm[28] = key_i_var[28];
    key_var_perm[27] = key_i_var[1];
    key_var_perm[26] = key_i_var[9];
    key_var_perm[25] = key_i_var[17];
    key_var_perm[24] = key_i_var[25];

    key_var_perm[23] = key_i_var[33];
    key_var_perm[22] = key_i_var[41];
    key_var_perm[21] = key_i_var[49];
    key_var_perm[20] = key_i_var[57];
    key_var_perm[19] = key_i_var[2];
    key_var_perm[18] = key_i_var[10];
    key_var_perm[17] = key_i_var[18];
    key_var_perm[16] = key_i_var[26];

    key_var_perm[15] = key_i_var[34];
    key_var_perm[14] = key_i_var[42];
    key_var_perm[13] = key_i_var[50];
    key_var_perm[12] = key_i_var[58];
    key_var_perm[11] = key_i_var[3];
    key_var_perm[10] = key_i_var[11];
    key_var_perm[9] = key_i_var[19];
    key_var_perm[8] = key_i_var[27];

    key_var_perm[7] = key_i_var[35];
    key_var_perm[6] = key_i_var[43];
    key_var_perm[5] = key_i_var[51];
    key_var_perm[4] = key_i_var[59];
    key_var_perm[3] = key_i_var[36];
    key_var_perm[2] = key_i_var[44];
    key_var_perm[1] = key_i_var[52];
    key_var_perm[0] = key_i_var[60];

    //std::cout << "@time " <<  psml_time_stamp(owner) << " in des::des_proc()"\
              << " key_var_perm " << key_var_perm\
              << " key_i_var " << key_i_var\
              << std::endl;

    data_i_var = my->data_i_des_proc->fetch();
    L_i_var[31] = data_i_var[6];
    L_i_var[30] = data_i_var[14];
    L_i_var[29] = data_i_var[22];
    L_i_var[28] = data_i_var[30];
    L_i_var[27] = data_i_var[38];
    L_i_var[26] = data_i_var[46];
    L_i_var[25] = data_i_var[54];
    L_i_var[24] = data_i_var[62];

    L_i_var[23] = data_i_var[4];
    L_i_var[22] = data_i_var[12];
    L_i_var[21] = data_i_var[20];
    L_i_var[20] = data_i_var[28];
    L_i_var[19] = data_i_var[36];
    L_i_var[18] = data_i_var[44];
    L_i_var[17] = data_i_var[52];
    L_i_var[16] = data_i_var[60];

    L_i_var[15] = data_i_var[2];
    L_i_var[14] = data_i_var[10];
    L_i_var[13] = data_i_var[18];
    L_i_var[12] = data_i_var[26];
    L_i_var[11] = data_i_var[34];
    L_i_var[10] = data_i_var[42];
    L_i_var[9] = data_i_var[50];
    L_i_var[8] = data_i_var[58];

    L_i_var[7] = data_i_var[0];
    L_i_var[6] = data_i_var[8];
    L_i_var[5] = data_i_var[16];
    L_i_var[4] = data_i_var[24];
    L_i_var[3] = data_i_var[32];
    L_i_var[2] = data_i_var[40];
    L_i_var[1] = data_i_var[48];
    L_i_var[0] = data_i_var[56];

    R_i_var[31] = data_i_var[7];
    R_i_var[30] = data_i_var[15];
    R_i_var[29] = data_i_var[23];
    R_i_var[28] = data_i_var[31];
    R_i_var[27] = data_i_var[39];
    R_i_var[26] = data_i_var[47];
    R_i_var[25] = data_i_var[55];
    R_i_var[24] = data_i_var[63];

    R_i_var[23] = data_i_var[5];
    R_i_var[22] = data_i_var[13];
    R_i_var[21] = data_i_var[21];
    R_i_var[20] = data_i_var[29];
    R_i_var[19] = data_i_var[37];
    R_i_var[18] = data_i_var[45];
    R_i_var[17] = data_i_var[53];
    R_i_var[16] = data_i_var[61];

    R_i_var[15] = data_i_var[3];
    R_i_var[14] = data_i_var[11];
    R_i_var[13] = data_i_var[19];
    R_i_var[12] = data_i_var[27];
    R_i_var[11] = data_i_var[35];
    R_i_var[10] = data_i_var[43];
    R_i_var[9] = data_i_var[51];
    R_i_var[8] = data_i_var[59];

    R_i_var[7] = data_i_var[1];
    R_i_var[6] = data_i_var[9];
    R_i_var[5] = data_i_var[17];
    R_i_var[4] = data_i_var[25];
    R_i_var[3] = data_i_var[33];
    R_i_var[2] = data_i_var[41];
    R_i_var[1] = data_i_var[49];
    R_i_var[0] = data_i_var[57];

    //std::cout << "@time " <<  psml_time_stamp(owner) << " in des::des_proc()"\
              << " R_i_var " << R_i_var\
              << " data_i_var " << data_i_var\
              << std::endl;

    data_o_var_t.range(63, 32) = my->stage1_R_o_des_proc->fetch();
    data_o_var_t.range(31, 0) = my->stage1_L_o_des_proc->fetch();

    //std::cout << "@time " <<  psml_time_stamp(owner) << " in des::des_proc()"\
              << " data_o_var_t " << data_o_var_t\
              << " stage1_R_o " << my->stage1_R_o_des_proc->fetch()\
              << " stage1_L_o " << my->stage1_L_o_des_proc->fetch()\
              << std::endl;

    data_o_var[63] = data_o_var_t[24];
    data_o_var[62] = data_o_var_t[56];
    data_o_var[61] = data_o_var_t[16];
    data_o_var[60] = data_o_var_t[48];
    data_o_var[59] = data_o_var_t[8];
    data_o_var[58] = data_o_var_t[40];
    data_o_var[57] = data_o_var_t[0];
    data_o_var[56] = data_o_var_t[32];

    data_o_var[55] = data_o_var_t[25];
    data_o_var[54] = data_o_var_t[57];
    data_o_var[53] = data_o_var_t[17];
    data_o_var[52] = data_o_var_t[49];
    data_o_var[51] = data_o_var_t[9];
    data_o_var[50] = data_o_var_t[41];
    data_o_var[49] = data_o_var_t[1];
    data_o_var[48] = data_o_var_t[33];

    data_o_var[47] = data_o_var_t[26];
    data_o_var[46] = data_o_var_t[58];
    data_o_var[45] = data_o_var_t[18];
    data_o_var[44] = data_o_var_t[50];
    data_o_var[43] = data_o_var_t[10];
    data_o_var[42] = data_o_var_t[42];
    data_o_var[41] = data_o_var_t[2];
    data_o_var[40] = data_o_var_t[34];

    data_o_var[39] = data_o_var_t[27];
    data_o_var[38] = data_o_var_t[59];
    data_o_var[37] = data_o_var_t[19];
    data_o_var[36] = data_o_var_t[51];
    data_o_var[35] = data_o_var_t[11];
    data_o_var[34] = data_o_var_t[43];
    data_o_var[33] = data_o_var_t[3];
    data_o_var[32] = data_o_var_t[35];

    data_o_var[31] = data_o_var_t[28];
    data_o_var[30] = data_o_var_t[60];
    data_o_var[29] = data_o_var_t[20];
    data_o_var[28] = data_o_var_t[52];
    data_o_var[27] = data_o_var_t[12];
    data_o_var[26] = data_o_var_t[44];
    data_o_var[25] = data_o_var_t[4];
    data_o_var[24] = data_o_var_t[36];

    data_o_var[23] = data_o_var_t[29];
    data_o_var[22] = data_o_var_t[61];
    data_o_var[21] = data_o_var_t[21];
    data_o_var[20] = data_o_var_t[53];
    data_o_var[19] = data_o_var_t[13];
    data_o_var[18] = data_o_var_t[45];
    data_o_var[17] = data_o_var_t[5];
    data_o_var[16] = data_o_var_t[37];

    data_o_var[15] = data_o_var_t[30];
    data_o_var[14] = data_o_var_t[62];
    data_o_var[13] = data_o_var_t[22];
    data_o_var[12] = data_o_var_t[54];
    data_o_var[11] = data_o_var_t[14];
    data_o_var[10] = data_o_var_t[46];
    data_o_var[9] = data_o_var_t[6];
    data_o_var[8] = data_o_var_t[38];

    data_o_var[7] = data_o_var_t[31];
    data_o_var[6] = data_o_var_t[63];
    data_o_var[5] = data_o_var_t[23];
    data_o_var[4] = data_o_var_t[55];
    data_o_var[3] = data_o_var_t[15];
    data_o_var[2] = data_o_var_t[47];
    data_o_var[1] = data_o_var_t[7];
    data_o_var[0] = data_o_var_t[39];


    my->next_data_o_des_proc->put(data_o_var, owner);

    //std::cout << "@time " <<  psml_time_stamp(owner) << " in des::des_proc data_o_var " << data_o_var << std::endl;


    my->stage1_iteration_i_des_proc->put(my->stage1_iter_des_proc->fetch(), owner);

    my->next_ready_o_des_proc->put(0, owner);
    my->stage1_L_i_des_proc->put(my->stage1_L_o_des_proc->fetch(), owner);
    my->stage1_R_i_des_proc->put(my->stage1_R_o_des_proc->fetch(), owner);
    my->stage1_round_key_i_des_proc->put(my->stage1_round_key_o_des_proc->fetch(), owner);

    switch(my->stage1_iter_des_proc->fetch())
    {
        case 0:
            if(my->load_i_des_proc->fetch())
            {
                my->next_stage1_iter_des_proc->put(1, owner);
                my->stage1_L_i_des_proc->put(L_i_var, owner);
                my->stage1_R_i_des_proc->put(R_i_var, owner);
                my->stage1_round_key_i_des_proc->put(key_var_perm, owner);
                my->next_data_ready_des_proc->put(0, owner);
            }
            else if(!my->data_ready_des_proc->fetch())
            {
                my->next_stage1_iter_des_proc->put(0, owner);

                my->next_ready_o_des_proc->put(1, owner);
                my->next_data_ready_des_proc->put(1, owner);
            }
            break;
        case 15:
            my->next_stage1_iter_des_proc->put(0, owner);
            break;
        default:
            my->next_stage1_iter_des_proc->put(my->stage1_iter_des_proc->fetch() + 1, owner);
            break;
    }
}
