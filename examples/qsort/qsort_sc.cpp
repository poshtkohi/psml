/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "qsort_sc.h"


void quicksort::run()
{

#if defined __FiberLess_Enabled__
    data[counter] = indata.read();

    //std::cout << "@time " << sc_time_stamp();\
    std::cout << " in run() read " << data[counter] << " write " << data_out[counter] << std::endl;

    odata.write(data_out[counter]);

    counter++;

    if(counter == SIZE)
    {
        sort(data);

        for(int x = 0 ; x < SIZE ; x++)
        {
            data_out[x] = data[x];
        }

        counter = 0;
    }
#else
    counter = 0;
    sc_uint<32>  data[SIZE], data_out[SIZE];
    memset(data, 0, sizeof(sc_int<32>) * SIZE);
	memset(data_out, 0, sizeof(sc_int<32>) * SIZE);

	wait();
	//wait(1, SC_NS);

	while(true)
	{
		for(int x = 0 ; x < SIZE ; x++)
		{
			data[x] = indata.read();
			odata.write(data_out[x]);
            //std::cout << "@time " << sc_time_stamp();\
            std::cout << " in run() read " << data[x] << " write " << data_out[x] << std::endl;
			wait();
			//wait(1, SC_NS);
		}


		// Main quick sort routine
		sort(data);


		for(int x = 0 ; x < SIZE ; x++)
		{
			data_out[x] = data[x];
		}
		//  wait() ;

		counter++;
	}
#endif
}
//--------------------------
// Main sorting function
//-------------------------
void quicksort::sort(sc_uint<32> *arr)
{


  // Variables declaration
  int  piv, beg[SIZE], end[SIZE], i=0, L, R;

  beg[0]=0;
  end[0]=SIZE;

  while (i>=0) {

    L=beg[i]; R=end[i]-1;
    if (L<R) {
      piv=arr[L];

      while (L<R) {
        while (arr[R]>=piv && L<R)
	  R--;
	if (L<R)
	  arr[L++]=arr[R];

	while (arr[L]<=piv && L<R)
	  L++;
	if(L<R)
	  arr[R--]=arr[L];
      }

      arr[L]=piv;
      beg[i+1]=L+1;
      end[i+1]=end[i];
      end[i++]=L;

      if (end[i]-beg[i]>end[i-1]-beg[i-1]){
	swap(&end[i], &beg[i]);

      }
    }

    else{
      i--;
    }
  } // end while

}

void quicksort::swap(int *end, int *beg){

  int swap;

  swap=*beg;
  *beg=*(beg-1);
  *(beg-1)=swap;

  swap=*end;
  *end=*(end-1);
  *(end-1)=swap;

}
