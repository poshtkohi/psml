#ifndef __s1_h__

#define __s1_h__

#include <psml.h>



class s1 : public psml_component
{
    psml_inport<psml_wire<psml_uint<6>  > > stage1_input_s1_box;

    psml_outport<psml_wire<psml_uint<4>  > > stage1_output_s1_box;

    public: psml_process *s1_box_handle;

    public: psml_proxy<psml_wire<psml_uint<6>  > > stage1_input;
    public: psml_proxy<psml_wire<psml_uint<4>  > > stage1_output;

    static void s1_box(psml_process *owner);

    s1( const String &name = "" )
    {
    	set_name(name);


        register_proxy(stage1_input, stage1_input_s1_box);
        register_proxy(stage1_output, stage1_output_s1_box);
        s1_box_handle = register_process(s1_box, false, "s1_box");
        s1_box_handle->register_port(stage1_input_s1_box);
        s1_box_handle->register_port(stage1_output_s1_box);
        s1_box_handle->set_delay(psml_time(0, PSML_NS));
        s1_box_handle->register_sensitivity(stage1_input_s1_box);
        //s1_box_handle->dont_initialize();
    }
};


#endif
