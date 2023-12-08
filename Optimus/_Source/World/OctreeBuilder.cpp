#include "OctreeBuilder.h"
#include <fstream>

static int count = 0;

void OctreeBuilder::OctreeCreator::ReadMeshData()
{
	unsigned int entityCount, effectFileLength, textureFileLength, normalMapFileLength, m_specExpo, alphaValue, primitiveCount, indexCount, m_dataReadTillNow = 0;
	char c;
	std::ifstream scenefile( "../Assets/Scenes/Collisionsceneinfo.txt", std::ifstream::in );
	while( !scenefile.eof() )
	{
		scenefile >> c;
		if( c == '-' )
		{
			scenefile >>entityCount;
		}
	}
	scenefile.close();
	OctreeBuilder::OctreeCreator::getInstance()->m_entityCount = entityCount;
	m_meshes = new s_mesh[entityCount];
		
	std::ifstream meshfile( "../Assets/Meshes/Collision.bin", std::ifstream::in | std::ifstream::binary );
	for( unsigned int itr = 0; itr < entityCount; itr++ )
	{
		meshfile.read(  reinterpret_cast<char*>(&effectFileLength), sizeof( unsigned int ) );
		char* effectExpected = new char[effectFileLength];
		meshfile.read(  reinterpret_cast<char*>(effectExpected), ( effectFileLength + 1 ) );
		meshfile.read(  reinterpret_cast<char*>(&textureFileLength), sizeof( unsigned int ) );
		char* textureExpected = new char[textureFileLength];
		meshfile.read(  reinterpret_cast<char*>(textureExpected), ( textureFileLength + 1 ) );
		meshfile.read(  reinterpret_cast<char*>(&normalMapFileLength), sizeof( unsigned int ) );
		char* normalMapTexture = new char[normalMapFileLength];
		meshfile.read(  reinterpret_cast<char*>(normalMapTexture), ( normalMapFileLength + 1 ) );
		meshfile.read(  reinterpret_cast<char*>(&m_specExpo), sizeof( unsigned int ) );
		meshfile.read(  reinterpret_cast<char*>(&alphaValue), sizeof( unsigned int ) );
		meshfile.read(  reinterpret_cast<char*>(&primitiveCount), sizeof( unsigned int ) );
		meshfile.read(  reinterpret_cast<char*>(&indexCount), sizeof( unsigned int ) );

		s_vertex* vertexData = new s_vertex[primitiveCount];
		for( unsigned int c=0; c< primitiveCount; c++ )
		{
			s_vertex temp;
			meshfile.read( reinterpret_cast<char*>(&temp), sizeof( s_vertex ) );
			vertexData[c] = temp;
		}
		OctreeBuilder::OctreeCreator::getInstance()->m_meshes[itr].m_primitiveCount = primitiveCount;
		OctreeBuilder::OctreeCreator::getInstance()->m_meshes[itr].m_vertices = vertexData;
		unsigned int* indexData = new unsigned int[primitiveCount];
		for( unsigned int c =0; c< indexCount; c++ )
		{
			unsigned int temp1;
			meshfile.read( reinterpret_cast<char*>(&temp1), sizeof( unsigned int ) );
			indexData[c] = temp1;
		}
		Optimus::Vector3 temp;
		int tempID;
		bool tempDynamic;
		meshfile.read( reinterpret_cast<char*>(&temp), sizeof( Optimus::Vector3 ) );
		meshfile.read( reinterpret_cast<char*>(&temp), sizeof( Optimus::Vector3 ) );
		meshfile.read( reinterpret_cast<char*>(&tempID), sizeof( unsigned int ) );
		meshfile.read( reinterpret_cast<char*>(&tempDynamic), sizeof( bool ) );
		OctreeBuilder::OctreeCreator::getInstance()->m_meshes[itr].m_indexCount = indexCount;
		OctreeBuilder::OctreeCreator::getInstance()->m_meshes[itr].m_indices = indexData;

	}
	meshfile.close();
}

void OctreeBuilder::OctreeCreator::CreateRootNode()
{
	Optimus::Vector3	Origin_Root, halfDimension;

	float maxX = 0, maxY = 0, maxZ = 0;
	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;

	for( unsigned int i = 0; i < m_entityCount; i++ )
	{
		for( unsigned int j = 0; j < m_meshes[i].m_primitiveCount; j++ )
		{
			if( m_meshes[i].m_vertices[j].x > maxX )
				maxX = m_meshes[i].m_vertices[j].x;

			if( m_meshes[i].m_vertices[j].y > maxY )
				maxY = m_meshes[i].m_vertices[j].y;

			if( m_meshes[i].m_vertices[j].z > maxZ )
				maxZ = m_meshes[i].m_vertices[j].z;

			if( m_meshes[i].m_vertices[j].x < minX )
				minX = m_meshes[i].m_vertices[j].x;

			if( m_meshes[i].m_vertices[j].y < minY )
				minY = m_meshes[i].m_vertices[j].y;

			if( m_meshes[i].m_vertices[j].z < minZ )
				minZ = m_meshes[i].m_vertices[j].z;
		}
	}

	halfDimension = Optimus::Vector3( ( maxX - minX ) / 2, ( maxY - minY ) / 2, ( maxZ - minZ ) / 2 );

	Origin_Root = Optimus::Vector3( ( maxX + minX )/2, ( maxY + minY )/2, ( maxZ + minZ )/2 );

	m_octree->m_rootNode->m_center = Origin_Root;
	m_octree->m_rootNode->m_halfDimension = halfDimension;
	m_octree->m_rootNode->m_isLeafNode = false;
	for( unsigned int i = 0; i < m_entityCount; i++ )
	{
		for( unsigned int j = 0; j < m_meshes[i].m_primitiveCount; j+=3 )
		{
			unsigned int itr0 = m_meshes[i].m_indices[j];
			unsigned int itr1 = m_meshes[i].m_indices[j+2];
			unsigned int itr2 = m_meshes[i].m_indices[j+1];
			Optimus::Vector3 first = Optimus::Vector3( m_meshes[i].m_vertices[itr0].x, m_meshes[i].m_vertices[itr0].y, m_meshes[i].m_vertices[itr0].z );
			Optimus::Vector3 second = Optimus::Vector3( m_meshes[i].m_vertices[itr1].x, m_meshes[i].m_vertices[itr1].y, m_meshes[i].m_vertices[itr1].z );
			Optimus::Vector3 third = Optimus::Vector3( m_meshes[i].m_vertices[itr2].x, m_meshes[i].m_vertices[itr2].y, m_meshes[i].m_vertices[itr2].z );
			Triangle t = Triangle( first, second, third );
			m_octree->m_rootNode->m_listOrderedPoints.push_back( t );
		}
	}
}

bool OctreeBuilder::OctreeCreator::ShouldPushDown( OctreeNode* i_node, Optimus::Vector3& i_first, Optimus::Vector3& i_second, Optimus::Vector3& i_third )
{
	if( ( ( i_node->m_center - i_node->m_halfDimension ) <= i_first ) && ( ( i_node->m_center + i_node->m_halfDimension ) >= i_first ) )
	{
		if( ( ( i_node->m_center - i_node->m_halfDimension ) <= i_second ) && ( ( i_node->m_center + i_node->m_halfDimension ) >= i_second ) )
		{
			if( ( ( i_node->m_center - i_node->m_halfDimension ) <= i_third ) && ( ( i_node->m_center + i_node->m_halfDimension ) >= i_third ) )
			{
				return true;
			}
		}
	}
	return false;
}

void OctreeBuilder::OctreeCreator::FillData( OctreeNode* i_node, std::vector<Triangle>& i_listOrderedPoints )
{
	bool erased = false;
	if( i_node->m_currentDepth != 0 )
	{
		for( unsigned int i = 0; i< i_listOrderedPoints.size(); i++ )
		{
			if( ShouldPushDown( i_node, i_listOrderedPoints[i].m_first, i_listOrderedPoints[i].m_second, i_listOrderedPoints[i].m_third ) )
			{
				i_node->m_listOrderedPoints.push_back( i_listOrderedPoints[i] );
				i_listOrderedPoints.erase( i_listOrderedPoints.begin() + i );
				erased = true;
			}
			if( erased )
				continue;
		}
	}

	if( ( i_node->m_currentDepth == this->m_octree->m_granularityLevel ) || ( i_node->m_listOrderedPoints.size() == 0 ) )
		i_node->m_isLeafNode = true;
	else
		i_node->m_isLeafNode = false;

	if( i_node->m_currentDepth < this->m_octree->m_granularityLevel )
	{
		if( !i_node->m_isLeafNode )
		{
			Divide( i_node );

			for( unsigned int i = 0; i< 8; i++ )
			{
				FillData( i_node->m_children[i], i_node->m_listOrderedPoints );
			}
		}
	}
}

void OctreeBuilder::OctreeCreator::Divide( OctreeNode* i_node )
{
	for( unsigned int i = 0; i < 8; i++ )
	{
		OctreeNode* childNode = new OctreeNode();
		childNode->m_halfDimension = i_node->m_halfDimension / 2.0f;

		switch( i )
		{
			case 0:	//Upper Right Forward
			{
				childNode->m_center = Optimus::Vector3( i_node->m_center.x() + childNode->m_halfDimension.x(), 
														i_node->m_center.y() + childNode->m_halfDimension.y(), 
														i_node->m_center.z() + childNode->m_halfDimension.z() );
				break;
			}
			case 1: // Upper Left Forward
			{
				childNode->m_center = Optimus::Vector3( i_node->m_center.x() - childNode->m_halfDimension.x(), 
														i_node->m_center.y() + childNode->m_halfDimension.y(), 
														i_node->m_center.z() + childNode->m_halfDimension.z() );
				break;
			}
			case 2: //Upper Right Behind
			{
				childNode->m_center = Optimus::Vector3( i_node->m_center.x() + childNode->m_halfDimension.x(), 
														i_node->m_center.y() + childNode->m_halfDimension.y(), 
														i_node->m_center.z() - childNode->m_halfDimension.z() );
				break;
			}
			case 3: //Upper Left Behind
			{
				childNode->m_center = Optimus::Vector3( i_node->m_center.x() - childNode->m_halfDimension.x(), 
														i_node->m_center.y() + childNode->m_halfDimension.y(), 
														i_node->m_center.z() - childNode->m_halfDimension.z() );
				break;
			}
			case 4: //Lower Right Forward
			{
				childNode->m_center = Optimus::Vector3( i_node->m_center.x() + childNode->m_halfDimension.x(), 
														i_node->m_center.y() - childNode->m_halfDimension.y(), 
														i_node->m_center.z() + childNode->m_halfDimension.z() );
				break;
			}
			case 5: //Lower Left Forward
				{
					childNode->m_center = Optimus::Vector3( i_node->m_center.x() - childNode->m_halfDimension.x(), 
															i_node->m_center.y() - childNode->m_halfDimension.y(), 
															i_node->m_center.z() + childNode->m_halfDimension.z() );
					break;
				}
			case 6: //Lower Right Behind
			{
				childNode->m_center = Optimus::Vector3( i_node->m_center.x() + childNode->m_halfDimension.x(), 
														i_node->m_center.y() - childNode->m_halfDimension.y(), 
														i_node->m_center.z() - childNode->m_halfDimension.z() );
				break;
			}
			case 7: //Lower Left Behind
			{
				childNode->m_center = Optimus::Vector3( i_node->m_center.x() - childNode->m_halfDimension.x(), 
														i_node->m_center.y() - childNode->m_halfDimension.y(), 
														i_node->m_center.z() - childNode->m_halfDimension.z() );
				break;
			}
		}
		i_node->m_children[i] = childNode;
		i_node->m_children[i]->m_currentDepth = i_node->m_currentDepth + 1;
	}
}

void OctreeBuilder::OctreeCreator::SortByDFS( OctreeNode* i_node, std::vector<OctreeNode*>& i_sortedNodes )
{
	if( i_node != 0 )
	{
		i_sortedNodes.push_back( i_node );

		if( !i_node->m_isLeafNode )
		{
			for( unsigned int i = 0; i < 8; i++ )
			{
				SortByDFS( i_node->m_children[i], i_sortedNodes );
			}
		}
	}
}

void OctreeBuilder::OctreeCreator::UnSort( OctreeNode* i_node, std::vector<OctreeNode*>& i_sortedNodes, int i_gran )
{
	if( i_node->m_center == Optimus::v_Zero )
	{
		i_node->m_center = i_sortedNodes[count]->m_center;
		i_node->m_halfDimension = i_sortedNodes[count]->m_halfDimension;
		i_node->m_currentDepth = i_sortedNodes[count]->m_currentDepth;
		i_node->m_isLeafNode = i_sortedNodes[count]->m_isLeafNode;
		if( i_sortedNodes[count]->m_listOrderedPoints.size() != 0 )
		{
			for( unsigned int i = 0 ; i < i_sortedNodes[count]->m_listOrderedPoints.size(); i++ )
			{
				i_node->m_listOrderedPoints.push_back( i_sortedNodes[count]->m_listOrderedPoints[i] );
			}
		}
		count++;
	}
	if( !i_node->m_isLeafNode )
	{	
		for( unsigned int i = 0; i < 8; i++ )
		{
			assert( !i_node->m_isLeafNode );
			OctreeNode* newNode = new OctreeNode();
			i_node->m_children[i] = newNode;
			
			//if( count < i_sortedNodes.size() )
			{
				UnSort( i_node->m_children[i], i_sortedNodes, i_gran );
			}
		}
	}
}

void OctreeBuilder::OctreeCreator::WriteData()
{
	std::vector<OctreeNode*> sortedNodes;

	SortByDFS( m_octree->m_rootNode, sortedNodes );

	std::ofstream outFile( "../Assets/Meshes/OctreeMeshTXT.txt", std::ios::out );
	outFile << "Octree Begin\n";
	outFile << "Granularity Level:" << m_octree->m_granularityLevel <<"\n";
	outFile << "Octree Size: " << sortedNodes.size() << "\n";

	for( unsigned int i = 0; i < sortedNodes.size(); i++ )
	{
		outFile << "Node Number " << i;
		outFile << ":\nNode Begin:\n";

		outFile << "Node Center: " << sortedNodes[i]->m_center.x() << "," << sortedNodes[i]->m_center.y() << "," << sortedNodes[i]->m_center.z() << "\n";
		outFile << "Node Half Dimensions: " << sortedNodes[i]->m_halfDimension.x() << "," << sortedNodes[i]->m_halfDimension.y() << "," << sortedNodes[i]->m_halfDimension.z() << "\n";
		outFile << "Node Depth: " << sortedNodes[i]->m_currentDepth << "\n";
		outFile << "Node Leaf Node?: " << sortedNodes[i]->m_isLeafNode << "\n";
		outFile << "Node Triangle Number: " << sortedNodes[i]->m_listOrderedPoints.size() << "\n";
		outFile << "Node Points: " << "\n";
		if( sortedNodes[i]->m_listOrderedPoints.size() != 0 )
		{
			for( unsigned int j = 0; j < sortedNodes[i]->m_listOrderedPoints.size(); j++ )
			{
				outFile << sortedNodes[i]->m_listOrderedPoints[j].m_first.x() << "," << sortedNodes[i]->m_listOrderedPoints[j].m_first.y() << "," << sortedNodes[i]->m_listOrderedPoints[j].m_first.z() << "\n";
				outFile << sortedNodes[i]->m_listOrderedPoints[j].m_second.x() << "," << sortedNodes[i]->m_listOrderedPoints[j].m_second.y() << "," << sortedNodes[i]->m_listOrderedPoints[j].m_second.z() << "\n";
				outFile << sortedNodes[i]->m_listOrderedPoints[j].m_third.x() << "," << sortedNodes[i]->m_listOrderedPoints[j].m_third.y() << "," << sortedNodes[i]->m_listOrderedPoints[j].m_third.z() << "\n";
			}
		}
		else
		{
			outFile << "No Triangles\n";
		}
		outFile<< "Node end\n";
	}
	outFile << "Octree End";
	outFile.close();
	unsigned int octreeSize = sortedNodes.size();

	int gran = m_octree->m_granularityLevel;

	std::ofstream outBinFile( "../Assets/Meshes/OctreeMesh.bin", std::ios::out | std::ios::binary );
	outBinFile.write( reinterpret_cast<const char*>(&gran), sizeof( int ) );
	outBinFile.write( reinterpret_cast<const char*>(&octreeSize), sizeof( unsigned int ) );
	for( unsigned int i = 0; i< sortedNodes.size(); i++ )
	{
		float c_x = sortedNodes[i]->m_center.x();
		float c_y = sortedNodes[i]->m_center.y();
		float c_z = sortedNodes[i]->m_center.z();

		float hf_x = sortedNodes[i]->m_halfDimension.x();
		float hf_y = sortedNodes[i]->m_halfDimension.y();
		float hf_z = sortedNodes[i]->m_halfDimension.z();

		int depth = sortedNodes[i]->m_currentDepth;
		bool leaf = sortedNodes[i]->m_isLeafNode;
		int trianSize = sortedNodes[i]->m_listOrderedPoints.size();

		outBinFile.write( reinterpret_cast<const char*>(&c_x), sizeof( float ) );
		outBinFile.write( reinterpret_cast<const char*>(&c_y), sizeof( float ) );
		outBinFile.write( reinterpret_cast<const char*>(&c_z), sizeof( float ) );

		outBinFile.write( reinterpret_cast<const char*>(&hf_x), sizeof( float ) );
		outBinFile.write( reinterpret_cast<const char*>(&hf_y), sizeof( float ) );
		outBinFile.write( reinterpret_cast<const char*>(&hf_z), sizeof( float ) );

		outBinFile.write( reinterpret_cast<const char*>(&depth), sizeof( int ) );
		outBinFile.write( reinterpret_cast<const char*>(&leaf), sizeof( bool ) );
		outBinFile.write( reinterpret_cast<const char*>(&trianSize), sizeof( int ) );

		if( trianSize != 0 )
		{
			for( unsigned int j = 0; j < sortedNodes[i]->m_listOrderedPoints.size(); j++ )
			{
				float p1_x = sortedNodes[i]->m_listOrderedPoints[j].m_first.x();
				float p1_y = sortedNodes[i]->m_listOrderedPoints[j].m_first.y();
				float p1_z = sortedNodes[i]->m_listOrderedPoints[j].m_first.z();

				outBinFile.write( reinterpret_cast<const char*>(&p1_x), sizeof( float ) );
				outBinFile.write( reinterpret_cast<const char*>(&p1_y), sizeof( float ) );
				outBinFile.write( reinterpret_cast<const char*>(&p1_z), sizeof( float ) );

				float p2_x = sortedNodes[i]->m_listOrderedPoints[j].m_second.x();
				float p2_y = sortedNodes[i]->m_listOrderedPoints[j].m_second.y();
				float p2_z = sortedNodes[i]->m_listOrderedPoints[j].m_second.z();

				outBinFile.write( reinterpret_cast<const char*>(&p2_x), sizeof( float ) );
				outBinFile.write( reinterpret_cast<const char*>(&p2_y), sizeof( float ) );
				outBinFile.write( reinterpret_cast<const char*>(&p2_z), sizeof( float ) );

				float p3_x = sortedNodes[i]->m_listOrderedPoints[j].m_third.x();
				float p3_y = sortedNodes[i]->m_listOrderedPoints[j].m_third.y();
				float p3_z = sortedNodes[i]->m_listOrderedPoints[j].m_third.z();

				outBinFile.write( reinterpret_cast<const char*>(&p3_x), sizeof( float ) );
				outBinFile.write( reinterpret_cast<const char*>(&p3_y), sizeof( float ) );
				outBinFile.write( reinterpret_cast<const char*>(&p3_z), sizeof( float ) );
			}
		}
	}
	outBinFile.close();
}

void OctreeBuilder::OctreeCreator::ReadOctree()
{
	std::vector<OctreeNode*> i_sortedList;
	
	std::ifstream inBinFile( "../Assets/Meshes/OctreeMesh.bin", std::ios::in | std::ios::binary );
	unsigned int octreeSize;
	int depth, trianSize, gran;
	bool isLeaf;
	float c_x, c_y, c_z, hf_x, hf_y, hf_z, p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, p3_x, p3_y, p3_z;
	inBinFile.read( reinterpret_cast<char*>(&gran), sizeof( int ) );
	inBinFile.read( reinterpret_cast<char*>(&octreeSize), sizeof( unsigned int ) );

	for( unsigned int i = 0; i< octreeSize; i++ )
	{
		OctreeNode* node = new OctreeNode();
		inBinFile.read( reinterpret_cast<char*>(&c_x), sizeof( float ) );
		inBinFile.read( reinterpret_cast<char*>(&c_y), sizeof( float ) );
		inBinFile.read( reinterpret_cast<char*>(&c_z), sizeof( float ) );

		inBinFile.read( reinterpret_cast<char*>(&hf_x), sizeof( float ) );
		inBinFile.read( reinterpret_cast<char*>(&hf_y), sizeof( float ) );
		inBinFile.read( reinterpret_cast<char*>(&hf_z), sizeof( float ) );

		inBinFile.read( reinterpret_cast<char*>(&depth), sizeof( int ) );
		inBinFile.read( reinterpret_cast<char*>(&isLeaf), sizeof( bool ) );
		inBinFile.read( reinterpret_cast<char*>(&trianSize), sizeof( int ) );
		
		if( trianSize != 0 )
		{
			for( int j = 0; j < trianSize; j++ )
			{
				inBinFile.read( reinterpret_cast<char*>(&p1_x), sizeof( float ) );
				inBinFile.read( reinterpret_cast<char*>(&p1_y), sizeof( float ) );
				inBinFile.read( reinterpret_cast<char*>(&p1_z), sizeof( float ) );

				inBinFile.read( reinterpret_cast<char*>(&p2_x), sizeof( float ) );
				inBinFile.read( reinterpret_cast<char*>(&p2_y), sizeof( float ) );
				inBinFile.read( reinterpret_cast<char*>(&p2_z), sizeof( float ) );

				inBinFile.read( reinterpret_cast<char*>(&p3_x), sizeof( float ) );
				inBinFile.read( reinterpret_cast<char*>(&p3_y), sizeof( float ) );
				inBinFile.read( reinterpret_cast<char*>(&p3_z), sizeof( float ) );

				node->m_listOrderedPoints.push_back( Triangle( Optimus::Vector3( p1_x, p1_y, p1_z ), Optimus::Vector3( p2_x, p2_y, p2_z ), Optimus::Vector3( p3_x, p3_y, p3_z ) ) );
			}
		}
		
		node->m_center = Optimus::Vector3( c_x, c_y, c_z );
		node->m_halfDimension = Optimus::Vector3( hf_x, hf_y, hf_z );
		node->m_currentDepth = depth;
		node->m_isLeafNode = (bool)isLeaf;

		i_sortedList.push_back( node );
	}

	inBinFile.close();

	/*OctreeNode* reRoot = new OctreeNode();

	Octree* reTree = new Octree( reRoot, gran );*/

	m_octree = new Octree( new OctreeNode(), gran );

	UnSort( m_octree->m_rootNode, i_sortedList, gran );

	for( unsigned int i = 0; i < i_sortedList.size(); i++ )
	{
		if( i_sortedList[i] )
		{
			delete i_sortedList[i];
			i_sortedList[i] = NULL;
		}
	}
}

void OctreeBuilder::OctreeCreator::CreateOctree()
{
	ReadMeshData();

	m_octree = new Octree( new OctreeNode(), 3 );

	CreateRootNode();
		
	FillData( m_octree->m_rootNode, m_octree->m_rootNode->m_listOrderedPoints );

	WriteData();

}

void OctreeBuilder::OctreeCreator::Shutdown()
{
	if( m_octree )
	{
		std::vector<OctreeNode*> sortedList;
		SortByDFS( m_octree->m_rootNode, sortedList );

		for( unsigned int i =  m_octree->m_granularityLevel; i > 0; i-- )
		{
			for( unsigned int j = 0; j < sortedList.size(); j++ )
			{
				if( sortedList[j] )
				{
					if( sortedList[j]->m_currentDepth == i )
					{
						delete sortedList[j];
						sortedList[j] = NULL;
						continue;
					}
				}
			}
		}

		if( m_octree->m_rootNode )
		{
			delete m_octree->m_rootNode;
			m_octree->m_rootNode = NULL;
		}

		if( m_octree )
		{
			delete m_octree;
			m_octree = NULL;
		}
	}
	if( m_instance )
	{
		delete m_instance;
		m_instance = NULL;
	}
}