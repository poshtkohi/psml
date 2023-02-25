#ifndef __s7_h__

#define __s7_h__

#include <psml.h>


class s7 : public psml_component
{
    psml_inport<psml_wire<psml_uint<6>  > > stage1_input_s7_box;

    psml_outport<psml_wire<psml_uint<4>  > > stage1_output_s7_box;

    public: psml_process *s7_box_handle;

    public: psml_proxy<psml_wire<psml_uint<6>  > > stage1_input;
    public: psml_proxy<psml_wire<psml_uint<4>  > > stage1_output;

    static void s7_box(psml_process *owner);

    s7( const String &name = "" )
    {
        set_name(name);

        register_proxy(stage1_input, stage1_input_s7_box);
        register_proxy(stage1_output, stage1_output_s7_box);
        s7_box_handle = register_process(s7_box, false, "s7_box");
        s7_box_handle->register_port(stage1_input_s7_box);
        s7_box_handle->register_port(stage1_output_s7_box);
        s7_box_handle->set_delay(psml_time(0, PSML_NS));
        s7_box_handle->register_sensitivity(stage1_input_s7_box);
        //s7_box_handle->dont_initialize();
    }
};


#endif
