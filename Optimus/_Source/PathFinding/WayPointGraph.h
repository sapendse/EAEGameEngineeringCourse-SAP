#ifndef _WAYPOINT_GRAPH_H
#define _WAYPOINT_GRAPH_H

#include "WayPointNode.h"

namespace Optimus
{
	class WayPointGraph
	{
	private:
		static WayPointGraph* m_instance;
		WayPointGraph() {}

	public:
		std::vector<WayPointNode*>	m_listNodes;
		std::vector<WayPointNode*>	m_currentPath;
		WayPointNode*				m_destinationNode;
		static WayPointGraph* GetInstance()
		{
			if( m_instance == NULL )
				m_instance = OPT_NEW WayPointGraph();
			return m_instance;
		}
		void CalculatePath( Optimus::Vector3& i_start, Optimus::Vector3& i_end, DWORD i_lineColor );
		void AddWayPoint( WayPointNode* i_node );
		WayPointNode* GetNeighbour( Optimus::Vector3& i_position );
		WayPointNode* GetClosest( Optimus::Vector3& i_position );
		void ShutDown();
	};
}

#endif