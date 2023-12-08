#ifndef OCTREE_BUILDER_H
#define OCTREE_BUILDER_H

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <vector>
#include "..\Optimus\_Source\Math\Vector3.h"
#include "..\Optimus\_Source\Utilities\New.h"

namespace OctreeBuilder
{
	struct s_vertex
	{
		float x, y, z;
		D3DCOLOR color;	

		float u,v;
		float nx,ny,nz;
		float tanx, tany, tanz;
		float bix, biy, biz;	
	};

	struct s_mesh
	{
		unsigned int m_primitiveCount, m_indexCount;
		unsigned int* m_indices;
		s_vertex* m_vertices;
	};

	struct Triangle
	{
		Optimus::Vector3 m_first, m_second, m_third;
	public:
		Triangle( Optimus::Vector3& i_first, Optimus::Vector3& i_second, Optimus::Vector3& i_third ) :
		m_first( i_first ),
		m_second( i_second ),
		m_third( i_third )
		  {}
	};

	struct OctreeNode
	{
		OctreeNode*								m_children[8];
		std::vector<OctreeBuilder::Triangle>	m_listOrderedPoints;
		Optimus::Vector3						m_halfDimension, m_center;
		int										m_currentDepth;
		bool									m_isLeafNode;
		
	public:
		OctreeNode() :
		m_isLeafNode( false ),
		m_currentDepth( 0 ),
		m_halfDimension( 0.0f ),
		m_center( 0.0f )
		{
			for( unsigned int i = 0; i < 8; i++ )
				m_children[i] = NULL;
		}
	};


	struct Octree
	{
		OctreeNode* m_rootNode;
		int			m_granularityLevel;
	public:
		//Constructor
		Octree( OctreeNode* i_rootNode, int i_granularityLevel ) :
		m_rootNode( i_rootNode ),
		m_granularityLevel( i_granularityLevel )
		{}
	};

	class OctreeCreator
	{
	public:		
		static OctreeCreator* m_instance;

		static OctreeCreator* getInstance()
		{
				if( m_instance == 0 )
					m_instance = new OctreeCreator();
				return m_instance;
		}

		Octree*			m_octree;
		s_mesh*			m_meshes;
		unsigned int	m_entityCount;

		void	CreateOctree();
		void	ReadMeshData();
		void	CreateRootNode();
		bool	ShouldPushDown( OctreeNode* i_node, Optimus::Vector3& i_first, Optimus::Vector3& i_second, Optimus::Vector3& i_third );
		void	FillData( OctreeNode* i_node, std::vector<Triangle>& i_listOrderedPoints );
		void	Divide( OctreeNode* i_node );
		void	WriteData();
		void	SortByDFS( OctreeNode* i_node, std::vector<OctreeNode*>& i_sortedNodes );
		void	UnSort( OctreeNode* i_node, std::vector<OctreeNode*>& i_sortedNodes, int i_gran );
		void	ReadOctree();
		void	Shutdown();
	};
}

#endif