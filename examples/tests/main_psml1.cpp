#define FILTER_TAPS 10

#include <psml.h>

#include <vector>
#include <iostream>
using namespace std;
std::vector<psml_uint<8> > input_data;
psml_uint<16> input_coef[FILTER_TAPS] = {12, 34, 65, 3, 2, 4, 56, 4, 3, 34};
//-----------------------------------------------
class dut : public psml_component
{
public:
    psml_proxy<psml_wire<bool > > terminated;
    psml_proxy<psml_wire<psml_uint<8> > > in_data[FILTER_TAPS - 1];
    psml_proxy<psml_wire<psml_uint<8> > > filter_output;

    psml_inport<psml_wire<bool > > terminated_;
    psml_inport<psml_wire<psml_uint<8> > > in_data_[FILTER_TAPS - 1];
    psml_outport<psml_wire<psml_uint<8> > > filter_output_;

    psml_process *p;

    class process_state : public psml_process_state
    {
    public:
        psml_uint<8> filter_output_function;
        psml_uint<8> in_data_read[9];
        psml_uint<16> coeff_read[9] ;
        process_state()
        {
        }
    };

    process_state _process_state;

    dut(const String &name = "")
	{
        set_name(name);

        register_proxy(terminated, terminated_);
        register_proxy(filter_output, filter_output_);
        for(UInt32 i = 0 ; i < FILTER_TAPS - 1 ; i++)
            register_proxy(in_data[i], in_data_[i]);

        p = register_special_process(process, &_process_state, name + "::process");
        p->register_port(terminated_);
        p->register_port(filter_output_);
        for(UInt32 i = 0 ; i < FILTER_TAPS - 1 ; i++)
            p->register_port(in_data_[i]);
        p->set_delay(psml_time(0, PSML_NS));
	}

    static void process(psml_process *ownerProcess)
    {
        dut *my = (dut *)ownerProcess->get_owner_component();
        process_state *state = (process_state *)ownerProcess->get_process_state();
        static psml_time delay = psml_time(1, PSML_NS);

        switch(state->get_label())
        {
            case 0: goto L0;
            case -2: return;
        }

        while(true)
        {
            if(my->terminated_->fetch())
                break;

            for(int i = 0 ; i < FILTER_TAPS - 1 ; i++)
            {
                state->in_data_read[i] = my->in_data_[i]->fetch();
                state->coeff_read[i] = input_coef[i];
            }

            // Filter function
            state->filter_output_function = my->fir_filter(state->in_data_read, state->coeff_read);

            my->filter_output_->put(state->filter_output_function, ownerProcess);


            psml_wait(delay, ownerProcess); state->set_label(0); return; L0: {}
        }

        state->set_label(-2);
    }

    // Filter function
    psml_uint<8> fir_filter( psml_uint<8>  *ary, psml_uint<16>  *coeff)
    {
        int sop=0;
        psml_uint <8> filter_result ;
        int i ;

        // Sum of product (SOP) generation
        for(i = 0 ; i< FILTER_TAPS - 1 ; i++)
        {
            sop += ary[i] * coeff[i] ;
        }

        // Sign adjustment and rounding to sc_unit <8>)
        if ( sop < 0 ){
            sop = 0 ;
        }

        filter_result = psml_uint<8>(sop);

        return filter_result;
    }
};
//-----------------------------------------------
class tb : public psml_component
{
public:

    psml_proxy<psml_wire<psml_uint<8> > > filter_output;
    psml_proxy<psml_wire<psml_uint<8> > > in_data[FILTER_TAPS - 1];
    psml_proxy<psml_wire<bool > > terminated;

    psml_inport<psml_wire<psml_uint<8> > > filter_output_;
    psml_outport<psml_wire<psml_uint<8> > > in_data_[FILTER_TAPS - 1];
    psml_outport<psml_wire<bool > > terminated_;

    psml_connector _terminated_c;
    psml_wire<bool> _terminated_w;
    psml_inport<psml_wire<bool > > terminated_receive;
    psml_outport<psml_wire<bool > > terminated_send;

    psml_process *p1;
    psml_process *p2;

    class send_process_state : public psml_process_state
    {
    public:
        unsigned int counter;
        unsigned int counter1;
        send_process_state()
        {
            counter = 0;
            counter1 = 0;
        }
    };

    class receive_process_state : public psml_process_state
    {
    public:
        unsigned int filter_out_write;
        receive_process_state()
        {
            filter_out_write = 0;
        }
    };

    send_process_state _send_process_state;
    receive_process_state _receive_process_state;

    tb(const String &name = "")
	{
        set_name(name);

        register_proxy(filter_output, filter_output_);
        register_proxy(terminated, terminated_);
        for(UInt32 i = 0 ; i < FILTER_TAPS - 1 ; i++)
            register_proxy(in_data[i], in_data_[i]);

        p1 = register_special_process(send_process, &_send_process_state, name + "::send_process");
        p1->register_port(terminated_);
        p1->register_port(terminated_send);
        for(UInt32 i = 0 ; i < FILTER_TAPS - 1 ; i++)
            p1->register_port(in_data_[i]);
        p1->set_delay(psml_time(0, PSML_NS));

        p2 = register_special_process(receive_process, &_receive_process_state, name + "::receive_process");
        p2->register_port(filter_output_);
        p2->register_port(terminated_receive);
        p2->set_delay(psml_time(0, PSML_NS));

        terminated_send.bind(_terminated_c, _terminated_w);
        terminated_receive.bind(_terminated_c, _terminated_w);
	}

    static void send_process(psml_process *ownerProcess)
    {
        tb *my = (tb *)ownerProcess->get_owner_component();
        send_process_state *state = (send_process_state *)ownerProcess->get_process_state();
        static psml_time delay = psml_time(1, PSML_NS);

        switch(state->get_label())
        {
            case 0: goto L0;
            case 1: goto L1;
            case -2: return;
        }

        my->terminated_->put(false, ownerProcess);
        my->terminated_send->put(false, ownerProcess);

        while(true)
        {
            //if(state->counter >= input_data.size())
            //    break;

            if(state->counter1 >= 10000000)
				break;
            if(state->counter == input_data.size())
				state->counter = 0;

            for(int i = 0 ; i < FILTER_TAPS - 1 ; i++)
            {
                //my->in_data_[i]->put(input_data[state->counter + i], ownerProcess);
                //state->counter++;
                my->in_data_[i]->put(input_data[i] & state->counter, ownerProcess);
            }

            state->counter1++;
            state->counter++;

            psml_wait(delay, ownerProcess); state->set_label(0); return; L0: {}
        }

        my->terminated_->put(true, ownerProcess);
        my->terminated_send->put(true, ownerProcess);

        psml_wait(delay, ownerProcess); state->set_label(1); return; L1: {}

        state->set_label(-2);
    }

    static void receive_process(psml_process *ownerProcess)
    {
        tb *my = (tb *)ownerProcess->get_owner_component();
        receive_process_state *state = (receive_process_state *)ownerProcess->get_process_state();
        static psml_time delay = psml_time(1, PSML_NS);

        switch(state->get_label())
        {
            case 0: goto L0;
            case -2: return;
        }

        while(true)
        {
            if(my->terminated_receive->fetch())
                break;

            state->filter_out_write = my->filter_output_->fetch();

            //std::cout << "@time " << psml_time_stamp(ownerProcess) << " " << state->filter_out_write << std::endl;

            psml_wait(delay, ownerProcess); state->set_label(0); return; L0: {}
        }

        state->set_label(-2);
    }
};
//-----------------------------------------------
class top// : public psml_component
{
public:

    dut d;
    tb t;

    psml_connector terminated_c, in_data_c[FILTER_TAPS - 1], filter_output_c;

    psml_wire<bool> terminated_w;
    psml_wire<psml_uint<8> >  in_data_w[FILTER_TAPS - 1];
    psml_wire<psml_uint<8> > filter_output_w;

    top(const String &name = "") : d("d"), t("t")
	{
        //set_name(name);

        //d = new dut("d");
        //t = new tb("t");

        for(int x = 0 ; x < FILTER_TAPS - 1 ; x++)
        {
            d.in_data[x].bind(in_data_c[x], in_data_w[x]);
            t.in_data[x].bind(in_data_c[x], in_data_w[x]);
        }

        d.terminated.bind(terminated_c, terminated_w);
        d.filter_output.bind(filter_output_c, filter_output_w);

        t.terminated.bind(terminated_c, terminated_w);
        t.filter_output.bind(filter_output_c, filter_output_w);
	}
};
//-----------------------------------------------
void build_inputs()
{
	ifstream *reader = new ifstream("fir_in_data.txt", ios::in);
	if(!reader->is_open())
	{
		std::cout << "Could not open the file fir_in_data.txt" << std::endl;
		abort();
    }
	//string line;
	int read;
	while(!reader->eof())
	{
        *reader >> read;
        input_data.push_back(read);
        //std::cout << read << std::endl;
	}

	reader->close();
	delete reader;
}
//----------------------------------------------------
int main(int argc, char * argv[])
{
    //for(int i = 0 ; i < 1000 ; i++)
    //    std::cout << "t" << i << ", ";
    //std::cout << std::endl; exit(0);

    build_inputs();

    Int32 retError = 0;

	Int32 numOfCores =  1;  //4096;
	UInt64 simUntil = 99999999999999; //100000;
	UInt32 numOfLogicalProcessors = 1;//132;

    // Verify two args were passed in
    if(argc == 2)
    {
        fprintf(stderr, "USAGE: ./tests_psml.exe <Integer value>\n");

        Int32 num = atoi(argv[1]);

        if(num <= 0)
        {
            fprintf(stderr, "USAGE: %d must be >= 1\n", num);
            exit(1);
        }
        else
        {
            numOfLogicalProcessors = num;
        }
    }

	psml_set_time_resolution(1, PSML_NS);

	try
	{
        //top tp("tp");
        top tps[96];

		// Initializes the simulator and time resolution in the entire simulation.
		psml_simulator sim = psml_simulator(numOfLogicalProcessors);
		// This must be come after psml_simulator.
		psml_time duration(simUntil, PSML_NS);
		//sim.start();
		sim.start(duration);

		//cout << "\n\n\n_____numberOfActivatedProcesses: " << numberOfActivatedProcesss << endl;
	}
	catch(System::Exception &e)
	{
		cout << "Caught exception: " << e.get_Message().get_BaseStream() << endl;
		retError = -1;
		goto Cleanup;
	}
	catch(...)
	{
		retError = -1;
		cout << "Unknown exception caught" << endl;
		goto Cleanup;
	}

	//top tp("tp");

	//top tps[10];

	//sc_start(100, SC_NS);
	//sc_start();

Cleanup:
	return retError;
}
//----------------------------------------------------
