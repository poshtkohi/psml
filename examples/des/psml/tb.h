#ifndef __tb_h__
#define __tb_h__
#include <psml.h>

#include "CorePartitioner.h"
using namespace Parvicursor::psml;
//---------------------------------------
class tb : public psml_component
{
    /*---------------------process states---------*/
    private: class send_process_state : public psml_process_state
    {
        public: Int32 next_input;
        public: psml_uint<64> input;
        public: psml_uint<64> result;
        public: send_process_state()
        {
            next_input = 0;
        }
        // Overridden metods
        //public: void serialize();
        //public: void deserialize();
    };
    private: class recv_process_state : public psml_process_state
    {
        public: psml_uint<64> result;
        public: recv_process_state()
        {
        }
        // Overridden metods
        //public: void serialize();
        //public: void deserialize();
    };
    private: send_process_state _send_process_state;
    private: recv_process_state _recv_process_state;
    /*---------------------fields-----------------*/
    psml_inport<psml_wire<psml_uint<64>  > > rt_des_data_i_recv;
    psml_inport<psml_wire<bool > > rt_des_ready_i_recv;
    psml_inport<psml_wire<bool > > clk_send;

    psml_outport<psml_wire<bool > > rt_decrypt_o_send;
    psml_outport<psml_wire<psml_uint<64>  > > rt_des_data_o_send;
    psml_outport<psml_wire<psml_uint<64>  > > rt_des_key_o_send;
    psml_outport<psml_wire<bool > > rt_load_o_send;
    psml_outport<psml_wire<bool > > rt_reset_send;

    psml_process *recv_handle;
    psml_process *send_handle;


    public: psml_proxy<psml_wire<bool > > clk;
    public: psml_proxy<psml_wire<psml_uint<64>  > > rt_des_data_i;
    public: psml_proxy<psml_wire<bool > > rt_des_ready_i;


    public: psml_proxy<psml_wire<bool > > rt_load_o;
    public: psml_proxy<psml_wire<psml_uint<64>  > >rt_des_data_o;
    public: psml_proxy<psml_wire<psml_uint<64>  > >rt_des_key_o;
    public: psml_proxy<psml_wire<bool > > rt_decrypt_o;
    public: psml_proxy<psml_wire<bool > > rt_reset;
    /*---------------------methods----------------*/
    static void send(psml_process *owner);
    static void recv(psml_process *owner);

    tb(const String &name, psml_core *core)
    {
        set_name(name);

        register_proxy(clk, clk_send);
        register_proxy(rt_decrypt_o, rt_decrypt_o_send);
        register_proxy(rt_des_data_o, rt_des_data_o_send);
        register_proxy(rt_des_key_o, rt_des_key_o_send);
        register_proxy(rt_load_o, rt_load_o_send);
        register_proxy(rt_reset, rt_reset_send);
        send_handle = register_special_process(send, &_send_process_state, "tb_send");
        send_handle->register_port(clk_send);
        send_handle->register_port(rt_decrypt_o_send);
        send_handle->register_port(rt_des_data_o_send);
        send_handle->register_port(rt_des_key_o_send);
        send_handle->register_port(rt_load_o_send);
        send_handle->register_port(rt_reset_send);
        send_handle->set_delay(psml_time(0, PSML_NS));
        send_handle->register_sensitivity(clk_send, PSML_POS_EDGE);
        //send_handle->dont_initialize();//

        register_proxy(rt_des_data_i, rt_des_data_i_recv);
        register_proxy(rt_des_ready_i, rt_des_ready_i_recv);
        recv_handle = register_special_process(recv, &_recv_process_state, "tb_recv");
        recv_handle->register_port(rt_des_data_i_recv);
        recv_handle->register_port(rt_des_ready_i_recv);
        recv_handle->set_delay(psml_time(0, PSML_NS));
        recv_handle->register_sensitivity(rt_des_ready_i_recv, PSML_POS_EDGE);
        //recv_handle->dont_initialize();//

        core->add_process(send_handle);
        core->add_process(recv_handle);
    }

    ~tb()
    {
    }
    /*--------------------------------------------*/

};
//---------------------------------------

#endif
