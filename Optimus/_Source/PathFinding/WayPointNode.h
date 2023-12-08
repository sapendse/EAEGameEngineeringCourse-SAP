#ifndef _WAYPOINT_NODE_H
#define _WAYPOINT_NODE_H

#include <vector>
#include "..\Renderer\cScene.h"
#include <math.h>

namespace Optimus
{
	struct WayPointNeighbour;

	class WayPointNode
	{
	private:
		std::vector<WayPointNeighbour>	m_neighbours;
		TriggerBox*						m_wayPointTriggerBox;
		bool							m_closedNode;
		unsigned int					m_ID;

	public:
		WayPointNode( Optimus::Vector3& i_position, int i_ID );
		WayPointNode() {}
		~WayPointNode();

		Optimus::Vector3 GetPosition();
		TriggerBox* GetTriggerBox();
		unsigned int GetNeighbourCount();
		unsigned int	GetID();
		void AddNeighbour( WayPointNode* i_node );
		void DeleteLastNeighbour();
		void SetClosedNode( bool i_badNode );
		bool GetClosedNode();
		WayPointNeighbour GetNeighbour( unsigned int i_index );

	};

	struct WayPointNeighbour
	{
	public:
		WayPointNode*	m_neighbour;
		float			m_distance;

		WayPointNeighbour() : m_neighbour( NULL ) {}
	};
}

#endif