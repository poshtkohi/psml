#ifndef __key_gen_h__
#define __key_gen_h__

#include <psml.h>
//---------------------------------------
class key_gen : public psml_component
{
    psml_inport<psml_wire<bool > > decrypt_generate_key;
    psml_inport<psml_wire<psml_uint<4>  > > iteration_generate_key;
    psml_inport<psml_wire<psml_uint<56>  > > previous_key_generate_key;

    psml_outport<psml_wire<psml_uint<48>  > > new_key_generate_key;
    psml_outport<psml_wire<psml_uint<56>  > > non_perm_key_generate_key;

    public: psml_process *generate_key_handle;

    public: psml_proxy<psml_wire<psml_uint<56>  > > previous_key;
    public: psml_proxy<psml_wire<psml_uint<4>  > > iteration;
    public: psml_proxy<psml_wire<bool  > > decrypt;
    public: psml_proxy<psml_wire<psml_uint<56>  > > non_perm_key;
    public: psml_proxy<psml_wire<psml_uint<48>  > > new_key;

    static void generate_key(psml_process *owner);

    key_gen( const String &name = "")
    {
        set_name(name);

        register_proxy(decrypt, decrypt_generate_key);
        register_proxy(iteration, iteration_generate_key);
        register_proxy(previous_key, previous_key_generate_key);
        register_proxy(new_key, new_key_generate_key);
        register_proxy(non_perm_key, non_perm_key_generate_key);
        generate_key_handle = register_process(generate_key, false, "key_gen_generate_key");
        generate_key_handle->register_port(decrypt_generate_key);
        generate_key_handle->register_port(iteration_generate_key);
        generate_key_handle->register_port(previous_key_generate_key);
        generate_key_handle->register_port(new_key_generate_key);
        generate_key_handle->register_port(non_perm_key_generate_key);
        generate_key_handle->set_delay(psml_time(0, PSML_NS));
        generate_key_handle->register_sensitivity(previous_key_generate_key);
        generate_key_handle->register_sensitivity(iteration_generate_key);
        generate_key_handle->register_sensitivity(decrypt_generate_key);
        //generate_key_handle->dont_initialize();///
    }
};
//---------------------------------------

#endif
