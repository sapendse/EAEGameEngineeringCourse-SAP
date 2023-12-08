#ifndef _FBX_IMPORTER_H
#define _FBX_IMPORTER_H

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <fbxsdk.h>
#include <vector>
#include "..\Optimus\_Source\Utilities\New.h"
#include "..\Optimus\_Source\Math\Vector3.h"

namespace FBXImporter
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
		Optimus::Vector3 m_min, m_max;
		unsigned int m_primitiveCount, m_indexCount, m_ID;
		char*		 m_textureName;
		unsigned int* m_indices;
		s_vertex* m_vertices;
		float alphaValue;
		bool m_dynamic;
	};


	class FBXImporter
	{
	public:
		//Constructor
		FBXImporter()
		{
			m_pFBXSDKManager = 0;
			if( m_pFBXSDKManager == 0 )
			{
				m_pFBXSDKManager = FbxManager::Create();

				FbxIOSettings* pIOsettings = FbxIOSettings::Create( m_pFBXSDKManager, IOSROOT );
				m_pFBXSDKManager->SetIOSettings( pIOsettings );
			}

		}

		static FBXImporter*		m_instance;

		static FBXImporter* getInstance()
		{
			if( m_instance == 0 )
				m_instance = OPT_NEW FBXImporter();
			return m_instance;
		}

		FbxManager*				m_pFBXSDKManager;
		//std::vector<s_vertex> m_listVertices;
		std::vector<s_mesh>		m_listMeshes;
		std::string				m_fileName;
		unsigned int*			m_indices;

		HRESULT InitiateFBX( std::string i_fileName );
		void	DisplayContent( FbxScene* i_pScene );
		void	DisplayContent( FbxNode* i_pNode );
		void	ParseNode( FbxNode* i_pNode );
		void	FindBounds( s_mesh& i_mesh );
		void	WriteToFile( std::vector<s_mesh> i_listMeshes );
		void	UpdateSceneFile( std::string i_meshFileName );
	};

}

#endif