#ifndef __des_h__
#define __des_h__

#include <psml.h>

#include "round.h"
#include "s1.h"
#include "s2.h"
#include "s3.h"
#include "s4.h"
#include "s5.h"
#include "s6.h"
#include "s7.h"
#include "s8.h"

#include "CorePartitioner.h"
using namespace Parvicursor::psml;
//---------------------------------------
class des : public psml_component
{
    psml_inport<psml_wire<psml_uint<64>  > > data_i_des_proc;
    psml_inport<psml_wire<bool > > data_ready_des_proc;
    psml_inport<psml_wire<psml_uint<64>  > > key_i_des_proc;
    psml_inport<psml_wire<bool > > load_i_des_proc;
    psml_inport<psml_wire<psml_uint<32>  > > stage1_L_o_des_proc;
    psml_inport<psml_wire<psml_uint<32>  > > stage1_R_o_des_proc;
    psml_inport<psml_wire<psml_uint<4>  > > stage1_iter_des_proc;
    psml_inport<psml_wire<psml_uint<56>  > > stage1_round_key_o_des_proc;
    psml_inport<psml_wire<bool > > clk_reg_signal;
    psml_inport<psml_wire<psml_uint<64>  > > next_data_o_reg_signal;
    psml_inport<psml_wire<bool > > next_data_ready_reg_signal;
    psml_inport<psml_wire<bool > > next_ready_o_reg_signal;
    psml_inport<psml_wire<psml_uint<4>  > > next_stage1_iter_reg_signal;
    psml_inport<psml_wire<bool > > reset_reg_signal;

    psml_outport<psml_wire<psml_uint<64>  > > next_data_o_des_proc;
    psml_outport<psml_wire<bool > > next_data_ready_des_proc;
    psml_outport<psml_wire<bool > > next_ready_o_des_proc;
    psml_outport<psml_wire<psml_uint<4>  > > next_stage1_iter_des_proc;
    psml_outport<psml_wire<psml_uint<32>  > > stage1_L_i_des_proc;
    psml_outport<psml_wire<psml_uint<32>  > > stage1_R_i_des_proc;
    psml_outport<psml_wire<psml_uint<4>  > > stage1_iteration_i_des_proc;
    psml_outport<psml_wire<psml_uint<56>  > > stage1_round_key_i_des_proc;
    psml_outport<psml_wire<psml_uint<64>  > > data_o_reg_signal;
    psml_outport<psml_wire<bool > > data_ready_reg_signal;
    psml_outport<psml_wire<bool > > ready_o_reg_signal;
    psml_outport<psml_wire<psml_uint<4>  > > stage1_iter_reg_signal;

    psml_process *des_proc_handle;
    psml_process *reg_signal_handle;

    public: psml_proxy<psml_wire<bool  > > clk;
    public: psml_proxy<psml_wire<bool  > > reset;

    public: psml_proxy<psml_wire<bool  > > load_i;
    public: psml_proxy<psml_wire<bool  > > decrypt_i;
    public: psml_proxy<psml_wire<psml_uint<64>  > > data_i;
    public: psml_proxy<psml_wire<psml_uint<64>  > > key_i;

    public: psml_proxy<psml_wire<psml_uint<64>  > > data_o;
    public: psml_proxy<psml_wire<bool  > > ready_o;


    psml_wire<psml_uint<4>  > stage1_iter, next_stage1_iter;
    psml_connector stage1_iter_c;
    psml_connector next_stage1_iter_c;
    psml_wire<bool  > next_ready_o;
    psml_connector next_ready_o_c;
    psml_wire<psml_uint<64>  > next_data_o;
    psml_connector next_data_o_c;
    psml_wire<bool  > data_ready, next_data_ready;
    psml_connector data_ready_c;
    psml_connector next_data_ready_c;


    psml_wire<psml_uint<32>  > stage1_L_i;
    psml_connector stage1_L_i_c;
    psml_wire<psml_uint<32>  > stage1_R_i;
    psml_connector stage1_R_i_c;
    psml_wire<psml_uint<56>  > stage1_round_key_i;
    psml_connector stage1_round_key_i_c;

    psml_wire<psml_uint<4>  > stage1_iteration_i;
    psml_connector stage1_iteration_i_c;
    psml_wire<psml_uint<32>  > stage1_R_o;
    psml_connector stage1_R_o_c;
    psml_wire<psml_uint<32>  > stage1_L_o;
    psml_connector stage1_L_o_c;
    psml_wire<psml_uint<56>  > stage1_round_key_o;
    psml_connector stage1_round_key_o_c;

    psml_wire<psml_uint<6>  > s1_stag1_i, s2_stag1_i, s3_stag1_i, s4_stag1_i, s5_stag1_i, s6_stag1_i, s7_stag1_i, s8_stag1_i;
    psml_connector s1_stag1_i_c;
    psml_connector s2_stag1_i_c;
    psml_connector s3_stag1_i_c;
    psml_connector s4_stag1_i_c;
    psml_connector s5_stag1_i_c;
    psml_connector s6_stag1_i_c;
    psml_connector s7_stag1_i_c;
    psml_connector s8_stag1_i_c;
    psml_wire<psml_uint<4>  > s1_stag1_o, s2_stag1_o, s3_stag1_o, s4_stag1_o, s5_stag1_o, s6_stag1_o, s7_stag1_o, s8_stag1_o;
    psml_connector s1_stag1_o_c;
    psml_connector s2_stag1_o_c;
    psml_connector s3_stag1_o_c;
    psml_connector s4_stag1_o_c;
    psml_connector s5_stag1_o_c;
    psml_connector s6_stag1_o_c;
    psml_connector s7_stag1_o_c;
    psml_connector s8_stag1_o_c;

    static void des_proc(psml_process *owner);
    static void reg_signal(psml_process *owner);

    desround *rd1;

    s1 *sbox1;
    s2 *sbox2;
    s3 *sbox3;
    s4 *sbox4;
    s5 *sbox5;
    s6 *sbox6;
    s7 *sbox7;
    s8 *sbox8;

    des(const String &name, psml_core *core)
    {
    	set_name(name);

    	register_proxy(decrypt_i);

        register_proxy(clk, clk_reg_signal);
        register_proxy(reset, reset_reg_signal);
        register_proxy(data_o, data_o_reg_signal);
        register_proxy(ready_o, ready_o_reg_signal);
        reg_signal_handle = register_process(reg_signal, false, "des_reg_signal");
        reg_signal_handle->register_port(clk_reg_signal);
        reg_signal_handle->register_port(next_data_o_reg_signal);
        reg_signal_handle->register_port(next_data_ready_reg_signal);
        reg_signal_handle->register_port(next_ready_o_reg_signal);
        reg_signal_handle->register_port(next_stage1_iter_reg_signal);
        reg_signal_handle->register_port(reset_reg_signal);
        reg_signal_handle->register_port(data_o_reg_signal);
        reg_signal_handle->register_port(data_ready_reg_signal);
        reg_signal_handle->register_port(ready_o_reg_signal);
        reg_signal_handle->register_port(stage1_iter_reg_signal);
        reg_signal_handle->set_delay(psml_time(0, PSML_NS));
        //reg_signal_handle->dont_initialize(); ///
        next_data_o_reg_signal.bind(next_data_o_c, next_data_o);
        next_data_ready_reg_signal.bind(next_data_ready_c, next_data_ready);
        next_ready_o_reg_signal.bind(next_ready_o_c, next_ready_o);
        next_stage1_iter_reg_signal.bind(next_stage1_iter_c, next_stage1_iter);
        data_ready_reg_signal.bind(data_ready_c, data_ready);
        stage1_iter_reg_signal.bind(stage1_iter_c, stage1_iter);
        reg_signal_handle->register_sensitivity(clk_reg_signal, PSML_POS_EDGE);
        reg_signal_handle->register_sensitivity(reset_reg_signal, PSML_NEG_EDGE);

        register_proxy(data_i, data_i_des_proc);
        register_proxy(key_i, key_i_des_proc);
        register_proxy(load_i, load_i_des_proc);
        des_proc_handle = register_process(des_proc, false, "des_des_proc");
        des_proc_handle->register_port(data_i_des_proc);
        des_proc_handle->register_port(data_ready_des_proc);
        des_proc_handle->register_port(key_i_des_proc);
        des_proc_handle->register_port(load_i_des_proc);
        des_proc_handle->register_port(stage1_L_o_des_proc);
        des_proc_handle->register_port(stage1_R_o_des_proc);
        des_proc_handle->register_port(stage1_iter_des_proc);
        des_proc_handle->register_port(stage1_round_key_o_des_proc);
        des_proc_handle->register_port(next_data_o_des_proc);
        des_proc_handle->register_port(next_data_ready_des_proc);
        des_proc_handle->register_port(next_ready_o_des_proc);
        des_proc_handle->register_port(next_stage1_iter_des_proc);
        des_proc_handle->register_port(stage1_L_i_des_proc);
        des_proc_handle->register_port(stage1_R_i_des_proc);
        des_proc_handle->register_port(stage1_iteration_i_des_proc);
        des_proc_handle->register_port(stage1_round_key_i_des_proc);
        des_proc_handle->set_delay(psml_time(0, PSML_NS));
        //des_proc_handle->dont_initialize(); ///
        data_ready_des_proc.bind(data_ready_c, data_ready);
        stage1_L_o_des_proc.bind(stage1_L_o_c, stage1_L_o);
        stage1_R_o_des_proc.bind(stage1_R_o_c, stage1_R_o);
        stage1_iter_des_proc.bind(stage1_iter_c, stage1_iter);
        stage1_round_key_o_des_proc.bind(stage1_round_key_o_c, stage1_round_key_o);
        next_data_o_des_proc.bind(next_data_o_c, next_data_o);
        next_data_ready_des_proc.bind(next_data_ready_c, next_data_ready);
        next_ready_o_des_proc.bind(next_ready_o_c, next_ready_o);
        next_stage1_iter_des_proc.bind(next_stage1_iter_c, next_stage1_iter);
        stage1_L_i_des_proc.bind(stage1_L_i_c, stage1_L_i);
        stage1_R_i_des_proc.bind(stage1_R_i_c, stage1_R_i);
        stage1_iteration_i_des_proc.bind(stage1_iteration_i_c, stage1_iteration_i);
        stage1_round_key_i_des_proc.bind(stage1_round_key_i_c, stage1_round_key_i);
        des_proc_handle->register_sensitivity(data_i_des_proc);
        des_proc_handle->register_sensitivity(key_i_des_proc);
        des_proc_handle->register_sensitivity(load_i_des_proc);
        des_proc_handle->register_sensitivity(stage1_iter_des_proc);
        des_proc_handle->register_sensitivity(data_ready_des_proc);
        des_proc_handle->register_sensitivity(stage1_L_o_des_proc);
        des_proc_handle->register_sensitivity(stage1_R_o_des_proc);
        des_proc_handle->register_sensitivity(stage1_round_key_o_des_proc);

        rd1 = new desround("round1", core);

        sbox1 = new s1("s1");
        sbox2 = new s2("s2");
        sbox3 = new s3("s3");
        sbox4 = new s4("s4");
        sbox5 = new s5("s5");
        sbox6 = new s6("s6");
        sbox7 = new s7("s7");
        sbox8 = new s8("s8");

        rd1->clk.bind(clk);
        rd1->reset.bind(reset);
        rd1->iteration_i.bind(stage1_iteration_i_c, stage1_iteration_i);
        rd1->decrypt_i.bind(decrypt_i);
        rd1->R_i.bind(stage1_R_i_c, stage1_R_i);
        rd1->L_i.bind(stage1_L_i_c, stage1_L_i);
        rd1->Key_i.bind(stage1_round_key_i_c, stage1_round_key_i);
        rd1->R_o.bind(stage1_R_o_c, stage1_R_o);
        rd1->L_o.bind(stage1_L_o_c, stage1_L_o);
        rd1->Key_o.bind(stage1_round_key_o_c, stage1_round_key_o);
        rd1->s1_o.bind(s1_stag1_i_c, s1_stag1_i);
        rd1->s2_o.bind(s2_stag1_i_c, s2_stag1_i);
        rd1->s3_o.bind(s3_stag1_i_c, s3_stag1_i);
        rd1->s4_o.bind(s4_stag1_i_c, s4_stag1_i);
        rd1->s5_o.bind(s5_stag1_i_c, s5_stag1_i);
        rd1->s6_o.bind(s6_stag1_i_c, s6_stag1_i);
        rd1->s7_o.bind(s7_stag1_i_c, s7_stag1_i);
        rd1->s8_o.bind(s8_stag1_i_c, s8_stag1_i);
        rd1->s1_i.bind(s1_stag1_o_c, s1_stag1_o);
        rd1->s2_i.bind(s2_stag1_o_c, s2_stag1_o);
        rd1->s3_i.bind(s3_stag1_o_c, s3_stag1_o);
        rd1->s4_i.bind(s4_stag1_o_c, s4_stag1_o);
        rd1->s5_i.bind(s5_stag1_o_c, s5_stag1_o);
        rd1->s6_i.bind(s6_stag1_o_c, s6_stag1_o);
        rd1->s7_i.bind(s7_stag1_o_c, s7_stag1_o);
        rd1->s8_i.bind(s8_stag1_o_c, s8_stag1_o);

        sbox1->stage1_input.bind(s1_stag1_i_c, s1_stag1_i);
        sbox1->stage1_output.bind(s1_stag1_o_c, s1_stag1_o);

        sbox2->stage1_input.bind(s2_stag1_i_c, s2_stag1_i);
        sbox2->stage1_output.bind(s2_stag1_o_c, s2_stag1_o);

        sbox3->stage1_input.bind(s3_stag1_i_c, s3_stag1_i);
        sbox3->stage1_output.bind(s3_stag1_o_c, s3_stag1_o);

        sbox4->stage1_input.bind(s4_stag1_i_c, s4_stag1_i);
        sbox4->stage1_output.bind(s4_stag1_o_c, s4_stag1_o);

        sbox5->stage1_input.bind(s5_stag1_i_c, s5_stag1_i);
        sbox5->stage1_output.bind(s5_stag1_o_c, s5_stag1_o);

        sbox6->stage1_input.bind(s6_stag1_i_c, s6_stag1_i);
        sbox6->stage1_output.bind(s6_stag1_o_c, s6_stag1_o);

        sbox7->stage1_input.bind(s7_stag1_i_c, s7_stag1_i);
        sbox7->stage1_output.bind(s7_stag1_o_c, s7_stag1_o);

        sbox8->stage1_input.bind(s8_stag1_i_c, s8_stag1_i);
        sbox8->stage1_output.bind(s8_stag1_o_c, s8_stag1_o);

        core->add_process(des_proc_handle);
        core->add_process(reg_signal_handle);
        core->add_process(sbox1->s1_box_handle);
        core->add_process(sbox2->s2_box_handle);
        core->add_process(sbox3->s3_box_handle);
        core->add_process(sbox4->s4_box_handle);
        core->add_process(sbox5->s5_box_handle);
        core->add_process(sbox6->s6_box_handle);
        core->add_process(sbox7->s7_box_handle);
        core->add_process(sbox8->s8_box_handle);
    }
};
//---------------------------------------

#endif

