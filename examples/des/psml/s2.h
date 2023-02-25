#ifndef __s2_h__

#define __s2_h__

#include <psml.h>


class s2 : public psml_component
{
    psml_inport<psml_wire<psml_uint<6>  > > stage1_input_s2_box;

    psml_outport<psml_wire<psml_uint<4>  > > stage1_output_s2_box;

    public: psml_process *s2_box_handle;

    public: psml_proxy<psml_wire<psml_uint<6>  > > stage1_input;
    public: psml_proxy<psml_wire<psml_uint<4>  > > stage1_output;

    static void s2_box(psml_process *owner);

    s2( const String &name = "" )
    {
    	set_name(name);


        register_proxy(stage1_input, stage1_input_s2_box);
        register_proxy(stage1_output, stage1_output_s2_box);
        s2_box_handle = register_process(s2_box, false, "s2_box");
        s2_box_handle->register_port(stage1_input_s2_box);
        s2_box_handle->register_port(stage1_output_s2_box);
        s2_box_handle->set_delay(psml_time(0, PSML_NS));
        s2_box_handle->register_sensitivity(stage1_input_s2_box);
        s2_box_handle->dont_initialize();
    }
};


#endif
