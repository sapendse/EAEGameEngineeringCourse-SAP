#include "WayPointGraph.h"

void Optimus::WayPointGraph::AddWayPoint( WayPointNode* i_node )
{
	m_listNodes.push_back( i_node );
}

Optimus::WayPointNode* Optimus::WayPointGraph::GetNeighbour( Optimus::Vector3& i_position )
{
	int retval = -1;
	for( unsigned int i = 0; i < m_listNodes.size(); i++ )
	{
		if( m_listNodes[i]->GetPosition() == i_position )
		{
			retval = i;
			break;
		}
	}
	return m_listNodes[retval];
}

Optimus::WayPointNode* Optimus::WayPointGraph::GetClosest( Optimus::Vector3& i_position )
{
	if( m_listNodes.size() == 0 )
		return NULL;
	int index = -1;
	Optimus::Vector3 _minDis = Optimus::Vector3( FLT_MAX );
	for( unsigned int i = 0; i < m_listNodes.size(); i++ )
	{
		if( ( i_position - m_listNodes[i]->GetPosition() ) < _minDis )
		{
			_minDis = i_position - m_listNodes[i]->GetPosition();
			index = i;
		}
	}
	assert( index != -1 );
	return m_listNodes[index];
}

void Optimus::WayPointGraph::CalculatePath( Optimus::Vector3& i_start, Optimus::Vector3& i_end, DWORD i_lineColor )
{
	if( m_listNodes.size() == 0 ) //If there are no way points; return immediately
		return;

	//Clear bad nodes and current path
	m_currentPath.clear();
	for( unsigned int i = 0; i < m_listNodes.size(); i++ )
	{
		if( m_listNodes[i]->GetClosedNode() )
		{
			m_listNodes[i]->SetClosedNode( false );
		}
	}

	WayPointNode* _currentNode;
	WayPointNode* _startNode;
	WayPointNode* _prevNode = NULL;
	Optimus::Vector3 _minDistance1 = Optimus::Vector3( FLT_MAX );
	Optimus::Vector3 _minDistance2 = Optimus::Vector3( FLT_MAX );
	unsigned int index1 = -1, index2 = -1;
	for( unsigned int i = 0; i < m_listNodes.size(); i++ )
	{
		if( ( i_start - m_listNodes[i]->GetPosition() ) < _minDistance1 )
		{
			_minDistance1 = ( i_start - m_listNodes[i]->GetPosition() );
			index1 = i;
		}
		if( ( i_end - m_listNodes[i]->GetPosition() ) < _minDistance2 )
		{
			_minDistance2 = ( i_end - m_listNodes[i]->GetPosition() );
			index2 = i;
		}
	}
	if( index1 >= 0 && index2 >= 0 )
	{
		_currentNode = m_listNodes[index1];
		_startNode = m_listNodes[index1];
		m_destinationNode = m_listNodes[index2];
	}

	while( _currentNode->GetID() != m_destinationNode->GetID() )
	{
		int _fastestIndex = -1;
		float _f = FLT_MAX;

		for( unsigned int i = 0; i < _currentNode->GetNeighbourCount(); i++ )
		{
			if( _currentNode->GetNeighbour( i ).m_neighbour->GetClosedNode() )
				continue;

			Optimus::Vector3 _distToDest = m_destinationNode->GetPosition() - _currentNode->GetNeighbour( i ).m_neighbour->GetPosition();
			float _distance = Optimus::Vector3::magSqr( _distToDest );
			_distance = std::sqrt(_distance);

			//f = g + h
			float _tempF = _currentNode->GetNeighbour( i ).m_distance + _distance;
			if( _tempF < _f )
			{
				_f = _tempF;
				_fastestIndex = i;
			}
		}

		if( _fastestIndex != -1 )
		{
			if( _prevNode != NULL && _prevNode->GetID() == _currentNode->GetNeighbour(_fastestIndex).m_neighbour->GetID() )
			{
				_currentNode->SetClosedNode( true );
			}
			//if( _currentNode->GetID() == _startNode->GetID() )
			//{
			//	_currentNode->SetClosedNode( true );
			//}
			m_currentPath.push_back( _currentNode );
			_prevNode = _currentNode;
			_currentNode = _currentNode->GetNeighbour( _fastestIndex ).m_neighbour;

			//Add destination node to current path to draw
			if( _currentNode->GetID() == m_destinationNode->GetID() )
				m_currentPath.push_back( _currentNode );
		}
		else
			break;
	}
	if( m_currentPath.size() != 0 )
	{
		/*if( _currentNode->GetID() == _destinationNode->GetID() )
		{
			m_currentPath.push_back( _destinationNode );
		}*/
		for( unsigned int i = 0; i < m_currentPath.size(); i++ )
		{
			if( i != m_currentPath.size() -1 )
			{
				Optimus::cSceneHandler::getInstance()->AddDebugLine( m_currentPath[i]->GetPosition() + Optimus::Vector3( 0.0f, 25.0f, 0.0f ), m_currentPath[i+1]->GetPosition() + Optimus::Vector3( 0.0f, 25.0f, 0.0f ), i_lineColor );
			}
		}
	}
}

void Optimus::WayPointGraph::ShutDown()
{
	for( unsigned int i = 0; i < m_listNodes.size(); i++ )
	{
		if( m_listNodes[i] )
		{
			delete m_listNodes[i];
			m_listNodes[i] = NULL;
		}
	}
	if( m_instance != NULL )
	{
		delete m_instance;
		m_instance = NULL;
	}
}