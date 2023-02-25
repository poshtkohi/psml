#include <systemc.h>

template <class T>
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
};

class reader : virtual public sc_interface
{
  public:
    virtual unsigned int read(void) = 0;
};

class my_sc_fifo : public sc_channel, public reader, public writer
{
    private: RingBuffer<unsigned int> *rb;
    private: sc_event e;
    //----------------------------------------------------
    public: my_sc_fifo(sc_module_name name) : sc_channel(name)
    {
        rb = new RingBuffer<unsigned int>(128);
    }
    //----------------------------------------------------
    public: ~my_sc_fifo()
    {
        delete rb;
    }
    //----------------------------------------------------
    public: unsigned int read()
    {
        if(rb->GetCount() == 0)
            wait(e);

        return rb->Dequeue();
    }
    //----------------------------------------------------
    public: void write(unsigned int val)
    {
        rb->Enqueue(val);

        if(rb->GetCount() - 1 == 0)
            e.notify(SC_ZERO_TIME);
    }
    //----------------------------------------------------
};
