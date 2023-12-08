#include "WayPointNode.h"

Optimus::WayPointNode::WayPointNode( Optimus::Vector3& i_position, int i_ID )
{
	m_wayPointTriggerBox = Optimus::cSceneHandler::getInstance()->AddTriggerBox( i_position, Optimus::Vector3( 50.0f ) );
	m_ID = i_ID;
}

Optimus::WayPointNode::~WayPointNode()
{

}

Optimus::Vector3 Optimus::WayPointNode::GetPosition()
{
	return m_wayPointTriggerBox->GetCenter();
}

Optimus::TriggerBox* Optimus::WayPointNode::GetTriggerBox()
{
	return m_wayPointTriggerBox;
}

unsigned int Optimus::WayPointNode::GetNeighbourCount()
{
	return m_neighbours.size();
}

unsigned int Optimus::WayPointNode::GetID()
{
	return m_ID;
}

void Optimus::WayPointNode::SetClosedNode( bool i_badNode )
{
	m_closedNode = i_badNode;
}

bool Optimus::WayPointNode::GetClosedNode()
{
	return m_closedNode;
}

void Optimus::WayPointNode::AddNeighbour( WayPointNode* i_node )
{
	WayPointNeighbour _neighbour;
	_neighbour.m_neighbour = i_node;
	Optimus::Vector3 _diff = m_wayPointTriggerBox->GetCenter() - i_node->GetPosition();
	_neighbour.m_distance = std::sqrt( Optimus::Vector3::magSqr(_diff) );
	m_neighbours.push_back( _neighbour );
}

void Optimus::WayPointNode::DeleteLastNeighbour()
{
	if( !m_neighbours.empty() )
		m_neighbours.pop_back();
}

Optimus::WayPointNeighbour Optimus::WayPointNode::GetNeighbour( unsigned int i_index )
{
	if( i_index < m_neighbours.size() )
		return m_neighbours[i_index];
	else
		return WayPointNeighbour();
}