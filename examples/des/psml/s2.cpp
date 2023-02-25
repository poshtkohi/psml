

#include "s2.h"


void s2::s2_box(psml_process *owner)
{
	s2 *my = (s2 *)owner->get_owner_component();

  switch(my->stage1_input_s2_box->fetch())
    {
    case 0:
      my->stage1_output_s2_box->put(15, owner);
      break;
    case 1:
      my->stage1_output_s2_box->put(3, owner);
      break;
    case 2:
      my->stage1_output_s2_box->put(1, owner);
      break;
    case 3:
      my->stage1_output_s2_box->put(13, owner);
      break;
    case 4:
      my->stage1_output_s2_box->put(8, owner);
      break;
    case 5:
      my->stage1_output_s2_box->put(4, owner);
      break;
    case 6:
      my->stage1_output_s2_box->put(14, owner);
      break;
    case 7:
      my->stage1_output_s2_box->put(7, owner);
      break;
    case 8:
      my->stage1_output_s2_box->put(6, owner);
      break;
    case 9:
      my->stage1_output_s2_box->put(15, owner);
      break;
    case 10:
      my->stage1_output_s2_box->put(11, owner);
      break;
    case 11:
      my->stage1_output_s2_box->put(2, owner);
      break;
    case 12:
      my->stage1_output_s2_box->put(3, owner);
      break;
    case 13:
      my->stage1_output_s2_box->put(8, owner);
      break;
    case 14:
      my->stage1_output_s2_box->put(4, owner);
      break;
    case 15:
      my->stage1_output_s2_box->put(14, owner);
      break;
    case 16:
      my->stage1_output_s2_box->put(9, owner);
      break;
    case 17:
      my->stage1_output_s2_box->put(12, owner);
      break;
    case 18:
      my->stage1_output_s2_box->put(7, owner);
      break;
    case 19:
      my->stage1_output_s2_box->put(0, owner);
      break;
    case 20:
      my->stage1_output_s2_box->put(2, owner);
      break;
    case 21:
      my->stage1_output_s2_box->put(1, owner);
      break;
    case 22:
      my->stage1_output_s2_box->put(13, owner);
      break;
    case 23:
      my->stage1_output_s2_box->put(10, owner);
      break;
    case 24:
      my->stage1_output_s2_box->put(12, owner);
      break;
    case 25:
      my->stage1_output_s2_box->put(6, owner);
      break;
    case 26:
      my->stage1_output_s2_box->put(0, owner);
      break;
    case 27:
      my->stage1_output_s2_box->put(9, owner);
      break;
    case 28:
      my->stage1_output_s2_box->put(5, owner);
      break;
    case 29:
      my->stage1_output_s2_box->put(11, owner);
      break;
    case 30:
      my->stage1_output_s2_box->put(10, owner);
      break;
    case 31:
      my->stage1_output_s2_box->put(5, owner);
      break;
    case 32:
      my->stage1_output_s2_box->put(0, owner);
      break;
    case 33:
      my->stage1_output_s2_box->put(13, owner);
      break;
    case 34:
      my->stage1_output_s2_box->put(14, owner);
      break;
    case 35:
      my->stage1_output_s2_box->put(8, owner);
      break;
    case 36:
      my->stage1_output_s2_box->put(7, owner);
      break;
    case 37:
      my->stage1_output_s2_box->put(10, owner);
      break;
    case 38:
      my->stage1_output_s2_box->put(11, owner);
      break;
    case 39:
      my->stage1_output_s2_box->put(1, owner);
      break;
    case 40:
      my->stage1_output_s2_box->put(10, owner);
      break;
    case 41:
      my->stage1_output_s2_box->put(3, owner);
      break;
    case 42:
      my->stage1_output_s2_box->put(4, owner);
      break;
    case 43:
      my->stage1_output_s2_box->put(15, owner);
      break;
    case 44:
      my->stage1_output_s2_box->put(13, owner);
      break;
    case 45:
      my->stage1_output_s2_box->put(4, owner);
      break;
    case 46:
      my->stage1_output_s2_box->put(1, owner);
      break;
    case 47:
      my->stage1_output_s2_box->put(2, owner);
      break;
    case 48:
      my->stage1_output_s2_box->put(5, owner);
      break;
    case 49:
      my->stage1_output_s2_box->put(11, owner);
      break;
    case 50:
      my->stage1_output_s2_box->put(8, owner);
      break;
    case 51:
      my->stage1_output_s2_box->put(6, owner);
      break;
    case 52:
      my->stage1_output_s2_box->put(12, owner);
      break;
    case 53:
      my->stage1_output_s2_box->put(7, owner);
      break;
    case 54:
      my->stage1_output_s2_box->put(6, owner);
      break;
    case 55:
      my->stage1_output_s2_box->put(12, owner);
      break;
    case 56:
      my->stage1_output_s2_box->put(9, owner);
      break;
    case 57:
      my->stage1_output_s2_box->put(0, owner);
      break;
    case 58:
      my->stage1_output_s2_box->put(3, owner);
      break;
    case 59:
      my->stage1_output_s2_box->put(5, owner);
      break;
    case 60:
      my->stage1_output_s2_box->put(2, owner);
      break;
    case 61:
      my->stage1_output_s2_box->put(14, owner);
      break;
    case 62:
      my->stage1_output_s2_box->put(15, owner);
      break;
    case 63:
      my->stage1_output_s2_box->put(9, owner);
      break;

    }
}
