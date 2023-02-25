#include <psml.h>
#define T unsigned int
#define DELAY 26120/2, PSML_US
//#define DELAY 1, PSML_NS

class MyRingBuffer
{
    /*---------------------fields----------------*/
    // volatile is only used to keep compiler from placing values in registers.
    private: /*volatile*/ int head; // UInt32 is to optimize n%DeltaQueueCapacity
    private: /*volatile*/ int tail;
    //private: LockFreeCounter count;
    private: unsigned int count;
    private: unsigned int  capacity;
    private: T *elements;
    /*---------------------methods----------------*/
    // MyRingBuffer Class constructor.
    public: MyRingBuffer(unsigned int capacity = 128)
    {
        this->capacity = capacity;
        elements = (T *)::malloc(capacity * sizeof(T));

        head = tail = -1;
        //count = LockFreeCounter();
        count = 0;
    }
    //----------------------------------------------------
    // MyRingBuffer Class destructor.
    public: ~MyRingBuffer()
    {
        //if(!disposed)
        {
            ::free(elements);
            //disposed = true;
        }
    }
    //----------------------------------------------------
    // Removes all items from the queue
    public: inline void Clear()
    {
        head = -1;
        tail = -1;
        //count = LockFreeCounter(0);
    }
    //----------------------------------------------------
    // Enters an item in the queue
    public: inline void Enqueue(T val)
    {
        if(count == capacity)
        {
            unsigned int capacity_ = capacity;
            capacity *= 2;
            T *elements_ = (T *)::malloc(capacity * sizeof(T));
            int head_ = head;
            for(register unsigned int i = 0 ; i < capacity_ ; i++)
            {
                head_++;
                register unsigned int index1 = head_ % capacity_;
                register unsigned int index2 = head_ % capacity;
                elements_[index2] = elements[index1];
            }
            ::free(elements);
            elements = elements_;
        }
        // Increments tail index
        tail++;
        register unsigned int index = tail % capacity;
        elements[index] = val;
        count++;
    }
    //----------------------------------------------------
    // Removes an item from the queue. Returns null if queue is empty.
    public: inline T Dequeue()
    {
        head++;
        register unsigned int index = head % capacity;
        T val = elements[index];
        count--;
        return val;
    }
    //----------------------------------------------------
    public: inline T Peek()
    {
        register unsigned int index = (head + 1) % capacity;
        //Object *obj = elements[index]->obj;
        //timestamp = elements[index]->timestamp;
        T val = elements[index];
        return val;
    }
    //----------------------------------------------------
    public: inline unsigned int GetCount()
    {
        return count;
    }
    //----------------------------------------------------
};

/*class writer : public psml_shared_interface
{
  public:
    virtual void write(T d) = 0;
};

class reader : public psml_shared_interface
{
  public:
    virtual T read(void) = 0;
};*/

class my_psml_fifo : public psml_shared_interface // public reader, public writer
{
    //private: MyRingBuffer<unsigned int> *rb;
    private: MyRingBuffer *rb;
    //private: sc_event e;
    //----------------------------------------------------
    public: my_psml_fifo()
    {
        //rb = new MyRingBuffer<unsigned int>(128);
        rb = new MyRingBuffer(128);
    }
    //----------------------------------------------------
    public: ~my_psml_fifo()
    {
        delete rb;
    }
    //----------------------------------------------------
    public: inline unsigned int read()
    {
        //if(rb->GetCount() == 0)
        //    wait(e);

        return rb->Dequeue();
    }
    //----------------------------------------------------
    public: inline void write(unsigned int val)
    {
        rb->Enqueue(val);

        //if(rb->GetCount() - 1 == 0)
        //    e.notify(SC_ZERO_TIME);
            //e.notify();
    }
    //----------------------------------------------------
    public: inline UInt32 get_count()
    {
        return rb->GetCount();
    }
    //----------------------------------------------------
    private: void Execute(InOut psml_remote_param_interface *param)
    {

    }
    public: void CallExecute(InOut psml_remote_param_interface *param)
    {

    }
    private: void EnterCriticalSection()
    {
    }
    private: void LeaveCriticalSection()
    {
    }
    private: void Serialize(In const psml_remote_param_interface *param, InOut char **buffer, In Int32 size, Out Int32 &newSize)
    {
    }
    private: virtual void Deserialize(In psml_remote_param_interface *param, In char *buffer, In Int32 size)
    {
    }
    //----------------------------------------------------
};
//-----------------------------------------------
class dut : public psml_component
{
public:
    psml_proxy<my_psml_fifo> in, out;
    psml_gport<my_psml_fifo> in_, out_;

    psml_process *p;

    class process_state : public psml_process_state
    {
    public:
        T temp, i;
        process_state()
        {
            i = temp = 0;
        }
    };

    process_state _process_state;

    dut(const String &name = "")
	{
        set_name(name);

        register_proxy(in, in_);
        register_proxy(out, out_);

        p = register_special_process(process, &_process_state, name + "::process");
        p->register_port(in_);
        p->register_port(out_);
        p->set_delay(psml_time(0, PSML_NS));
	}

    static void process(psml_process *ownerProcess)
    {
        dut *my = (dut *)ownerProcess->get_owner_component();
        process_state *state = (process_state *)ownerProcess->get_process_state();
        static psml_time delay = psml_time(DELAY);

        switch(state->get_label())
        {
            case 0: goto L0;
            case -2: return;
        }

        while(true)
        {
            //std::cout << "process " << my->in_->get_count() << std::endl;
            if(my->in_->get_count() != 0)
            {
                state->temp = my->in_->read();

                if(state->temp == 1)
                {
                    my->out_->write(1);
                    break;
                }

                for(int j = 0; j < 1000000 ; j++)
                    state->i += j;

                my->out_->write(state->temp);
            }

            psml_wait(delay, ownerProcess); state->set_label(0); return; L0: {}
        }

        state->set_label(-2);
    }
};
//-----------------------------------------------
class tb : public psml_component
{
public:

    //psml_proxy<psml_wire<psml_uint<8> > > filter_output;
    //psml_inport<psml_wire<psml_uint<8> > > filter_output_;
    psml_proxy<my_psml_fifo> in, out;
    psml_gport<my_psml_fifo> in_, out_;

    psml_process *p1;
    psml_process *p2;

    class send_process_state : public psml_process_state
    {
    public:
        T counter;
        T counter1;
        send_process_state()
        {
            counter = 0;
            counter1 = 2;
        }
    };

    class receive_process_state : public psml_process_state
    {
    public:
        T read;
        receive_process_state()
        {
        }
    };

    send_process_state _send_process_state;
    receive_process_state _receive_process_state;

    tb(const String &name = "")
	{
        set_name(name);

        register_proxy(in, in_);
        register_proxy(out, out_);

        p1 = register_special_process(send_process, &_send_process_state, name + "::send_process");
        p1->register_port(out_);
        p1->set_delay(psml_time(0, PSML_NS));

        p2 = register_special_process(receive_process, &_receive_process_state, name + "::receive_process");
        p2->register_port(in_);
        p2->set_delay(psml_time(0, PSML_NS));
	}

    static void send_process(psml_process *ownerProcess)
    {
        tb *my = (tb *)ownerProcess->get_owner_component();
        send_process_state *state = (send_process_state *)ownerProcess->get_process_state();
        static psml_time delay = psml_time(DELAY);

        switch(state->get_label())
        {
            case 0: goto L0;
            case -2: return;
        }

        while(true)
        {
			if(state->counter1 >= 10000)
			{
                my->out_->write(1);
				break;
            }

			my->out_->write(state->counter1);
			state->counter1++;

            psml_wait(delay, ownerProcess); state->set_label(0); return; L0: {}
        }

        state->set_label(-2);
    }

    static void receive_process(psml_process *ownerProcess)
    {
        tb *my = (tb *)ownerProcess->get_owner_component();
        receive_process_state *state = (receive_process_state *)ownerProcess->get_process_state();
        static psml_time delay = psml_time(DELAY);

        switch(state->get_label())
        {
            case 0: goto L0;
            case -2: return;
        }

        while(true)
        {
            //std::cout << "receive_process " << my->in_->get_count() << std::endl;

            if(my->in_->get_count() != 0)
            {
                state->read = my->in_->read();

                if(state->read == 1)
                    break;

                //std::cout << "@time " << psml_time_stamp(ownerProcess) << " " << state->read << std::endl;
            }

            psml_wait(delay, ownerProcess); state->set_label(0); return; L0: {}
        }

        state->set_label(-2);
    }
};
//-----------------------------------------------
class top : public psml_component
{
public:

    dut d;
    tb t;

    psml_connector in_c, out_c;
    my_psml_fifo in, out;

    top(const String &name = "") : d("d"), t("t")
	{
        d.in.bind(in_c, in);
		d.out.bind(out_c, out);

        t.in.bind(out_c, out);
		t.out.bind(in_c, in);
	}
};
//-----------------------------------------------
class Top : public psml_component
{
public:

	top t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24,\
        t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35;//, t36, t37, t38, t39, t40, t41, t42, t43, t44, t45, t46, t47;//

    Top(const String &name = "") : t0("t0"), t1("t1"), t2("t2"), t3("t3"), t4("t4"), t5("t5"), t6("t6"), t7("t7"), t8("t8"), t9("t9"), t10("t10"), t11("t11"), t12("t"), t13("t13"), t14("t14"), t15("t15"), t16("t16"), t17("t17"), t18("t18"), t19("t19"), t20("t20"), t21("t21"), t22("t22"), t23("t23"), t24("t24"),\
                   t25("t25"), t26("t26"), t27("t27"), t28("t28"), t29("t29"), t30("t30"), t31("t31"), t32("t32"), t33("t33"), t34("t34"), t35("t35")//, t36("t36"), t37("t37"), t38("t38"), t39("t39"), t40("t40"), t41("t41"), t42("t42"), t43("t43"), t44("t44"), t45("t45"), t46("t46"), t47("t47")
	{
	}
};
//----------------------------------------------------
int main(int argc, char * argv[])
{
    //for(int i = 0 ; i < 1000 ; i++)
    //    std::cout << "t" << i << ", ";
    //std::cout << std::endl; exit(0);

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

	//psml_set_time_resolution(1, PSML_NS);

	//try
	{
        Top t("t");
        //top tps[96];

		// Initializes the simulator and time resolution in the entire simulation.
		psml_simulator sim = psml_simulator(numOfLogicalProcessors);
		// This must be come after psml_simulator.
		psml_time duration(simUntil, PSML_NS);
		//sim.start();
		sim.start(duration);

		//cout << "\n\n\n_____numberOfActivatedProcesses: " << numberOfActivatedProcesss << endl;
	}
	/*catch(System::Exception &e)
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
	}*/

	//top tp("tp");

	//top tps[10];

	//sc_start(100, SC_NS);
	//sc_start();

Cleanup:
	return retError;
}
//----------------------------------------------------
