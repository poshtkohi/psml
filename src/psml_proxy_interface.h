/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_proxy_interface_h__
#define __Parvicursor_psml_psml_proxy_interface_h__

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
#include <map>

using namespace System;

using namespace std;

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations
		class psml_port;
		class psml_component;
		class psml_process;
		class psml_simulator;
		class psml_interface;
		class psml_connector;

		interface class psml_proxy_interface : public Object
		{
			friend class psml_component;
			friend class psml_process;
			friend class psml_simulator;
			/*---------------------fields-----------------*/
			private: static std::vector<psml_proxy_interface *> *__proxies;
			/*---------------------methods----------------*/
			protected: psml_proxy_interface();
			protected: ~psml_proxy_interface();
			public: virtual const String &GetName() = 0;
		    // Registers a port with this proxy.
			protected: virtual void SetOwnerPort(psml_port *port) = 0;
		    // Get the port registered with this proxy.
			protected: virtual psml_port *GetOwnerPort() = 0;
		    // Gets the bound ports of this proxy. Only internal ports to a component can be bound as child
			// ports to a proxy.
			protected: virtual std::map<psml_port *, psml_port *> *GetBoundPorts() = 0;
			/// Structural Casuality
            // If there are any structural casuality errors in the simulation, we have to
            // suspect this fast for loop. Then, replace it as you have seen such behaviour.
			protected: virtual std::vector<psml_port *> *GetBoundPortsVector() = 0;
			// Gets the bound interface to this proxy. This interface may be bound explicitly by user
			// to a connector through a port or implicitly by PSML framework for hierarchical components.
			protected: virtual psml_interface *GetBoundInterface() = 0;
			protected: virtual void SetBoundInterface(psml_interface *_if) = 0;
			// Gets the bound connector to this proxy. This connector may be bound explicitly by user
			// to an interface through a port or implicitly by PSML framework for hierarchical components.
			protected: virtual psml_connector *GetBoundConnector() = 0;
			protected: virtual void SetBoundConnector(psml_connector *conn) = 0;
		    // Gets the bound proxies to this proxy instance. This is mainly used in the case of connecting
			// components in a hierarchical fashion.
			protected: virtual std::map<psml_proxy_interface *, psml_proxy_interface *> *GetBoundProxies() = 0;
			// Registers a component instance with this proxy.
			protected: virtual void SetOwnerComponent(psml_component *ownerComponent) = 0;
            // Gets the owner component of this proxy.
			protected: virtual psml_component *GetOwnerComponent() = 0;
			protected: void Traverse(psml_proxy_interface *root, psml_connector *conn, psml_interface *_if);
		};
		//----------------------------------------------------
	};
};

//**************************************************************************************************************//

#endif
