/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_interface_h__
#define __Parvicursor_psml_psml_interface_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>

#include <iostream>
#include <string>
#include <vector>

using namespace System;

using namespace std;

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		enum pdsml_interface_type { __UnknownInteface, __NonSharedInterface, __SharedInterface };

		// Forward declarations
		class psml_simulator;
		class psml_pdes_lp;
		class psml_port;
		class psml_process;
		class psml_connector;
		class psml_proxy_interface;
		template <class IF> class psml_inport;
		template <class IF> class psml_outport;
		template <class IF> class psml_proxy;

		interface class psml_interface : public Object
		{
			friend class psml_simulator;
			friend class psml_pdes_lp;
			friend class psml_port;
			friend class psml_process;
			friend class psml_proxy_interface;
			friend class psml_connector;
			template <class IF> friend class psml_inport;
			template <class IF> friend class psml_outport;
			template <class IF> friend class psml_proxy;
			/*---------------------fields-----------------*/
			private: psml_connector *_ownerConnector;
			private: static std::vector<psml_interface *> *__interfaces;
			/*---------------------methods----------------*/
			protected: psml_interface();
			protected: ~psml_interface();
			// Gets the interface type. This method can only be used by the psml framework.
			protected: virtual pdsml_interface_type GetInterfaceType() = 0;
			protected: inline psml_connector *GetOwnerConnector() { return _ownerConnector; }
			protected: inline void RegisterOwnerConnector(psml_connector *conn) { _ownerConnector = conn; }
			// This method is invoked after elaboration phase.
			public: virtual void before_static_event_elaboration();
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
