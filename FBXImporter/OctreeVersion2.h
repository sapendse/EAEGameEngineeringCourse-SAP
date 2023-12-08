#ifndef OCTREETEST_H
#define OCTREETEST_H

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <vector>
#include "..\Optimus\_Source\Math\Vector3.h"
#include "..\Optimus\_Source\Utilities\New.h"

namespace OctreeBuilder2
{
	struct s_vertex2
	{
		float x, y, z;
		D3DCOLOR color;	

		float u,v;
		float nx,ny,nz;
		float tanx, tany, tanz;
		float bix, biy, biz;	
	};

	struct s_mesh2
	{
		unsigned int m_primitiveCount, m_indexCount;
		unsigned int* m_indices;
		s_vertex2* m_vertices;
	};

	struct Triangle2
	{
		Optimus::Vector3 m_first, m_second, m_third;
	public:
		Triangle2( Optimus::Vector3& i_first, Optimus::Vector3& i_second, Optimus::Vector3& i_third ) :
		m_first( i_first ),
		m_second( i_second ),
		m_third( i_third )
		  {}
	};

	struct OctreeNode2
	{
		std::vector<OctreeNode2>					m_children;
		std::vector<OctreeBuilder2::Triangle2>	m_listOrderedPoints;
		Optimus::Vector3						m_halfDimension, m_center;
		int										m_currentDepth;
		bool									m_isLeafNode;

	public:
		OctreeNode2() :
		m_halfDimension( Optimus::v_Zero ),
		m_center( Optimus::v_Zero ),
		m_isLeafNode( false ),
		m_currentDepth( 0 )
		{

		}
	};

	struct Octree2
	{
		OctreeNode2								m_rootNode;
		int										m_granLevel;

	public:
		Octree2(){}

		Octree2( OctreeNode2 i_rootNode, int i_granularityLevel ) :
		m_rootNode( i_rootNode ),
		m_granLevel( i_granularityLevel )
		{}
	};

	class OctreeCreator2
	{
		public:		
		static OctreeCreator2* m_instance;

		static OctreeCreator2* getInstance()
		{
				if( m_instance == 0 )
					m_instance = OPT_NEW OctreeCreator2();
				return m_instance;
		}

		Octree2			m_octree;
		s_mesh2*			m_meshes;
		unsigned int	m_entityCount;

		void	CreateOctree();
		void	ReadMeshData();
		void	CreateRootNode();
		bool	ShouldPushDown( OctreeNode2& i_node, Optimus::Vector3& i_first, Optimus::Vector3& i_second, Optimus::Vector3& i_third );
		void	FillData( OctreeNode2& i_node, std::vector<Triangle2>& i_listOrderedPoints );
		void	Divide( OctreeNode2& i_node );
		void	WriteData();
		void	SortByDFS( OctreeNode2& i_node, std::vector<OctreeNode2>& i_sortedNodes );
		void	UnSort( OctreeNode2& i_node, std::vector<OctreeNode2>& i_sortedNodes, int i_gran );
		void	ReadOctree();
		void	Shutdown();
	};

}


#endif