#ifndef __s4_h__

#define __s4_h__

#include <psml.h>


class s4 : public psml_component
{
    psml_inport<psml_wire<psml_uint<6>  > > stage1_input_s4_box;

    psml_outport<psml_wire<psml_uint<4>  > > stage1_output_s4_box;

    public: psml_process *s4_box_handle;

    public: psml_proxy<psml_wire<psml_uint<6>  > > stage1_input;
    public: psml_proxy<psml_wire<psml_uint<4>  > > stage1_output;

    static void s4_box(psml_process *owner);

    s4( const String &name = "" )
    {
    	set_name(name);


        register_proxy(stage1_input, stage1_input_s4_box);
        register_proxy(stage1_output, stage1_output_s4_box);
        s4_box_handle = register_process(s4_box, false, "s4_box");
        s4_box_handle->register_port(stage1_input_s4_box);
        s4_box_handle->register_port(stage1_output_s4_box);
        s4_box_handle->set_delay(psml_time(0, PSML_NS));
        s4_box_handle->register_sensitivity(stage1_input_s4_box);
        ///s4_box_handle->dont_initialize();
    }
};


#endif
