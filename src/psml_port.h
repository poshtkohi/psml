/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_port_h__
#define __Parvicursor_psml_psml_port_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>

#include "psml_edge_trigger_type.h"

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
		enum psml_port_type {__UnknownPort, __InPort, __OutPort, __GeneralPort};

		// Forward declarations
		class psml_connector;
		class psml_component;
		class psml_process;
		class psml_interface;
		class psml_simulator;
		class psml_nonshared_interface;
		class psml_pdes_lp;
		class psml_pdes_event;
		class psml_proxy_interface;
		class psml_time;
		template <class IF> class psml_proxy;
		template <class T> class psml_wire;
		template <class IF> class psml_inport;
		template <class IF> class psml_outport;
		template <class IF> class psml_gport;
		//enum psml_edge_trigger_type;
		class psml_clock;
		class psml_connector;

		interface class psml_port : public Object
		{
			friend class psml_process;
			friend class psml_component;
			friend class psml_simulator;
			friend class psml_nonshared_interface;
			friend class psml_pdes_lp;
			friend class psml_clock;
			friend class psml_connector;
			template <class IF> friend class psml_proxy;
			template <class T> friend class psml_wire;
			template <class IF> friend class psml_inport;
			template <class IF> friend class psml_outport;
			template <class IF> friend class psml_gport;
			/*---------------------fields-----------------*/
			private: bool _sensitive;
			private: UInt16 _portID;
			private: psml_process *_ownerProcess;
			private: psml_connector *_connector;
			private: psml_interface *_interface;
			private: static std::vector<psml_port *> *__ports;
			/*---------------------methods----------------*/
			protected: psml_port();
			protected: ~psml_port();
			//public: psml_port() : portID(-1), psml_port_type(__UnknownPort) { }
			// Binds the port to a connector specified by conn.
			//public: virtual void bind(psml_connector *conn, psml_interface *_if) {}
			//public: virtual void bind(psml_connector *conn, psml_interface *_if) = 0;
			// Gets the bound connector instance to this port. This method can by used by the psml framework.
			protected: inline psml_connector *GetBoundConnector() const { return _connector; }
			// Sets the bound connector instance to this port. This method can by used by the psml framework.
			protected: inline void SetBoundConnector(psml_connector *conn) { _connector = conn; }
			// Gets the bound interface instance to this port. This method can by used by the psml framework.
			protected: inline psml_interface *GetBoundInterface() { return _interface; }
			// Sets the bound interface instance to this port. This method can by used by the psml framework.
			protected: inline void SetBoundInterface(psml_interface *_if) { _interface = _if; }
			// Gets the bound proxy instance to this port. This method can by used by the psml framework.
			protected: virtual psml_proxy_interface *GetBoundProxy() = 0;
			// Gets the bound proxy instance to this port. This method can by used by the psml framework.
			protected: virtual void SetBoundProxy(psml_proxy_interface *proxy) = 0;
			// Sets the owner component of this port. This method can be only used by the psml framework.
			protected: virtual void SetOwnerComponent(psml_component *owner) = 0;
			// Gets the owner component of this port. This method can be only used by the psml framework.
			protected: virtual psml_component *GetOwnerComponent() = 0;
			// Sets the owner process of this port. This method can be only used by the user.
			protected: inline void SetOwnerProcess(psml_process *owner) { _ownerProcess = owner; }
			// Gets the owner process of this port. This method can be only used by the psml framework.
			protected: inline psml_process *GetOwnerProcess() const { return _ownerProcess; };
			// Gets the port ID. This method can only be used by the psml framework.
			protected: inline UInt16 GetPortID() const { return _portID; }
			// Sets the port ID. This method can only be used by the psml framework.
			protected: inline void SetPortID(UInt16 id) { _portID = id; }
			// Gets the port type. This method can only be used by the psml framework.
			protected: virtual psml_port_type GetPortType() = 0;//{ return psml_port_type; }
			// Sets the port type. This method can only be used by the psml framework.
			//protected: inline void Setpsml_port_type(psml_port_type type) { psml_port_type = type; }
			// Gets the name of the port instance.
			public: virtual const String &GetName() const = 0;
			// The following two methods are only used for psml_inport type.
			// Sets the sensitivity of the port.
			protected: inline void SetSensitive(bool sensitive) { _sensitive = sensitive; }
			// Gets the sensitivity of the port.
			protected: inline bool GetSensitive() { return _sensitive; }
			// The following two methods are only used for psml_inport type.
			// Sets the sensitivity of the port based on clock edges.
			protected: virtual void SetEdgeTriggerType(psml_edge_trigger_type type);
			// Gets the clock edge type of the port.
			protected: virtual psml_edge_trigger_type GetEdgeTriggerType();
			//protected: virtual psml_port *GetBoundPort() = 0;
			protected: virtual psml_pdes_event *GetEvent();
			protected: virtual void SetEvent(psml_pdes_event *e);
		};
		//----------------------------------------------------
	};
};

//**************************************************************************************************************//

#endif
