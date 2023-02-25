#ifndef __round_h__
#define __round_h__

#include <psml.h>

#include "key_gen.h"

#include "CorePartitioner.h"
using namespace Parvicursor::psml;
//---------------------------------------
class desround : public psml_component
{
    psml_inport<psml_wire<psml_uint<32>  > > R_i_registers;
    psml_inport<psml_wire<bool > > clk_registers;
    psml_inport<psml_wire<psml_uint<32>  > > next_R_registers;
    psml_inport<psml_wire<psml_uint<56>  > > non_perm_key_registers;
    psml_inport<psml_wire<bool > > reset_registers;
    psml_inport<psml_wire<psml_uint<56>  > > Key_i_round_proc;
    psml_inport<psml_wire<psml_uint<32>  > > L_i_round_proc;
    psml_inport<psml_wire<psml_uint<32>  > > R_i_round_proc;
    psml_inport<psml_wire<bool > > decrypt_i_round_proc;
    psml_inport<psml_wire<psml_uint<4>  > > iteration_i_round_proc;
    psml_inport<psml_wire<psml_uint<48>  > > new_key_round_proc;
    psml_inport<psml_wire<psml_uint<4>  > > s1_i_round_proc;
    psml_inport<psml_wire<psml_uint<4>  > > s2_i_round_proc;
    psml_inport<psml_wire<psml_uint<4>  > > s3_i_round_proc;
    psml_inport<psml_wire<psml_uint<4>  > > s4_i_round_proc;
    psml_inport<psml_wire<psml_uint<4>  > > s5_i_round_proc;
    psml_inport<psml_wire<psml_uint<4>  > > s6_i_round_proc;
    psml_inport<psml_wire<psml_uint<4>  > > s7_i_round_proc;
    psml_inport<psml_wire<psml_uint<4>  > > s8_i_round_proc;

    psml_outport<psml_wire<psml_uint<56>  > > Key_o_registers;
    psml_outport<psml_wire<psml_uint<32>  > > L_o_registers;
    psml_outport<psml_wire<psml_uint<32>  > > R_o_registers;
    psml_outport<psml_wire<bool > > decrypt_round_proc;
    //psml_outport<psml_wire<psml_uint<32>  > > expanRSig_round_proc;
    psml_outport<psml_wire<psml_uint<4>  > > iteration_round_proc;
    psml_outport<psml_wire<psml_uint<32>  > > next_R_round_proc;
    psml_outport<psml_wire<psml_uint<56>  > > previous_key_round_proc;
    psml_outport<psml_wire<psml_uint<6>  > > s1_o_round_proc;
    psml_outport<psml_wire<psml_uint<6>  > > s2_o_round_proc;
    psml_outport<psml_wire<psml_uint<6>  > > s3_o_round_proc;
    psml_outport<psml_wire<psml_uint<6>  > > s4_o_round_proc;
    psml_outport<psml_wire<psml_uint<6>  > > s5_o_round_proc;
    psml_outport<psml_wire<psml_uint<6>  > > s6_o_round_proc;
    psml_outport<psml_wire<psml_uint<6>  > > s7_o_round_proc;
    psml_outport<psml_wire<psml_uint<6>  > > s8_o_round_proc;

    psml_process *registers_handle;
    psml_process *round_proc_handle;

    public: psml_proxy<psml_wire<bool  > > clk;
    public: psml_proxy<psml_wire<bool  > > reset;

    public: psml_proxy<psml_wire<psml_uint<4>  > > iteration_i;
    public: psml_proxy<psml_wire<bool  > > decrypt_i;
    public: psml_proxy<psml_wire<psml_uint<32>  > > R_i;
    public: psml_proxy<psml_wire<psml_uint<32>  > > L_i;
    public: psml_proxy<psml_wire<psml_uint<56>  > > Key_i;

    public: psml_proxy<psml_wire<psml_uint<32>  > > R_o;
    public: psml_proxy<psml_wire<psml_uint<32>  > > L_o;
    public: psml_proxy<psml_wire<psml_uint<56>  > > Key_o;

    public: psml_proxy<psml_wire<psml_uint<6>  > > s1_o, s2_o, s3_o, s4_o, s5_o, s6_o, s7_o, s8_o;
    public: psml_proxy<psml_wire<psml_uint<4>  > > s1_i, s2_i, s3_i, s4_i, s5_i, s6_i, s7_i, s8_i;

    static void registers(psml_process *owner);
    static void round_proc(psml_process *owner);

    psml_wire<psml_uint<56>  > previous_key;
    psml_connector previous_key_c;
    psml_wire<psml_uint<4>  > iteration;
    psml_connector iteration_c;
    psml_wire<bool  > decrypt;
    psml_connector decrypt_c;
    psml_wire<psml_uint<56>  > non_perm_key;
    psml_connector non_perm_key_c;
    psml_wire<psml_uint<48>  > new_key;
    psml_connector new_key_c;

    psml_wire<psml_uint<32>  > next_R;
    psml_connector next_R_c;

    //psml_wire<psml_uint<32>  > expanRSig;
    //psml_connector expanRSig_c;


    key_gen *kg1;

    desround(const String &name, psml_core *core)
    {
    	set_name(name);

        kg1 = new key_gen("key_gen");
        kg1->previous_key.bind(previous_key_c, previous_key);
        kg1->iteration.bind(iteration_c, iteration);
        kg1->decrypt.bind(decrypt_c, decrypt);
        kg1->new_key.bind(new_key_c, new_key);
        kg1->non_perm_key.bind(non_perm_key_c, non_perm_key);

        register_proxy(R_i, R_i_registers);
        register_proxy(clk, clk_registers);
        register_proxy(reset, reset_registers);
        register_proxy(Key_o, Key_o_registers);
        register_proxy(L_o, L_o_registers);
        register_proxy(R_o, R_o_registers);
        registers_handle = register_process(registers, false, "desround_registers");
        registers_handle->register_port(R_i_registers);
        registers_handle->register_port(clk_registers);
        registers_handle->register_port(next_R_registers);
        registers_handle->register_port(non_perm_key_registers);
        registers_handle->register_port(reset_registers);
        registers_handle->register_port(Key_o_registers);
        registers_handle->register_port(L_o_registers);
        registers_handle->register_port(R_o_registers);
        registers_handle->set_delay(psml_time(0, PSML_NS));
        //registers_handle->dont_initialize(); ///
        next_R_registers.bind(next_R_c, next_R);
        non_perm_key_registers.bind(non_perm_key_c, non_perm_key);
        registers_handle->register_sensitivity(clk_registers, PSML_POS_EDGE);
        registers_handle->register_sensitivity(reset_registers, PSML_NEG_EDGE);

        register_proxy(Key_i, Key_i_round_proc);
        register_proxy(L_i, L_i_round_proc);
        register_proxy(R_i, R_i_round_proc);
        register_proxy(decrypt_i, decrypt_i_round_proc);
        register_proxy(iteration_i, iteration_i_round_proc);
        register_proxy(s1_i, s1_i_round_proc);
        register_proxy(s2_i, s2_i_round_proc);
        register_proxy(s3_i, s3_i_round_proc);
        register_proxy(s4_i, s4_i_round_proc);
        register_proxy(s5_i, s5_i_round_proc);
        register_proxy(s6_i, s6_i_round_proc);
        register_proxy(s7_i, s7_i_round_proc);
        register_proxy(s8_i, s8_i_round_proc);
        register_proxy(s1_o, s1_o_round_proc);
        register_proxy(s2_o, s2_o_round_proc);
        register_proxy(s3_o, s3_o_round_proc);
        register_proxy(s4_o, s4_o_round_proc);
        register_proxy(s5_o, s5_o_round_proc);
        register_proxy(s6_o, s6_o_round_proc);
        register_proxy(s7_o, s7_o_round_proc);
        register_proxy(s8_o, s8_o_round_proc);
        round_proc_handle = register_process(round_proc, false, "desround_round_proc");
        round_proc_handle->register_port(Key_i_round_proc);
        round_proc_handle->register_port(L_i_round_proc);
        round_proc_handle->register_port(R_i_round_proc);
        round_proc_handle->register_port(decrypt_i_round_proc);
        round_proc_handle->register_port(iteration_i_round_proc);
        round_proc_handle->register_port(new_key_round_proc);
        round_proc_handle->register_port(s1_i_round_proc);
        round_proc_handle->register_port(s2_i_round_proc);
        round_proc_handle->register_port(s3_i_round_proc);
        round_proc_handle->register_port(s4_i_round_proc);
        round_proc_handle->register_port(s5_i_round_proc);
        round_proc_handle->register_port(s6_i_round_proc);
        round_proc_handle->register_port(s7_i_round_proc);
        round_proc_handle->register_port(s8_i_round_proc);
        round_proc_handle->register_port(decrypt_round_proc);
        //round_proc_handle->register_port(expanRSig_round_proc);
        round_proc_handle->register_port(iteration_round_proc);
        round_proc_handle->register_port(next_R_round_proc);
        round_proc_handle->register_port(previous_key_round_proc);
        round_proc_handle->register_port(s1_o_round_proc);
        round_proc_handle->register_port(s2_o_round_proc);
        round_proc_handle->register_port(s3_o_round_proc);
        round_proc_handle->register_port(s4_o_round_proc);
        round_proc_handle->register_port(s5_o_round_proc);
        round_proc_handle->register_port(s6_o_round_proc);
        round_proc_handle->register_port(s7_o_round_proc);
        round_proc_handle->register_port(s8_o_round_proc);
        round_proc_handle->set_delay(psml_time(0, PSML_NS));
        //round_proc_handle->dont_initialize(); ///
        new_key_round_proc.bind(new_key_c, new_key);
        decrypt_round_proc.bind(decrypt_c, decrypt);
        //expanRSig_round_proc.bind(expanRSig_c, expanRSig);
        iteration_round_proc.bind(iteration_c, iteration);
        next_R_round_proc.bind(next_R_c, next_R);
        previous_key_round_proc.bind(previous_key_c, previous_key);

        round_proc_handle->register_sensitivity(R_i_round_proc);
        round_proc_handle->register_sensitivity(L_i_round_proc);
        round_proc_handle->register_sensitivity(Key_i_round_proc);
        round_proc_handle->register_sensitivity(iteration_i_round_proc);
        round_proc_handle->register_sensitivity(decrypt_i_round_proc);
        round_proc_handle->register_sensitivity(new_key_round_proc);
        round_proc_handle->register_sensitivity(s1_i_round_proc);
        round_proc_handle->register_sensitivity(s2_i_round_proc);
        round_proc_handle->register_sensitivity(s3_i_round_proc);
        round_proc_handle->register_sensitivity(s4_i_round_proc);
        round_proc_handle->register_sensitivity(s5_i_round_proc);
        round_proc_handle->register_sensitivity(s6_i_round_proc);
        round_proc_handle->register_sensitivity(s7_i_round_proc);
        round_proc_handle->register_sensitivity(s8_i_round_proc);

        Key_o_registers.SetName("Key_o_registers");
        L_o_registers.SetName("L_o_registers");
        R_o_registers.SetName("R_o_registers");
        decrypt_round_proc.SetName("decrypt_round_proc");
        //expanRSig_round_proc.SetName("expanRSig_round_proc");
        iteration_round_proc.SetName("iteration_round_proc");
        next_R_round_proc.SetName("next_R_round_proc");
        previous_key_round_proc.SetName("previous_key_round_proc");
        s1_o_round_proc.SetName("s1_o_round_proc");
        s2_o_round_proc.SetName("s2_o_round_proc");
        s3_o_round_proc.SetName("s3_o_round_proc");
        s4_o_round_proc.SetName("s4_o_round_proc");
        s5_o_round_proc.SetName("s5_o_round_proc");
        s6_o_round_proc.SetName("s6_o_round_proc");
        s7_o_round_proc.SetName("s7_o_round_proc");
        s8_o_round_proc.SetName("s8_o_round_proc");

        core->add_process(registers_handle);
        core->add_process(round_proc_handle);
        core->add_process(kg1->generate_key_handle);
    }
};
//---------------------------------------

#endif
