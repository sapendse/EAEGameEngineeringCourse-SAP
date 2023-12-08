#include "cFPSCamera.h"
#include "UserInput.h"
#include "cTime.h"
#include "..\Collision\Collision.h"
#include "..\Timer\TimeSystem.h"
#include <fstream>
#include <cmath>

//OctreeBuilder::OctreeCreator* OctreeBuilder::OctreeCreator::m_instance = NULL;

namespace Optimus
{
	cFPSCamera::cFPSCamera() :
		m_position( 0.0f, 0.0f, -50.0f),
		m_lookAt( 0.0f, 0.0f, 1.0f),
		m_up(0.0f, 1.0f, 0.0f),
		m_right( 1.0f, 0.0f, 0.0f ),
		entityCount( 0 ),
		myMeshes( NULL ),
		m_collisionHappened_Ground( false ),
		m_collisionHappened_Wall( false ),
		m_playerHeight( 75.0f )
	{
	}

	const D3DXMATRIX cFPSCamera::GetWorldToViewTransform() const
	{
		/*D3DXMATRIX transform_worldToView;
					
		D3DXMatrixRotationAxis( &transform_worldToView, &m_up, m_yawRotation );

		D3DXMATRIX translation;

		D3DXMatrixTranslation( &translation, -m_position.x, -m_position.y, -m_position.z);
		transform_worldToView = transform_worldToView * translation;

		return transform_worldToView;*/

		D3DXMATRIX viewMatrix;
		D3DXMatrixIdentity(&viewMatrix);

		viewMatrix._11 = m_right.x; viewMatrix._12 = m_up.x; viewMatrix._13 = m_lookAt.x;
		viewMatrix._21 = m_right.y; viewMatrix._22 = m_up.y; viewMatrix._23 = m_lookAt.y;
		viewMatrix._31 = m_right.z; viewMatrix._32 = m_up.z; viewMatrix._33 = m_lookAt.z;

		viewMatrix._41 = - D3DXVec3Dot( &m_position, &m_right ); 
		viewMatrix._42 = - D3DXVec3Dot( &m_position, &m_up );
		viewMatrix._43 = - D3DXVec3Dot( &m_position, &m_lookAt );

		/*D3DXMATRIX translationMatrix;
		D3DXMATRIX rotationMatrix;
		D3DXMatrixTranslation( &translationMatrix, -m_position.x, -m_position.y, -m_position.z);
		D3DXMatrixRotationY( &rotationMatrix, m_yawRotation );

		viewMatrix = translationMatrix * rotationMatrix;*/
		return viewMatrix;
	}

	bool cFPSCamera::Initialize( IDirect3DDevice9* i_direct3dDevice )
	{
		m_direct3dDevice = i_direct3dDevice;
		//ReadMeshData();
		ReadData();
		return true;
	}

	void cFPSCamera::UpdateInfo( Optimus::Vector3& i_pos, Optimus::Vector3& i_lookAt, Optimus::Vector3& i_speed, Optimus::Vector3& i_groundRay, Optimus::Vector3& i_for, Optimus::Vector3& i_back )
	{
		i_pos = m_player->m_gameObject->getPosition();
		i_lookAt = Optimus::Vector3( m_lookAt.x, m_lookAt.y, m_lookAt.z );
		i_speed = i_lookAt * 100.0f;
		i_speed += m_player->m_gameObject->getAcceleration() * Optimus::TimeSystem::getInstance()->getDeltaTime();

		i_groundRay = i_pos;
		i_groundRay.y( i_pos.y() - m_playerHeight );
		i_for = i_pos + i_speed * Optimus::TimeSystem::getInstance()->getDeltaTime();	
		i_back = i_pos - i_speed * Optimus::TimeSystem::getInstance()->getDeltaTime();
	}

	void cFPSCamera::FindTrianglesToCollide( Optimus::Vector3& a, Optimus::Vector3& b, OctreeBuilder::OctreeNode* i_node, std::vector<OctreeBuilder::Triangle>& i_triangleList  )
	{
		Optimus::Vector3 minBound = i_node->m_center - i_node->m_halfDimension;
		Optimus::Vector3 maxBound = i_node->m_center + i_node->m_halfDimension;

		if( Optimus::Collision::getInstance()->IntersectSegmentAABB( a, b, i_node->m_center, i_node->m_halfDimension ) )
		{
			if( i_node->m_listOrderedPoints.size() != 0 )
			{
				for( unsigned int j = 0; j < i_node->m_listOrderedPoints.size(); j++ )
				{
					i_triangleList.push_back( i_node->m_listOrderedPoints[j] );
				}
			}

			{
				const DWORD line_fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;

				Line* line_vertices = OPT_NEW Line[15];

				line_vertices[0].x = minBound.x();
				line_vertices[0].y = minBound.y();
				line_vertices[0].z = minBound.z();
				line_vertices[0].color = D3DCOLOR_XRGB (0, 255, 255);
			
				line_vertices[1].x = maxBound.x();
				line_vertices[1].y = minBound.y();
				line_vertices[1].z = minBound.z();
				line_vertices[1].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[2].x = maxBound.x();
				line_vertices[2].y = maxBound.y();
				line_vertices[2].z = minBound.z();
				line_vertices[2].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[3].x = minBound.x();
				line_vertices[3].y = maxBound.y();
				line_vertices[3].z = minBound.z();
				line_vertices[3].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[4].x = minBound.x();
				line_vertices[4].y = minBound.y();
				line_vertices[4].z = minBound.z();
				line_vertices[4].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[5].x = minBound.x();
				line_vertices[5].y = minBound.y();
				line_vertices[5].z = maxBound.z();
				line_vertices[5].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[6].x = minBound.x();
				line_vertices[6].y = maxBound.y();
				line_vertices[6].z = maxBound.z();
				line_vertices[6].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[7].x = minBound.x();
				line_vertices[7].y = maxBound.y();
				line_vertices[7].z = minBound.z();
				line_vertices[7].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[8].x = maxBound.x();
				line_vertices[8].y = maxBound.y();
				line_vertices[8].z = minBound.z();
				line_vertices[8].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[9].x = maxBound.x();
				line_vertices[9].y = maxBound.y();
				line_vertices[9].z = maxBound.z();
				line_vertices[9].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[10].x = minBound.x();
				line_vertices[10].y = maxBound.y();
				line_vertices[10].z = maxBound.z();
				line_vertices[10].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[11].x = minBound.x();
				line_vertices[11].y = minBound.y();
				line_vertices[11].z = maxBound.z();
				line_vertices[11].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[12].x = maxBound.x();
				line_vertices[12].y = minBound.y();
				line_vertices[12].z = maxBound.z();
				line_vertices[12].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[13].x = maxBound.x();
				line_vertices[13].y = minBound.y();
				line_vertices[13].z = minBound.z();
				line_vertices[13].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[14].x = maxBound.x();
				line_vertices[14].y = minBound.y();
				line_vertices[14].z = maxBound.z();
				line_vertices[14].color = D3DCOLOR_XRGB (0, 255, 255);

				line_vertices[15].x = maxBound.x();
				line_vertices[15].y = maxBound.y();
				line_vertices[15].z = maxBound.z();
				line_vertices[15].color = D3DCOLOR_XRGB (0, 255, 255);

				m_direct3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 15, line_vertices, sizeof(Line));
			}
			if( !i_node->m_isLeafNode )
			{
				for( int i = 0; i < 8; i++ )
				{		
					FindTrianglesToCollide( a, b, i_node->m_children[i], i_triangleList );
				}
			}
		}
		else
		{
			return;
		}
	}

	void cFPSCamera::CollisionDetection()
	{
		Optimus::Vector3 pos, lookAt, speed, groundRay, nextForPos, nextBackPos;
		UpdateInfo( pos, lookAt, speed, groundRay,nextForPos, nextBackPos );
		
		/*c.x( a.x() - std::sin( m_yawRotation )* 100.0f );
		c.z( a.z() + std::cos( m_yawRotation )* 100.0f );
		nextForPos = pos + lookAt * 150.0f;
			
		d.x( a.x() + std::sin( m_yawRotation )* 100.0f );
		d.z( a.z() - std::cos( m_yawRotation )* 100.0f );
		nextBackPos = pos - lookAt * 150.0f;
		q = D3DXVECTOR3( m_playerPos.x - sin(m_player->m_angle) * 100, m_playerPos.y, m_playerPos.z + cos(m_player->m_angle) * 100);*/
		std::vector<OctreeBuilder::Triangle> triangleList;
		FindTrianglesToCollide( pos, groundRay, OctreeBuilder::OctreeCreator::getInstance()->m_octree->m_rootNode, triangleList );
		float u,v,w,t;
		bool groundNot = false;

		const DWORD line_fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
		
		Line* raycastPoints = OPT_NEW Line[1];

		raycastPoints[0].x = pos.x();
		raycastPoints[0].y = pos.y();
		raycastPoints[0].z = pos.z();
		raycastPoints[0].color = D3DCOLOR_XRGB (255, 0, 0);

		raycastPoints[1].x = nextForPos.x();
		raycastPoints[1].y = nextForPos.y() - 20;
		raycastPoints[1].z = nextForPos.z();
		raycastPoints[1].color = D3DCOLOR_XRGB (255, 0, 0);

		m_direct3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, raycastPoints, sizeof(Line));

		if( triangleList.size() != 0 )
		{
			for( unsigned int i = 0; i < triangleList.size(); i++ )
			{
				Optimus::Vector3 first = triangleList[i].m_first;
				Optimus::Vector3 second = triangleList[i].m_second;
				Optimus::Vector3 third = triangleList[i].m_third;

				if( Optimus::Collision::getInstance()->IntersectSegmentTriangle( pos, groundRay, first, second, third, u, v, w, t ) == 1 )
				{
					m_collisionHappened_Ground = true;
					
					groundNot = true;

					CollisionResponse( 0, Optimus::Vector3( groundRay - pos ), first, second, third, u , v, w, t );
					UpdateInfo( pos, lookAt, speed, groundRay,nextForPos, nextBackPos );
					//Optimus::Physics::getInstance()->setGrounded( m_collisionHappened_Ground );
				}
			}
		}
		
		triangleList.clear();
		FindTrianglesToCollide( pos, nextForPos, OctreeBuilder::OctreeCreator::getInstance()->m_octree->m_rootNode, triangleList );
		if( triangleList.size() != 0 )
		{
			for( unsigned int i = 0; i < triangleList.size(); i++ )
			{
				Optimus::Vector3 first = triangleList[i].m_first;
				Optimus::Vector3 second = triangleList[i].m_second;
				Optimus::Vector3 third = triangleList[i].m_third;

				if( Optimus::Collision::getInstance()->IntersectSegmentTriangle( pos, nextForPos, first, second, third, u, v, w, t ) == 1 )
				{
					m_collisionHappened_Wall = true;
					/*Optimus::Vector3 n3 = q - e;
					Optimus::Vector3 n4 = w - e;
										
					Optimus::Vector3 normal2 = Optimus::Vector3::crossProd( n3, n4 );
					normal2.normalise();
					Optimus::Collision::getInstance()->setNormal( normal2.x(), normal2.y(), normal2.z() );
					Optimus::Collision::getInstance()->respondToCollision( m_playerPositionInArray, -1 );*/

					Optimus::Vector3 intersectionPoint = first * u + second * v + third * w;
					CollisionResponse( 1, Optimus::Vector3( nextForPos - pos ), first, second, third, u , v, w, t );
					UpdateInfo( pos, lookAt, speed, groundRay,nextForPos, nextBackPos );
					//m_player->m_gameObject->setPosition( c - ( c * m ) );
				}
			}
		}

		triangleList.clear();
		FindTrianglesToCollide( pos, nextBackPos, OctreeBuilder::OctreeCreator::getInstance()->m_octree->m_rootNode, triangleList );
		if( triangleList.size() != 0 )
		{
			for( unsigned int i = 0; i < triangleList.size(); i++ )
			{
				Optimus::Vector3 first = triangleList[i].m_first;
				Optimus::Vector3 second = triangleList[i].m_second;
				Optimus::Vector3 third = triangleList[i].m_third;

				if( Optimus::Collision::getInstance()->IntersectSegmentTriangle( pos, nextBackPos, first, second, third, u, v, w, t ) == 1 )
				{
					m_collisionHappened_Wall = true;

					CollisionResponse( 1, Optimus::Vector3( nextBackPos - pos ), first, second, third, u , v, w, t );
					UpdateInfo( pos, lookAt, speed, groundRay,nextForPos, nextBackPos );
					//m_player->m_gameObject->setPosition( d - ( d * m ) );
				}
			}
		}
		triangleList.clear();
		if( groundNot == false )
		{
			m_collisionHappened_Ground = false;
			//Optimus::Physics::getInstance()->setGrounded( m_collisionHappened_Ground );
		}
	}

	void cFPSCamera::CollisionResponse( int i_type, Optimus::Vector3& i_ray, Optimus::Vector3& i_first, Optimus::Vector3& i_second, Optimus::Vector3& i_third, float u, float v, float w, float t )
	{
		if( i_type == 0 )
		{
			//Optimus::Vector3 n1 = i_first - i_third;
			//Optimus::Vector3 n2 = i_second - i_third;
			Optimus::Vector3 intersectionPoint = i_first * u+ i_second * v+ i_third * w; // m_player->m_gameObject->getPosition() + i_ray;
			//m_position.x = intersectionPoint.x();
			//m_position.z = intersectionPoint.z();
			//m_position.y = intersectionPoint.y() + 175.0f;
			intersectionPoint.y( intersectionPoint.y() + m_playerHeight );// - (  ( 1 - t ) * std::sqrt( Optimus::Vector3::magSqr( i_ray ) ) ) );
			m_player->m_gameObject->setPosition( intersectionPoint );
		}
		else
		{
			Optimus::Vector3 n3 = i_first - i_third;
			Optimus::Vector3 n4 = i_second - i_third;
			Optimus::Vector3 normal = Optimus::Vector3::crossProd( n3, n4 );
			normal = normal.normalise();
			/*Optimus::Collision::getInstance()->setNormal( normal2.x(), normal2.y(), normal2.z() );
			Optimus::Collision::getInstance()->respondToCollision( m_playerPositionInArray, -1 );*/

			float dotProd = Optimus::Vector3::dotProd( m_player->m_gameObject->getVelocity() * -1, normal.normalise() );
			
			Optimus::Vector3 temp = normal.normalise() * 2 * dotProd - m_player->m_gameObject->getVelocity() * -1;
			//temp.y( temp.y() * 4.0f );
			Optimus::Vector3 tempPos = m_player->m_gameObject->getPosition();
			tempPos+= temp* Optimus::TimeSystem::getInstance()->getDeltaTime();
			m_player->m_gameObject->setPosition( tempPos );

			//Optimus::Vector3 intersectionPoint = i_first * u + i_second * v + i_third * w;
			//intersectionPoint -= i_ray * ( 1 - t );
			//m_player->m_gameObject->setPosition( intersectionPoint );
		}
	}

	bool cFPSCamera::Update()
	{
		
		if( m_player != NULL )
		{
			Optimus::Vector3::convertToD3DXVECTOR3( m_player->m_gameObject->getPosition(), m_position ); //( m_player->m_gameObject->getPosition().x(), m_player->m_gameObject->getPosition().y(), m_player->m_gameObject->getPosition().z() );

			float fpsrot = m_yawRotation;
			float phyrot = m_player->m_gameObject->getRotation();
			if( m_player->getHasRotated() )
			{					
				m_yawRotation = m_player->m_gameObject->getRotation();
				m_lookAt = D3DXVECTOR3( m_lookAt.x * std::cos( phyrot - fpsrot ) - m_lookAt.z * std::sin( phyrot - fpsrot ), m_lookAt.y, m_lookAt.x * std::sin( phyrot - fpsrot ) + m_lookAt.z * std::cos( phyrot - fpsrot ) );
				m_right = D3DXVECTOR3( m_right.x * std::cos( phyrot - fpsrot ) - m_right.z * std::sin( phyrot - fpsrot ), m_right.y, m_right.x * std::sin( phyrot - fpsrot ) + m_right.z * std::cos( phyrot - fpsrot ) );
				m_player->setHasRotated( false );
			}

			CollisionDetection();
			m_player->m_gameObject->setGrounded( m_collisionHappened_Ground );
		}
		else
		{
			if( UserInput::IsKeyPressed( 'W' ) )
			{
				m_position.x += 10.0f;
			}
			if( UserInput::IsKeyPressed( 'S' ) )
			{
				m_position.x -= 10.0f;
			}
			if( UserInput::IsKeyPressed( 'D' ) )
			{
				m_position.z += 10.0f;
			}
			if( UserInput::IsKeyPressed( 'A' ) )
			{
				m_position.z -= 10.0f;
			}
		}
		return ( m_collisionHappened_Wall || m_collisionHappened_Ground );
	}

	void cFPSCamera::ReadMeshData()
	{
		unsigned int effectFileLength, textureFileLength, normalMapFileLength, m_specExpo, m_alphaValue, m_primitiveCount, m_vertexCount, m_dataReadTillNow = 0;
		char c;
		std::ifstream scenefile( "../Assets/Scenes/Collisionsceneinfo.txt", std::ifstream::in );
		while( !scenefile.eof() )
		{
			scenefile >> c;
			if( c == '-' )
			{
				scenefile >>cFPSCamera::getInstance()->entityCount;
			}
		}
		scenefile.close();
		cFPSCamera::getInstance()->myMeshes = OPT_NEW s_mesh[cFPSCamera::getInstance()->entityCount];
		
		std::ifstream meshfile( "../Assets/Meshes/FBXCollsionMeshes.bin", std::ifstream::in | std::ifstream::binary );
		for( unsigned int itr = 0; itr <cFPSCamera::getInstance()->entityCount; itr++ )
		{
			meshfile.read(  reinterpret_cast<char*>(&effectFileLength), sizeof( unsigned int ) );
			char* effectExpected = OPT_NEW char[effectFileLength];
			meshfile.read(  reinterpret_cast<char*>(effectExpected), ( effectFileLength + 1 ) );
			meshfile.read(  reinterpret_cast<char*>(&textureFileLength), sizeof( unsigned int ) );
			char* textureExpected = OPT_NEW char[textureFileLength];
			meshfile.read(  reinterpret_cast<char*>(textureExpected), ( textureFileLength + 1 ) );
			meshfile.read(  reinterpret_cast<char*>(&normalMapFileLength), sizeof( unsigned int ) );
			char* normalMapTexture = OPT_NEW char[normalMapFileLength];
			meshfile.read(  reinterpret_cast<char*>(normalMapTexture), ( normalMapFileLength + 1 ) );
			meshfile.read(  reinterpret_cast<char*>(&m_specExpo), sizeof( unsigned int ) );
			meshfile.read(  reinterpret_cast<char*>(&m_alphaValue), sizeof( unsigned int ) );
			meshfile.read(  reinterpret_cast<char*>(&m_primitiveCount), sizeof( unsigned int ) );
			meshfile.read(  reinterpret_cast<char*>(&m_vertexCount), sizeof( unsigned int ) );

			s_vertex* vertexData = OPT_NEW s_vertex[m_vertexCount];
			for( unsigned int c=0; c< m_vertexCount; c++ )
			{
				s_vertex temp;
				meshfile.read( reinterpret_cast<char*>(&temp), sizeof( s_vertex ) );
				vertexData[c] = temp;
			}
			cFPSCamera::getInstance()->myMeshes[itr].vertexCount = m_vertexCount;
			cFPSCamera::getInstance()->myMeshes[itr].myVertex = vertexData;
			unsigned int* indexData = OPT_NEW unsigned int[m_primitiveCount];
			for( unsigned int c =0; c< m_primitiveCount; c++ )
			{
				unsigned int temp1;
				meshfile.read( reinterpret_cast<char*>(&temp1), sizeof( unsigned int ) );
				indexData[c] = temp1;
			}
			cFPSCamera::getInstance()->myMeshes[itr].indexCount = m_primitiveCount;
			cFPSCamera::getInstance()->myMeshes[itr].myIndex = indexData;

			/*
			m_dataReadTillNow += ( ( 7 + ( m_primitiveCount ) ) * sizeof( unsigned int ) + ( m_vertexCount * sizeof( s_vertex ) ) + effectFileLength +
			normalMapFileLength + textureFileLength + 3 + sizeof( bool ) );*/

		}
	}

	void cFPSCamera::ReadData()
	{
		OctreeBuilder::OctreeCreator::getInstance()->ReadOctree();
		OctreeBuilder::OctreeCreator::getInstance()->WriteData();
	}
}