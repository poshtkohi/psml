#include <systemc.h>
//#include "my_sc_fifo.h"

#define T unsigned int
#define DELAY 26120/2, SC_US
//#define DELAY 1, SC_NS
//template <class T>
class RingBuffer
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
    // RingBuffer Class constructor.
    public: RingBuffer(unsigned int capacity = 128)
    {
        this->capacity = capacity;
        elements = (T *)::malloc(capacity * sizeof(T));

        head = tail = -1;
        //count = LockFreeCounter();
        count = 0;
    }
    //----------------------------------------------------
    // RingBuffer Class destructor.
    public: ~RingBuffer()
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

class writer : virtual public sc_interface
{
  public:
    virtual void write(unsigned int d) = 0;
    virtual unsigned int get_count() = 0;
};

class reader : virtual public sc_interface
{
  public:
    virtual unsigned int read(void) = 0;
    virtual unsigned int get_count() = 0;
};

class my_sc_fifo : public sc_channel, public reader, public writer
{
    //private: RingBuffer<unsigned int> *rb;
    private: RingBuffer *rb;
    private: sc_event e;
    //----------------------------------------------------
    public: my_sc_fifo(sc_module_name name) : sc_channel(name)
    {
        //rb = new RingBuffer<unsigned int>(128);
        rb = new RingBuffer(128);
    }
    //----------------------------------------------------
    public: ~my_sc_fifo()
    {
        delete rb;
    }
    //----------------------------------------------------
    public: unsigned int read()
    {
        //if(rb->GetCount() == 0)
        //    wait(e);

        return rb->Dequeue();
    }
    //----------------------------------------------------
    public: void write(unsigned int val)
    {
        rb->Enqueue(val);

        //if(rb->GetCount() - 1 == 0)
        //    e.notify(SC_ZERO_TIME);
            //e.notify();
    }
    //----------------------------------------------------
    public: inline unsigned get_count()
    {
        return rb->GetCount();
    }
    //----------------------------------------------------
};

//-----------------------------------------------
//class dut : sc_module
SC_MODULE(dut)
{
//public:
    //SC_HAS_PROCESS(dut);

	//sc_in<sc_uint<32> > in;
    //sc_out<sc_uint<32> > out;
    sc_port<reader> in;
    sc_port<writer> out;

    struct state
    {
        T temp;
		T i;
    };
    state s;

    //dut(sc_module_name mn): sc_module(mn)
    SC_CTOR(dut)
	{
		SC_THREAD(process);
		s.i = 0;
	}

	void process(void)
	{
        // Variables declaration
		//sc_uint<32> temp;
		//unsigned int i = 0;

		while(true)
		{
			s.temp = in->read();

			if(s.temp == 1)
			{
                out->write(1);
                break;
            }

            for(int j = 0 ; j < 1000000 ; j++)
                s.i += j;

            out->write(s.temp);
            //i++;

			wait(DELAY);
			//wait(1);
		}
	}
};
//-----------------------------------------------
//class tb : sc_module
SC_MODULE(tb)
{
//public:
    //SC_HAS_PROCESS(tb);

    //sc_in<sc_uint<32> > in;
	//sc_out<sc_uint<32> > out;
    sc_port<reader> in;
    sc_port<writer> out;

    //tb(sc_module_name mn): sc_module(mn)
    SC_CTOR(tb)
	{
		SC_THREAD(send_process);
		SC_THREAD(receive_process);
	}

	void send_process(void)
	{
        unsigned int counter = 0;
		unsigned int counter1 = 2;

		while(true)
		{
            //if(counter >= input_data.size())
            //    break;
			if(counter1 >= 10000)
			{
                out->write(1);
				break;
            }

			out->write(counter1);
			counter1++;

			//wait(1, SC_NS);
			wait(DELAY);
		}

        //std::cout << "send_process was terminated" << std::endl;
		//sc_stop();
	}

    void receive_process(void)
	{
        unsigned int read = 0;

		while(true)
		{
            read = in->read();

            if(read == 1)
				break;

            //std::cout << "@time " << sc_time_stamp() << " " << read << std::endl;
            //std::cout << "@time " << sc_time_stamp() << std::endl;

			//wait(1, SC_NS);
			wait(DELAY);
		}

        //std::cout << "receive_process was terminated" << std::endl;
		//sc_stop();
	}
};
//-----------------------------------------------
//class top : sc_module
SC_MODULE(top)
{
    //SC_HAS_PROCESS(top);

    dut d;
    tb t;

	//sc_signal<sc_uint<32> > in, out;
    my_sc_fifo in, out;

    //top(sc_module_name mn = ""): sc_module(mn), d("d"), t("t")
    SC_CTOR(top) : d("d"), t("t"), in("in"), out("out")
	{
	}

    void before_end_of_elaboration()
    {
        d.in.bind(in);
		d.out.bind(out);

        t.in.bind(out);
		t.out.bind(in);
    }
};
//-----------------------------------------------
//class Top : sc_module
SC_MODULE(Top)
{
//public:

	top t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24,\
        t25, t26, t27, t28, t29, t30, t31, t32, t33, t34, t35;//, t36, t37, t38, t39, t40, t41, t42, t43, t44, t45, t46, t47;

    //Top(sc_module_name mn): sc_module(mn)
    SC_CTOR(Top) : t0("t0"), t1("t1"), t2("t2"), t3("t3"), t4("t4"), t5("t5"), t6("t6"), t7("t7"), t8("t8"), t9("t9"), t10("t10"), t11("t11"), t12("t"), t13("t13"), t14("t14"), t15("t15"), t16("t16"), t17("t17"), t18("t18"), t19("t19"), t20("t20"), t21("t21"), t22("t22"), t23("t23"), t24("t24"),\
                   t25("t25"), t26("t26"), t27("t27"), t28("t28"), t29("t29"), t30("t30"), t31("t31"), t32("t32"), t33("t33"), t34("t34"), t35("t35")//, t36("t36"), t37("t37"), t38("t38"), t39("t39"), t40("t40"), t41("t41"), t42("t42"), t43("t43"), t44("t44"), t45("t45"), t46("t46"), t47("t47")
	{
	}
};
//----------------------------------------------------
int sc_main(int argc, char * argv[])
{
    //sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING );
	//sc_set_time_resolution(1, SC_NS);

	Top t("t");

	//sc_start(100, SC_NS);
	sc_start();

	return 0;
}
//----------------------------------------------------
