#include "tb.h"
//---------------------------------------
//Mutex *__mtx__ = new Mutex();
extern std::vector<psml_uint<64> > __patterns__;
extern psml_uint<64> __input_key__;
//---------------------------------------
void tb::send(psml_process *owner)
{
	tb *my = (tb *)owner->get_owner_component();

    tb::send_process_state *state = (tb::send_process_state *)owner->get_process_state();
    switch(state->get_label())
    {
        case 0:
            goto L0;
            break;
        case 1:
            goto L1;
            break;
        case 2:
            goto L2;
            break;
        case 3:
            goto L3;
            break;
    }

    my->rt_reset_send->put(0, owner);
    psml_wait(owner); state->set_label(0); return; L0: {}
    my->rt_reset_send->put(1, owner);
    //cout << "Reseted" << endl;

     while(true)
     {
        psml_wait(owner); state->set_label(1); return; L1: {}

        state->input = __patterns__[state->next_input];

        //std::cout << "@time " <<  psml_time_stamp(owner) << " in send() input " << input << std::endl;//<< " key " << __input_key__<< std::endl;

        my->rt_load_o_send->put(1, owner);//
        my->rt_des_data_o_send->put(state->input, owner);
        my->rt_des_key_o_send->put(__input_key__, owner);
        my->rt_decrypt_o_send->put(0, owner);



        psml_wait(owner); state->set_label(2); return; L2: {}
        my->rt_load_o_send->put(0, owner);

        psml_wait(16, owner); state->set_label(3); return; L3: {}

        state->next_input++;
        if(state->next_input == __patterns__.size())
           state->next_input = 0;
     }
}
//---------------------------------------
void tb::recv(psml_process *owner)
{
	tb *my = (tb *)owner->get_owner_component();

    tb::recv_process_state *state = (tb::recv_process_state *)owner->get_process_state();
    switch(state->get_label())
    {
        case 0:
            goto L0;
            break;
    }

    while(true)
    {

        psml_wait(owner); state->set_label(0); return; L0: {}

        state->result = my->rt_des_data_i_recv->fetch();

        //__mtx__->Lock();
        std::cout << "@time " <<  psml_time_stamp(owner) << " in recv() " << state->result << std::endl;
        //__mtx__->Unlock();
    }
}
//---------------------------------------
