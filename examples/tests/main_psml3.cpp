// Description : Sobel Filter

#include <psml.h>

#include <vector>
#include <iostream>
using namespace std;

unsigned char *load_bitmapfile(const char *image);
unsigned char *bitmapData;

#include <stdio.h>

//#define RESHD ///

// High Resolution case 1920x1080
#ifdef  RESHD
   #define BYTES_PER_PIXEL 3
   #define COLS 1920* BYTES_PER_PIXEL
   #define ROWS 1080* BYTES_PER_PIXEL
   #define SIZE COLS*ROWS
   #define IMAGE_IN "batmanjoker.bmp"
   #define IMAGE_OUT "batmanjoker_out.bmp"
   #define IMAGE_GOLDEN "batmanjoker_golden.bmp"


// 512x512 case
#else

   #define BYTES_PER_PIXEL 1
   #define COLS 512 * BYTES_PER_PIXEL
   #define ROWS 512 * BYTES_PER_PIXEL
   #define SIZE ROWS* COLS
   #define IMAGE_IN "lena512.bmp"
   #define IMAGE_OUT "lena512_out.bmp"
   #define IMAGE_GOLDEN "lena512_golden.bmp"

#endif

#define DIFFFILENAME "sobel_diff.txt"

#define SIZE_BUFFER 3

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef char BYTE;

#pragma pack(push,1)
typedef struct {
  WORD  bfType;                       // The type of the image

  DWORD bfSize;                       //size of the file
  WORD  bfReserved;                 // reserved type
  WORD  bfReserved2;
  DWORD bfOffBits;                   //offset bytes from the file header to the image data
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;                   //the size of the header
  LONG  biWidth;                 //the width in pixels
  LONG  biHeight;                //the height in pixels
  WORD  biPlanes;                //the no. of planes in the bitmap
  WORD  biBitCount;              //bits per pixel
  DWORD biCompression;           //compression specifications
  DWORD biSizeImage;            //size of the bitmap data
  LONG  biXPelsPerMeter;        //horizontal res(pixels per meter)
  LONG  biYPelsPerMeter;        //vertical res(pixels per meter)
  DWORD biClrUsed;              //colours used in the image
  DWORD biClrImportant;        //num of important colours used
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack(pop)

//-----------------------------------------------
class dut : public psml_component
{
public:
    psml_proxy<psml_wire<bool > > terminated;
    psml_proxy<psml_wire<psml_uint<8> > > input_row[SIZE_BUFFER];
    psml_proxy<psml_wire<psml_uint<8> > > output_row;

    psml_inport<psml_wire<bool > > terminated_;
    psml_inport<psml_wire<psml_uint<8> > > _input_row[SIZE_BUFFER];
    psml_outport<psml_wire<psml_uint<8> > > _output_row;

    psml_process *p;
    psml_uint<8> line_buffer[SIZE_BUFFER][SIZE_BUFFER];

    class process_state : public psml_process_state
    {
    public:
        public: int j, x;
        process_state()
        {
        }
    };

    process_state _process_state;

    dut(const String &name = "")
	{
        set_name(name);

        register_proxy(terminated, terminated_);
        register_proxy(output_row, _output_row);
        for(register UInt32 i = 0 ; i < SIZE_BUFFER ; i++)
            register_proxy(input_row[i], _input_row[i]);

        p = register_special_process(process, &_process_state, name + "::process");
        p->register_port(terminated_);
        p->register_port(_output_row);
        for(register UInt32 i = 0 ; i < SIZE_BUFFER ; i++)
            p->register_port(_input_row[i]);
        p->set_delay(psml_time(0, PSML_NS));
	}

    static void process(psml_process *owner)
    {
        dut *myComponent = (dut *)owner->get_owner_component();
        process_state *myState = (process_state *)owner->get_process_state();
        static psml_time delay = psml_time(1, PSML_NS);

        psml_uint<8> output_row_write;
        psml_uint<8> input_row_read[3];
        int i;

        switch(myState->get_label())
        {
            case 0: goto L0;
            case -2: return;
        }

        while(true)
        {
            if(myComponent->terminated_->fetch())
                break;

            // Reading triplet data needed for filter
            for(myState->x = 0 ; myState->x < ROWS ; myState->x++)
            {
                for(myState->j = 1 ; myState->j < COLS ; myState->j++)
                {
                    for(i = myState->j - 1 ; i <= myState->j + 1 ; i++)
                    {
                        if(i == myState->j - 1)
                            input_row_read[0] = myComponent->_input_row[0]->fetch();
                        else if(i == myState->j)
                            input_row_read[1] = myComponent->_input_row[1]->fetch();
                        else
                            input_row_read[2] = myComponent->_input_row[2]->fetch();
                    }

                    // Perform the filtering of a 3x3 pixel image segment
                    output_row_write = myComponent->sobel_filter(input_row_read);

                    // Writting filtered output back
                    myComponent->_output_row->put(output_row_write, owner);
                    psml_wait(delay, owner); myState->set_label(0); return; L0: {}
                }
            }
        }

        myState->set_label(-2);
    }

    psml_uint<8> sobel_filter(psml_uint<8> *input_row_r)
    {

      unsigned int X, Y;
      unsigned char orow;
      int sumX, sumY;
      int SUM, rowOffset, colOffset;



      const char Gx[3][3] ={{-1 ,-2 ,-1},
              { 0, 0, 0},
              { +1, 2, +1}};


      const char Gy[3][3] ={{1, -2, 1},
              {0, 0, 0},
              {-1, 0, 1}};


      /* Shifting 3x3 line buffer by one row  */

       for(Y=2;Y>0;Y--){
         for(X=0;X< 3;X++){
           line_buffer[Y][X]=line_buffer[Y-1][X];
         }
       }

       // Reading new data into the line buffer
       for(X=0; X<SIZE_BUFFER; X++)
         line_buffer[0][X] = input_row_r[X];



       Y=1;
       X=1;
       sumX = 0;
       sumY = 0;

       // Convolution starts here
       //-------X GRADIENT APPROXIMATION------
       //-------Y GRADIENT APPROXIMATION------
       for(rowOffset = -1; rowOffset <= 1; rowOffset++){
         for(colOffset = -1; colOffset <=1; colOffset++){
           sumX = sumX + line_buffer[Y -rowOffset][X+colOffset] * Gx[1+rowOffset][1+colOffset];
           sumY = sumY + line_buffer[Y -rowOffset][X+colOffset] * Gy[1+rowOffset][1+colOffset];
        }
        }


          if(sumX > 255)    sumX = 255;
          else if(sumX < 0) sumX = 0;

          if(sumY > 255)    sumY = 255;
          else if(sumY < 0) sumY = 0;


          SUM = sumX + sumY;

          if(SUM > 255)    SUM = 255;
             else if(SUM < 0) SUM = 0;

          orow = 255  - (unsigned char)(SUM);
          return ((psml_uint<8>) orow);

    }
};
//-----------------------------------------------
class tb : public psml_component
{
public:
    psml_proxy<psml_wire<psml_uint<8> > > output_row;
    psml_proxy<psml_wire<psml_uint<8> > > input_row[3];
    psml_proxy<psml_wire<bool > > terminated;

    psml_inport<psml_wire<psml_uint<8> > > _output_row;
    psml_outport<psml_wire<psml_uint<8> > > _input_row[3];
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
        int i, j;
        send_process_state()
        {
            counter = 0;
            counter1 = 0;
            i = j = 0;
        }
    };

    class receive_process_state : public psml_process_state
    {
    public:
        int k, m;
        receive_process_state()
        {
        }
    };

    send_process_state _send_process_state;
    receive_process_state _receive_process_state;

    tb(const String &name = "")
	{
        set_name(name);

        register_proxy(terminated, terminated_);
        register_proxy(output_row, _output_row);
        for(register UInt32 i = 0 ; i < SIZE_BUFFER ; i++)
            register_proxy(input_row[i], _input_row[i]);

        p1 = register_special_process(send_process, &_send_process_state, name + "::send_process");
        p1->register_port(terminated_);
        p1->register_port(terminated_send);
        for(register UInt32 i = 0 ; i < SIZE_BUFFER ; i++)
            p1->register_port(_input_row[i]);
        p1->set_delay(psml_time(0, PSML_NS));

        p2 = register_special_process(receive_process, &_receive_process_state, name + "::receive_process");
        p2->register_port(_output_row);
        p2->register_port(terminated_receive);
        p2->set_delay(psml_time(0, PSML_NS));

        terminated_send.bind(_terminated_c, _terminated_w);
        terminated_receive.bind(_terminated_c, _terminated_w);
	}

    static void send_process(psml_process *owner)
    {
        tb *myComponent = (tb *)owner->get_owner_component();
        send_process_state *myState = (send_process_state *)owner->get_process_state();
        static psml_time delay = psml_time(1, PSML_NS);

        int k;
        psml_uint<8> input_row_write[3];

        switch(myState->get_label())
        {
            case 0: goto L0;
            case 1: goto L1;
            case -2: return;
        }

        myComponent->terminated_->put(false, owner);
        myComponent->terminated_send->put(false, owner);

        while(true)
        {
            // Send Image data to sobel filter
            for(myState->i = 0 ; myState->i < ROWS ; myState->i++)
            {
                for(myState->j = 1 ; myState->j < COLS ; myState->j++)
                {
                    for(k = myState->j - 1 ; k <= myState->j + 1 ; k++)
                    {
                        // Send triplets at a time
                        if(k == myState->j - 1)
                        {
                            input_row_write[0] = bitmapData[(myState->i * ROWS) + k];
                            myComponent->_input_row[0]->put(input_row_write[0], owner);
                        }
                        else if(k == myState->j)
                        {
                            input_row_write[1] = bitmapData[(myState->i * ROWS) + k];
                            myComponent->_input_row[1]->put(input_row_write[1], owner);
                        }
                        else
                        {
                            input_row_write[2] = bitmapData[(myState->i * ROWS) + k];
                            myComponent->_input_row[2]->put(input_row_write[2], owner);
                        }
                    }

                    psml_wait(delay, owner); myState->set_label(0); return; L0: {}
                }
            }

            break;
        }

        myComponent->terminated_->put(true, owner);
        myComponent->terminated_send->put(true, owner);

        psml_wait(delay, owner); myState->set_label(1); return; L1: {}

        myState->set_label(-2);
    }

    static void receive_process(psml_process *owner)
    {
        tb *myComponent = (tb *)owner->get_owner_component();
        receive_process_state *myState = (receive_process_state *)owner->get_process_state();
        static psml_time delay = psml_time(1, PSML_NS);

        psml_uint<8> output_row_read;

        switch(myState->get_label())
        {
            case 0: goto L0;
            case -2: return;
        }

        while(true)
        {
            for(myState->k = 0 ; myState->k < ROWS ; myState->k++)
            {
                for(myState->m = 1 ; myState->m < COLS ; myState->m++)
                {
                    output_row_read = myComponent->_output_row->fetch();
                    ///bitmapFinalImage[(myState->k * ROWS) + myState->m]= output_row_read;
                    if(myComponent->terminated_receive->fetch())
                        goto End;

                    psml_wait(delay, owner); myState->set_label(0); return; L0: {}
                }
            }
        }

    End:
        myState->set_label(-2);
    }
};
//-----------------------------------------------
class top : public psml_component
{
public:

    dut d;
    tb t;

    psml_connector terminated_c, input_row_c[3], output_row_c;

    psml_wire<bool> terminated_w;
    psml_wire<psml_uint<8> > input_row_w[3], output_row_w;

    top(const String &name = "") : d("d"), t("t")
	{
        set_name(name);

        d.terminated.bind(terminated_c, terminated_w);
        d.output_row.bind(output_row_c, output_row_w);
		for(register UInt32 i = 0 ; i < SIZE_BUFFER ; i++)
			d.input_row[i].bind(input_row_c[i], input_row_w[i]);

        t.terminated.bind(terminated_c, terminated_w);
		t.output_row.bind(output_row_c, output_row_w);
		for(register UInt32 i = 0 ; i < SIZE_BUFFER ; i++)
			t.input_row[i].bind(input_row_c[i], input_row_w[i]);
	}
};
//----------------------------------------------------
int main(int argc, char * argv[])
{
    //for(int i = 0 ; i < 1000 ; i++)
    //    std::cout << "t" << i << ", ";
    //std::cout << std::endl; exit(0);

    bitmapData = load_bitmapfile("lena512.bmp");

    Int32 retError = 0;

	Int32 numOfCores =  1;  //4096;
	UInt64 simUntil = 9999999999999; //100000;
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
        top tps[480];

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

Cleanup:
	return retError;
}
//----------------------------------------------------
//
//  Bitmap loading function
//
unsigned char *load_bitmapfile(const char *image)
{
    unsigned char *bitmapImage;
    int l;

    BITMAPINFOHEADER bitmapInfoHeader;
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    unsigned char biColourPalette[1024];

    // Open bmp file to be filtered
    FILE *ifptr = fopen(image,"rb");
    if(!ifptr){
        cout << "Cannot open the file " << image << endl;
        exit(-1);
    }


    if(ifptr == NULL)
    {
        exit (-1);
    }

    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,ifptr);

#ifdef PRINTD
    printf("%c Bitmap identifies\n",bitmapFileHeader.bfType);
    printf("%d Bitmap identifies the size of file\n",bitmapFileHeader.bfSize);
#endif

    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(ifptr);
    }

    // Read the bitmap info header
    fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,ifptr);

    // Read colour palette
    fread(&biColourPalette,1,bitmapInfoHeader.biClrUsed*4,ifptr);

#ifdef PRINTD
    printf("\n%d the size of info header",bitmapInfoHeader.biSize);
    printf("\n%x the horizontal width",bitmapInfoHeader.biWidth);
    printf("\n%x the vertical height",bitmapInfoHeader.biHeight);
    printf("\n%x num of planes",bitmapInfoHeader.biPlanes);
    printf("\n%d bits per pixel",bitmapInfoHeader.biBitCount);
    printf("\n%d compression specs",bitmapInfoHeader.biCompression);
    printf("\n%d the size of the image",bitmapInfoHeader.biSizeImage);
    printf("\n%x the num of colours used",bitmapInfoHeader.biClrUsed);

#endif


    //move file point to the begging of bitmap data
    fseek(ifptr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader.biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        cout << endl << "Cannot allocate memory for image" << endl;
        free(bitmapImage);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,1, bitmapInfoHeader.biSizeImage,ifptr);

    if (bitmapImage == NULL)
    {
        cout << endl << "Data could not be read" << endl;
        return NULL;
    }

#ifdef PRINTD
    cout << endl << "Finished reading bmp file" << endl;

#endif


    fclose(ifptr);
    return bitmapImage;
}
//----------------------------------------------------
