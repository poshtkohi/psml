/**
	#define meta ...
	printf("%s\n", meta);
**/

/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_clock_edge_type_h__
#define __Parvicursor_psml_psml_clock_edge_type_h__

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------

		enum psml_clock_edge_type
		{
            PSML_NEG_EDGE,
            PSML_POS_EDGE,
            PSML_BOTH_EDGES
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
