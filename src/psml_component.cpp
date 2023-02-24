/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_component.h"
#include "psml_proxy.h"

//**************************************************************************************************************//

//#define PROCESS_NAME(name, val) name = #val;

//#define DECLARE_AND_SET(type, varname, value) type varname = value; type orig_##varname = varname;

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		std::vector<psml_component *> *psml_component::__components = null;
		//std::map<psml_component *, psml_component *> psml_component::__components;
		//----------------------------------------------------
		psml_component::psml_component()
		{
			//std::cout << "psml_component::psml_component() " << std::endl;

            if(__components == null)
                __components = new std::vector<psml_component *>;

			__components->push_back(this);
		}
		//----------------------------------------------------
		psml_component::~psml_component()
		{
			/*cout << "psml_component::~psml_component()" << std::endl;

			for(register UInt32 i = 0 ; i < ___processes.size() ; i++)
			{
				ProcessInfo *pi = ___processes[i];
				pi->pm(pi->ownerComponent);
			}*/
		}
		//----------------------------------------------------
		psml_process *psml_component::register_process(ProcessMethod pm, bool stackful, const String &name)
		{
			if(pm == null)
				throw ArgumentNullException("pm", "pm is null");

			//const char *name;
			//PROCESS_NAME(name, pm);
			//cout << "process::name():: " << name << std::endl;
			//DECLARE_AND_SET( int, area, 2 * 6 );

			// We should check here if pm already is in __processes or not. A C++ map STL container
			// have optimum performance than a vector for a large number of registered processes.

			psml_process *p = new psml_process();
			p->_pm = pm;
			p->_objName = name;
			p->_ownerComponent = this;
			// Change __processes to map.
			if(stackful)
			{
				p->_fib_process = new Fiber(__PMSL_DEFAULT_STACK_SIZE, psml_process::Wrapper_To_Call_Fiber, (void *)p);
				p->_process_type = __STACKFULL_PROCESS__;
			}
			else
                p->_process_type = __STACKLESS_PROCESS__;
			psml_process::__processes->push_back(p);
			//psml_process::__processes[p] = p;
			return p;
			//cout << "psml_component::register_process() " << typeid(pi).name() << std::endl;
		}
        //----------------------------------------------------
		psml_process *psml_component::register_special_process(ProcessMethod pm, const psml_process_state *state, const String &name)
		{
			if(pm == null)
				throw ArgumentNullException("pm", "pm is null");

            if(state == null)
				throw ArgumentNullException("state", "state is null");

			psml_process *p = new psml_process();
			p->_pm = pm;
			p->_objName = name;
			p->_ownerComponent = this;
			p->_process_type = __SPECIAL_PROCESS__;
			p->_fib_process = null;
			p->_process_state = state;
			psml_process::__processes->push_back(p);
			//psml_process::__processes[p] = p;
			return p;
		}
		//----------------------------------------------------
		/*void psml_component::register_proxy(psml_proxy_interface *proxy, psml_port *port)
		{
			if(proxy == null)
				throw ArgumentNullException("proxy", "proxy is null");

			if(port == null)
				throw ArgumentNullException("port", "port is null");

			if(proxy->GetPort() != null)
				throw InvalidOperationException("The port has already been registered with the proxy");

			proxy->RegisterPort(port);
		}
		//----------------------------------------------------
		void psml_component::register_proxy(psml_proxy_interface &proxy, psml_port &port)
		{
			register_proxy(&proxy, &port);
		}*/
		//----------------------------------------------------
		const String &psml_component::GetName()
		{
			return _objName;
		}
		//----------------------------------------------------
		void psml_component::set_name(const String &name)
		{
			if(name == "")
				_objName = Guid().NewGuid();
			else
				_objName = name;
		}
		//----------------------------------------------------
		/*void psml_component::register_proxy(psml_proxy_interface *proxy, psml_port *ownerPort)
		{
			if(proxy == null)
				throw ArgumentNullException("proxy", "proxy is null");

			if(ownerPort == null)
				throw ArgumentNullException("ownerPort", "ownerPort is null");

			if(_proxies.count(proxy) != 0)
				throw InvalidOperationException("The proxy has already been registered with this component");

			if(proxy->GetOwnerPort() == ownerPort)
				throw InvalidOperationException("The proxy has already been registered with the port");

			if(ownerPort->GetBoundInterface() != null || ownerPort->GetBoundConnector() != 0)
				throw InvalidOperationException("An ownerPort bound to a pair of (connector, interface) can be registered with a proxy");

            proxy->SetOwnerComponent(this);
			proxy->SetOwnerPort(ownerPort);
			ownerPort->SetBoundProxy(proxy);

			_proxies[proxy] = proxy;
		}*/
		//----------------------------------------------------
		void psml_component::register_proxy(psml_proxy_interface *proxy, psml_port *port)
		{
			if(proxy == null)
				throw ArgumentNullException("proxy", "proxy is null");

			if(port == null)
				throw ArgumentNullException("port", "port is null");

			//if(_proxies.count(proxy) != 0)
			//	throw InvalidOperationException("The proxy has already been registered with this component");

			//if(proxy->GetOwnerPort() == ownerPort)
			//	throw InvalidOperationException("The proxy has already been registered with the port");

			/// ?
			if(port->GetBoundInterface() != null || port->GetBoundConnector() != null)
				throw InvalidOperationException("An ownerPort bound to a pair of (connector, interface) can be registered with a proxy");

			if(_proxies.count(proxy) == 0)
			{
				_proxies[proxy] = proxy;
				proxy->SetOwnerComponent(this);
			}
			//proxy->SetOwnerPort(ownerPort);

			std::map<psml_port *, psml_port *> *boundPorts = proxy->GetBoundPorts();

			if(boundPorts->count(port) != 0)
				throw InvalidOperationException("The proxy has already been registered with the port");

			port->SetBoundProxy(proxy);
			(*boundPorts)[port] = port;

			/// Structural Casuality
            // If there are any structural casuality errors in the simulation, we have to
            // suspect this fast for loop. Then, replace it as you have seen such behaviour.
            std::vector<psml_port *> *boundPortsVector = proxy->GetBoundPortsVector(); ///
			boundPortsVector->push_back(port); ///

		}
		//----------------------------------------------------
		void psml_component::register_proxy(psml_proxy_interface &proxy, psml_port &ownerPort)
		{
			register_proxy(&proxy, &ownerPort);
		}
		//----------------------------------------------------
		void psml_component::register_proxy(psml_proxy_interface *proxy)
		{
			if(proxy == null)
				throw ArgumentNullException("proxy", "proxy is null");

			if(_proxies.count(proxy) != 0)
				throw InvalidOperationException("The proxy has already been registered with this component");

            proxy->SetOwnerComponent(this);

			///proxy->SetOwnerPort(null);
			_proxies[proxy] = proxy;
		}
		//----------------------------------------------------
		void psml_component::register_proxy(psml_proxy_interface &proxy)
		{
			register_proxy(&proxy);
		}
		//----------------------------------------------------
		/*void psml_component::register_proxy_child(psml_proxy_interface *proxy, psml_port *child)
		{
			if(proxy == null)
				throw ArgumentNullException("proxy", "proxy is null");

			if(child == null)
				throw ArgumentNullException("child", "child is null");

			if(_proxies.count(proxy) == 0)
				throw InvalidOperationException("The proxy has not been registered with this component through the method psml_component::register_proxy(proxy, port)");

			if(proxy->GetOwnerPort() == child)
				throw InvalidOperationException("child has already been registered with the proxy as the owner(parent) port");

			if(child->GetBoundInterface() != null || child->GetBoundConnector() != 0)
				throw InvalidOperationException("A child port bound to a pair of (connector, interface) can be registered with a proxy");

			std::map<psml_port *, psml_port *> *boundPorts = proxy->GetBoundPorts();

			if(boundPorts->count(child) != 0)
				throw InvalidOperationException("The proxy has already been registered with the child port");

			child->SetBoundProxy(proxy);
			(*boundPorts)[child] = child;
		}
		//----------------------------------------------------
		/*void psml_component::register_proxy_child(psml_proxy_interface &proxy, psml_port &child)
		{
			register_proxy_child(&proxy, &child);
		}*/
		//----------------------------------------------------
		void psml_component::before_static_event_elaboration()
		{
		}
		//----------------------------------------------------
		void psml_component::before_end_of_elaboration()
		{
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
