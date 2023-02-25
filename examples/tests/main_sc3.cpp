// Description : ADPC: Adaptive Differential Pulse-Code | FFmpeg

#include <systemc.h>

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
class dut : sc_module
{
public:
    SC_HAS_PROCESS(dut);

    sc_in<bool> terminated;
    sc_in <sc_uint <8> > input_row[SIZE_BUFFER];
    sc_out<sc_uint<8> > output_row;

    sc_uint<8>  line_buffer[SIZE_BUFFER][SIZE_BUFFER];

    dut(sc_module_name mn): sc_module(mn)
	{
		SC_THREAD(process);
	}

	void process()
	{
        sc_uint<8> input_row_read[3];
        sc_uint<8> output_row_write;
        int i,j,x;

		while(true)
		{
            if(terminated.read())
                break;

              // Reading triplet data needed for filter
              for(x=0;x<ROWS; x++){
            for(j=1;j<COLS;j++){
              for(i=j-1;i<= j+1;i++){

                if(i== j-1)
                  input_row_read[0] = input_row[0].read();
                if(i==j)
                  input_row_read[1] = input_row[1].read();
                else
                  input_row_read[2] = input_row[2].read();
              }

              // Perform the filtering of a 3x3 pixel image segment
             output_row_write =  sobel_filter(input_row_read);


              // Writting filtered output back
              output_row.write(output_row_write);
              wait(1, SC_NS);
            }
            }
		}
	}

	sc_uint<8>  sobel_filter(sc_uint<8> *input_row_r)
    {

      unsigned int X, Y;
      unsigned char orow;
      int sumX, sumY;
      int SUM, rowOffset, colOffset;



      char Gx[3][3] ={{-1 ,-2 ,-1},
              { 0, 0, 0},
              { +1, 2, +1}};


      char Gy[3][3] ={{1, -2, 1},
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
          return ((sc_uint<8>) orow);

    }
};
//-----------------------------------------------
class tb : sc_module
{
public:
    SC_HAS_PROCESS(tb);

    sc_out<bool> terminated;
    sc_signal<bool> _terminated;
    sc_in<sc_uint<8> > output_row;
    sc_out<sc_uint <8> > input_row[3];

    tb(sc_module_name mn): sc_module(mn)
	{
		SC_THREAD(send_process);
		SC_THREAD(receive_process);
	}

	void send_process()
	{
        int i,j,k;
        sc_uint<8> input_row_write[3];
        i=0;
        j=0;

		while(true)
		{

            // Send Image data to sobel filter
            for(i=0;i<ROWS;i++){
                for(j=1;j<COLS;j++){
                for(k=j-1;k<=j+1;k++){

                     // Send triplets at a time
                     if(k==j-1){
                       input_row_write[0] = bitmapData[(i*ROWS)+k];
                       input_row[0].write(input_row_write[0]);
                     }
                     else if(k==j){
                       input_row_write[1] = bitmapData[(i*ROWS)+k];
                       input_row[1].write(input_row_write[1]);
                     }
                     else{
                       input_row_write[2]= bitmapData[(i*ROWS)+k];
                       input_row[2].write(input_row_write[2]);
                     }
                   }

                    wait(1, SC_NS);
                 }

             }

             break;
		}

        terminated.write(true);
        _terminated.write(true);
        wait(1, SC_NS);
        //std::cout << "send_process was terminated" << std::endl;
		//sc_stop();
	}

    void receive_process()
	{
        sc_uint<8> output_row_read;
        int f=0,k,m;

		while(true)
		{
            for(k=0;k<ROWS;k++)
            {
                for(m=1;m<COLS;m++)
                {
                    if(terminated.read())
                        goto End;
                    output_row_read = output_row.read();
                    wait(1, SC_NS);
                }
            }
		}
    End: {}
        //std::cout << "receive_process was terminated" << std::endl;
		//sc_stop();
	}
};
//-----------------------------------------------
class top : sc_module
{
public:
    SC_HAS_PROCESS(top);

    dut d;
    tb t;
    sc_signal<bool> terminated;
    sc_signal<sc_uint<8> > input_row[3];
    sc_signal<sc_uint<8> > output_row;

    top(sc_module_name mn = ""): sc_module(mn), d("d"), t("t")
	{
	}

    void before_end_of_elaboration()
    {
        d.terminated.bind(terminated);
        d.output_row(output_row);
        for(int i =0 ; i < 3 ; i++)
            d.input_row[i](input_row[i]);

        t.terminated.bind(terminated);
        t.output_row(output_row);
        for(int i = 0 ; i < 3 ; i++)
            t.input_row[i](input_row[i]);
    }
};
//----------------------------------------------------
int sc_main(int argc, char * argv[])
{
    bitmapData = load_bitmapfile("lena512.bmp");

	sc_set_time_resolution(1, SC_NS);

	//top tp("tp");
	top tps[480];

	//sc_start(1000, SC_NS);
	sc_start();

	return 0;
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
