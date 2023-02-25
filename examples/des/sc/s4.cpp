/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "s4.h"

void s4::s4_box()
{
  switch(stage1_input.read())
    {
    case 0:
      stage1_output.write(7);
      break;
    case 1:
      stage1_output.write(13);
      break;
    case 2:
      stage1_output.write(13);
      break;
    case 3:
      stage1_output.write(8);
      break;
    case 4:
      stage1_output.write(14);
      break;
    case 5:
      stage1_output.write(11);
      break;
    case 6:
      stage1_output.write(3);
      break;
    case 7:
      stage1_output.write(5);
      break;
    case 8:
      stage1_output.write(0);
      break;
    case 9:
      stage1_output.write(6);
      break;
    case 10:
      stage1_output.write(6);
      break;
    case 11:
      stage1_output.write(15);
      break;
    case 12:
      stage1_output.write(9);
      break;
    case 13:
      stage1_output.write(0);
      break;
    case 14:
      stage1_output.write(10);
      break;
    case 15:
      stage1_output.write(3);
      break;
    case 16:
      stage1_output.write(1);
      break;
    case 17:
      stage1_output.write(4);
      break;
    case 18:
      stage1_output.write(2);
      break;
    case 19:
      stage1_output.write(7);
      break;
    case 20:
      stage1_output.write(8);
      break;
    case 21:
      stage1_output.write(2);
      break;
    case 22:
      stage1_output.write(5);
      break;
    case 23:
      stage1_output.write(12);
      break;
    case 24:
      stage1_output.write(11);
      break;
    case 25:
      stage1_output.write(1);
      break;
    case 26:
      stage1_output.write(12);
      break;
    case 27:
      stage1_output.write(10);
      break;
    case 28:
      stage1_output.write(4);
      break;
    case 29:
      stage1_output.write(14);
      break;
    case 30:
      stage1_output.write(15);
      break;
    case 31:
      stage1_output.write(9);
      break;
    case 32:
      stage1_output.write(10);
      break;
    case 33:
      stage1_output.write(3);
      break;
    case 34:
      stage1_output.write(6);
      break;
    case 35:
      stage1_output.write(15);
      break;
    case 36:
      stage1_output.write(9);
      break;
    case 37:
      stage1_output.write(0);
      break;
    case 38:
      stage1_output.write(0);
      break;
    case 39:
      stage1_output.write(6);
      break;
    case 40:
      stage1_output.write(12);
      break;
    case 41:
      stage1_output.write(10);
      break;
    case 42:
      stage1_output.write(11);
      break;
    case 43:
      stage1_output.write(1);
      break;
    case 44:
      stage1_output.write(7);
      break;
    case 45:
      stage1_output.write(13);
      break;
    case 46:
      stage1_output.write(13);
      break;
    case 47:
      stage1_output.write(8);
      break;
    case 48:
      stage1_output.write(15);
      break;
    case 49:
      stage1_output.write(9);
      break;
    case 50:
      stage1_output.write(1);
      break;
    case 51:
      stage1_output.write(4);
      break;
    case 52:
      stage1_output.write(3);
      break;
    case 53:
      stage1_output.write(5);
      break;
    case 54:
      stage1_output.write(14);
      break;
    case 55:
      stage1_output.write(11);
      break;
    case 56:
      stage1_output.write(5);
      break;
    case 57:
      stage1_output.write(12);
      break;
    case 58:
      stage1_output.write(2);
      break;
    case 59:
      stage1_output.write(7);
      break;
    case 60:
      stage1_output.write(8);
      break;
    case 61:
      stage1_output.write(2);
      break;
    case 62:
      stage1_output.write(4);
      break;
    case 63:
      stage1_output.write(14);
      break;
    }

}
