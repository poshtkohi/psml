/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_proxy_interface.h"
#include "psml_connector.h"
#include "psml_interface.h"
#include "psml_simulator.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		std::vector<psml_proxy_interface *> *psml_proxy_interface::__proxies = null;
		//----------------------------------------------------
		psml_proxy_interface::psml_proxy_interface()
		{
            if(__proxies == null)
                __proxies = new std::vector<psml_proxy_interface *>;

			__proxies->push_back(this);
		}
		//----------------------------------------------------
		psml_proxy_interface::~psml_proxy_interface()
		{
		}
		//----------------------------------------------------
		void psml_proxy_interface::Traverse(psml_proxy_interface *root, psml_connector *conn, psml_interface *_if)
		{
			std::map<psml_proxy_interface *, psml_proxy_interface *> *boundProxies = root->GetBoundProxies();

			if(boundProxies == null)
				return;

			if(boundProxies->size() == 0)
				return;

			///std::cout << "psml_proxy_interface::Traverse 2" << std::endl;

			std::string children;

			for(std::map<psml_proxy_interface *, psml_proxy_interface *>::iterator it = boundProxies->begin() ; it != boundProxies->end() ; ++it)
			{
				psml_proxy_interface *childProxy = it->first;

				children += " " + psml_simulator::to_string(childProxy);

				/*if(childProxy == root)
				{
					std::cout << "psml_proxy_interface::Traverse 3" << std::endl;
					continue;
				}*/

				if(childProxy->GetBoundConnector() != null)
					throw InvalidOperationException("Proxy (" + String(psml_simulator::to_string(childProxy).c_str()) + ", " + childProxy->GetName() + ") has already been bound to a connector and interface");

				childProxy->SetBoundConnector(conn);
				childProxy->SetBoundInterface(_if);

				Traverse(childProxy, conn, _if);
			}

			///std::cout << "psml_proxy_interface::Traverse root " << root << " boundProxies " << boundProxies->size() << " childeren " << childeren << std::endl;
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
